/*
 * @Author: gongluck 
 * @Date: 2020-11-20 17:23:22 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-20 18:14:44
 */

// g++ *.cc *.cpp `pkg-config --cflags --libs protobuf`

#include <iostream>
#include "addressbook.pb.h"

using namespace std;

int main()
{
    char buf[1024];
    int len;

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    Test::Person obj;
    obj.set_name("gongluck");
    obj.set_id(1);
    *obj.mutable_email() = "https://github.com.cnpmjs.org/gongluck/CVIP";
    len = obj.ByteSize();
    cout << "len = " << len << endl;
    obj.SerializeToArray(buf, len);

    Test::Person obj2;
    obj2.ParseFromArray(buf, len);
    cout << "name = " << obj2.name() << endl;
    cout << "id = " << obj2.id() << endl;
    cout << "email = " << obj2.email() << endl;

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}