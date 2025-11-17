#include "log.hpp"

#include <iostream>

namespace utils {

log::log(const char *name) :  
    _name(name) {
}

log::~log() {
}

void log::emerg(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log::alert(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log::crit(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log::err(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log::warning(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log::notice(const char * format, ...) const {
    va_list args;
    va_start(args, format); 

    va_end(args);
}

void log::info(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log::debug(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

} // utils