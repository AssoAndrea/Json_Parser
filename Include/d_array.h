#pragma once
#include<stdlib.h>
#include<string.h>
#include"JSON_Parser.h"

typedef struct dArr
{
	int lastIndex;
	int size;
	int padding;
	void** data;
	char* valueTypes;
}dArr;



dArr* dArrCreate()
{
	dArr* arr = (dArr*)malloc(sizeof(dArr));
	if (arr)
	{
		arr->lastIndex = -1;
		arr->size = 3;
		arr->padding = 3;
		arr->data = (void**)malloc(arr->size * sizeof(void*));
		arr->valueTypes = (char*)malloc(100*sizeof(char));
		return arr;
	}
}

void dArrDestroy(dArr** arr)
{
	dArr* ptr = *arr;
	free(ptr->data);
	free(ptr);
	*arr = NULL;
}

int dArrLenght(dArr* arr)
{
	return arr->lastIndex + 1;
}

void dArrRemoveAt(dArr* arr, int index)
{
	for (size_t i = index; i < arr->lastIndex; i++)
	{
		arr->data[i] = arr->data[i + 1];
	}
	//free(arr->data[arr->lastIndex]);
	arr->lastIndex--;
}

void dArrAppend(dArr* arr, void* obj,char type)
{
	arr->data[arr->lastIndex + 1] = obj;
	arr->lastIndex++;
	//printf("last index: %d", arr->lastIndex);
	if (arr->size - arr->lastIndex < arr->padding)
	{
		int newSize = arr->size + arr->padding;

		/*arr->data = realloc(arr->data, sizeof(void*) * newSize);
		arr->valueTypes = realloc(arr->valueTypes, sizeof(char) * newSize);*/
		void** dtmp = (void**)malloc(sizeof(void*) * newSize);
		char* vtmp = (char*)malloc(sizeof(char*) * newSize);
		memcpy(dtmp, arr->data, sizeof(void*) * arr->size);
		memcpy(vtmp, arr->valueTypes, sizeof(char*) * arr->size);

		free(arr->data);
		free(arr->valueTypes);

		arr->data = dtmp;
		arr->valueTypes = vtmp;

		arr->size += arr->padding;



		
	}
}