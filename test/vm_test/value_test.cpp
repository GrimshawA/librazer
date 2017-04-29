#include "value_test.h"
#include <stdio.h>
#include <string>
#include <RazerRuntime/RzValue.h>

void UT_Begin(std::string msg)
{
	printf("==> %s: BEGIN\n\n", msg.c_str());
}

void UT_Print(std::string message, bool pass)
{
	std::string passStr = pass ? "PASS" : "FAIL";
	printf("%s: %s\n", message.c_str(), passStr.c_str());
}

void unit_test_values()
{
	UT_Begin("AEValue tests");

	RzValue v1;

	UT_Print("AEValue default value is undefined", v1.type() == RzValue::VALUE_UNDEFINED);
	UT_Print("AEValue default str() is undefined", v1.str() == "undefined");

	v1 = 5;

	UT_Print("AEValue retains the integer value", v1.toInteger() == 5);
	UT_Print("AEValue str correctly shows the integers", v1.str() == "5");

	v1 = "tstring";
	UT_Print("AEValue retains a string value", v1.str() == "tstring");

	RzValue v2 = v1;
	UT_Print("Assign string to new value", v2.str() == v1.str());

	v2 = "newstring";
	UT_Print("Altering the value on the copy string value, affects the original", v1.str() == v2.str());

	RzValue vList = { 0, 1, 2, 3, 4 };
	UT_Print("Initializer list constructor [int] retained", vList.length() == 5);
	UT_Print("Array of ints format", vList.str() == "[0, 1, 2, 3, 4]");

	vList.setValue(0, RzValue(std::string("A")));
	UT_Print("Replacing one of the array elements with string", vList.str() == "[A, 1, 2, 3, 4]");

	RzValue nestedList = { 0, 1 };
	vList.setValue(1, nestedList);
	UT_Print("Replacing one element with a nested array", vList.str() == "[A, [0, 1], 2, 3, 4]");

	int i = 0;
	RzValue fn = [&i](){
		i = 1;
	};
	fn.call();
	UT_Print("Calling a native method through a lambda value", i == 1);
}
