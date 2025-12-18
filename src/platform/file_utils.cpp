#include "platform/file_utils.h"
#include <filesystem>
#include <fstream>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

namespace platform {

// 文件路径操作
std::string get_current_directory() {
    return std::filesystem::current_path().string();
}

std::string get_executable_path() {
#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if (length > 0) {
        return std::string(buffer);
    }
#endif
    return "";
}

bool file_exists(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}

bool directory_exists(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

// 路径分隔符处理
std::string normalize_path(const std::string& path) {
    // 将路径中的反斜杠替换为正斜杠（如果需要）
    std::string normalized = path;
    for (auto& c : normalized) {
        if (c == '\\') {
            c = '/';
        }
    }
    return normalized;
}

std::string join_paths(const std::string& base, const std::string& relative) {
    return (std::filesystem::path(base) / relative).string();
}

// 文件读写操作
bool read_file_to_string(const std::string& path, std::string& content) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    content.assign((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
    return true;
}

bool write_string_to_file(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }

    file << content;
    return true;
}

// 目录遍历
std::vector<std::string> list_directory(const std::string& path) {
    std::vector<std::string> result;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            result.push_back(entry.path().string());
        }
    } catch (...) {
        // 忽略错误
    }
    return result;
}

} // namespace platform