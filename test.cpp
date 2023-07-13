#include "libs/crow_all.h"
#include "libs/sqlite-3.42.0/sqlite3.h"
#include <string>
#include <regex>
#include <iostream>
#include <string>
#include <codecvt>
#include <fstream>
#include "libs/utf8.h"

int done = 0;
char *zErrMsg = 0;
sqlite3 *db;
int rc;
std::wofstream myfile;

int main() {
    rc = sqlite3_open("Hb5_utf8_rev2.sqlite", &db);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    std::string sql = "SELECT * FROM Hb5_utf8_rev1 WHERE Book='1' AND Chapter='1'";
    rc = sqlite3_exec(db, sql.c_str(), [](void *param, int argc, char **argv, char **azColName) -> int {
        int i;
        for(i = 0; i<argc; i++){
            std::string s = argv[i] ? argv[i] : "NULL";
            printf("%s = %s\n", azColName[i], s.c_str());
        }
        return 0;
    }, NULL, &zErrMsg);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Operation done successfully\n");
    }


    sqlite3_close(db);
    return 0;
}
