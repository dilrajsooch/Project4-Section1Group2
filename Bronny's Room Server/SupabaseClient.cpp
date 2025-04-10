#define _CRT_SECURE_NO_WARNINGS

#include "SupabaseClient.h"

#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

#ifndef WINHTTP_NO_HEADER_NAME     
#define WINHTTP_NO_HEADER_NAME nullptr
#endif


#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

/* ───────── helpers ───────── */

static std::wstring Widen(const std::string& s)
{
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    std::wstring w(n - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &w[0], n);
    return w;
}

/* ───────── singleton ───────── */

SupabaseClient& SupabaseClient::Instance()
{
    static SupabaseClient inst;
    return inst;
}

SupabaseClient::SupabaseClient()
{
    const char* url = std::getenv("SUPABASE_URL");
    const char* key = std::getenv("SUPABASE_SERVICE_KEY");

    m_url = url ? url : SUPABASE_URL;
    m_key = key ? key : SUPABASE_SERVICE_KEY;

    /* strip trailing slash if present */
    if (!m_url.empty() && m_url.back() == '/')
        m_url.pop_back();
}

SupabaseClient::~SupabaseClient() = default;

/* ───────── public API ───────── */

void SupabaseClient::LogLoginEvent(const std::string& accountId,
    const std::string& username,
    const std::string& ip)
{
    std::string json = std::string("[{")
        + "\"account_id\":\"" + accountId + "\","
        + "\"username\":\"" + username + "\","
        + "\"ip\":\"" + ip + "\""
        + "}]";

    PostJson(L"/rest/v1/login_events?return=minimal", json);
}

bool SupabaseClient::AuthenticateUser(const std::string& username,
    const std::string& password,
    std::string& accountId)
{
    std::wstring path = L"/rest/v1/accounts"
        L"?username=eq." + Widen(username) +
        L"&password=eq." + Widen(password) +
        L"&select=id";

    std::string resp;
    if (!PostJson(path, "", resp)) return false;   // GET

    auto p = resp.find("\"id\":\"");
    if (p == std::string::npos) return false;
    p += 6;
    auto q = resp.find('"', p);
    accountId = resp.substr(p, q - p);
    return true;
}

bool SupabaseClient::RegisterUser(const std::string& username,
    const std::string& password,
    std::string& accountId)
{
    std::string body = std::string("[{")
        + "\"username\":\"" + username + "\","
        + "\"password\":\"" + password + "\""
        + "}]";

    std::string resp;
    if (!PostJson(L"/rest/v1/accounts?select=id", body, resp)) return false;

    auto p = resp.find("\"id\":\"");
    if (p == std::string::npos) return false;
    p += 6;
    auto q = resp.find('"', p);
    accountId = resp.substr(p, q - p);
    return true;
}

/* ───────── low‑level HTTPS helper ───────── */

bool SupabaseClient::PostJson(const std::wstring& relPath,
    const std::string& body,
    std::string& responseOut)
{
    /* split host and ensure https:// stripped */
    std::string host = m_url;
    const std::string https = "https://";
    if (host.rfind(https, 0) == 0) host.erase(0, https.size());

    HINTERNET ses = WinHttpOpen(L"NBAChat/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);
    if (!ses) return false;

    HINTERNET con = WinHttpConnect(ses, Widen(host).c_str(),
        INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!con) { WinHttpCloseHandle(ses); return false; }

    HINTERNET req = WinHttpOpenRequest(
        con,
        body.empty() ? L"GET" : L"POST",
        relPath.c_str(),
        nullptr,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE);

    if (!req)
    {
        WinHttpCloseHandle(con); WinHttpCloseHandle(ses); return false;
    }

    /* build headers */
    std::wstring headers =
        L"Content-Type: application/json\r\n"
        L"apikey: " + Widen(m_key) + L"\r\n"
        L"Authorization: Bearer " + Widen(m_key) + L"\r\n"
        L"Prefer: return=minimal\r\n";

    WinHttpAddRequestHeaders(req, headers.c_str(), (DWORD)-1,
        WINHTTP_ADDREQ_FLAG_ADD);

    BOOL ok = WinHttpSendRequest(req,
        WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        body.empty() ? nullptr : const_cast<char *>(body.data()),
        (DWORD)body.size(),
        (DWORD)body.size(), 0)
        && WinHttpReceiveResponse(req, nullptr);

    if (!ok)
    {
        WinHttpCloseHandle(req); WinHttpCloseHandle(con); WinHttpCloseHandle(ses); return false;
    }

    /* read response body */
    DWORD size = 0;
    WinHttpQueryDataAvailable(req, &size);
    std::string buf(size, '\0');
    DWORD read = 0;
    if (size) WinHttpReadData(req, const_cast<char *>(buf.data()), size, &read);
    responseOut.assign(buf.data(), read);

    /* HTTP status */
    DWORD status = 0, len = sizeof(status);
    WinHttpQueryHeaders(req,
        WINHTTP_QUERY_STATUS_CODE | WINHTTP_HEADER_NAME_BY_INDEX,
        WINHTTP_NO_HEADER_NAME, &status, &len, WINHTTP_NO_HEADER_INDEX);

    WinHttpCloseHandle(req);
    WinHttpCloseHandle(con);
    WinHttpCloseHandle(ses);

    return status == 200 || status == 201 || status == 204;
}
