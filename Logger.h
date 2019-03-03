#pragma once

#include "spdlog/spdlog.h"

class Logger
{
public:

    Logger();

    static Logger* instance();

    std::shared_ptr<spdlog::logger> log();

private:

    std::shared_ptr<spdlog::logger> m_logger;
};

#define LOGGER Logger::instance()->log()