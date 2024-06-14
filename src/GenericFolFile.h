#ifndef CA_TAMP_GENERICFOLFILE_H
#define CA_TAMP_GENERICFOLFILE_H

#include <string>
#include <filesystem>

class GenericFolFile {
public:
 GenericFolFile(std::string originalFilePath, std::string modifiedFilePath);
 void createModifiedFolFile(const std::string &extraSection) const;
 void deleteModifiedFolFile() const;
 [[nodiscard]] std::string getModifiedFilePath() const;
private:
 std::string originalFilePath; ///< Path to the original file
 std::string modifiedFilePath; ///< Path to the modified file
 static std::string readFile(const std::string &filePath);
 static void writeFile(const std::string &filePath, const std::string &content);
};

#endif // CA_TAMP_GENERICFOLFILE_H
