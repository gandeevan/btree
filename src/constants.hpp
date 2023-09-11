#pragma once 

#include <stddef.h>
#include <sstream>
#include <stdexcept>

#include <iostream>

#define DEFAULT_ORDER 2
#define LEAF_NODE_CAPACITY(order) (2*order)
#define INTERNAL_NODE_CAPACITY(order) ((2*order)+1)

// TODO: move this to the utilities project
class my_exception : public std::runtime_error {
    std::string msg;
public:
    my_exception(const std::string &arg, const char *file, int line) :
    std::runtime_error(arg) {
        std::ostringstream o;
        o << file << ":" << line << ": " << arg;
        msg = o.str();
    }
    ~my_exception() throw() {}
    const char *what() const throw() {
        return msg.c_str();
    }
};
#define THROW_EXCEPTION(arg) throw my_exception(arg, __FILE__, __LINE__);