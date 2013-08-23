#include "GPLogger.h"

#include <cassert>

/*
    GPLog
*/

GPLog::GPLog(GPLogParams* params, std::string output_file_path, GPLog* forward, std::size_t buff_sz) :
    params(params), forward(forward),
    sink_(), buffer_(buff_sz + 1)
{
    if (params->to_file) {
        log_file_stream.open(output_file_path.c_str());
    }

    sink_.clear();
    char *base = &buffer_.front();
    setp(base, base + buffer_.size() - 1);
}

GPLog::~GPLog()
{}

GPLog::int_type GPLog::overflow(int_type ch)
{
    if (sink_ && ch != EOF)
    {
        assert(std::less_equal<char *>()(pptr(), epptr()));
        *pptr() = char(ch);
        pbump(1);
        if (forward_and_flush())
            return ch;
    }

    return EOF;
}

int GPLog::sync()
{
    return forward_and_flush() ? 0 : -1;
}

bool GPLog::forward_and_flush()
{
    for (char *p = pbase(), *e = pptr(); p != e; ++p)
    {
		/*
        if (*p == '.')
            cap_next_ = true;
        else if (std::isalpha(*p))
        {
            if (cap_next_)
                *p = char(std::toupper(*p));

            cap_next_ = false;
        }
        if (params->to_file)
            log_file_oseam << os << std::endl;

        if (params->to_cout)
            std::cout << os << std::endl;

        if (params->to_cerr)
            std::cerr << os << std::endl;

        if (forward)
            forward << os;
		*/
		std::cerr << *p;
    }
    std::ptrdiff_t n = pptr() - pbase();
    pbump(-n);

    return sink_.write(pbase(), n);
}

/*
    GPLogger
*/

GPLogger::GPLogger(GPLoggerParams* params, std::string seed_string, std::string output_dir_path) :
    params(params), seed_string(seed_string),
    log_buff(params->log_params, output_dir_path + seed_string + ".log", NULL),
    verbose_buff(params->verbose_params, output_dir_path + seed_string + ".log.verbose", NULL),
    debug_buff(params->debug_params, output_dir_path + seed_string + ".debug", NULL),
    error_buff(params->error_params, output_dir_path + seed_string + ".error", NULL),
    log(&log_buff),
    verbose(&verbose_buff),
    debug(&debug_buff),
    error(&error_buff)
{
}

GPLogger::~GPLogger() {
}

std::string GPLogger::get_seed_string() {
    return seed_string;
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
