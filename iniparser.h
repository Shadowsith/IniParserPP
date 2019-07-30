#include <fstream>
#include <iostream>
#include <map>
#include <vector>

typedef std::map<std::string, std::map<std::string, std::string>> inimap;
typedef std::vector<std::string> vecstr;

namespace ini {
  class IniParser {
    public:
    IniParser() {}
    IniParser(std::string file_path) {
        read(file_path);
    }

    ~IniParser() { 
        _lines.clear();
        _values.clear(); 
    }

    inimap parse(std::string file) {
        if(read(file))
            return parse();
        else {
            auto val = _values;
            val.clear();
            return val;
        }
    }

    inimap parse() {
        _values.clear();
        std::string head = "";
        std::string key;
        std::string val;
        vecstr vec;
        for(std::string line : _lines) {
            //ignoring ini comments at begin of line
            if(line[0] != '#' && line[0] != ';' && line[0] != ' ') {
                if(line[0] == '[') {
                    // remove [] chars
                    head = line.substr(1, line.length()-2);
                } else {
                    if(line.find("=") > 0 && line != "") {
                        vec = this->_split(line, "=");
                        key = vec[0];
                        val = vec[1];
                        if(key != "" && val != "") {
                            _values[head][key] = val;
                        }
                    }
                }
            }
        }
        return _values;
    }

    bool write(std::string file_path, inimap& data) {

        std::ofstream file(file_path);
        try {
            for(auto const& m1 : data) {
                if(m1.first != "") {
                    if(m1.first != data.begin()->first) {
                        file << std::endl; 
                    }
                    file << '[' << m1.first << ']' << std::endl;
                }
                for(auto const& m2 : m1.second) {
                   file << m2.first << '=' << m2.second << std::endl;
                }
            }
            file.close();
            return true;
        } catch(std::exception ex) {
            file.close();
            throw;
        }
        return false;
    }

    bool read(std::string file) {
        _lines.clear();
        std::string line;
        std::ifstream ini_file;
        try {
            ini_file.open(file);
            while(!ini_file.eof()) {
                std::getline(ini_file, line);
                _lines.push_back(line);
            }
            ini_file.close();
            return true;
        } catch(std::exception ex) {
            ini_file.close();
            throw;
        }
        return false;
    }

    private:
    vecstr _lines;
    inimap _values;

    vecstr _split(std::string& str, const std::string delim) {
        vecstr parts;
        std::size_t pos = 0;
        std::string token;
        while ((pos = str.find(delim)) != std::string::npos) {
            token = str.substr(0, pos);
            parts.push_back(token);
            str.erase(0, pos + delim.length());
        }
        parts.push_back(str);
        return parts;
    }
  };
}
