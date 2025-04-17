// include/UniversalIgnoreParser
#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>

class UniversalIgnoreParser {
public:
    void loadFromDirectory(const std::filesystem::path& root);
    void addManualIgnore(const std::string& pattern);
    bool shouldIgnore(const std::filesystem::path& relPath) const;

private:
    std::vector<std::regex> patterns;
    mutable std::unordered_map<std::string, bool> cache;  // ✅ Cache for performance

    std::regex convertToRegex(const std::string& pattern) const;
    void loadFromFile(const std::filesystem::path& file);

    const std::vector<std::string> knownIgnoreFiles = {
        ".gitignore", ".dockerignore", ".npmignore", ".dir2txtignore"
    };
};