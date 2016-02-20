#include <stdio.h>
#include "sqlite3/sqlite3.h"


int main(int argc, char **argv)
{
 	sqlite3 *db;
	int rc = sqlite3_open("test.db", &db);
	
	printf("%s\n", sqlite3_libversion()); 
   
    return 0;
}