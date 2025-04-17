// src/Config.cpp
#include "Config.h"
#include "cxxopts.hpp"
#include <iostream>

Config::Config(int argc, char* argv[]) {
    try {
        cxxopts::Options options(argv[0], "dir2txt by Shubhamoy Chakrabarty <me[at]shubhamoy[dot]com>. Generate a directory tree followed by content.");

        options.positional_help("<directory>")
               .show_positional_help();

        options.add_options()
            ("i,ignore", "Comma-separated list of files/folders to ignore", cxxopts::value<std::string>())
            ("include-dotfiles", "Include hidden dotfiles and dotfolders. It will still ignore .git, .cache, etc.")
            ("s,strip-comments", "Strip comments from source files")
            ("j,json", "Output results in JSON format")
            ("h,help", "Print usage")
            ("directory", "Root directory", cxxopts::value<std::string>());

        options.parse_positional({"directory"});

        auto result = options.parse(argc, argv);

        if (result.count("help") || !result.count("directory")) {
            std::cout << options.help() << std::endl;
            std::exit(0);
        }

        rootPath = std::filesystem::path(result["directory"].as<std::string>());

        if (!std::filesystem::exists(rootPath) || !std::filesystem::is_directory(rootPath)) {
            std::cerr << "❌ Invalid directory: " << rootPath << std::endl;
            std::exit(1);
        }

        if (result.count("ignore")) {
            std::string list = result["ignore"].as<std::string>();
            size_t pos;
            while ((pos = list.find(',')) != std::string::npos) {
                ignoredDirs.insert(list.substr(0, pos));
                list.erase(0, pos + 1);
            }
            if (!list.empty()) {
                ignoredDirs.insert(list);
            }
        }

        includeDotfiles = result.count("include-dotfiles") > 0;
        stripComments = result.count("strip-comments") > 0;
        asJson = result.count("json") > 0;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Argument parsing error: " << e.what() << std::endl;
        std::exit(1);
    }
}

const std::filesystem::path& Config::getRootPath() const {
    return rootPath;
}

const std::set<std::string>& Config::getIgnoredDirs() const {
    return ignoredDirs;
}

bool Config::shouldIncludeDotfiles() const {
    return includeDotfiles;
}

bool Config::shouldStripComments() const {
    return stripComments;
}

bool Config::outputAsJson() const {
    return asJson;
}
