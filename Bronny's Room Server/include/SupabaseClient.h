// SupabaseClient.h
#pragma once
#include <string>
#ifndef SUPABASE_URL
#define SUPABASE_URL "https://dtghwpdfdlxwpmsrqlwg.supabase.co"
#endif 

#ifndef SUPABASE_SERVICE_KEY
#define SUPABASE_SERVICE_KEY "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImR0Z2h3cGRmZGx4d3Btc3JxbHdnIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NDQxMzc5MDQsImV4cCI6MjA1OTcxMzkwNH0.f8g1FfiKNCxCbRUOiJeJG_MlImW9EfWwB9vXQfeRGGg"
#endif 



// Tiny helper that POSTS one JSON document to
//   https://<project>.supabase.co/rest/v1/login_events
// using the built‑in WinHTTP library.
class SupabaseClient
{
public:
    static SupabaseClient& Instance();

    // returns true on HTTP‑201/200, false otherwise
    void LogLoginEvent(const std::string& accountId,
        const std::string& username,
        const std::string& ip);
    // returns true if (username,password) exists,
    //and fills accountId with the row’s id (uuid)
    bool AuthenticateUser(const std::string& username,
        const std::string& password,
        std::string& accountId /* out */);

    // returns true on successful INSERT
    bool RegisterUser(const std::string& username,
        const std::string& password,
        std::string& accountId /* out */);


private:
    SupabaseClient();                            // singleton – ctor is private
    ~SupabaseClient();

    // Low‑level HTTPS POST/GET
    bool PostJson(const std::wstring& relPath,
        const std::string& body,            // may be empty for GET
        std::string& responseOut);          // response body
    bool PostJson(const std::wstring& relPath,
        const std::string& body)            // discard response
    {
        std::string dummy;
        return PostJson(relPath, body, dummy);
    }

    std::string m_url;   // https://… (no trailing slash)
    std::string m_key;   // service‑role key

    // no copy
    SupabaseClient(const SupabaseClient&) = delete;
    SupabaseClient& operator=(const SupabaseClient&) = delete;
};
