// include/IFileFilter.h
#pragma once

#include <filesystem>

class IFileFilter {
public:
    virtual ~IFileFilter() = default;

    virtual bool isBinary(const std::filesystem::path& filePath) const = 0;
    virtual bool shouldIgnore(const std::filesystem::path& path) const = 0;
};