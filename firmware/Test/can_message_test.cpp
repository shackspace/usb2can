#include "../CAN_Module.h"
#include <limits.h>
#include <gtest/gtest.h>



TEST(STRING_SUBSTRING, POSITIVE)
{
	char* test_string = (char*)"I12345678T4FFEEDDCC";
	char substring[MAX_BUFFER_LENGTH];

	get_substring(test_string, substring, 1, 5);

	EXPECT_EQ(0, strcmp(substring, "1234"));
}

TEST(STRING_ASCII_MESSAGE_EXISTS, POSITIVE)
{
	char* test_string = (char*)"I12345678T4FFEEDDCC\n";
	
	EXPECT_EQ(strlen(test_string)-1, ascii_message_exists(test_string));
	
}

TEST(STRING_EXTRACT_ASCII_MESSAGE_IF_EXISTS, POSITIVE)
{
	char* test_string = (char*)"I12345678T4FFEEDDCC\n5443455";
	char buffer[MAX_BUFFER_LENGTH];
	
	int end_of_ascii_string = ascii_message_exists(test_string);
	
	EXPECT_EQ(end_of_ascii_string, 19);

	get_substring(test_string, buffer, 0, end_of_ascii_string);
	
	EXPECT_EQ(0, strcmp("I12345678T4FFEEDDCC", buffer));
}

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

	EXPECT_EQ(0, strcmp(ascii_message, "I12345678T4FFEEDDCC"));
}

TEST(PARSE_ASCII, POSITIVE)
{
	char* ascii_message = (char*)"I12345678T4FFEEDDCC";

	EXPECT_EQ(ascii_message[0], 'I');
}

TEST(ASCII_TO_CAN_TEST, POSITIVE)
{
	char* ascii_message = (char*)"I12345678T4FFEEDDCC";

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

TEST(STRING_EXTRACT_ASCII_MESSAGE_AND_CONVERT_TO_CAN, POSITIVE)
{
	char* test_string = (char*)"I12345678T4FFEEDDCC\n5443455";
	char buffer[MAX_BUFFER_LENGTH];
	
	int end_of_ascii_string = ascii_message_exists(test_string);
	
	EXPECT_EQ(end_of_ascii_string, 19);

	get_substring(test_string, buffer, 0, end_of_ascii_string);
	
	EXPECT_EQ(0, strcmp("I12345678T4FFEEDDCC", buffer));

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

	EXPECT_TRUE(ascii2can(buffer, &can_converted));

	EXPECT_EQ(can_converted.id, can_message.id);
	EXPECT_EQ(can_converted.flags.rtr, can_message.flags.rtr);
	EXPECT_EQ(can_converted.flags.extended, can_message.flags.extended);
	EXPECT_EQ(can_converted.length, can_message.length);
	EXPECT_EQ(can_converted.data[0], can_message.data[0]);
	EXPECT_EQ(can_converted.data[1], can_message.data[1]);
	EXPECT_EQ(can_converted.data[2], can_message.data[2]);
	EXPECT_EQ(can_converted.data[3], can_message.data[3]);
}

TEST(STRING_EXTRACT_TWO_ASCII_MESSAGES_AND_CONVERT_TO_CAN, POSITIVE)
{
	char test_string[MAX_BUFFER_LENGTH];

	memcpy(test_string,(char*)"I12345678T4FFEEDDCC\nI12345678T4FFEEDDCC\n454545", strlen("I12345678T4FFEEDDCC\nI12345678T4FFEEDDCC\n454545") + 1);
	char buffer[MAX_BUFFER_LENGTH];
	
	int end_of_ascii_string = ascii_message_exists(test_string);
	
	EXPECT_EQ(end_of_ascii_string, 19);

	get_substring(test_string, buffer, 0, end_of_ascii_string);
	
	EXPECT_EQ(0, strcmp("I12345678T4FFEEDDCC", buffer));

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

	EXPECT_TRUE(ascii2can(buffer, &can_converted));

	EXPECT_EQ(can_converted.id, can_message.id);
	EXPECT_EQ(can_converted.flags.rtr, can_message.flags.rtr);
	EXPECT_EQ(can_converted.flags.extended, can_message.flags.extended);
	EXPECT_EQ(can_converted.length, can_message.length);
	EXPECT_EQ(can_converted.data[0], can_message.data[0]);
	EXPECT_EQ(can_converted.data[1], can_message.data[1]);
	EXPECT_EQ(can_converted.data[2], can_message.data[2]);
	EXPECT_EQ(can_converted.data[3], can_message.data[3]);

	EXPECT_EQ(46, strlen(test_string));

	//std::cout << test_string << std::endl;

	get_substring(test_string, buffer, end_of_ascii_string + 1, strlen(test_string));

	//std::cout << buffer << std::endl;

	EXPECT_EQ(0, strcmp("I12345678T4FFEEDDCC\n454545", buffer));

	memcpy(test_string, buffer, strlen(buffer)+1);
	
	//std::cout << test_string << std::endl;

	EXPECT_EQ(0, strcmp("I12345678T4FFEEDDCC\n454545", test_string));

	end_of_ascii_string = ascii_message_exists(test_string);
	
	EXPECT_EQ(end_of_ascii_string, 19);

	get_substring(test_string, buffer, 0, end_of_ascii_string);
	
	EXPECT_EQ(0, strcmp("I12345678T4FFEEDDCC", buffer));

	EXPECT_TRUE(ascii2can(buffer, &can_converted));

	EXPECT_EQ(can_converted.id, can_message.id);
	EXPECT_EQ(can_converted.flags.rtr, can_message.flags.rtr);
	EXPECT_EQ(can_converted.flags.extended, can_message.flags.extended);
	EXPECT_EQ(can_converted.length, can_message.length);
	EXPECT_EQ(can_converted.data[0], can_message.data[0]);
	EXPECT_EQ(can_converted.data[1], can_message.data[1]);
	EXPECT_EQ(can_converted.data[2], can_message.data[2]);
	EXPECT_EQ(can_converted.data[3], can_message.data[3]);

	get_substring(test_string, buffer, end_of_ascii_string + 1, strlen(test_string));

	EXPECT_EQ(0, strcmp("454545", buffer));

	memcpy(test_string, buffer, strlen(buffer)+1);

	EXPECT_EQ(0, strcmp("454545", test_string));

	end_of_ascii_string = ascii_message_exists(test_string);
	
	EXPECT_EQ(end_of_ascii_string, -1);

	
}



int main(int argv, char** argc)
{
	::testing::InitGoogleTest(&argv, argc);
	return RUN_ALL_TESTS();
}
