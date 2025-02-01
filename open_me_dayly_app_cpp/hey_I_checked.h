#ifndef HEY_I_CHECKED_H
#define HEY_I_CHECKED_H

#include <sqlite3.h>
#include <string>

#ifdef BUILDING_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

class DLL_EXPORT hey_I_checked {
public:
    hey_I_checked();
    ~hey_I_checked();
    void runner();
    void markPasswordEntered();

    void showLogs();

private:
    void executeSQL(const std::string& sql);
    sqlite3* db;
    const char* db_path = "log.db";
};

#endif
