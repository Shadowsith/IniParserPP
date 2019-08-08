[![Build status](https://ci.appveyor.com/api/projects/status/v7y5q5a0q45w1ib7?svg=true)](https://ci.appveyor.com/project/Shadowsith/iniparserpp)
# IniParser
A small, modern C++11 header only library for read, write and modify .INI 
configuration files with help of std::map

----
## Content
1. [Motiviaton](#Motiviaton)
2. [Documentation](#documentation)
    1. [Namespace](#namespace)
    2. [Typedefs](#typedefs)
    3. [IniParser](#class)
       1. [Constructors](#constructors)
       2. [Destructor](#desturctor)
       3. [Private Member](#privatemember)
       4. [Private Methods](#privatemethods)
       5. [Public Methods](#publicmethods)
3. [Examples](#examples)
    1. [Read and Write](#readwrite)
    2. [Create](#create)
5. [Unit Tests](#unit)
4. [License](#licence)
---
## Motiviaton
This project was highly inspired from the C#
[ini-parser](https://github.com/rickyah/ini-parser) by rickyah.


---
## Documentation
---

### Namespace
**ini**

### Typedefs
| Type | Description |
|------|-------------|
| inimap | Typedef for std::map\<std::string, std::map\<std::string, std::string\>\> |

<a href="#class"></a>
### Class IniParser
#### Constructors
| Signature | Description |
|-----------|-------------|
| IniParser() | Creates new instance of IniParser |
| IniParser(std::string file\_path) | Creates new instance and reads file  content |
| IniParser(std::string file, inimap& data) | Creates new instance, reads file and parse data to inimap by reference |

#### Destructor 
| Name | Description 
| ~IniParser() | clears all private container

<a href="#privatemember"></a>
#### Private Member
| Signature | Type | Description |
|-----------|------|-------------|
| \_lines | std::vector\<std::string\> | Stores all lines by reading file/string data |
| \_values | ini::inimap | Stores \_line values to std::map during parsing |
| \_keyValueDelim | std::string | Stores default key value delimiter of ini file (default "=") |
| \_lineSeparator | std::string | Stores default separator for ini data, default is newline ("\n") |

<a href="#privatemethods"></a>
#### Private Methods
| Signature | Return Value | Description |
|-----------|--------------|-------------|
| split(std::string delimiter) | std::vector\<std::string\> | Splits a string into parts |

<a href="#publicmethods"></a>
#### Public Methods
| Signature | Return Value | Description |
|-----------|--------------|-------------|
| parse() | ini::inimap | Parses data which was red before
| parseFile(std::string file\_path) | ini::inimap | read file if exists and parses data |
| parseData(std::string data) | ini::inimap | read string data and parse |
| readFile(std::string file\_path) | bool | reads file and store values internal, if successful returns true |
| readData(std::string data) | bool | reads string and store values internal, if successful returns true |
| writeFile(std::string file\_path) | bool | writes data into file 
| writeData(std::string file\_path) | bool | writes data into std::string


## Examples
<a href="#readwrite"></a>
### Read and write INI file
```ini
# comment
path=/home/test/config.txt
; comment
[Test]
value=120
exe=/home/test/bin
```

```cpp
#include "iniparser.h"
using IniParser = ini::IniParser;
// ...

// first example (savest)
IniParser ipa;
if(ini.readFile("test.ini")) {
    // only if file is successful red
    // parse value to std::map
    auto ini_data = ipa.parse(); 

    std::string val = ini_data["Test"]["value"] // "120"
    ini_data["Test"]["value"] = "90" // set value
    ini_data
    // add new value
    ini_data["New"]["counter"] = 20;

    //remove value from existing
    ini_data["Test"].erase("value");

    if(ipa.writeFile("test.ini", ini_data)) {
        // do something if writing was successful ...
    }
}

// second example (less code)
IniParser ipa("test.ini"); // be sure that file exists
auto ini_data = ipa.parse(); 
// etc.

// third example (fast written but unsafe)
ini::inimap ini_data;
// be sure that file exist and data is in INI format
IniParser("test.ini", data);
std::string val = ini_data["Test"]["value"] // "120"
// etc.

// ...
```

Output:
```ini
path=/home/test/config.txt

[New]
counter=20

[Test]
exe=/home/test/bin
```
<a href="#create"></a>
### Create new ini file
```cpp
#include "iniparser.h"
using IniParser = ini::IniParser;
// ...

IniParser ipa;
ini::inimap ini_data;

ini_data[""]["example"] = "hello";
ini_data["Settings"]["path"] = "/home/usr/dir";
ini_data["Settings"]["bin"] = "/home/usr/dir/bin";
ini_data["Counter"]["read"] = "10";
ini_data["Counter"]["write"] = "5";

ipa.WriteFile("test.ini");
```
Output (test.ini):
```ini
example=hello 

[Settings]
path=/home/usr/dir
bin=/home/usr/dir/bin

[Counter]
read=10
write=5
```

<a href="#unit"></a>
## Unit Tests
In subdir `test` are several unit tests written with the Catch2 C++ unit test library.
To call the tests you can use GNU Make with `make && make run`

## License
[__MIT__](https://choosealicense.com/licenses/mit/)
