#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>

#define DB_NAME "/Users/penglianjin/Desktop/test.db"

sqlite3 *db = NULL;
char *sql = NULL;
char *zErrMsg = NULL;
int ret = 0;
char *urlTypeResult = NULL;


typedef enum {
    false,
    true
}bool;

/*
 typedef int (*sqlite3_callback)(
 void*,         Data provided in the 4th argument of sqlite3_exec()
 由 sqlite3_exec() 的第四个参数传递而来
 int argc,      The number of columns in row
 表的列数
 char** argv,   An array of strings representing fields in the row
 指向查询结果的指针数组, 可以由 sqlite3_column_text() 得到
 char** azColName   An array of strings representing column names
 指向表头名的指针数组, 可以由 sqlite3_column_name() 得到
 );
 */

//查询的回调函数: 传给sqlite3_exec的回调函数,用来显示查询结果,对每一条查询结果调用一次该回调函数
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    
    if (argc > 0) {
//        for(int i = 0; i < argc; i++){
//            printf("%s = %s\n", azColName[i], argv[i] ?
//                   argv[i] : "NULL");
//        }
//        printf("\n");
        urlTypeResult = argv[2];
    }
    
    if (urlTypeResult != NULL) {
        printf("urlTypeResult: %s\n",urlTypeResult);
    }
    return 0;
}

bool connectDB() {
    
    ret = sqlite3_open(DB_NAME, &db);
    if (ret != SQLITE_OK) {
        fprintf(stderr,"Can't open database: %s\n",sqlite3_errmsg(db));
        sqlite3_free(zErrMsg);
        return false;
    }
    fprintf(stdout, "Opened database successfully\n");
    return true;
}

// 建表
bool createTable() {
    
    sql = "CREATE TABLE URL_TABLE("\
    "id INTEGER PRIMARY KEY    AUTOINCREMENT," \
    "url           TEXT    NOT NULL," \
    "urlType       TEXT    NOT NULL);";
    
    /* Execute SQL statement */
    ret = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( ret != SQLITE_OK ){
        fprintf(stderr, "Create table error SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    
    fprintf(stdout, "Table created successfully\n");
    return true;
}

bool insertRecords(char *url, char *urlType) {
    
    /* Create SQL statement */
    //    sql = "INSERT INTO URL_TABLE (url,urlType) VALUES ('baidu.com', 'w' );";
    
    asprintf(&sql, "%s('%s','%s');","INSERT INTO URL_TABLE (url,urlType) VALUES ",url,urlType);
    
    printf("%s\n",sql);
    
    /* Execute SQL statement */
    ret = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( ret != SQLITE_OK ){
        fprintf(stderr, "Insert records error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        free(sql);
        return false;
    }
    fprintf(stdout, "Inserted records successfully\n");
    free(sql);
    return true;
}

bool findRecordByUrl(char *url) {
    
    /* Create SQL statement */
    
    //    sql = "SELECT *FROM URL_TABLE WHERE url is 'baidu.com';";
    
    asprintf(&sql, "%s'%s';","SELECT *FROM URL_TABLE WHERE url is ",url);
    
    /* Execute SQL statement */
    ret = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( ret != SQLITE_OK ){
        fprintf(stderr, "Find records error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        free(sql);
        return false;
    }
    //    fprintf(stdout, "Find records successfully\n");
    free(sql);
    return true;
}

bool showAllRecords() {
    
    /* Create SQL statement */
    sql = "SELECT *FROM URL_TABLE;";
    
    /* Execute SQL statement */
    ret = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( ret != SQLITE_OK ){
        fprintf(stderr, "Find records error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    return true;
}

bool closeTable() {
    
    int ret = 0;
    ret = sqlite3_close(db);
    if ( ret == SQLITE_BUSY ){
        return false;
    }
    return true;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
    connectDB();
    createTable();
    insertRecords("baidu.com", "w");
    findRecordByUrl("baidu.com");
//    showAllRecords();
    return 0;
}


