#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include"JSON_Parser.h"
#include"d_array.h"
#include"uthash.h"
#include<stdlib.h>

#define RES_PATH "../x64/Debug/res/"


int* CreateInt(int value)
{
	int* ptr = (int*)malloc(sizeof(int));
	*ptr = value;
	return ptr;
}

int main()
{
	dArr* arr = dArrCreate();
	/*int* n1 = (int*)malloc(sizeof(int));
	int* n2 = (int*)malloc(sizeof(int));
	int* n3 = (int*)malloc(sizeof(int));
	*n1 = 1;
	*n2 = 2;
	*n3 = 3;
	dArrAppend(arr, n1, JINT);
	dArrAppend(arr, n2, JINT);
	dArrAppend(arr, n3, JINT);
	dArrRemoveAt(arr, 2);*/

	for (size_t i = 0; i < 10; i++)
	{
		int* v = CreateInt(i);
		dArrAppend(arr, v, JINT);
	}
	dArrRemoveAt(arr, 5);
	for (size_t i = 0; i < 9; i++)
	{
		printf("%d = %d\n", i, INT_DFR(arr->data[i]));
	}
	

	
	char* content = JsonOpen("../x64/Debug/res/mappa.json");
	JsonObj* obj = JsonCreateObj(content);

	JsonDict* d = JsonGetDictItem(obj, "height");
	printf("%d\n", INT_DFR(d->value));
	
	JsonDict* data = JsonGetDictItem(obj, "layers");
	dArr* dataArr = (dArr*)(data->value);

	JsonObj* arrObj = (JsonObj*)(dataArr->data[0]);
	

	JsonDict* onArr = JsonGetDictItem(arrObj, "data");

	dArr* arr2 = ARR_DFR(onArr->value);
	for (size_t i = 0; i < 40; i++)
	{
		printf("%d", INT_DFR(arr2->data[i]));

	}


	JsonDestroyObj(obj);

	JsonClose(&content);
	return 0;
}

