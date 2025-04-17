// include/Config.h
#pragma once

#include <filesystem>   // for std::filesystem::path
#include <set>          // for std::set
#include <string>       // for std::string

class Config {
    public:
        Config(int argc, char* argv[]);
        
        const std::filesystem::path& getRootPath() const;
        const std::set<std::string>& getIgnoredDirs() const;
        bool shouldIncludeDotfiles() const;
        bool shouldStripComments() const;
        bool outputAsJson() const;
    
    private:
        std::filesystem::path rootPath;
        std::set<std::string> ignoredDirs;
        bool includeDotfiles = false;
        bool stripComments = false;
        bool asJson = false;
    };