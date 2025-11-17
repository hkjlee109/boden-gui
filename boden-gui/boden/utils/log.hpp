#pragma once

namespace utils {

struct severity 
{
    enum severity_enum {
        emerg,
        alert,
        crit,
        err,
        warning,
        notice,
        info,
        debug
    };

    severity(severity_enum value) :
        value(value) {}

    const char * c_str() const 
    {
        switch(value) 
        {
            case emerg: return "emerg";
            case alert: return "alert";
            case crit: return "crit";
            case err: return "err";
            case warning: return "warning";
            case notice: return "notice";
            case info: return "info";
            case debug: return "debug";
        }
    }

    severity_enum value;
};

class log 
{
public:
    log(const char *name);
    ~log();

    void emerg(const char *, ...) const;
    void alert(const char *, ...) const;
    void crit(const char *, ...) const;
    void err(const char *, ...) const;
    void warning(const char *, ...) const;
    void notice(const char *, ...) const;
    void info(const char *, ...) const;
    void debug(const char * ...) const;

private: 
    const char *_name;
};

} // utils
