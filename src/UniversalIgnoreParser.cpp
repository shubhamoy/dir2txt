// src/UniversalIgnoreParser.cpp
#include "UniversalIgnoreParser.h"
#include <fstream>
#include <iostream>

void UniversalIgnoreParser::loadFromDirectory(const std::filesystem::path& root) {
    for (const auto& ignoreFile : knownIgnoreFiles) {
        loadFromFile(root / ignoreFile);
    }
}

void UniversalIgnoreParser::loadFromFile(const std::filesystem::path& file) {
    if (!std::filesystem::exists(file)) return;

    std::ifstream in(file);
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        try {
            patterns.push_back(convertToRegex(line));
        } catch (...) {
            std::cerr << "⚠️ Failed to parse ignore pattern: " << line << std::endl;
        }
    }
}

void UniversalIgnoreParser::addManualIgnore(const std::string& pattern) {
    try {
        patterns.push_back(convertToRegex(pattern));
    } catch (...) {
        std::cerr << "⚠️ Failed to compile manual ignore: " << pattern << std::endl;
    }
}

bool UniversalIgnoreParser::shouldIgnore(const std::filesystem::path& relPath) const {
    std::string pathStr = relPath.generic_string();

    auto it = cache.find(pathStr);
    if (it != cache.end()) {
        return it->second;
    }

    for (const auto& regex : patterns) {
        if (std::regex_match(pathStr, regex)) {
            cache[pathStr] = true;
            return true;
        }
    }

    cache[pathStr] = false;
    return false;
}

std::regex UniversalIgnoreParser::convertToRegex(const std::string& pattern) const {
    std::string regexStr = "^";

    if (pattern.back() == '/') {
        std::string dirName = pattern.substr(0, pattern.length() - 1);
        regexStr += "(" + dirName + ")(/.*)?$";
    } else if (pattern.find('/') == std::string::npos && pattern.find('*') == std::string::npos) {
        regexStr = ".*/" + pattern + "$|^" + pattern + "$";
    } else {
        for (char c : pattern) {
            switch (c) {
                case '*': regexStr += ".*"; break;
                case '.': regexStr += "\\."; break;
                case '/': regexStr += "/"; break;
                default:  regexStr += c; break;
            }
        }
        regexStr += "$";
    }

    return std::regex(regexStr);
}