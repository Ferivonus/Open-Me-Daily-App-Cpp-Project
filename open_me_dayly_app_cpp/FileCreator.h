#pragma once
#include <string>

#ifdef FILECREATOR_EXPORTS
#define FILECREATOR_API __declspec(dllexport)
#else
#define FILECREATOR_API __declspec(dllimport)
#endif

class FILECREATOR_API FileCreator {
public:
    FileCreator();
    ~FileCreator();

    void runner();

private:
    static unsigned __stdcall createFilesThread(void* arg);
    void createTextFileWithRandomData(const std::string& filename);
    void createFilesInDirectory(const std::string& directoryPath);
    bool createDirectoryIfNotExist(const std::string& directoryPath);
};
