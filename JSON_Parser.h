#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include"uthash.h"
#include<stdbool.h>
#include<ctype.h>
#include"d_array.h"
#include"JSON_Parser.h"

#define INT_DFR(v) (*(int*)(v))
#define STR_DFR(v) (char*)(v)
#define ARR_DFR(v) (dArr*)(v)
#define FLT_DFR(v) (*(float*)(v))


#define STARTOBJ ((int)'{')
#define ENDOBJ ((int)'}')

#define JINT 0
#define JSTR 1
#define JFLT 2
#define JARR 3
#define JOBJ 4


typedef struct JsonDict
{
	char* key;
	char type;
	void* value;
	UT_hash_handle hh;
}JsonDict;

typedef struct JsonObj
{
	JsonDict* dictionary;
}JsonObj;




static void __processKv(char* kv, JsonObj* obj);
static void __DictDestroyObj(JsonObj* obj);
static JsonObj* __parseObj(char* data);

static void __isolateVal(char** str)
{
	if (strchr(*str, ((int)'[')) || strchr(*str, ((int)'{')))
	{
		*str = *str + 1;

	}
	char* ptr = strchr(*str, ((int)']'));
	if (!ptr)
		ptr = strchr(*str, ((int)'}'));
	if (ptr)
	{
		*ptr = '\0';
	}

}

static char* __isolateString(char* str)
{

	char* sPtr = strchr(str, ((int)'"'));
	char* ePtr = strrchr(str, ((int)'"'));
	int len = (ePtr - sPtr) - 1;
	if (len<0)
	{
		return str;
	}
	char* res = (char*)malloc(len + 1);
	memcpy(res, sPtr+1, len);
	res[len] = '\0';
	return res;
}



static int __reformatObj(char* str)
{

	int len = strlen(str);
	int valDetected = 1;
	int objectOpened = 0;
	for (size_t i = 0; i < len; i++)
	{
		if (str[i] == '{' || str[i] == '[')
		{
			objectOpened++;
		}
		if (str[i] == '}' || str[i] == ']')
		{
			objectOpened--;
		}
		if (objectOpened == 1)
		{
			if (str[i] == ',')
			{
				valDetected++;

				str[i] = '|';
			}
			if (str[i] == ':')
			{
				str[i] = '@';

			}
		}
	}
	return valDetected;


}



static JsonDict* JsonGetDictItem(JsonObj* obj,char* key)
{
	JsonDict* res;
	if (!obj->dictionary)
	{
		return NULL;
	}
	//printf("dictkey :%s\n",obj->dictionary->key);

	HASH_FIND_STR(obj->dictionary,key, res);
	return res;
}


static char* __jsonMakeOneLine(const char* content)
{
	int nObj = 0;
	int len = strlen(content);
	char* newStr = (char*)malloc(len);
	if (!newStr)
	{
		perror("unable to allocate memory");
		return NULL;
	}
	int newIndex = 0;
	for (size_t i = 0; i < len; i++)
	{
		if (content[i]=='{')
		{
			nObj++;
		}
		if (content[i]=='}')
		{
			nObj--;
		}
		
		
		if(!(content[i] == ' ' || content[i] == '\n'))
		{
			newStr[newIndex] = content[i];
			newIndex++;
		}
		
		if (nObj <= 0)
		{
			break;
		}

	}
	char* finalString = (char*)malloc(newIndex + 1);
	if (!finalString)
	{
		perror("unable to allocate memory");
		return NULL;
	}
	memcpy(finalString, newStr, newIndex);
	finalString[newIndex] = '\0';
	free(newStr);
	return finalString;
}

static void __isolateKv(char** kv)
{
	char* ptr = strrchr(*kv, ((int)'}'));
	if (ptr && !strchr(ptr,((int)']')))
	{
		*ptr = '\0';
	}
	ptr = strchr(*kv, ((int)'{'));
	if (ptr)
	{
		*kv = *kv+1;
	}
}

static void __isolateArr(char** arr)
{
	if (strchr(*arr, ((int)'[')))
	{
		*arr = *arr + 1;
		
	}
	char* ptr = strrchr(*arr, ((int)']'));
	if (ptr)
	{
		*ptr = '\0';
	}

}

