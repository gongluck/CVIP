/*
 * @Author: gongluck 
 * @Date: 2020-11-14 21:35:15 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-14 23:03:37
 */

//gcc mongodb.c -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0/ -lmongoc-1.0 -lbson-1.0

#include <bson.h>
#include <mongoc.h>

int main()
{
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    bson_t *insert;
    bson_error_t error;

    //初始化libmongoc驱动
    mongoc_init();

    //创建连接对象
    client = mongoc_client_new("mongodb://localhost:27017");

    //获取指定数据库和集合
    collection = mongoc_client_get_collection(client, "gongluck", "test");

    //字段为hello，值为world字符串
    insert = BCON_NEW("hello", BCON_UTF8("world"));

    //插入文档
    if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, insert, NULL, &error))
    {
        fprintf(stderr, "%s\n", error.message);
    }

    bson_destroy(insert);

    mongoc_collection_destroy(collection); //释放表对象
    mongoc_client_destroy(client);         //释放连接对象
    mongoc_cleanup();                      //释放libmongoc驱动
    return 0;
}