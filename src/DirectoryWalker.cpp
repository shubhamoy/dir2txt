// src/DirectoryWalker.cpp
#include "DirectoryWalker.h"
#include <filesystem>

DirectoryWalker::DirectoryWalker(
    const std::filesystem::path& root,
    std::shared_ptr<IFileFilter> filter,
    std::shared_ptr<IWriter> writer)
    : rootPath(root), filter(std::move(filter)), writer(std::move(writer)) {}

void DirectoryWalker::walk() {
    writer->writeHeader(rootPath);
    writeTree(rootPath);
    writer->writeSectionDivider("📂 Dumping file contents...");
    writeFiles(rootPath);
}

void DirectoryWalker::writeTree(const std::filesystem::path& path, const std::string& prefix) {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (filter->shouldIgnore(entry.path())) continue;

        std::filesystem::path relPath;
        try {
            relPath = std::filesystem::relative(entry.path(), rootPath);
        } catch (...) {
            relPath = entry.path().filename();  // fallback
        }

        if (entry.is_directory()) {
            writer->writeTreeNode(relPath, "dir");
            writeTree(entry.path(), prefix + "│   ");
        } else {
            writer->writeTreeNode(relPath, "file");
        }
    }
}

void DirectoryWalker::writeFiles(const std::filesystem::path& path) {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (filter->shouldIgnore(entry.path())) continue;

        if (entry.is_directory()) {
            writeFiles(entry.path());
        } else if (entry.is_regular_file()) {
            if (filter->isBinary(entry.path())) {
                writer->writeFileSkipped(entry.path(), rootPath, "[BINARY FILE SKIPPED]");
            } else {
                writer->writeFileContents(entry.path(), rootPath);
            }
        }
    }
}