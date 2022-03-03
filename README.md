# JSON_Parser
a 3-header library for parse JSON file and interrogate it


## Usage
**Copy on the project folder** 
- d_array.h
- J SON_Parser.h
- uthash.h

**Open JSON file**<br>
JsonOpen(char* JsonPath);

**Close JSON file**<br>
JsonClose(char* JsonPath);

**Create a JSON object**<br>
JsonObj* obj = JsonCreateObj(char* rawJsonData);

**Get item from object field name**<br>
JsonDict* item = JsonGetDictItem(JsonObj* obj,char* fieldName);

**JsonDict field**<br>
- **hh** and **key** don't touch it
- **type** contains the data type, it can be:
    - JINT  for integer
    - JARR for dynamic array (dArr)
    - JSTR for string
    - JFLT for float
- **value** contains the pointer to data

**Macro for obtain value**<br>
you can use:
- **INT_DFR**(v) for dereference an **integer** obtained by a JsonDict
- **STR_DFR**(v) for dereference a **string** obtained by a JsonDict
- **FLT_DFR**(v) for dereference a **float** obtained by a JsonDict
- **ARR_DFR**(v) for dereference a **dArr** obtained by a JsonDict




 
