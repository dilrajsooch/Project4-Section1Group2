#pragma once
#include <string>
#include "Packet.h"

void LogPacket(const std::string& direction, const std::string& clientIP, const Packet& pkt);
void LogMessage(const std::string& msg);
