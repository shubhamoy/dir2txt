// src/FileWriterJson.cpp
#include "FileWriterJson.h"
#include "CommentStripper.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
using ordered_json = nlohmann::ordered_json;

FileWriterJson::FileWriterJson(const std::string& outputFile, bool stripComments)
    : outputFileName(outputFile), stripComments(stripComments) {
    out.open(outputFile);
    if (!out) {
        std::cerr << "❌ Failed to open output file: " << outputFile << std::endl;
        std::exit(1);
    }
}

FileWriterJson::~FileWriterJson() {
    if (out.is_open()) {
        try {
            if (!j.contains("tree")) j["tree"] = ordered_json::array();
            if (!j.contains("files")) j["files"] = ordered_json::object();

            ordered_json outJson;
            outJson["root"] = root.string();         // use `root` directly
            outJson["tree"] = ordered_json(j.value("tree", nlohmann::json::array()));
            outJson["files"] = ordered_json(j.value("files", nlohmann::json::object()));
            
            std::string serialized = outJson.dump(2);
            out.write(serialized.c_str(), serialized.size());
        } catch (const std::exception& e) {
            std::cerr << "❌ Failed to write JSON: " << e.what() << std::endl;
        }
        out.close();
    }
}

void FileWriterJson::writeHeader(const std::filesystem::path& rootPath) {
    root = rootPath;
    j["root"] = rootPath.string();
    j["tree"] = nlohmann::json::array();
    j["files"] = nlohmann::json::object();
}

void FileWriterJson::writeSectionDivider(const std::string& title) {
    // Not needed for JSON
}

void FileWriterJson::writeLine(const std::string& line) {
    // Not used in JSON
}

void FileWriterJson::writeTreeNode(const std::filesystem::path& relPath, const std::string& type) {
    j["tree"].push_back({
        {"path", relPath.generic_string()},
        {"type", type}
    });
}

void FileWriterJson::writeFileContents(const std::filesystem::path& filePath,
                                       const std::filesystem::path& rootPath) {
    std::string relPath = std::filesystem::relative(filePath, rootPath).string();
    std::vector<std::string> lines;

    if (stripComments) {
        lines = CommentStripper::strip(filePath);
    } else {
        std::ifstream file(filePath);
        if (!file) {
            j["files"][relPath]["error"] = "Unable to read file";
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
    }

    j["files"][relPath]["content"] = lines;
}

void FileWriterJson::writeFileSkipped(const std::filesystem::path& filePath,
                                      const std::filesystem::path& rootPath,
                                      const std::string& reason) {
    std::string relPath = std::filesystem::relative(filePath, rootPath).string();
    j["files"][relPath]["skipped"] = true;
    j["files"][relPath]["reason"] = reason;
}
