#include "include/Logging.h"
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() { }

Logger::~Logger() { }

std::string Logger::PacketTypeToString(Packet::PacketType type) {
    switch (type) {
    case Packet::GET_ROOMS:     return "GET_ROOMS";
    case Packet::ADD_ROOM:      return "ADD_ROOM";
    case Packet::ADD_POST:      return "ADD_POST";
    case Packet::DELETE_POST:   return "DELETE_POST";
    case Packet::GET_POST:      return "GET_POST";
    case Packet::AUTH_REQUEST:  return "AUTH_REQUEST";
    case Packet::AUTH_RESPONSE: return "AUTH_RESPONSE";
    default:                    return "UNKNOWN";
    }
}

void Logger::LogPacket(const std::string& direction, const std::string& clientIP, const Packet& pkt) {
    std::lock_guard<std::mutex> lock(mtx);

    // Open the log file in append mode
    std::ofstream log("server_log.txt", std::ios::app);
    if (!log.is_open())
        return;

    // Get current time and format it.
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    char timeBuf[64];
    std::strftime(timeBuf, sizeof(timeBuf), "[%Y-%m-%d %H:%M:%S]", &tm);

    // Convert the packet type to a string.
    std::string typeStr = PacketTypeToString(pkt.GetType());

    // Get packet text (make sure GetText() is not NULL).
    const char* text = pkt.GetText();
    if (text == nullptr)
        text = "";

    // Write a log entry.
    log << timeBuf << " " << direction << " from " << clientIP
        << " Type=" << typeStr
        << " Text=" << text << "\n";
}

void Logger::LogMessage(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mtx);

    std::ofstream log("server_log.txt", std::ios::app);
    if (!log.is_open())
        return;

    // Get current time and format it.
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    char timeBuf[64];
    std::strftime(timeBuf, sizeof(timeBuf), "[%Y-%m-%d %H:%M:%S]", &tm);

    log << timeBuf << " [INFO] " << msg << "\n";
}
