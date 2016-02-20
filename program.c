#include <stdio.h>
#include <stdlib.h>
#include "sqlite3/sqlite3.h"

#define DATABASE_NAME   "test.db"

int create_database(void);
int callback(void *NotUsed, int argc, char **argv, char **azColName);

sqlite3 *db;

//
// MAIN
//
int main(int argc, char **argv)
{
    char *sql;
    char *zErrMsg = 0;
        
    // open connection
	int rc = sqlite3_open(DATABASE_NAME, &db);

    if(rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
   } else {
        fprintf(stdout, "Opened database successfully\n");
        printf("sqlite3 version: %s\n", sqlite3_libversion());
        printf("\n");
   }
	
    rc = create_database();
    
    // insert
    sql = "INSERT INTO COMPANY (ID, NAME) VALUES (1, 'CONTOSO INC.') ;"
          "INSERT INTO PERSON  (ID, NAME, COMPANYID, SALARY) VALUES (1, 'John', 1, 12000.50) ;";
    
       
    // execute sql statement
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if(rc != SQLITE_OK){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
   }

    //http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
    
    
    // close fb connection 
    sqlite3_close(db);

    exit(EXIT_SUCCESS);
}

// 
// create the database tables
//
int create_database() {
    
    char *sql;
    int rc;
    char *zErrMsg = 0;
    
    sql = "DROP TABLE IF EXISTS PERSON;  "
          "DROP TABLE IF EXISTS COMPANY; "
          
          "PRAGMA foreign_keys = ON;     "
          
          "CREATE TABLE COMPANY("  
                "ID INT PRIMARY KEY     NOT NULL," 
                "NAME           TEXT    NOT NULL "
                ");" 
          
          "CREATE TABLE PERSON("
                "ID INT PRIMARY KEY     NOT NULL,"
                "NAME           TEXT    NOT NULL,"
                "COMPANYID      INT     NOT NULL,"
                "SALARY         REAL    NOT NULL,"
                "FOREIGN KEY(COMPANYID) REFERENCES COMPANY(ID)"
                ");";                                         

    // execute sql statement
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if(rc != SQLITE_OK){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
   }
    
   return rc;
}

//
// callback function for sqlite3
//
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
   
    printf("\n");
    return 0;
}
