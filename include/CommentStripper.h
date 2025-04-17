// include/CommentStripper.h
#pragma once

#include <string>
#include <vector>
#include <filesystem>

class CommentStripper {
public:
    // Strips comments from the input file and returns lines with comments removed
    static std::vector<std::string> strip(const std::filesystem::path& filePath);

private:
    static bool isSingleLineComment(const std::string& line);
    static bool startsCommentBlock(const std::string& line);
    static bool endsCommentBlock(const std::string& line);
    static std::string trim(const std::string& str);
};
