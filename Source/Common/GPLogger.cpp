#include "GPLogger.h"

/*
    GPLog
*/

GPLog::GPLog(GPLogParams* params, std::string output_file_path, GPLog* forward) :
    params(params), forward(forward)
{
    if (params->log_to_file) {
        log_stream.open(output_file_path.c_str());
    }
}

std::ostream& GPLog::operator<<(std::ostream& os) {
    if (params->to_file)
        log_file_oseam << os << std::endl;

    if (params->to_cout)
        std::cout << os << std::endl;

    if (params->to_cerr)
        std::cerr << os << std::endl;

    if (forward)
        forward << os;

    return os;
}

/*
    GPLogger
*/

GPLogger::GPLogger(GPLoggerParams* params, unsigned seed, std::string output_dir_path) :
    params(params), seed(seed)
{
    std::stringstream seed_stream;
    seed_stream << seed;
    std::string seed_string = seed_stream.str();

    log(params->log_params, output_dir_path + seed_string + ".log", NULL);
    verbose(params->verbose_params, output_dir_path + seed_string + ".log.verbose", log);
    debug(params->debug_params, output_dir_path + seed_string + ".debug", NULL);
    error(params->error_params, output_dir_path + seed_string + ".error", NULL);
}

GPLogger::~GPLogger() {
}

unsigned GPLogger::get_seed() {
    return seed;
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
