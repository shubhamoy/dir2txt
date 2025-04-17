// src/FileWriterText.cpp
#include "FileWriterText.h"
#include "CommentStripper.h"
#include <iostream>
#include <fstream>

FileWriterText::FileWriterText(const std::string& outputFile, bool stripComments)
    : stripComments(stripComments) {
    out.open(outputFile);
    if (!out) {
        std::cerr << "❌ Failed to open output file: " << outputFile << std::endl;
        std::exit(1);
    }
}

FileWriterText::~FileWriterText() {
    if (out.is_open()) out.close();
}

void FileWriterText::writeHeader(const std::filesystem::path& rootPath) {
    out << "==============================\n";
    out << "📁 Directory Tree of: " << rootPath << "\n";
    out << "==============================\n\n";
}

void FileWriterText::writeSectionDivider(const std::string& title) {
    out << "\n\n==============================\n";
    out << title << "\n";
    out << "==============================\n";
}

void FileWriterText::writeLine(const std::string& line) {
    out << line << "\n";
}

void FileWriterText::writeTreeNode(const std::filesystem::path& relPath, const std::string& type) {
    std::string prefix = "├── ";
    std::string line = prefix + relPath.string();
    if (type == "dir") line += "/";
    writeLine(line);
}

void FileWriterText::writeFileContents(const std::filesystem::path& filePath,
                                   const std::filesystem::path& rootPath) {
    std::string relPath = std::filesystem::relative(filePath, rootPath).string();

    out << "\n";
    out << "==============================\n";
    out << "📄 File: " << relPath << "\n";
    out << "------------------------------\n";

    if (stripComments) {
        auto lines = CommentStripper::strip(filePath);
        for (const auto& line : lines) {
            out << line << "\n";
        }
    } else {
        std::ifstream file(filePath);
        if (!file) {
            out << "[Unable to Read File]\n";
        } else {
            std::string line;
            while (std::getline(file, line)) {
                out << line << "\n";
            }
        }
    }

    out << "------------------------------\n";
    out << "📄 End: " << relPath << "\n";
    out << "==============================\n";
}

void FileWriterText::writeFileSkipped(const std::filesystem::path& filePath,
                                  const std::filesystem::path& rootPath,
                                  const std::string& reason) {
    std::string relPath = std::filesystem::relative(filePath, rootPath).string();
    out << "\n";
    out << "==============================\n";
    out << "📄 File: " << relPath << "\n";
    out << "------------------------------\n";
    out << reason << "\n";
    out << "------------------------------\n";
    out << "📄 End: " << relPath << "\n";
    out << "==============================\n";
}