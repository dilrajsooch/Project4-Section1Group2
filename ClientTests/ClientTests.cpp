#include "pch.h"
#include "CppUnitTest.h"
#include "Packet.h"
#include <cstring>
#include <User.h>
#include <Post.h>

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
			Packet::PacketType testType = Packet::ADD_POST;
			
			packet.SetType(testType);
			
			// Create a serialized packet to verify type
			char* serialized = packet.SerializeData();
			Packet deserialized(serialized);
			
			Assert::AreEqual((int)Packet::ADD_POST, (int)deserialized.Head.type);
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
			original.SetType(Packet::ADD_POST);
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
			Assert::AreEqual((int)Packet::ADD_POST, (int)header->type);
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
			Packet::PacketType testType = Packet::ADD_POST;
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
			original.SetType(Packet::ADD_POST);
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

	TEST_CLASS(PostTests)
	{
	public:
		TEST_METHOD(Constructor_TextOnly_InitializesCorrectly)
		{
			// Create a test user
			User testUser(0);
			
			// Create a post with just text
			Post post(1, "Test post content", testUser.GetId());
			
			// Verify the post was initialized correctly
			Assert::AreEqual(1, post.GetRoomNumber());
			Assert::AreEqual("Test post content", post.GetText().c_str());
			Assert::AreEqual(0, post.GetUserID());
		}

		TEST_METHOD(Constructor_WithImage_InitializesCorrectly)
		{
			// Create a test user
			User testUser(0);
			
			// Create a test image
			Image testImage = GenImageColor(100, 100, RED);
			
			// Create a post with text and image
			Post post(2, "Test post with image", testUser.GetId());
			
			// Verify the post was initialized correctly
			Assert::AreEqual(2, post.GetRoomNumber());
			Assert::AreEqual("Test post with image", post.GetText().c_str());
			Assert::AreEqual(0, post.GetUserID());
			
			// Verify image was set (check if it's not a null image)
			Image retrievedImage = post.GetImage();
			Assert::IsTrue(retrievedImage.data != nullptr);
		}

		TEST_METHOD(GetRoomNumber_ReturnsCorrectValue)
		{
			User testUser(0);
			Post post(42, "Test post", testUser.GetId());
			
			Assert::AreEqual(42, post.GetRoomNumber());
		}

		TEST_METHOD(GetText_ReturnsCorrectValue)
		{
			User testUser(0);
			string testText = "This is a test post with special characters: !@#$%^&*()";
			Post post(1, testText, testUser.GetId());
			
			Assert::AreEqual(testText.c_str(), post.GetText().c_str());
		}

		TEST_METHOD(GetAuthor_ReturnsCorrectUser)
		{
			User testUser(0);
			Post post(1, "Test post", testUser.GetId());
			
			Assert::AreEqual(0, post.GetUserID());
		}
	};
}
