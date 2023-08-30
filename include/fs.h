/**
 * @file fs.h
 * @brief Filesystem utility functions
*/

#pragma once

#include <nds.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Structure for storing a date
    */
    typedef struct
    {
        u16 year;
        u8 month;
        u8 day;
    } fsDate;

    /**
     * @brief Enum for storing the status of filesystem initialization
    */
    typedef enum
    {
        /**
         * @brief Filesystem was initialized with no errors
        */
        FS_INIT_STATUS_OK,

        /**
         * @brief FAT failed to initialize
        */
        FS_INIT_STATUS_FAT_ERROR,

        /**
         * @brief NitroFS failed to initialize
        */
        FS_INIT_STATUS_NITROFS_ERROR,
    } fsInitStatus;

    /**
     * @brief Initialize filesyste,
     * @return initialization status
     * @see fsInitStatus
    */
    fsInitStatus fsInit(void);

    /**
     * @brief Create a directory
     * @param name the name of the directory
    */
    void fsCreateDir(const char *name);

    /**
     * @brief Create a file
     * @param name the name of the file
    */
    void fsCreateFile(const char *name);

    /**
     * @brief Write data to a file
     * @param file file name
     * @param data the data to write
    */
    void fsWrite(const char *file, const char *data);

    /**
     * @brief Delete a file
     * @param name name of the file to delete
    */
    void fsDeleteFile(const char *name);

    /**
     * @brief Delete a directory
     * @param name name of the directory to delete
    */
    void fsDeleteDir(const char *name);

    /**
     * @brief Check if a file exists
     * @param name name of the file to check
    */
    bool fsFileExists(const char *name);

    /**
     * @brief Check if a directory exists
     * @param name name of the directory to check
    */
    bool fsFolderExists(const char *name); // TODO rename to fsDirExists (for consistency)

    /**
     * @brief Check if the given item is a directory
     * @param name name of the item
    */
    bool fsIsDir(const char *name);

    /**
     * @brief Read a file
     * @param name name of the file
     * @note This function allocates the buffer using `malloc`. Don't forget to `free` it!
    */
    char *fsReadFile(const char *name);

    /**
     * @brief Get the size of a file in bytes
     * @param name name of the file
    */
    long fsGetFileSize(const char *name);

    /**
     * @brief Get the size of a directory in bytes
     * @param name name of the directory
     *
     * This function goes through the directory and its subdirectories and adds up all the file sizes.
    */
    long fsGetDirSize(const char *name);

    /**
     * @brief Convert file size (in bytes) to a human-readable string (e.g. 1024 B -> 1 KiB)
     * @param size the file size to convert
     * @note This function allocates the buffer using `malloc`. Don't forget to `free` it!
    */
    char *fsHumanreadFileSize(double size);

    /**
     * @brief Get the creation date of a file
     * @param name name of the file to get the creation date of
    */
    fsDate fsGetFileCreationDate(const char *name);

#ifdef __cplusplus
}
#endif
