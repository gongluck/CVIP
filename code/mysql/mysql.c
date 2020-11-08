/*
 * @Author: gongluck 
 * @Date: 2020-11-08 17:54:54 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-08 18:10:02
 */

//gcc mysql.c -lmysqlclient

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char server[] = "127.0.0.1";
    char user[] = "gongluck";
    char password[] = "123";
    char database[] = "test";

    conn = mysql_init(NULL);
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        printf("%s\n", mysql_error(conn));
    }

    char sql[128] = {0};
    sprintf(sql, "insert into user_info(`name`,title)values('gongluck', 'test');");
    if (mysql_query(conn, sql))
    {
        printf("%s\n", mysql_error(conn));
    }

    if (mysql_query(conn, "select * from user_info"))
    {
        printf("%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s\t", row[0]);
        printf("%s\t", row[1]);
        printf("%s\n", row[2]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}