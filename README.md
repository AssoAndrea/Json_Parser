# JSON_Parser
a 3-header library for parse JSON file and interrogate it


## Usage
**Copy on the project folder** 
- d_array.h
- J SON_Parser.h
- uthash.h

**Open JSON file**<br>
JsonOpen(char* JsonPath);

**Create a JSON object**<br>
JsonCreateObj(char* rawJsonData);

**Get item from object field name**<br>
JsonGetDictItem(JsonObj* obj,char* fieldName);

**JsonDict field**<br>
- **hh** and **key** don't touch it
- **type** contains the data type, it can be:
    - JINT  for integer
    - JARR for dynamic array (dArr)
    - JSTR for string
    - JFLT for float
- **value** contains the pointer to data




 
