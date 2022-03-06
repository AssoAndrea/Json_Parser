#include "pch.h"
#include"../../Tiled_Json_Parser/Include/JSON_Parser.h"
#include"../../Tiled_Json_Parser/Include/d_array.h"

namespace
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
		for (size_t i = 0; i < 10; i++)
		{
			int* n = CreateInt(i);
			dArrAppend(arr, n, 'i');

		}
		for (size_t i = 0; i < 10; i++)
		{
			ASSERT_EQ(INT_DFR(arr->data[i]),i);
		}
	}
}
