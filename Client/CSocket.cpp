#include "CSocket.h"
#include "Logging.h"


CSocket* CSocket::_instance = nullptr;



Packet CSocket::SendPacket(Packet packet)
{
    Logger::getInstance().LogPacket(OUTGOING_PACKET, packet);
    ...
    return RecievePacket();
}

Packet CSocket::RecievePacket()
{
    ...
    Packet newPacket(buffer);
    Logger::getInstance().LogPacket(INCOMING_PACKET, newPacket);
    return newPacket;
}
