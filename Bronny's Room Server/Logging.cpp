#include "Logging.h"
#include <fstream>
#include <ctime>
#include <mutex>

static std::mutex g_logMutex; // to avoid concurrent write issues

void LogPacket(const std::string& direction, const std::string& clientIP, const Packet& pkt)
{
    std::lock_guard<std::mutex> lock(g_logMutex);

    // Open in append mode
    std::ofstream log("server_log.txt", std::ios::app);
    if (!log.is_open()) return;

    // Current time
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    char timeBuf[64];
    std::strftime(timeBuf, sizeof(timeBuf), "[%Y-%m-%d %H:%M:%S]", &tm);

    log << timeBuf << " " << direction << " from " << clientIP
        << " Type=" << pkt.GetType() // implement GetType() if you like
        << " Text=" << pkt.GetText() << "\n";

    log.close();
}

void LogMessage(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(g_logMutex);

    std::ofstream log("server_log.txt", std::ios::app);
    if (!log.is_open()) return;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    char timeBuf[64];
    std::strftime(timeBuf, sizeof(timeBuf), "[%Y-%m-%d %H:%M:%S]", &tm);

    log << timeBuf << " [INFO] " << msg << "\n";
    log.close();
}
