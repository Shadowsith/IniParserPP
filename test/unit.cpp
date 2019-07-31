#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../iniparser.h"
#include <map>
using IniParser = ini::IniParser;

IniParser ipa;

TEST_CASE("Ini readFile", "[readFile]") {
    try {
        REQUIRE(ipa.readFile("root.ini"));
        REQUIRE(ipa.readFile("test.ini"));
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
