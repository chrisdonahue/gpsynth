#include "GPLogger.h"

GPLogger::GPLogger(GPLoggerParams* params, unsigned seed, std::string output_dir_path) :
    params(params), seed(seed)
{
    std::stringstream seed_stream;
    seed_stream << seed;

    if (params->log_to_file) {
        std::string file_path = output_dir_path + seed_stream.str() + ".log";
        log_stream.open(file_path.c_str());
    }

    if (params->log_verbose_to_file) {
        std::string file_path = output_dir_path + seed_stream.str() + ".log.verbose";
        log_verbose_stream.open(file_path.c_str());
    }

    if (params->debug_to_file) {
        std::string file_path = output_dir_path + seed_stream.str() + ".debug";
        debug_stream.open(file_path.c_str());
    }
}

unsigned GPLogger::get_seed() {
    return seed;
}

void GPLogger::log(std::string str) {
    if (params->log_to_file)
        log_stream << str << std::endl;

    if (params->log_to_cout)
        std::cout << str << std::endl;

    if (params->log_to_cerr)
        std::cerr << str << std::endl;

    log_verbose(str);
}

void GPLogger::log_verbose(std::string str) {
    if (params->log_verbose_to_file)
        log_verbose_stream << str << std::endl;

    if (params->log_verbose_to_cout)
        std::cout << str << std::endl;

    if (params->log_verbose_to_cerr)
        std::cerr << str << std::endl;
}

void GPLogger::debug(std::string str) {
    if (params->debug_to_file)
        debug_stream << str << std::endl;

    if (params->debug_to_cout)
        std::cout << str << std::endl;

    if (params->debug_to_cerr)
        std::cerr << str << std::endl;
}

std::string GPLogger::net_to_string_print(GPNetwork* net) {
    return net->toString(params->print_precision);
}

std::string GPLogger::net_to_string_save(GPNetwork* net) {
    return net->toString(params->save_precision);
}

std::string GPLogger::get_system_info() {
    std::stringstream stream;

    // print time/date
    time_t now = time(0);
    struct tm tstruct;
    char buff[200];
    tstruct = *localtime(&now);
    strftime(buff, sizeof(buff), "Date run: %m.%d.%Y\nTime run: %H.%M.%S\n", &tstruct);
    stream << buff;

    // print github commit hash
    FILE *gitid = popen("git rev-parse HEAD", "r");
    char gitbuffer[200];
    while (fgets(gitbuffer, sizeof(gitbuffer) - 1, gitid) != NULL) {
        stream << "Git commit ID: " << gitbuffer;
    }
    pclose(gitid);

    // print host machine name
    FILE *hostname = popen("hostname", "r");
    char hostbuffer[200];
    while (fgets(hostbuffer, sizeof(hostbuffer) - 1, hostname) != NULL) {
        stream << "Host name: " << hostbuffer;
    }
    pclose(hostname);

    // print host machine CPU info
    FILE *lscpu = popen("lscpu", "r");
    char lscpubuffer[1024];
    while (fgets(lscpubuffer, sizeof(lscpubuffer) - 1, lscpu) != NULL) {
        stream << lscpubuffer;
    }
    pclose(lscpu);

    // print host machine memory info
    FILE *meminfo = popen("grep \"Mem\" /proc/meminfo", "r");
    char meminfobuffer[200];
    while (fgets(meminfobuffer, sizeof(meminfobuffer) - 1, meminfo) != NULL) {
        stream << meminfobuffer;
    }
    pclose(meminfo);

    return stream.str();
}