static dArr* __processArray(char* arr)
{

	int numberOfElement = __reformatObj(arr);
	__isolateArr(&arr);


	dArr* ret = dArrCreate();
	

	if (numberOfElement==1) //with only one element can't use the strtok_s because it can destroy the logic
	{
		if (arr[0]=='{')
		{
			JsonObj* val = __parseObj(arr);
			dArrAppend(ret,val,JOBJ);
		}
	}
	else
	{
		char* context;
		char* token = strtok_s(arr, "|", &context);

		while (true)
		{

			if (token[0] == '{')
			{
				JsonObj* val = __parseObj(token);
				dArrAppend(ret, val,JOBJ);
			}
			else if (strchr(token, ((int)'.'))) //float
			{

				float* val = (float*)malloc(sizeof(float));
				*val = (float)atof(token);
				dArrAppend(ret, val,JFLT);
			}
			else if (strchr(token, ((int)'"'))) //string
			{
				char* val = __isolateString(token);
				dArrAppend(ret, val,JSTR);
			}
			else //int
			{
				int* val = (int*)malloc(sizeof(int));
				*val = atoi(token);
				dArrAppend(ret, val,JINT);
			}
			token = strtok_s(NULL, "|", &context);
			if (!token)
			{
				break;
			}
		}
	}
	return ret;

}

static void __processKv(char* kv, JsonObj* obj)
{

	JsonDict* toAdd = (JsonDict*)malloc(sizeof(JsonDict));

	char* context;
	char* token = strtok_s(kv, "@", &context);


	char* k = __isolateString(token);
	toAdd->key = k;


	token = strtok_s(NULL, "@", &context);
	if (token)
	{

		if (token[0] == '"')//string
		{
			char* value = __isolateString(token);
			toAdd->value = value;
			toAdd->type = JSTR;


		}
		else if (token[0] == '{') //object
		{
			printf("not implemented");
			toAdd->type = JOBJ;
		}
		else if (token[0] == '[')//array
		{
			dArr* value = __processArray(token);
			toAdd->value = value;
			toAdd->type = JARR;

		}
		else if (strchr(token, ((int)'.')))//float
		{
			float* val = (float*)malloc(sizeof(float));
			*val = (float)atof(token);
			toAdd->value = val;
			toAdd->type = JFLT;
		}
		else //int
		{
			int* val = (int*)malloc(sizeof(int));
			__isolateVal(&token);
			*val = atoi(token);
			toAdd->value = val;
			toAdd->type = JINT;

		}

		HASH_ADD_STR(obj->dictionary, key, toAdd);

	}

}

static JsonObj* __parseObj(char* data)
{

	JsonObj* obj = (JsonObj*)malloc(sizeof(JsonObj));
	obj->dictionary = NULL;
	__reformatObj(data);


	char* kvContext;
	char* kvToken = strtok_s(data, "|", &kvContext);


	while (true)
	{
		__processKv(kvToken, obj);
		kvToken = strtok_s(NULL, "|", &kvContext);
		if (!kvToken)
		{
			break;
		}

	}
	return obj;
}

static void __DictDestroyArr(JsonDict* item)
{
	dArr* arr = (dArr*)(item->value);
	int len = dArrLenght(arr)-1;
	for (size_t i = 0; i < len; i++)
	{
		if (arr->valueTypes[i]==JOBJ)
		{
			__DictDestroyObj((JsonObj*)arr->data[i]);
		}
		else if(arr->valueTypes[i] == JARR)
		{
			printf("ciao");
			__DictDestroyArr((JsonDict*)arr->data[i]);
		}
	}
	dArrDestroy(arr);
}

static void __DictDestroyObj(JsonObj* obj)
{
	JsonDict* currentObj;
	JsonDict* tmp;
	HASH_ITER(hh, obj->dictionary, currentObj, tmp)
	{
		if (currentObj->type == JARR)
		{
			HASH_DEL(obj->dictionary, currentObj);
			__DictDestroyArr(currentObj);
		}
		else
		{

			free(currentObj->key);
			free(currentObj->value);
			HASH_DEL(obj->dictionary, currentObj);

			free(currentObj);

		}
		
	}
	free(obj);
}

static void JsonDestroyObj(JsonObj* obj)
{
	__DictDestroyObj(obj);
}


static JsonObj* JsonCreateObj(const char* content)
{
	char* data = __jsonMakeOneLine(content);
	JsonObj* obj = NULL;
	if (data[0]=='{')
	{
		obj = __parseObj(data);
	}
	free(data);
	return obj;
}

static char* JsonOpen(const char* path)
{
	FILE* f;
	fopen_s(&f, path, "r");
	if (!f)
	{
		perror("file non aperto");
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	rewind(f);
	char* fileContent = (char *)malloc(size + 1);
	if (fileContent)
	{
		fread(fileContent, size, 1, f);
		fclose(f);
		return fileContent;
	}
	else
	{
		fclose(f);
		perror("impossibile leggere il contenuto");
		return NULL;
	}
}

static void JsonClose(char* content)
{
	free(content);
}


