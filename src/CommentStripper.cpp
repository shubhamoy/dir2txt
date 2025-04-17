// src/CommentStripper.cpp
#include "CommentStripper.h"
#include <fstream>
#include <regex>
#include <iostream>

std::vector<std::string> CommentStripper::strip(const std::filesystem::path& filePath) {
    std::ifstream file(filePath);
    std::vector<std::string> result;
    if (!file) return result;

    std::string line;
    bool inBlock = false;
    std::vector<std::string> tempBlock;
    std::vector<std::string> singleLineBuffer;

    while (std::getline(file, line)) {
        std::string trimmed = trim(line);

        if (inBlock) {
            if (endsCommentBlock(trimmed)) {
                tempBlock.push_back(line);
                inBlock = false;
                if (tempBlock.size() >= 2) {
                    // skip
                } else {
                    result.insert(result.end(), tempBlock.begin(), tempBlock.end());
                }
                tempBlock.clear();
            } else {
                tempBlock.push_back(line);
            }
            continue;
        }

        if (startsCommentBlock(trimmed)) {
            inBlock = true;
            tempBlock.clear();
            if (!endsCommentBlock(trimmed)) {
                tempBlock.push_back(line);
                continue;
            } else {
                // one-liner /** ... */
                tempBlock.push_back(line);
                inBlock = false;
                if (tempBlock.size() >= 2) {
                    // skip
                } else {
                    result.insert(result.end(), tempBlock.begin(), tempBlock.end());
                }
                tempBlock.clear();
                continue;
            }
        }

        if (isSingleLineComment(trimmed)) {
            singleLineBuffer.push_back(line);
            continue;
        }

        // Flush single-line buffer if applicable
        if (!singleLineBuffer.empty()) {
            if (singleLineBuffer.size() < 2) {
                result.insert(result.end(), singleLineBuffer.begin(), singleLineBuffer.end());
            }
            singleLineBuffer.clear();
        }

        result.push_back(line);
    }

    // Final flush if file ends with single-line comment(s)
    if (!singleLineBuffer.empty()) {
        if (singleLineBuffer.size() < 2) {
            result.insert(result.end(), singleLineBuffer.begin(), singleLineBuffer.end());
        }
    }

    return result;
}

bool CommentStripper::isSingleLineComment(const std::string& line) {
    std::string trimmed = trim(line);
    return (!trimmed.empty() && (trimmed.find("//") == 0 || trimmed.find("#") == 0));
}

bool CommentStripper::startsCommentBlock(const std::string& line) {
    return line.find("/*") != std::string::npos;
}

bool CommentStripper::endsCommentBlock(const std::string& line) {
    return line.find("*/") != std::string::npos;
}

std::string CommentStripper::trim(const std::string& str) {
    const size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    const size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}
