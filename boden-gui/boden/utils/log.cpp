#include "log.hpp"

#include <iostream>

namespace boden {
namespace utils {

log_t::log_t(const char *name) :  
    _name(name) {
}

log_t::~log_t() {
}

void log_t::emerg(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log_t::alert(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log_t::crit(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log_t::err(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log_t::warning(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log_t::notice(const char * format, ...) const {
    va_list args;
    va_start(args, format); 

    va_end(args);
}

void log_t::info(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

void log_t::debug(const char *format, ...) const {
    va_list args;
    va_start(args, format);

    va_end(args);
}

} // utils
} // boden
