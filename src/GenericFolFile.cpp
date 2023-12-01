// Purpose: Source file for GenericFolFile class.
// Author: Arda Sarp Yenicesu.
// Date: 2023/11/17

#include "GenericFolFile.h"
#include <fstream>
#include <iostream>
#include <utility>

GenericFolFile::GenericFolFile(std::string originalFilePath, std::string modifiedFilePath)
        : originalFilePath(std::move(originalFilePath)), modifiedFilePath(std::move(modifiedFilePath)) {}

void GenericFolFile::createModifiedFolFile(const std::string &extraSection) {
    std::string content = readFile(originalFilePath);
    content += "\n" + extraSection;
    writeFile(modifiedFilePath, content);
}

void GenericFolFile::deleteModifiedFolFile() {
    if (!modifiedFilePath.empty() && remove(modifiedFilePath.c_str()) != 0) {
        std::cerr << "Error deleting file: " << modifiedFilePath << std::endl;
    }
}

std::string GenericFolFile::readFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

void GenericFolFile::writeFile(const std::string &filePath, const std::string &content) {
    std::ofstream file(filePath);
    if (!file) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return;
    }

    file << content;
}

std::string GenericFolFile::getModifiedFilePath() {
    return modifiedFilePath;
}
