#pragma once 

#include <stddef.h>
#include <sstream>
#include <stdexcept>

#include <iostream>

#define DEFAULT_ORDER 2
#define LEAF_NODE_CAPACITY(order) (2*order)
#define INTERNAL_NODE_CAPACITY(order) ((2*order)+1)


enum class BorrowResult {
    LEFT,
    RIGHT,
    NONE
};

// TODO: move the exception class to the utilities project

/**
 * @brief Custom exception class derived from std::runtime_error.
 * 
 * This class provides a custom exception with additional information about the file and line where the exception occurred.
 */
class Exception : public std::runtime_error {
    std::string msg;
public:
    /**
     * @brief Constructs a my_exception object with the given error message, file, and line.
     * 
     * @param arg The error message.
     * @param file The file where the exception occurred.
     * @param line The line number where the exception occurred.
     */
    Exception(const std::string &arg, const char *file, int line) :
    std::runtime_error(arg) {
        std::ostringstream o;
        o << file << ":" << line << ": " << arg;
        msg = o.str();
    }
    ~Exception() throw() {}

    /**
     * @brief Returns the error message with the file and line information.
     * 
     * @return The error message with the file and line information.
     */
    const char *what() const throw() {
        return msg.c_str();
    }
};

/**
 * @brief Macro for throwing a my_exception with the given error message, file, and line.
 * 
 * This macro simplifies the process of throwing a my_exception by automatically including the file and line information.
 * 
 * @param arg The error message.
 */
#define THROW_EXCEPTION(arg) throw my_exception(arg, __FILE__, __LINE__);