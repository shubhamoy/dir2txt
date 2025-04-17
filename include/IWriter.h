// include/IWriter.h
#pragma once

#include <filesystem>
#include <string>

class IWriter {
public:
    virtual ~IWriter() = default;

    virtual void writeHeader(const std::filesystem::path& rootPath) = 0;
    virtual void writeSectionDivider(const std::string& title) = 0;
    virtual void writeLine(const std::string& line) = 0;

    virtual void writeFileContents(const std::filesystem::path& filePath,
                                   const std::filesystem::path& rootPath) = 0;

    virtual void writeFileSkipped(const std::filesystem::path& filePath,
                                  const std::filesystem::path& rootPath,
                                  const std::string& reason) = 0;
    
    virtual void writeTreeNode(const std::filesystem::path& relPath,
                                    const std::string& type) = 0;
};