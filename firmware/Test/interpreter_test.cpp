#include "../Interpreter.h"

#include <limits.h>
#include <gtest/gtest.h>

TEST(GET_ONE_LINE, POSITIVE)
{
	char* test = (char*)"Test\n";
	char test_string[MAX_BUFFER_LENGTH];
	char buffer[MAX_BUFFER_LENGTH];

	memcpy(test_string,(char*)test, strlen(test) + 1);

	get_line(test_string, buffer);

	EXPECT_EQ(0, strcmp(buffer, "Test"));
}

TEST(GET_LINE, POSITIVE)
{
	char* test = (char*)"Test1\nTest2\nTest3\n";
	char test_string[MAX_BUFFER_LENGTH];
	char buffer[MAX_BUFFER_LENGTH];

	memcpy(test_string,(char*)test, strlen(test) + 1);

	get_line(test_string, buffer);

	EXPECT_EQ(0, strcmp(buffer, "Test1"));

	get_line(test_string, buffer);

    EXPECT_EQ(0, strcmp(buffer, "Test2"));

    get_line(test_string, buffer);

    EXPECT_EQ(0, strcmp(buffer, "Test3"));
}


