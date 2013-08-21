#ifndef GPLOGGERPARAMS_H
#define GPLOGGERPARAMS_H

struct GPLoggerParams {
    unsigned save_precision;
    unsigned print_precision;

    bool log_to_file;
    bool log_to_cout;
    bool log_to_cerr;
    bool log_verbose_to_file;
    bool log_verbose_to_cout;
    bool log_verbose_to_cerr;
    bool debug_to_file;
    bool debug_to_cout;
    bool debug_to_cerr;
};

#endif
