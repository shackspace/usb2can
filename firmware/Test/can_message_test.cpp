#include "../CAN_Module.h"
#include <limits.h>
#include <gtest/gtest.h>


TEST(CAN_TO_ASCII_TEST, POSITIVE)
{
	can_t can_message;
	can_message.id=0x12345678;
	can_message.flags.rtr = 0;
	can_message.flags.extended = 1;
	can_message.length = 4;
	can_message.data[0] = 0xFF;
	can_message.data[1] = 0xEE;
	can_message.data[2] = 0xDD;
	can_message.data[3] = 0xCC;

	char ascii_message[ASCII_CAN_MESSAGE_LENGTH];

	can2ascii(ascii_message, &can_message);

//	std::cout << ascii_message;

	EXPECT_EQ(0, strcmp(ascii_message, "I12345678P4FFEEDDCC"));
}

TEST(PARSE_ASCII, POSITIVE)
{
	char* ascii_message = "I12345678P4FFEEDDCC";

	EXPECT_EQ(ascii_message[0], 'I');
}

TEST(ASCII_TO_CAN_TEST, POSITIVE)
{
	char* ascii_message = "I12345678P4FFEEDDCC";

	can_t can_message;
	can_message.id=0x12345678;
	can_message.flags.rtr = 0;
	can_message.flags.extended = 1;
	can_message.length = 4;
	can_message.data[0] = 0xFF;
	can_message.data[1] = 0xEE;
	can_message.data[2] = 0xDD;
	can_message.data[3] = 0xCC;

	can_t can_converted;

	EXPECT_TRUE(ascii2can(ascii_message, &can_converted));

	EXPECT_EQ(can_converted.id, can_message.id);
	EXPECT_EQ(can_converted.flags.rtr, can_message.flags.rtr);
	EXPECT_EQ(can_converted.flags.extended, can_message.flags.extended);
	EXPECT_EQ(can_converted.length, can_message.length);
	EXPECT_EQ(can_converted.data[0], can_message.data[0]);
	EXPECT_EQ(can_converted.data[1], can_message.data[1]);
	EXPECT_EQ(can_converted.data[2], can_message.data[2]);
	EXPECT_EQ(can_converted.data[3], can_message.data[3]);
}


int main(int argv, char** argc)
{
	::testing::InitGoogleTest(&argv, argc);
	return RUN_ALL_TESTS();
}
