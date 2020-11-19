/*
 * @Author: gongluck 
 * @Date: 2020-11-19 17:11:24 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-19 17:27:23
 */

// g++ test.cpp -ltinyxml2

#include <iostream>
#include "tinyxml2.h"

int main(void)
{
    const char *xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>   \
                        <note>                      \
                            <to>beijing</to>             \
                            <from>shenzhen</from>           \
                            <heading>Reminder</heading> \
                            <body>Don't forget the meeting!</body> \
                        </note>";
    tinyxml2::XMLDocument doc;
    doc.Parse(xml);
    std::cout << doc.ErrorID() << std::endl;

    // 1. 第一种刷新到本地
    FILE *fp = fopen("memory_1.xml", "wb");
    tinyxml2::XMLPrinter printer(fp);
    doc.Print(&printer); // 打印到文件
    fclose(fp);

    // 2. 第二种刷新到本地
    doc.SaveFile("memory_2.xml");

    return 0;
}
