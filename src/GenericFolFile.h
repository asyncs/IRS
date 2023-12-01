// Purpose: Header file for GenericFolFile class.
// Author: Arda Sarp Yenicesu.
// Date: 2023/11/17

#ifndef CA_TAMP_GENERICFOLFILE_H
#define CA_TAMP_GENERICFOLFILE_H

#include <string>

/**
 * @brief This class is used to create and delete a modified first order logic file
 */
class GenericFolFile {
public:
    /**
     * @brief Constructor
     * @param originalFilePath Path to the original file
     * @param modifiedFilePath Path to the modified file
     */
    GenericFolFile(std::string originalFilePath, std::string modifiedFilePath);

    /**
     * @brief Creates the modified file
     * @param extraSection The extra section to be added to the end of the file
     */
    void createModifiedFolFile(const std::string &extraSection);

    /**
     * @brief Deletes the modified file
     */
    void deleteModifiedFolFile();

    /**
     * @brief Getter for the modified file path
     * @return The modified file path
     */
    std::string getModifiedFilePath();
private:
    std::string originalFilePath; ///< Path to the original file
    std::string modifiedFilePath; ///< Path to the modified file
    /**
     * @brief Reads the content of a file
     * @param filePath Path to the file
     * @return The content of the file
     */
    static std::string readFile(const std::string &filePath);

    /**
     * @brief Writes the content to a file
     * @param filePath Path to the file
     * @param content The content to be written
     */
    static void writeFile(const std::string &filePath, const std::string &content);
};


#endif //CA_TAMP_GENERICFOLFILE_H
