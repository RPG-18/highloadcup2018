#include "Logger.h"

#include "spdlog/sinks/basic_file_sink.h"

namespace spd = spdlog;

namespace {
Logger* lg = nullptr;
}

Logger::Logger()
{
    m_logger = spd::basic_logger_mt("basic_logger", "/tmp/highloadcup-log.txt");
}


Logger* Logger::instance()
{
    if(lg == nullptr) {
        lg = new Logger();
        std::atexit([]{
            delete lg;
        });
    }

    return lg;
}

std::shared_ptr<spdlog::logger> Logger::log()
{
    return m_logger;
}
