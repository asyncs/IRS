#include "GenericFolFile.h"
#include <fstream>
#include <iostream>
#include <utility>
#include <filesystem>

namespace fs = std::filesystem;

GenericFolFile::GenericFolFile(std::string originalFilePath, std::string modifiedFilePath)
        : originalFilePath(std::move(originalFilePath)), modifiedFilePath(std::move(modifiedFilePath)) {}

void GenericFolFile::createModifiedFolFile(const std::string &extraSection) const {
    std::string content = readFile(originalFilePath);
    content += "\n" + extraSection;
    writeFile(modifiedFilePath, content);
}

void GenericFolFile::deleteModifiedFolFile() const {
    if (!modifiedFilePath.empty() && fs::exists(modifiedFilePath)) {
        try {
            fs::remove(modifiedFilePath);
        } catch (const fs::filesystem_error &e) {
            std::cerr << "Error deleting file: " << e.what() << std::endl;
        }
    }
}

std::string GenericFolFile::readFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

void GenericFolFile::writeFile(const std::string &filePath, const std::string &content) {
    std::ofstream file(filePath);
    if (!file) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    file << content;
}

std::string GenericFolFile::getModifiedFilePath() const {
    return modifiedFilePath;
}
