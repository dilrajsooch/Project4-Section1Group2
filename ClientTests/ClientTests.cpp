#include "pch.h"
#include "CppUnitTest.h"
#include "../Client/Packet.h"
#include <cstring>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ClientTests
{
	TEST_CLASS(PacketTests)
	{
	public:
		TEST_METHOD(DefaultConstructor_InitializesCorrectly)
		{
			Packet packet;
			
			// Verify header initialization
			Assert::AreEqual(nullptr, packet.GetText()); // Text should be empty
			Assert::AreEqual(0, (int)(packet.GetSize() - sizeof(Packet::Header))); // Only header size
		}

		TEST_METHOD(SetRoomNumber_UpdatesCorrectly)
		{
			Packet packet;
			int testRoom = 42;
			
			packet.SetRoomNumber(testRoom);
			
			// Create a serialized packet to verify room number
			char* serialized = packet.SerializeData();
			Packet deserialized(serialized);
			
			Assert::AreEqual(42, deserialized.Head.roomNumber);
			Assert::AreEqual(0, (int)(deserialized.GetSize() - sizeof(Packet::Header)));
		}

		TEST_METHOD(SetType_UpdatesCorrectly)
		{
			Packet packet;
			PacketType testType = IMAGE_POST;
			
			packet.SetType(testType);
			
			// Create a serialized packet to verify type
			char* serialized = packet.SerializeData();
			Packet deserialized(serialized);
			
			Assert::AreEqual(1, (int)deserialized.Head.type);
			Assert::AreEqual(0, (int)(deserialized.GetSize() - sizeof(Packet::Header)));
		}

		TEST_METHOD(SetBody_TextOnly_UpdatesCorrectly)
		{
			Packet packet;
			const char* testText = "Hello World";
			int textSize = strlen(testText);
			
			packet.SetBody((char*)testText, textSize);
			
			// Verify text content
			Assert::AreEqual(0, strcmp(testText, packet.GetText()));
			Assert::AreEqual(textSize + 1, (int)(packet.GetSize() - sizeof(Packet::Header)));
		}

		TEST_METHOD(SerializeDeserialize_TextPacket_WorksCorrectly)
		{
			Packet original;
			const char* testText = "Test Message";
			int textSize = strlen(testText);
			
			original.SetBody((char*)testText, textSize);
			original.SetType(TEXT_POST);
			original.SetRoomNumber(1);
			
			// Serialize and deserialize
			char* serialized = original.SerializeData();
			Packet deserialized(serialized);
			
			// Verify all fields
			Assert::AreEqual(0, strcmp(testText, deserialized.GetText()));
			Assert::AreEqual(original.GetSize(), deserialized.GetSize());
		}

		TEST_METHOD(GetSize_CalculatesCorrectly)
		{
			Packet packet;
			const char* testText = "Size Test";
			int textSize = strlen(testText);
			
			packet.SetBody((char*)testText, textSize);
			
			int expectedSize = sizeof(Packet::Header) + textSize + 1;
			Assert::AreEqual(expectedSize, packet.GetSize());
		}

		TEST_METHOD(Serialize_EmptyPacket_ContainsOnlyHeader)
		{
			Packet packet;
			char* serialized = packet.SerializeData();
			
			// Verify header is copied correctly
			Packet::Header* header = (Packet::Header*)serialized;
			Assert::AreEqual(0, (int)header->postTextSize);
			Assert::AreEqual(0, header->imageSize);
			Assert::AreEqual(0, header->roomNumber);
			Assert::AreEqual((int)TEXT_POST, (int)header->type);
		}

		TEST_METHOD(Serialize_TextPacket_ContainsCorrectData)
		{
			Packet packet;
			const char* testText = "Hello World";
			int textSize = strlen(testText);
			
			packet.SetBody((char*)testText, textSize);
			char* serialized = packet.SerializeData();
			
			// Verify header
			Packet::Header* header = (Packet::Header*)serialized;
			Assert::AreEqual(textSize + 1, (int)header->postTextSize);
			
			// Verify text content
			char* textData = serialized + sizeof(Packet::Header);
			Assert::AreEqual(0, strcmp(testText, textData));
		}

		TEST_METHOD(Serialize_WithRoomNumber_HeaderContainsCorrectValue)
		{
			Packet packet;
			int testRoom = 42;
			packet.SetRoomNumber(testRoom);
			
			char* serialized = packet.SerializeData();
			Packet::Header* header = (Packet::Header*)serialized;
			
			Assert::AreEqual(testRoom, header->roomNumber);
		}

		TEST_METHOD(Serialize_WithType_HeaderContainsCorrectValue)
		{
			Packet packet;
			PacketType testType = IMAGE_POST;
			packet.SetType(testType);
			
			char* serialized = packet.SerializeData();
			Packet::Header* header = (Packet::Header*)serialized;
			
			Assert::AreEqual((int)testType, (int)header->type);
		}

		TEST_METHOD(Serialize_ThenDeserialize_ProducesIdenticalPacket)
		{
			Packet original;
			const char* testText = "Test Message";
			int textSize = strlen(testText);

			original.SetBody((char*)testText, textSize);
			original.SetType(COMMAND);
			original.SetRoomNumber(5);

			// Serialize and deserialize
			char* serialized = original.SerializeData();
			Packet deserialized(serialized);

			// Verify all fields match
			Assert::AreEqual(0, strcmp(original.GetText(), deserialized.GetText()));
			Assert::AreEqual(original.GetSize(), deserialized.GetSize());

			// Verify header fields
			char* deserializedSerialized = deserialized.SerializeData();
			Packet::Header* originalHeader = (Packet::Header*)serialized;
			Packet::Header* deserializedHeader = (Packet::Header*)deserializedSerialized;

			Assert::AreEqual(originalHeader->postTextSize, deserializedHeader->postTextSize);
			Assert::AreEqual(originalHeader->imageSize, deserializedHeader->imageSize);
			Assert::AreEqual(originalHeader->roomNumber, deserializedHeader->roomNumber);
			Assert::AreEqual((int)originalHeader->type, (int)deserializedHeader->type);
		}
	};
}
