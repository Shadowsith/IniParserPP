#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../iniparser.h"
#include <map>
using IniParser = ini::IniParser;

IniParser ipa;

TEST_CASE("Ini read", "[read]") {
    try {
        REQUIRE(ipa.read("root.ini"));
        REQUIRE(ipa.read("test.ini"));
    } catch(std::exception) {
        REQUIRE(false);
    }
}

TEST_CASE("Ini parse", "[parse]") {
    try {
        REQUIRE(ipa.read("test.ini"));
        auto data = ipa.parse();
        REQUIRE(data["Test"]["hello"] == "world");
        REQUIRE(data[""]["path"] == "/home/test/config.txt");
        auto rdata = ipa.parse("root.ini"); 
        REQUIRE(rdata.size() > 1);
        REQUIRE(rdata["Temp"]["Mask"] == "*.log");
    } catch(std::exception) {
        REQUIRE(false);
    }
}

TEST_CASE("Ini write", "[write]") {
    try {
        auto data = ipa.parse("test.ini");
        data["Settings"]["lang"] = "de_DE";
        data["New"]["counter"] = "20";
        REQUIRE(ipa.write("test1.ini", data));
        REQUIRE(ipa.read("test1.ini"));
        auto ndata = ipa.parse();
        REQUIRE(ndata["Settings"]["lang"] == "de_DE");
        REQUIRE(ndata["New"]["counter"] == "20");
    } catch(std::exception) {
        REQUIRE(false);
    }
}
