[![Build status](https://ci.appveyor.com/api/projects/status/v7y5q5a0q45w1ib7?svg=true)](https://ci.appveyor.com/project/Shadowsith/iniparserpp)
# IniParser
A small, modern C++11 header only library for read, write and modify .INI configuration files
with help of std::map


## Motiviaton
This project was highly inspired from the C#
[https://github.com/rickyah/ini-parser](ini-parser) by rickyah.

## Usage
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

int main {
    // creates new object and reads ini
    IniParser ipa("test.ini");

    // second way to read:
    IniParser ipa;
    if(ini.read("test.ini")) {
        // parse ini-data if file has red to 
        // std::map<std::string, std::map<std::string, std::string>>
        auto data = ini.parse();
        std::string path = data[""]["path"]; // --> "/home/test/config.txt"
        std::string value = data["Test"]["value"]; // --> "120"

        // add new value
        data["New"]["counter"] = 20;

        //remove value from existing
        data["Test"].erase("value");

        if(ipa.write("test.ini"), data) {
            // do something if writing was successful ...
        }
    }
    return 0;
}

```

Output:
```ini
path=/home/test/config.txt

[New]
counter=20

[Test]
exe=/home/test/bin

```

## License
MIT
