#ifndef GPLOGGER_H
#define GPLOGGER_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "../Synth/GPNetwork.h"

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

class GPLogger {
public:
    GPLogger(GPLoggerParams* params, unsigned seed, std::string output_dir_path);
    ~GPLogger();

    unsigned get_seed();

    void log(std::string str);
    void log_verbose(std::string str);
    void debug(std::string str);

    std::string net_to_string_print(GPNetwork* net);
    std::string net_to_string_save(GPNetwork* net);

    std::string get_system_info();

private:
    GPLoggerParams* params;
    unsigned seed;
    std::ofstream log_stream;
    std::ofstream log_verbose_stream;
    std::ofstream debug_stream;
};

#endif
