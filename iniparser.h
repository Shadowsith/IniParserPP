#include <fstream>
#include <map>
#include <vector>

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

    inimap parseFile(string file) {
        if(readFile(file))
            return parse();
        else {
            auto val = _values;
            val.clear();
            return val;
        }
    }

    inimap parseData(string& data) {
        if(readData(data)) {
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
        vecstr vec;
        for(string line : _lines) {
            //ignoring ini comments at begin of line
            if(line[0] != '#' && line[0] != ';' && line[0] != ' ') {
                if(line[0] == '[') {
                    // remove [] chars
                    head = line.substr(1, line.length()-2);
                } else {
                    if(line.find("=") > 0 && line != "") {
                        vec = this->_split(line, _keyValueDelim);
                        if(vec[0] != "" && vec[1] != "") {
                            _values[head][vec[0]] = vec[1];
                        }
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
                   file << m2.first << _keyValueDelim << m2.second
                       << _lineSeparator;
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

    bool readData(string& data, string delimiter = "\n") {
        _lines.clear();
        vecstr vec = _split(data, delimiter);
        for(string str : vec) {
            _lines.push_back(str);
        }
        return true;
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

    string getLineSeparator(string str) {
        return _lineSeparator;
    }

    private:
    vecstr _lines;
    inimap _values;
    string _keyValueDelim = "=";
    string _lineSeparator = "\n";

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
