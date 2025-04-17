// src/FileFilter.cpp
#include "FileFilter.h"
#include "Utils.h"
#include <fstream>

const std::set<std::string> FileFilter::alwaysIgnored = {
    ".git", ".svn", ".hg", ".cache"
};

FileFilter::FileFilter(bool includeDotfiles,
                       std::set<std::string> ignoredDirs,
                       const std::filesystem::path& rootPath)
    : includeDotfiles(includeDotfiles),
      ignoredDirs(std::move(ignoredDirs)),
      root(rootPath),
      ignoreParser(std::make_unique<UniversalIgnoreParser>()) {
        ignoreParser->loadFromDirectory(root);
        // Apply --ignore overrides
        for (const auto& entry : this->ignoredDirs) {
            ignoreParser->addManualIgnore(entry);
        }
    }

bool FileFilter::isBinary(const std::filesystem::path& filePath) const {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) return true;

    const size_t maxBytes = 4096;
    char buffer[maxBytes];
    file.read(buffer, maxBytes);
    std::streamsize bytesRead = file.gcount();

    size_t nonPrintable = 0;
    for (std::streamsize i = 0; i < bytesRead; ++i) {
        unsigned char c = static_cast<unsigned char>(buffer[i]);
        if (c == 9 || c == 10 || c == 13) continue;
        if (c < 32 || c > 126) ++nonPrintable;
        // 🚀 Early exit: if threshold is crossed
        if (nonPrintable > 0.3 * (i + 1)) return true;
    }
    return false;
}

bool FileFilter::shouldIgnore(const std::filesystem::path& path) const {
    std::string name = path.filename().string();

    if (alwaysIgnored.count(name)) return true;
    if (!includeDotfiles && Utils::startsWithDot(path)) return true;

    std::filesystem::path relPath = std::filesystem::relative(path, root);
    if (ignoreParser->shouldIgnore(relPath)) return true;

    return false;
}
