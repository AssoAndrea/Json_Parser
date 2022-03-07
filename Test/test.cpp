#include "pch.h"
#include"../../Tiled_Json_Parser/Include/JSON_Parser.h"
#include"../../Tiled_Json_Parser/Include/d_array.h"

#define RES_PATH "../../x64/Debug/res/TestFile/"

namespace Array
{

	class ArrayTest : public testing::Test
	{
	protected:
		dArr* arr;
		void SetUp() override
		{
			arr = dArrCreate();
		}

		void TearDown() override
		{
			dArrDestroy(&arr);
		}

		int* CreateInt(int value)
		{
			int* ptr = (int*)malloc(sizeof(int));
			*ptr = value;
			return ptr;
		}
	};

	TEST_F(ArrayTest, Create) {
		dArr* arr = dArrCreate();
		EXPECT_NE(arr, nullptr);
	}

	TEST_F(ArrayTest, Destroy) {
		dArr* arr = dArrCreate();
		dArrDestroy(&arr);
		EXPECT_EQ(arr, nullptr);
	}

	TEST_F(ArrayTest, AppendOneElement)
	{
		int* n = CreateInt(1);
		dArrAppend(arr,n,JINT);
		ASSERT_EQ(INT_DFR(arr->data[0]), 1);
	}

	TEST_F(ArrayTest, AppendTwoElement)
	{
		int* n1 = CreateInt(1);
		int* n2 = CreateInt(2);
		dArrAppend(arr, n1, JINT);
		dArrAppend(arr, n2, JINT);
		ASSERT_EQ(INT_DFR(arr->data[0]), 1);
		ASSERT_EQ(INT_DFR(arr->data[1]), 2);
		
	}

	TEST_F(ArrayTest, AppendOverPadding)
	{
		for (size_t i = 0; i < 20; i++)
		{
			int* n = CreateInt(i);
			dArrAppend(arr, n, 'i');

		}
		for (size_t i = 0; i < 10; i++)
		{
			ASSERT_EQ(INT_DFR(arr->data[i]),i);
		}
	}

	TEST_F(ArrayTest, RemoveAt)
	{
		for (size_t i = 0; i < 10; i++)
		{
			int* v = CreateInt(i);
			dArrAppend(arr, v, JINT);
		}
		dArrRemoveAt(arr, 5);
		for (size_t i = 0; i < 9; i++)
		{
			int eq = i;
			if (i>=5)
			{
				eq++;
			}
			ASSERT_EQ(INT_DFR(arr->data[i]), eq);
		}
	}

	TEST_F(ArrayTest, RemoveFirst)
	{
		for (size_t i = 0; i < 10; i++)
		{
			int* v = CreateInt(i);
			dArrAppend(arr, v, JINT);
		}
		dArrRemoveAt(arr, 0);
		for (size_t i = 0; i < 9; i++)
		{
			int eq = i;
			if (i >= 0)
			{
				eq++;
			}
			ASSERT_EQ(INT_DFR(arr->data[i]), eq);
		}
	}

	TEST_F(ArrayTest, RemoveLast)
	{
		for (size_t i = 0; i < 10; i++)
		{
			int* v = CreateInt(i);
			dArrAppend(arr, v, JINT);
		}
		dArrRemoveAt(arr, 9);
		for (size_t i = 0; i < 9; i++)
		{
			int eq = i;
			if (i >= 9)
			{
				eq++;
			}
			ASSERT_EQ(INT_DFR(arr->data[i]), eq);
		}
	}
}


namespace JsonParser
{
	TEST(JsonParser, OpenFile)
	{
		char* res = JsonOpen(RES_PATH"1int.json");
		EXPECT_NE(res, nullptr);
	}

	TEST(JsonParser, CloseFile)
	{
		char* res = JsonOpen(RES_PATH"1int.json");
		ASSERT_NE(res, nullptr);
		JsonClose(&res);
		EXPECT_EQ(res, nullptr);

	}
	
	TEST(JsonParser, MakeOneLineSingleVal)
	{
		char* toTest = JsonOpen(RES_PATH"OneValue.json");
		//char* expected = JsonOpen(RES_PATH"OneValueExp.txt");
		char* expected = "{\"value\":1}";
		char* res = __jsonMakeOneLine(toTest);
		EXPECT_STRCASEEQ(res, expected);
		JsonClose(&toTest);
	}

	//https://www.browserling.com/tools/remove-all-whitespace for fast remove space and breakline
	TEST(JsonParser, MakeOneLineDifferentVal)
	{
		char* toTest = JsonOpen(RES_PATH"DifferentValue.json");
		char* expected = "{\"v1\":3,\"v2\":\"ciao\",\"v3\":10.3,\"v4\":[1,2,3,4,5,70,50],\"v5\":[{\"a1\":40,\"a2\":\"pippo\",\"a3\":[12,3,5,{\"b1\":30}]}]}";
		char* res = __jsonMakeOneLine(toTest);
		EXPECT_STRCASEEQ(res, expected);
		JsonClose(&toTest);
	}
}