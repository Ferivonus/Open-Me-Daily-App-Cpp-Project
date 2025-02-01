#include "check_opened.h"
#include "FileCreator.h"
#include <sqlite3.h>
#include <sstream>

check_opened::check_opened() {
    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        return;
    }

    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS log (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date TEXT NOT NULL UNIQUE,
            boot_count INTEGER DEFAULT 0,
            is_password_entered INTEGER DEFAULT 0
        );
    )";
    executeSQL(createTableSQL);
}

check_opened::~check_opened() {
    sqlite3_close(db);
}

void check_opened::executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
    }
}

std::string getCurrentDate() {
    time_t now = time(0);
    struct tm ltm;
    localtime_s(&ltm, &now);

    std::stringstream ss;
    ss << (1900 + ltm.tm_year) << "-" << (1 + ltm.tm_mon) << "-" << ltm.tm_mday;
    return ss.str();
}

void check_opened::logAppOpened() {
    std::string date = getCurrentDate();

    sqlite3_stmt* stmt;
    const char* checkQuery = "SELECT id, boot_count FROM log WHERE date = ?;";
    int bootCount = 0;
    int rowId = -1;

    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            rowId = sqlite3_column_int(stmt, 0);
            bootCount = sqlite3_column_int(stmt, 1);
        }
        sqlite3_finalize(stmt);
    }

    if (rowId != -1) {
        bootCount++;
        const char* updateQuery = "UPDATE log SET boot_count = ? WHERE id = ?;";
        if (sqlite3_prepare_v2(db, updateQuery, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, bootCount);
            sqlite3_bind_int(stmt, 2, rowId);
            sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        const char* insertQuery = "INSERT INTO log (date, boot_count, is_password_entered) VALUES (?, 1, 0);";
        if (sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
}

void check_opened::runner() {
    logAppOpened();
}

void check_opened::checkPreviousDayLogin() {
    time_t now = time(0);
    struct tm ltm;
    localtime_s(&ltm, &now);

    ltm.tm_mday -= 1;
    mktime(&ltm);

    std::stringstream ss;
    ss << (1900 + ltm.tm_year) << "-" << (1 + ltm.tm_mon) << "-" << ltm.tm_mday;
    std::string prevDate = ss.str();

    sqlite3_stmt* stmt;
    const char* checkQuery = "SELECT is_password_entered FROM log WHERE date = ?;";

    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, prevDate.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int passwordEntered = sqlite3_column_int(stmt, 0);
            if (!passwordEntered) {
                FileCreator fileCreator;
                fileCreator.runner();
            }
        }
        sqlite3_finalize(stmt);
    }
}
