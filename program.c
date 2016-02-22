#include <stdio.h>
#include <stdlib.h>
#include "sqlite3/sqlite3.h"


/* ********************************************************************************************
 *  indica al linker di eseguire la ricerca della libreria SQLITE3.LIB durante il collegamento.  
 *  Il linker esegue la ricerca prima nella directory di lavoro, 
 *  quindi nel percorso specificato nella variabile di ambiente LIB.
 *  In alternativa compilare con l'opzione  /link sqlite3.lib
 *********************************************************************************************/
#pragma comment(lib,"sqlite3.lib") //sqlite3 Library

#define DATABASE_NAME   "test.db"

int create_database(void);
int callback(void *data, int argc, char **argv, char **azColName);

sqlite3 *db;

//
// MAIN
//
int main(int argc, char **argv)
{
    const char* data = "Callback function called";
    char *sql;
    char *zErrMsg = NULL;
    
    // open connection
    int rc = sqlite3_open(DATABASE_NAME, &db);

    if(rc != SQLITE_OK){
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
          "INSERT INTO PERSON  (ID, NAME, COMPANYID, SALARY) VALUES (1, 'John', 1, 12000.50) ;"
          "INSERT INTO PERSON  (ID, NAME, COMPANYID, SALARY) VALUES (2, 'Paul', 1, 15000.00) ;"
          "INSERT INTO PERSON  (ID, NAME, COMPANYID, SALARY) VALUES (3, 'Steven', 1, 11000.00) ;";
    
       
    // execute sql statement
    rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
    
    if(rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }

    // massive insert
    int j = 0;
    sql = (char*)malloc(255);
    for(j = 4; j <= 100; j++)
    {
        sprintf(sql, "INSERT INTO PERSON  (ID, NAME, COMPANYID, SALARY) VALUES (%d, 'Name%d', 1, 0) ;", j, j);
        // execute sql statement
        rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
        
        if(rc != SQLITE_OK){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            break;
        }
    }
   
    // select
    sql = "SELECT PERSON.ID, PERSON.NAME, PERSON.SALARY, COMPANY.NAME AS COMPANY "
          "FROM PERSON INNER JOIN COMPANY ON PERSON.COMPANYID = COMPANY.ID "
          "ORDER BY PERSON.ID;";
             
    // execute sql statement
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

    printf("----->>>>>>-----\n");
    
    if(rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
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
    rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
    
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
static int callback(void *data, int argc, char **argv, char **azColName){
    fprintf(stderr, "%s\n", (const char*)data);
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
   
    printf("\n");
    return 0;
}
