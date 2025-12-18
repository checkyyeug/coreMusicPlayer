#include "platform/platform_utils.h"
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <sys/sysinfo.h>
    #include <unistd.h>
#elif __APPLE__
    #include <sys/types.h>
    #include <sys/sysctl.h>
    #include <unistd.h>
#endif

namespace platform {

std::string PlatformUtils::getPlatformName() {
#ifdef _WIN32
    return "Windows";
#elif __linux__
    return "Linux";
#elif __APPLE__
    return "macOS";
#else
    return "Unknown";
#endif
}

bool PlatformUtils::isWindows() {
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

bool PlatformUtils::isLinux() {
#ifdef __linux__
    return true;
#else
    return false;
#endif
}

bool PlatformUtils::isMacOS() {
#ifdef __APPLE__
    return true;
#else
    return false;
#endif
}

std::string PlatformUtils::getArchitecture() {
#ifdef _WIN32
    #ifdef _WIN64
        return "x86_64";
    #else
        return "x86";
    #endif
#elif __linux__
    #if defined(__x86_64__)
        return "x86_64";
    #elif defined(__i386__)
        return "x86";
    #elif defined(__aarch64__)
        return "ARM64";
    #elif defined(__arm__)
        return "ARM";
    #else
        return "Unknown";
    #endif
#elif __APPLE__
    #if defined(__x86_64__)
        return "x86_64";
    #elif defined(__aarch64__)
        return "ARM64";
    #else
        return "Unknown";
    #endif
#else
    return "Unknown";
#endif
}

size_t PlatformUtils::getTotalMemory() {
#ifdef _WIN32
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    return statex.ullTotalPhys;
#elif __linux__
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    return memInfo.totalram * memInfo.mem_unit;
#elif __APPLE__
    int mib[2];
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    uint64_t memSize = 0;
    size_t len = sizeof(memSize);
    sysctl(mib, 2, &memSize, &len, NULL, 0);
    return memSize;
#else
    return 0;
#endif
}

int PlatformUtils::getNumCpuCores() {
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
#elif __linux__
    return sysconf(_SC_NPROCESSORS_ONLN);
#elif __APPLE__
    int numCpus = 0;
    size_t len = sizeof(numCpus);
    sysctlbyname("hw.ncpu", &numCpus, &len, NULL, 0);
    return numCpus;
#else
    return 1;
#endif
}

std::string PlatformUtils::getTempDirectory() {
#ifdef _WIN32
    char tempPath[MAX_PATH];
    DWORD result = GetTempPathA(MAX_PATH, tempPath);
    if (result > 0 && result < MAX_PATH) {
        return std::string(tempPath);
    }
    return "C:\\temp\\";
#elif __linux__ || __APPLE__
    const char* tmpDir = getenv("TMPDIR");
    if (tmpDir != nullptr) {
        return std::string(tmpDir);
    }
    return "/tmp/";
#else
    return "/tmp/";
#endif
}

bool PlatformUtils::createDirectory(const std::string& path) {
#ifdef _WIN32
    return CreateDirectoryA(path.c_str(), NULL) != 0;
#elif __linux__ || __APPLE__
    #include <sys/stat.h>
    return mkdir(path.c_str(), 0755) == 0;
#else
    return false;
#endif
}

bool PlatformUtils::fileExists(const std::string& path) {
#ifdef _WIN32
    DWORD attributes = GetFileAttributesA(path.c_str());
    return (attributes != INVALID_FILE_ATTRIBUTES) && 
           !(attributes & FILE_ATTRIBUTE_DIRECTORY);
#elif __linux__ || __APPLE__
    #include <sys/stat.h>
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
#else
    return false;
#endif
}

bool PlatformUtils::deleteFile(const std::string& path) {
#ifdef _WIN32
    return DeleteFileA(path.c_str()) != 0;
#elif __linux__ || __APPLE__
    return unlink(path.c_str()) == 0;
#else
    return false;
#endif
}

} // namespace platform