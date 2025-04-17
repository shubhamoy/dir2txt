// include/Utils.h
#pragma once

#include <filesystem>
#include <string>

namespace Utils {
    std::string getRelativePath(const std::filesystem::path& path,
                                const std::filesystem::path& root);

    bool startsWithDot(const std::filesystem::path& path);
    std::string getCurrentTimestamp();
    std::string generateOutputFilename(const std::filesystem::path& rootPath);
}