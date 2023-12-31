#include "libs/crow_all.h"
#include "libs/sqlite-3.42.0/sqlite3.h"
#include <string>
#include <regex>
#include <iostream>
#include <string>
#include <codecvt>
#include <fstream>

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

    rc = sqlite3_open("Hb5_utf8_rev2.sqlite", &db);

    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    CROW_ROUTE(app, "/")([]() {
        crow::response res;
        res.set_static_file_info("frontend/index.html");
        res.set_header("content-type", "text/html");
        return res;
    });
    CROW_ROUTE(app, "/index.js")([]() {
        crow::response res;
        res.set_static_file_info("frontend/index.js");
        res.set_header("content-type", "text/javascript");
        return res;
    });
    CROW_ROUTE(app, "/index.css")([]() {
        crow::response res;
        res.set_static_file_info("frontend/index.css");
        res.set_header("content-type", "text/css");
        return res;
    });
    CROW_ROUTE(app, "/vite.svg")([]() {
        crow::response res;
        res.set_static_file_info("frontend/vite.svg");
        res.set_header("content-type", "image/svg+xml");
        return res;
    });

    CROW_ROUTE(app, "/api/get-verses/<int>/<int>")([](int book, int chapter){
        // std::string res;
        // res = std::to_string(b) + " - " + std::to_string(c);
        // std::cout << res << std::endl;

        // Open a JSON array
        std::string res = "[";

        // SQL query (No need to worry about SQL injection since parameters are only allowed to be integers)
        std::string sql = "SELECT * FROM Hb5_utf8_rev1 WHERE Book='" + std::to_string(book) + "' AND Chapter='" + std::to_string(chapter) + "'";
        // Execute SQL query
        rc = sqlite3_exec(db, sql.c_str(), [](void *param, int argc, char **argv, char **azColName) -> int {
            // Get string pointer from param
            std::string* s = (std::string*) param;
            // Create a string for this row and open the JSON object
            std::string verse = "{";

            int i;
            for(i = 0; i<argc; i++){
                // sprintf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
                // Add entry to the JSON object
                verse = verse + "\"" + azColName[i] + "\":\"" + (argv[i] ? argv[i] : "NULL") + "\",";
            }

            // Remove the comma from the last entry
            verse.pop_back();
            // Close the JSON object
            verse += "},";
            // Add row to the JSON array
            *s = *s + verse;

            return 0;
        }, &res, &zErrMsg);
        // Detect for errors
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Operation done successfully\n");
        }

        // Remove the comma from the last object
        if (res.at(res.length() - 1) == ',') {
            res.pop_back();
        }
        // Close JSON array
        res += "]";
        // Replace all \n characters
        res = std::regex_replace(res, std::regex("\n"), "\\n");

        return res;
    });

    app.port(80).run();
    sqlite3_close(db);
    return 0;
}
