#include "libs/crow_all.h"
#include "libs/sqlite-3.42.0/sqlite3.h"
#include <string>

int done = 0;
char *zErrMsg = 0;
sqlite3 *db;
int rc;

int main() {
    crow::App<crow::CORSHandler> app;
    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors
      .global()
        .headers("X-Custom-Header", "Upgrade-Insecure-Requests")
        .methods("POST"_method, "GET"_method)
      .prefix("/cors")
        .origin("127.0.0.1:5173")
      .prefix("/nocors")
        .ignore();

    rc = sqlite3_open("test.sqlite", &db);

    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    CROW_ROUTE(app, "/api/get-verses/<int>/<int>")([](int b, int c){
        std::string res;
        res = std::to_string(b) + " - " + std::to_string(c);
        std::cout << res << std::endl;

        std::string s = "";
        std::string* param = &s;

        /* Create SQL statement */
        char* sql = "SELECT * from test";
        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, [](void *param, int argc, char **argv, char **azColName) -> int {
            std::string* s = (std::string*) param;

            int i;
            for(i = 0; i<argc; i++){
                // sprintf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
                *s = *s + azColName[i] + ": " + (argv[i] ? argv[i] : "NULL") + "\n";
            }

            return 0;
        }, param, &zErrMsg);

        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Operation done successfully\n");
        }

        std::cout << s << std::endl;
        return s;
    });

    app.port(18080).run();
    sqlite3_close(db);
    return 0;
}
