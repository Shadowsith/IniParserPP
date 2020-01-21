#include <fstream>
#include <map>
#include <vector>
#include <iostream>

namespace ini {

typedef std::map<std::string, std::map<std::string, std::string>> inimap;

class IniParser {
using string = std::string;
typedef std::vector<string> vecstr;

    public:
    IniParser() {}
    IniParser(string file_path) {
        readFile(file_path);
    }
    IniParser(string file_path, inimap& data) {
        if(readFile(file_path)) {
            data = parse();
        }
    }

    ~IniParser() { 
        _lines.clear();
        _values.clear(); 
    }

    inimap parseString(string str) {
        if(readString(str)) {
            return parse();
        } else {
            auto val = _values;
            val.clear();
            return val;
        }
    }

    inimap parseFile(string file) {
        if(readFile(file)) {
            return parse();
        } else {
            auto val = _values;
            val.clear();
            return val;
        }
    }

    inimap parse() {
        _values.clear();
        string head = "";
        string headBefore = "";
        vecstr vec;
        int i = 0;
        for(string line : _lines) {
            //ignoring ini comments at begin of line
            if(line[0] != ' ' && line != "") {
                if(line[0] == '[') {
                    // remove [] chars
                    head = line.substr(1, line.length()-2);
                } else {
                    if(line.find(_keyValueDelim) != string::npos && line != "") {
                        vec = _split(line, _keyValueDelim);
                        if(vec[0] != "" && vec[1] != "") {
                            _values[head][vec[0]] = vec[1];
                        }
                    } else if(_comments && line[0] == _commentSign) {
                        if(i != 0 && head != headBefore)
                            i = 0;
                        _values[head][_commentSign + std::to_string(i)] = line;
                        headBefore = head;
                        i++;
                    }
                }
            }
        }
        return _values;
    }

    bool writeFile(string file_path, inimap& data) {
        std::ofstream file(file_path);
        try {
            for(auto const& m1 : data) {
                if(m1.first != "") {
                    if(m1.first != data.begin()->first) {
                        file << _lineSeparator; 
                    }
                    file << '[' << m1.first << ']' << _lineSeparator;
                }
                for(auto const& m2 : m1.second) {
                    if(_comments && (m2.first[0] == _commentSign || m2.first.find(_commentSign)
                            != string::npos)) {
                        file << m2.second << _lineSeparator;
                    } else {
                        file << m2.first << _keyValueDelim << m2.second
                            << _lineSeparator;
                    }
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

    /*
    bool writeLine(string file_path, string section, string key, string value) {
        vecstr lines;
        std::fstream file;
        bool isSection = false;
        try {
            file.open(file_path);
            if(file.good()) {
                for(string line; getline(file, line);) {
                    lines.push_back(line);
                }
                file.close();
            } else {
                file.close();
                return false;
            }
            for(string l : lines) {
                if(l.find("[" + section + "]") != string::npos && 
                    !isSection) {
                   isSection = true; 
                   continue;
                }
                if(l.find("[") != string::npos && isSection) {
                    return false;
                }
            }
        } catch(std::exception) {
            file.close();
            return false;
        }
        return true;
    }
    */

    string writeData(inimap& data) {
        try {
            std::string str = "";
            for(auto const& m1 : data) {
                if(m1.first != "") {
                    if(m1.first != data.begin()->first) {
                        str += _lineSeparator; 
                    }
                    str += '[' + m1.first + ']' + _lineSeparator;
                }
                for(auto const& m2 : m1.second) {
                   str += m2.first + _keyValueDelim + m2.second + 
                        _lineSeparator;
                }
            }
            return str;
        } catch(std::exception ex) {
            throw;
        }
    }

    bool readString(string str) {
        try {
            _lines.clear();
            string line;
            _lines = _split(str, _lineSeparator);
        } catch(std::exception ex) {
            return false;
        }
        return true;
    }

    bool readFile(string file) {
        _lines.clear();
        string line;
        std::ifstream ini_file;
        try {
            ini_file.open(file);
            if(ini_file.good()) {
                while(!ini_file.eof()) {
                    std::getline(ini_file, line);
                    _lines.push_back(line);
                }
                ini_file.close();
                return true;
            } else {
                return false;
            }
        } catch(std::exception ex) {
            ini_file.close();
            throw;
        }
        return false;
    }

    void setKeyValueDelimiter(string str) {
        _keyValueDelim = str;
    }

    string getKeyValueDelimiter() {
        return _keyValueDelim;
    }

    void setLineSeparator(string str) {
        _lineSeparator = str;
    }

    string getLineSeparator() {
        return _lineSeparator;
    }

    void setCommentSign(char c) {
        _commentSign = c;
    }

    char getCommentSign() {
        return _commentSign;
    }

    void setAllowComments(bool b) {
        _comments = b;
    }

    bool getAllowComments() {
        return _comments;
    }

    private:
    vecstr _lines;
    inimap _values;
    string _keyValueDelim = "=";
    string _lineSeparator = "\n";
    char _commentSign = ';';
    bool _comments = false;

    vecstr _split(string& str, const string delim) {
        vecstr parts;
        std::size_t pos = 0;
        string token;
        while ((pos = str.find(delim)) != string::npos) {
            token = str.substr(0, pos);
            parts.push_back(token);
            str.erase(0, pos + delim.length());
        }
        parts.push_back(str);
        return parts;
    }
};}
