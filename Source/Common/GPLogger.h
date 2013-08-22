#ifndef GPLOGGER_H
#define GPLOGGER_H

#include <string>
#include <streambuf>
#include <sstream>
#include <iostream>
#include <fstream>
#include "../Synth/GPNetwork.h"

struct GPLogParams {
    bool to_file;
    bool to_cout;
    bool to_cerr;
}

// log stream
class GPLog : public std::streambuf {
    public:
        GPLog(GPLogParams* params, std::string output_file_path, GPLog* forward);
        ~GPLogger();

        std::ostream& operator<<(std::ostream& os);

    private:
        GPLog* forward;
        GPLogParams* params;
        std::ofstream log_file_stream;
};

// params for log stream
struct GPLoggerParams {
    unsigned save_precision;
    unsigned print_precision;

    GPLogParams* log_params;
    GPLogParams* log_verbose_params;
    GPLogParams* debug_params;
    GPLogParams* error_params;
};

class GPLogger {
    GPLogger(GPLoggerParams* params, unsigned seed, std::string output_dir_path);
    ~GPLogger();

    public:    
        unsigned get_seed();
        std::string get_system_info();

        std::string net_to_string_print(GPNetwork* net);
        std::string net_to_string_save(GPNetwork* net);

        GPLog& log;
        GPLog& verbose;
        GPLog& debug;
        GPLog& error;

    private:
        GPLoggerParams* params;
};

#endif
