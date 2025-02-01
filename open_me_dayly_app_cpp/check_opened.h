#ifndef CHECK_OPENED_H // Header guard
#define CHECK_OPENED_H

#include <sqlite3.h>
#include <string>

#ifdef BUILDING_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

class DLL_EXPORT check_opened {
public:
    check_opened();
    ~check_opened();

    void runner();
    void logAppOpened();
    void checkPreviousDayLogin();

private:
    void executeSQL(const std::string& sql);

    sqlite3* db;
    const char* db_path = "log.db";
};

#endif // CHECK_OPENED_H
