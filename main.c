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
		printf("%d,", INT_DFR(arr2->data[i]));

	}


	JsonDestroyObj(obj);

	JsonClose(&content);
	return 0;
}

