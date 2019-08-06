#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../iniparser.h"
#include <map>
#include <thread>
#include <cstdio>
using IniParser = ini::IniParser;

IniParser ipa;

TEST_CASE("Ini readFile", "[readFile]") {
    try {
        REQUIRE(ipa.readFile("root.ini"));
        REQUIRE(ipa.readFile("test.ini"));
        // check non exist
        REQUIRE(!ipa.readFile("abcdef.ini"));
    } catch(std::exception ex) {
        REQUIRE(false);
    }
}

TEST_CASE("Ini parse", "[parse]") {
    try {
        REQUIRE(ipa.readFile("test.ini"));
        auto data = ipa.parse();
        REQUIRE(data["Test"]["hello"] == "world");
        REQUIRE(data[""]["path"] == "/home/test/config.txt");
        auto rdata = ipa.parseFile("root.ini"); 
        REQUIRE(rdata.size() > 1);
        REQUIRE(rdata["Temp"]["Mask"] == "*.log");
    } catch(std::exception ex) {
        REQUIRE(false);
    }
}

TEST_CASE("Ini writeFile", "[writeFile]") {
    try {
        auto data = ipa.parseFile("test.ini");
        data["Settings"]["lang"] = "de_DE";
        data["New"]["counter"] = "20";
        REQUIRE(ipa.writeFile("test1.ini", data));
        REQUIRE(ipa.readFile("test1.ini"));
        auto ndata = ipa.parse();
        REQUIRE(ndata["Settings"]["lang"] == "de_DE");
        REQUIRE(ndata["New"]["counter"] == "20");
    } catch(std::exception ex) {
        REQUIRE(false);
    }
}

TEST_CASE("Ini read/write data", "[read/write data]") {
    std::string inistr1 = "";
    inistr1 += "exe=/home/test/bin/\n\n";
    inistr1 += ";comment\n";
    inistr1 += "#comment\n";
    inistr1 += "[Settings]\n";
    inistr1 += "volume=120\n";
    inistr1 += "lang=en_US\n";
    inistr1 += "\n";
    inistr1 += "[Test]\"";
    inistr1 += "hello=world";

    try {
        IniParser parser;
        REQUIRE(parser.readData(inistr1));
        auto data = parser.parse();
        REQUIRE(data["Settings"]["volume"] == "120");
        data["Settings"]["volume"] = "90";
        parser.setKeyValueDelimiter(" : ");
        std::string wback = parser.writeData(data);
        data = parser.parseData(wback);
        REQUIRE(data["Settings"]["volume"] == "90");
        std::cout << wback << std::endl;
    } catch(std::exception ex) {
        REQUIRE(false);
    }
}

TEST_CASE("Ini other standard", "[read/write non standard]") {
    IniParser p;
    p.setKeyValueDelimiter(" : ");
    REQUIRE(p.readFile("other.ini"));
    auto data = p.parse();
    REQUIRE(data["Test"]["hello"] == "world");
}

void add_cat_values(int i, ini::inimap& data) {
    for(int j = 0; j < 1000; j++) {
        data[std::to_string(i)][std::to_string(j)] = std::to_string(i+j); 
    }
}

TEST_CASE("Ini performance write/read", "[performance]") {
    std::vector<std::thread> thrds;
    ini::inimap data;
    for(int i = 0; i < 1000; i++) {
        std::thread t(add_cat_values, i, std::ref(data));
        thrds.push_back(std::move(t));
    }
    for(std::thread &t : thrds) {
        t.join();
    }

    IniParser p;
    p.writeFile("performance.ini", data);
    REQUIRE(p.readFile("performance.ini"));
    auto x = p.parse();
    REQUIRE(x["20"]["30"] != "");
    std::remove("performance.ini");
}


