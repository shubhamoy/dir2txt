// include/FileWriterJson.h
#pragma once

#include "IWriter.h"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <string>

class FileWriterJson : public IWriter {
public:
    explicit FileWriterJson(const std::string& outputFile = "output.json", bool stripComments = false);
    ~FileWriterJson();

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
    nlohmann::json j;
    std::ofstream out;
    std::string outputFileName;
    std::filesystem::path root;
    bool stripComments;
};
