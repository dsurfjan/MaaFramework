#include "Utils/Logger.h"

#pragma message("MAA_VERSION: " MAA_VERSION)

MAA_NS_BEGIN

static constexpr std::string_view kSplitLine = "-----------------------------";

Logger& Logger::get_instance()
{
    static Logger unique_instance;
    return unique_instance;
}

void Logger::start_logging(std::filesystem::path dir)
{
    log_dir_ = std::move(dir);
    log_path_ = log_dir_ / kLogFilename;
    reinit();
}

void Logger::flush()
{
    internal_dbg() << kSplitLine;
    internal_dbg() << "Flush log";
    internal_dbg() << kSplitLine;

    bool rotated = rotate();
    open();

    if (rotated) {
        log_proc_info();
    }
}

void Logger::reinit()
{
    rotate();
    open();
    log_proc_info();
}

bool Logger::rotate()
{
    if (log_path_.empty() || !std::filesystem::exists(log_path_)) {
        return false;
    }

    std::unique_lock<std::mutex> m_trace_lock(trace_mutex_);
    if (ofs_.is_open()) {
        ofs_.close();
    }

    constexpr uintmax_t MaxLogSize = 4ULL * 1024 * 1024;
    const uintmax_t log_size = std::filesystem::file_size(log_path_);
    if (log_size < MaxLogSize) {
        return false;
    }

    const std::filesystem::path bak_path = log_dir_ / kLogbakFilename;
    try {
        std::filesystem::rename(log_path_, bak_path);
    }
    catch (...) {
        return false;
    }

    return true;
}

void Logger::open()
{
    if (log_path_.empty()) {
        return;
    }

    std::filesystem::create_directories(log_dir_);

    std::unique_lock<std::mutex> m_trace_lock(trace_mutex_);
    if (ofs_.is_open()) {
        ofs_.close();
    }
    ofs_.open(log_path_, std::ios::out | std::ios::app);
}

void Logger::close()
{
    internal_dbg() << kSplitLine;
    internal_dbg() << "Close log";
    internal_dbg() << kSplitLine;

    std::unique_lock<std::mutex> m_trace_lock(trace_mutex_);
    if (ofs_.is_open()) {
        ofs_.close();
    }
}

void Logger::log_proc_info()
{
    internal_dbg() << kSplitLine;
    internal_dbg() << "MAA Process Start";
    internal_dbg() << "Version" << MAA_VERSION;
    internal_dbg() << "Built at" << __DATE__ << __TIME__;
    internal_dbg() << "Log Path" << log_path_;
    internal_dbg() << kSplitLine;
}

Logger::LogStream Logger::internal_dbg()
{
    return debug("Logger");
}

MAA_NS_END
