# Test Sqlite3 

I sorgenti si scaricano da https://www.sqlite.org/2016/sqlite-amalgamation-3110000.zip

- shell.c
- sqlite3.c
- sqlite3.h
- sqlite3ext.h

I compilati si scaricano da https://www.sqlite.org/2016/sqlite-dll-win32-x86-3110000.zip

- sqlite3.def
- sqlite3.dll

Generare il file .lib dalla finestra command VSTools utilizzando l'utility lib.exe:

lib /def:sqlite3.def

compilare e linkare la libreria:

cl miofile.c /link sqlite3.lib
