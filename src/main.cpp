// src/main.cpp
#include <iostream>
#include "Config.h"
#include "FileFilter.h"
#include "FileWriterText.h"
#include "FileWriterJson.h"
#include "DirectoryWalker.h"
#include "Utils.h"

#include <memory>

int main(int argc, char* argv[]) {
    std::cout << R"(
       _ _     ____  _        _     _                 _           _     _                                 
    __| (_)_ _|___ \| |___  _| |_  | |__  _   _   ___| |__  _   _| |__ | |__   __ _ _ __ ___   ___  _   _ 
   / _  | |  __|__) | __\ \/ / __| |  _ \| | | | / __|  _ \| | | |  _ \|  _ \ / _  |  _   _ \ / _ \| | | |
  | (_| | | |  / __/| |_ >  <| |_  | |_) | |_| | \__ \ | | | |_| | |_) | | | | (_| | | | | | | (_) | |_| |
   \____|_|_| |_____|\__/_/\_\\__| |____/ \__/ / |___/_| |_|\____|____/|_| |_|\__,_|_| |_| |_|\___/ \__/ /
                                          |___/                                                     |___/ 
       )" << "\n\n";

    Config config(argc, argv);

    auto filter = std::make_shared<FileFilter>(
        config.shouldIncludeDotfiles(),
        config.getIgnoredDirs(),
        config.getRootPath()
    );

    std::string outputFilename = Utils::generateOutputFilename(config.getRootPath());
    std::shared_ptr<IWriter> writer;

    if (config.outputAsJson()) {
        outputFilename = outputFilename.substr(0, outputFilename.find_last_of('.')) + ".json";
        writer = std::make_shared<FileWriterJson>(outputFilename, config.shouldStripComments());
    } else {
        writer = std::make_shared<FileWriterText>(outputFilename, config.shouldStripComments());
    }

    DirectoryWalker walker(
        config.getRootPath(),
        filter,
        writer
    );

    walker.walk();

    std::cout << "✅ Done! Output written to: " << outputFilename << "\n";
    return 0;
}