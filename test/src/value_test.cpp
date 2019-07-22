#include "value_test.h"
#include <stdio.h>
#include <string>
#include <razer/runtime/RzValue.h>

#include <gtest/gtest.h>

TEST(ValueTest, testOne)
{
	RzValue v1;

    ASSERT_EQ(v1.type(), RzValue::VALUE_UNDEFINED);
    ASSERT_EQ(v1.str(), "undefined");

	v1 = 5;

    ASSERT_EQ(v1.toInteger(), 5);
    ASSERT_EQ(v1.str(), "5");

	v1 = "tstring";
    ASSERT_EQ(v1.str(), "tstring");

	RzValue v2 = v1;
    ASSERT_EQ(v2.str(), v1.str());

	v2 = "newstring";
    ASSERT_EQ(v1.str(), v2.str());

	RzValue vList = { 0, 1, 2, 3, 4 };
    ASSERT_EQ(vList.length(), 5);
    ASSERT_EQ(vList.str(), "[0, 1, 2, 3, 4]");

	vList.setValue(0, RzValue(std::string("A")));
    ASSERT_EQ(vList.str(), "[A, 1, 2, 3, 4]");

	RzValue nestedList = { 0, 1 };
	vList.setValue(1, nestedList);
    ASSERT_EQ(vList.str(), "[A, [0, 1], 2, 3, 4]");
}
