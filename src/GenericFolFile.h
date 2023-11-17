//
// Created by asy on 17.11.2023.
//

#ifndef CA_TAMP_GENERICFOLFILE_H
#define CA_TAMP_GENERICFOLFILE_H

#include <string>

class GenericFolFile {
public:
    GenericFolFile(std::string  originalFilePath, std::string  modifiedFilePath);
    void createModifiedFile(const std::string& extraSection);
    void deleteModifiedFile();

private:
    std::string originalFilePath;
    std::string modifiedFilePath;
    static std::string readFile(const std::string& filePath);
    static void writeFile(const std::string& filePath, const std::string& content);
};


#endif //CA_TAMP_GENERICFOLFILE_H
