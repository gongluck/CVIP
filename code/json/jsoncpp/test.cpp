/*
 * @Author: gongluck 
 * @Date: 2020-11-19 09:40:21 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-19 09:46:42
 */

// g++ test.cpp -ljsoncpp

#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

void readJson()
{
    std::string strValue = "{\"name\":\"json\",\"array\":[{\"cpp\":\"jsoncpp\"},{\"java\":\"jsoninjava\"},{\"php\":\"support\"}]}";

    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strValue, value))
    {
        std::string out = value["name"].asString();
        std::cout << out << std::endl;
        const Json::Value arrayObj = value["array"];
        for (unsigned int i = 0; i < arrayObj.size(); i++)
        {
            if (!arrayObj[i].isMember("cpp"))
                continue;
            out = arrayObj[i]["cpp"].asString();
            std::cout << out;
            if (i != (arrayObj.size() - 1))
                std::cout << std::endl;
        }
    }
}

void writeJson()
{
    Json::Value root;
    Json::Value arrayObj;
    Json::Value item;

    item["cpp"] = "jsoncpp";
    item["java"] = "jsoninjava";
    item["php"] = "support";
    arrayObj.append(item);

    root["name"] = "json";
    root["array"] = arrayObj;

    root.toStyledString();
    std::string out = root.toStyledString();
    std::cout << out << std::endl;
}

int main(int argc, char **argv)
{
    readJson();
    writeJson();
    return 0;
}