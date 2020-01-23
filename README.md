[![Build status](https://ci.appveyor.com/api/projects/status/v7y5q5a0q45w1ib7?svg=true)](https://ci.appveyor.com/project/Shadowsith/iniparserpp)
# IniParser++
A small, modern C++11 header-only library for read, write and modify .INI 
configuration files with help of std::map

----
## Content
- [IniParser++](#iniparser)
  - [Content](#content)
  - [- License](#ul-lilicenseli-ul)
  - [Motiviaton](#motiviaton)
  - [## Documentation](#h2-id%22documentation-126%22documentationh2)
    - [Namespace](#namespace)
    - [Typedefs](#typedefs)
  - [| inimap | Typedef for std::map\<std::string, std::map\<std::string, std::string\>\> |](#inimap--typedef-for-stdmapstdstring-stdmapstdstring-stdstring)
    - [Class IniParser](#class-iniparser)
      - [Constructors](#constructors)
      - [Destructor](#destructor)
      - [Private Member](#private-member)
      - [Private Methods](#private-methods)
      - [Public Methods](#public-methods)
      - [Setters (Configuration)](#setters-configuration)
  - [Examples](#examples)
    - [Read and write .ini file](#read-and-write-ini-file)
    - [Create new .ini file](#create-new-ini-file)
    - [Hanlde comments](#hanlde-comments)
  - [More examples are in the unit tests under test/unit.cpp](#more-examples-are-in-the-unit-tests-under-testunitcpp)
  - [Unit Tests](#unit-tests)
  - [License](#license)
---
<a id="motiviation"></a>
## Motiviaton
This project was highly inspired from the C#
[ini-parser](https://github.com/rickyah/ini-parser) by rickyah and its good usability.


---
<a id="documentation"></a>
## Documentation
---
<a id="namespace"></a>
### Namespace
**ini**

---
<a id="typedefs"></a>
### Typedefs
| Type | Description |
|------|-------------|
| inimap | Typedef for std::map\<std::string, std::map\<std::string, std::string\>\> |
---

<a id="class"></a>
### Class IniParser
<a id="constructors"></a>
#### Constructors
| Signature | Description |
|-----------|-------------|
| IniParser() | Creates new instance of IniParser |
| IniParser(std::string file\_path) | Creates new instance and reads file  content |
| IniParser(std::string file, inimap& data) | Creates new instance, reads file and parse data to inimap by reference |

<a id="destructor"></a>
#### Destructor 
| Name | Description |
|------|-------------|
| ~IniParser() | clears all private container

<a id="privatemember"></a>
#### Private Member
| Signature | Type | Description |
|-----------|------|-------------|
| \_lines | std::vector\<std::string\> | Stores all lines by reading file/string data |
| \_values | ini::inimap | Stores \_line values to std::map during parsing |
| \_keyValueDelim | std::string | Stores default key value delimiter of ini file (default "=") |
| \_lineSeparator | std::string | Stores default separator for ini data, default is newline ("\n") |
| \_comments | bool | used for that parser knows if it needs to handle comments (read/write) |
| \_commentSign | char | Line prefix for comment lines in ini file |


<a id="privatemethods"></a>
#### Private Methods
| Signature | Return Value | Description |
|-----------|--------------|-------------|
| \_split(std::string delimiter) | std::vector\<std::string\> | Splits a string into parts |

<a id="publicmethods"></a>
#### Public Methods
| Signature | Return Value | Description |
|-----------|--------------|-------------|
| parse() | ini::inimap | Parses data which was red before
| parseFile(std::string file\_path) | ini::inimap | read file if exists and parses data |
| parseString(std::string& data) | ini::inimap | read string and parse |
| readFile(std::string file\_path) | bool | reads file and store values internal, if successful returns true |
| readString(std::string data) | bool | reads string and store values internal, if successful returns true |
| writeFile(std::string file\_path) | bool | writes data into file 
| writeString(std::string file\_path) | bool | writes data into std::string

<a id="configuration"></a>
#### Setters (Configuration)
Because INI files are not standardized there are different configuration setters which you can adjust.

| Signature | Return Value/Type | Description |
|-----------|-------------------|-------------|
| setKeyValueDelimiter(std::string delimiter) | void | Set the delimiter between keys and values (default is "=" ) |
| setLineSeparator(std::string seperator) | void | Set the seperator for the ini lines |
| setAllowComments(bool b) | void | Set if comments are saved in the inimap data (default false) |
| setCommentSign(char c) | void | Set which sign is used for comments in the INI file (default is ';' ) |
| setSectionOpen(char c) | void | Set which opening sign is used for a INI section |
| setSectionClose(char c) | void | Set which closing sign is used for a INI section |
| setSectionTag(char c1, char c2) | void | Set opening and closing tag for a INI section |

---
<a id="examples"></a>
## Examples
<a id="readwrite"></a>
### Read and write .ini file
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
<a id="create"></a>
### Create new .ini file
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

ipa.WriteFile("test.ini", ini_data);
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
<a id="comments"></a>
### Hanlde comments
Two kinds of comments are supported:
* On top of the file
* On top of the each section

Attribute specific comments are unfortunately not supported.

Ini file (comments.ini)
```ini
# this is a example to read and write comments
# some additional information

[Settings]
# here are the settings
path=/home/usr/dir
```
```cpp
IniParser p;
p.setAllowComments(true);
p.setCommentSign('#');
ini::inimap data = p.parseFile("commments.ini");
// please remember that the prefix (#) depends to the set commment sign
data[""]["#0"]; // --> # this is a example to read and write comments
data[""]["#1"]; // --> # some additional information
data["Settings"]["#0"]; // --> here are the settings
data["Settings"]["#1"] = "# a new comment";
p.writeFile("comments.ini", data);
```
Output:
```ini
# this is a example to read and write comments
# some additional information

[Settings]
# here are the settings
# a new comment
path=/home/usr/dir
```

More examples are in the unit tests under test/unit.cpp
---
<a id="unit"></a>
## Unit Tests
In subdir `test` are several unit tests written with the Catch2 C++ unit test library.
To call the tests you can use GNU Make with `make && make run`

---
<a id="license"></a>
## License
[__MIT__](https://choosealicense.com/licenses/mit/)
