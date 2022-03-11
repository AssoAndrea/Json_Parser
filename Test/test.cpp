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


namespace TestParser
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
		char* expected = "{\"value\":1}";
		char* res = __jsonMakeOneLine(toTest);
		EXPECT_STRCASEEQ(res, expected);
		JsonClose(&toTest);
	}

	//https://www.browserling.com/tools/remove-all-whitespace for fast remove space and breakline
	TEST(JsonParser, MakeOneLineDifferentVal)
	{
		char* toTest = JsonOpen(RES_PATH"DifferentValue.json");
		char* expected = "{\"v1\":3,\"v2\":\"ciao\",\"v3\":10.3,\"v4\":[1,2,3,4,5,70,50],\"v5\":[{\"a1\":40,\"a2\":\"pippo\",\"a3\":[12,3,5,{\"b1\":30}]},{\"b1\":10,\"b2\":7.5}]}";
		char* res = __jsonMakeOneLine(toTest);
		EXPECT_STRCASEEQ(res, expected);
		JsonClose(&toTest);
	}

	TEST(JsonParser, ReformatObj)
	{
		char* expectedString = "{\"v1\"@3|\"v2\"@\"ciao\"|\"v3\"@10.3|\"v4\"@[1,2,3,4,5,70,50]|\"v5\"@[{\"a1\":40,\"a2\":\"pippo\",\"a3\":[12,3,5,{\"b1\":30}]}]}";
		char* stringToParse = "{\"v1\":3,\"v2\":\"ciao\",\"v3\":10.3,\"v4\":[1,2,3,4,5,70,50],\"v5\":[{\"a1\":40,\"a2\":\"pippo\",\"a3\":[12,3,5,{\"b1\":30}]}]}";
		char* stringToParseMem = (char*)malloc(strlen(expectedString)+1);
		strcpy(stringToParseMem, stringToParse);
		__reformatObj(stringToParseMem);
		EXPECT_STRCASEEQ(stringToParseMem, expectedString);
		free(stringToParseMem);
	}

	TEST(JsonParser, IsolateString)
	{
		char* toTest = "\"test\"";
		char* res = __isolateString(toTest);
		char* expected = "test";
		EXPECT_STRCASEEQ(res, expected);
		toTest = "\"\"test2\"\"";
		res = __isolateString(toTest);
		expected = "\"test2\"";
		EXPECT_STRCASEEQ(res, expected);

	}

	TEST(JsonParser, IsolateArray)
	{
		char* string = "[1,4,6,7,10]";
		char* strmem = (char*)malloc(strlen(string)+1);
		strcpy(strmem, string);
		char* expected = "1,4,6,7,10";
		__isolateArr(&strmem);
		EXPECT_STRCASEEQ(strmem, expected);
		free(strmem - 1); //for don't malloc new string isolate array add 1 to string pointer, normally free is not required but for test i have to subtract 1 for free memory
	}

	TEST(JsonParser, IsolateVal)
	{
		char* string = "[1";
		char* strmem = (char*)malloc(strlen(string) + 1);
		strcpy(strmem, string);
		char* expected = "1";
		__isolateVal(&strmem);
		EXPECT_STRCASEEQ(strmem, expected);
		free(strmem - 1); //same of isolateArray

		char* string2 = "10]";
		char* strmem2 = (char*)malloc(strlen(string2) + 1);
		strcpy(strmem2, string2);
		char* expected2 = "10";
		__isolateVal(&strmem2);
		EXPECT_STRCASEEQ(strmem2, expected2);

		char* string3 = "ciao}";
		char* strmem3 = (char*)malloc(strlen(string3) + 1);
		strcpy(strmem3, string3);
		char* expected3 = "ciao";
		__isolateVal(&strmem3);
		EXPECT_STRCASEEQ(strmem3, expected3);

		char* string4 = "{123";
		char* strmem4 = (char*)malloc(strlen(string4) + 1);
		strcpy(strmem4, string4);
		char* expected4 = "123";
		__isolateVal(&strmem4);
		EXPECT_STRCASEEQ(strmem4, expected4);
	}

	TEST(JsonParser, ProcessKv)
	{
		char* t1 = "{\"test\"@10";
		char* tptr = (char*)malloc(strlen(t1));
		strcpy(tptr, t1);
		JsonObj* obj = (JsonObj*)malloc(sizeof(JsonObj));
		obj->dictionary = NULL;

		char* expectedKey = "test";
		int expectedValue = 10;
		__processKv(tptr, obj);
		JsonDict* item;
		HASH_FIND_STR(obj->dictionary, "test", item);
		EXPECT_STRCASEEQ(item->key, expectedKey);
		EXPECT_EQ(INT_DFR(item->value), expectedValue);


		char* t2 = "{\"test\"@[1,2,3,4,5]";
		char* tptr2 = (char*)malloc(strlen(t2));
		strcpy(tptr2, t2);
		JsonObj* obj2 = (JsonObj*)malloc(sizeof(JsonObj));
		obj2->dictionary = NULL;

		char* expectedKey2 = "test";
		__processKv(tptr2, obj2);
		JsonDict* item2;
		HASH_FIND_STR(obj2->dictionary, "test", item2);
		dArr* arr = (dArr*)(item2->value);
		EXPECT_STRCASEEQ(item2->key, expectedKey2);
		EXPECT_EQ(INT_DFR(arr->data[0]), 1);
		EXPECT_EQ(INT_DFR(arr->data[1]), 2);
		EXPECT_EQ(INT_DFR(arr->data[2]), 3);
		EXPECT_EQ(INT_DFR(arr->data[3]), 4);
		EXPECT_EQ(INT_DFR(arr->data[4]),5);
		

		JsonDestroyObj(obj);
		JsonDestroyObj(obj2);
	}

	TEST(JsonParser, ParseObj)
	{
		char* str = "{\"v1\":3,\"v2\":\"ciao\",\"v3\":10.3,\"v4\":[1,2,3,4,5,70,50],\"v5\":[{\"a1\":40,\"a2\":\"pippo\",\"a3\":[12,3,5,{\"b1\":30}]},{\"b1\":10,\"b2\":7.5}]}";
		char* data = (char*)malloc(strlen(str)+1);
		strcpy(data, str);
		JsonObj* obj = __parseObj(data);
		JsonDict* v1 = JsonGetDictItem(obj, "v1");
		EXPECT_EQ(INT_DFR(v1->value), 3);
		EXPECT_EQ(v1->type, JINT);

		JsonDict* v2 = JsonGetDictItem(obj, "v2");
		EXPECT_STRCASEEQ(STR_DFR(v2->value), "ciao");
		EXPECT_EQ(v2->type, JSTR);


		JsonDict* v3 = JsonGetDictItem(obj, "v3");
		EXPECT_FLOAT_EQ(FLT_DFR(v3->value), 10.3);
		EXPECT_EQ(v3->type, JFLT);


		JsonDict* v4 = JsonGetDictItem(obj, "v4");
		EXPECT_EQ(v4->type, JARR);

		dArr* arr = (dArr*)(v4->value);

		JsonDict* v5 = JsonGetDictItem(obj, "v5"); //v5 have a object array
		EXPECT_EQ(v5->type, JARR);
		dArr* v5arr = (dArr*)(v5->value);
		JsonObj* v5obj1 = (JsonObj*)(v5arr->data[0]);
		JsonDict* a1 = JsonGetDictItem(v5obj1, "a1");
		EXPECT_EQ(a1->type, JINT);
		EXPECT_EQ(INT_DFR(a1->value), 40);

		JsonObj* v5obj2 = (JsonObj*)(v5arr->data[1]);
		JsonDict* b2 = JsonGetDictItem(v5obj2, "b2");
		EXPECT_EQ(b2->type, JFLT);
		EXPECT_FLOAT_EQ(FLT_DFR(b2->value), 7.5);

		free(data);


	}
}