#ifndef PLATFORM_FILE_UTILS_H
#define PLATFORM_FILE_UTILS_H

#include <string>
#include <vector>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

namespace platform {

// 文件路径操作
std::string get_current_directory();
std::string get_executable_path();
bool file_exists(const std::string& path);
bool directory_exists(const std::string& path);

// 路径分隔符处理
std::string normalize_path(const std::string& path);
std::string join_paths(const std::string& base, const std::string& relative);

// 文件读写操作
bool read_file_to_string(const std::string& path, std::string& content);
bool write_string_to_file(const std::string& path, const std::string& content);

// 目录遍历
std::vector<std::string> list_directory(const std::string& path);

} // namespace platform

#endif // PLATFORM_FILE_UTILS_H