#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include"JSON_Parser.h"
#include"d_array.h"
#include"uthash.h"

#define RES_PATH "../x64/Debug/res/"


struct mySt
{
	char name[10];             /* key (string is WITHIN the structure) */
	int id;
	UT_hash_handle hh;
};

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
	printf("%d", INT_DFR(arr2->data[2]));


	JsonDestroyObj(obj);

	JsonClose(content);
	return 0;
}

