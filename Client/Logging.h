#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <mutex>
#include "Packet.h"

#define INCOMING_PACKET    "RX"
#define OUTGOING_PACKET    "TX"

class Logger {
public:
    static Logger& getInstance();

    /// Logs a packet action (send/receive).

    void LogPacket(const std::string& direction, Packet& pkt);


    /// Logs a general client-side message
    void LogMessage(const std::string& msg);

private:
    std::mutex mtx;

    Logger();
    ~Logger();


    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string PacketTypeToString(Packet::PacketType type);
};
