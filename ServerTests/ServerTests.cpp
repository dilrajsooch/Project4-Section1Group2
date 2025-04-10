#include "pch.h"
#include "CppUnitTest.h"
#include "Packet.h"
#include "Logging.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ServerTests
{
    TEST_CLASS(LoggingTests)
    {
    public:

        //Helper function to delete the log file before each test.
        void ClearLogFile()
        {
            std::remove("server_log.txt");
        }

        //Helper function to read the content of the log file.
        std::string ReadLogFile()
        {
            std::ifstream logFile("server_log.txt");
            std::stringstream buffer;
            buffer << logFile.rdbuf();
            return buffer.str();
        }

        TEST_METHOD(TestLogMessage)
        {
            //Arrange
            ClearLogFile();
            std::string testMessage = "Unit test log message.";

            //Act
            Server::Logger::getInstance().LogMessage(testMessage);

            //Assert
            std::string content = ReadLogFile();
            //The log entry should include a timestamp, the [INFO] tag, and the test message.
            Assert::IsTrue(content.find("[INFO]") != std::string::npos, L"Log file should contain the [INFO] tag.");
            Assert::IsTrue(content.find(testMessage) != std::string::npos, L"Log file should contain the test message.");
        }

        TEST_METHOD(TestLogPacket)
        {
            //Arrange
            ClearLogFile();

            Packet pkt;
            std::string packetText = "Hello Packet";
            pkt.SetBody(packetText.c_str(), static_cast<int>(packetText.size()));
            pkt.SetType(Packet::ADD_POST);

            std::string clientIP = "192.168.1.1";

            //Act
            Server::Logger::getInstance().LogPacket(INCOMING_PACKET, clientIP, pkt);

            //Assert
            std::string content = ReadLogFile();

            Assert::IsTrue(content.find(INCOMING_PACKET) != std::string::npos, L"Log file should contain the packet direction.");
            Assert::IsTrue(content.find(clientIP) != std::string::npos, L"Log file should contain the client IP.");
            Assert::IsTrue(content.find("ADD_POST") != std::string::npos, L"Log file should indicate the packet type.");
            Assert::IsTrue(content.find(packetText) != std::string::npos, L"Log file should contain the packet text.");
        }
    };

	TEST_CLASS(ServerTests)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
		}
	};



}
