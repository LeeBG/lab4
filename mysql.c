/*
 * 20153230 안준홍
 * mysql 전화번호부
 * 설치 : mysql-server, mysql-client
 * 테이블 미리 생성해야함.
 * 컴파일 : gcc -o xx xx.c -lmysqlclient
 */
#include </usr/include/mysql/mysql.h>
#include <string.h>
#include <stdio.h>

#define DB_HOST ""
#define DB_USER "root"
#define DB_PASS "1234"
#define DB_NAME "test"
#define CHOP(x) x[strlen(x) - 1] = ' '

int main(void) {
    MYSQL       *connection = NULL, conn;
    MYSQL_RES   *sql_result;
    MYSQL_ROW   sql_row;

    int query_stat;

    char name[12];
    char address[80];
    char tel[12];
    char query[255];

    mysql_init(&conn);

    connection = mysql_real_connect(&conn, DB_HOST,
                    DB_USER, DB_PASS,
                    DB_NAME, 0,
                    (char *)NULL, 0);
    if (connection == NULL) {
        fprintf(stderr, "MySQL connection error : %s", mysql_error(&conn));
        return 1;
    }
    query_stat = mysql_query(connection, "select * from address");
    if(query_stat != 0) {
        fprintf(stderr, "MySQL query error : %s", mysql_error(&conn));
        return 1;
    }

    sql_result = mysql_store_result(connection);

    printf("%+11s   %-30s   %-10s", "이름", "주소", "전화번호\n");
    while((sql_row = mysql_fetch_row(sql_result)) != NULL) {
        printf("%+11s   %-30s   %-10s\n", sql_row[0], sql_row[1], sql_row[2]);
    }
    mysql_free_result(sql_result);

    printf("이름 : ");
    fgets(name, 12, stdin);
    CHOP(name);

    printf("주소 : ");
    fgets(address, 80, stdin);
    CHOP(name);

    printf("전화 : ");
    fgets(tel, 12, stdin);
    CHOP(tel);

    sprintf(query, "insert into address values "
                    "('%s', '%s', '%s')",
                    name, address, tel);

    query_stat = mysql_query(connection, query);
    if(query_stat != 0) {
        fprintf(stderr, "MySQL query error : %s", mysql_error(&conn));
        return 1;
    }
    mysql_close(connection);
}