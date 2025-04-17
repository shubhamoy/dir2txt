// src/Utils.cpp
#include "Utils.h"
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Utils {

    std::string getRelativePath(const std::filesystem::path& path,
                                const std::filesystem::path& root) {
        try {
            return std::filesystem::relative(path, root).string();
        } catch (...) {
            return path.filename().string();  // fallback
        }
    }

    bool startsWithDot(const std::filesystem::path& path) {
        std::string name = path.filename().string();
        return !name.empty() && name[0] == '.';
    }

    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm = *std::localtime(&now_time);
    
        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%d%m%y%H%M%S");
        return oss.str();
    }

    std::string generateOutputFilename(const std::filesystem::path& rootPath) {
        std::filesystem::path cleaned = rootPath;
        while (cleaned.has_filename() == false && cleaned.has_parent_path()) {
            cleaned = cleaned.parent_path();
        }
        std::string dirName = cleaned.filename().string();
        std::string timestamp = getCurrentTimestamp();
        return dirName + "_" + timestamp + "_dir2txt.txt";
    }
}