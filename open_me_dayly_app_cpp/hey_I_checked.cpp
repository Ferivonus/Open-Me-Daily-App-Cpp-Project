#include "hey_I_checked.h"
#include <iostream>
#include <ctime>

hey_I_checked::hey_I_checked() {
    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        std::cerr << "Database opening failed!" << std::endl;
    }
    else {
        std::cout << "Database connected successfully." << std::endl;
    }

    std::string createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS log (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date TEXT NOT NULL UNIQUE,
            boot_count INTEGER DEFAULT 0,
            is_password_entered BOOLEAN DEFAULT 0
        );
    )";
    executeSQL(createTableSQL);
}

hey_I_checked::~hey_I_checked() {
    sqlite3_close(db);
}

void hey_I_checked::executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "SQLite Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void hey_I_checked::markPasswordEntered() {
    time_t now = time(0);
    struct tm ltm;

    localtime_s(&ltm, &now);

    std::string date = std::to_string(1900 + ltm.tm_year) + "-" +
        std::to_string(1 + ltm.tm_mon) + "-" +
        std::to_string(ltm.tm_mday);

    sqlite3_stmt* stmt;
    const char* checkQuery = "SELECT id FROM log WHERE date = ?;";

    bool recordExists = false;

    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            recordExists = true;
        }
    }
    sqlite3_finalize(stmt);

    if (recordExists) {
        const char* updateQuery = "UPDATE log SET is_password_entered = 1 WHERE date = ?;";
        if (sqlite3_prepare_v2(db, updateQuery, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);
            sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        const char* insertQuery = "INSERT INTO log (date, boot_count, is_password_entered) VALUES (?, 1, 1);";
        if (sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);
            sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
}

void hey_I_checked::showLogs() {
    sqlite3* db;
    sqlite3_stmt* stmt;

    if (sqlite3_open("log.db", &db) != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* query = "SELECT * FROM log;";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to fetch logs: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    std::cout << "Displaying all user logs:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int boot_count = sqlite3_column_int(stmt, 2);
        int password_entered = sqlite3_column_int(stmt, 3);

        std::cout << "ID: " << id << " | Date: " << date
            << " | Boot Count: " << boot_count
            << " | Password Entered: " << (password_entered ? "Yes" : "No") << "\n";
    }


    sqlite3_finalize(stmt);
    sqlite3_close(db);


    std::cout << "Press Enter to exit...";
    std::cin.get();
}

void hey_I_checked::runner() {
    markPasswordEntered();
}
