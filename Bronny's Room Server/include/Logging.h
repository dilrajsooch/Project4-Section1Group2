#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <mutex>
#include "Packet.h"
#include "GlobalDataModel.h"

#define INCOMING_PACKET    "RX"
#define OUTGOING_PACKET    "TX"

namespace Server {
    class Logger {
    public:
        static Logger& getInstance();

        void LogPacket(const std::string& direction, const std::string& clientIP, Packet& pkt);

        void LogMessage(const std::string& msg);

    private:
        std::mutex mtx;


        Logger();
        ~Logger();

        // Deleted copy constructor & assignment operator.
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        std::string PacketTypeToString(Packet::PacketType type);
    };
};