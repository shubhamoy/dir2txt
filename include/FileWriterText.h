// include/FileWriterText.h
#pragma once

#include "IWriter.h"
#include <fstream>

class FileWriterText : public IWriter {
public:
    explicit FileWriterText(const std::string& outputFile = "output.txt", bool stripComments = false);
    ~FileWriterText();

    void writeHeader(const std::filesystem::path& rootPath) override;
    void writeSectionDivider(const std::string& title) override;
    void writeLine(const std::string& line) override;

    void writeFileContents(const std::filesystem::path& filePath,
                           const std::filesystem::path& rootPath) override;

    void writeFileSkipped(const std::filesystem::path& filePath,
                          const std::filesystem::path& rootPath,
                          const std::string& reason) override;

    void writeTreeNode(const std::filesystem::path& relPath,
                            const std::string& type) override;

private:
    std::ofstream out;
    bool stripComments;
};