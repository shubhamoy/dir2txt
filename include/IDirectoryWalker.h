// include/IDirectoryWalker.h
#pragma once

#include <filesystem>

class IDirectoryWalker {
public:
    virtual ~IDirectoryWalker() = default;
    virtual void walk() = 0;
};