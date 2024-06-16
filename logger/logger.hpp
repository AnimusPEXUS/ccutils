#ifndef WAYROUND_I2P_20240616_100728_802316
#define WAYROUND_I2P_20240616_100728_802316

#include <algorithm>
#include <any>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include <experimental/scope>

#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::logger
{

// todo: make ref variants for Log functions?

enum LoggerMSGType : unsigned char
{
    Status,
    Text,
    Info,
    Warning,
    Error,
    Failure,
    ExpectedFailure,
    Success,
    UnexpectedSuccess,
    ToDo,
    FixMe,
};

using LoggerFunctionCB_T = std::function<
    void(
        wayround_i2p::ccutils::logger::LoggerMSGType,
        wayround_i2p::ccutils::unicode::UString
    )>;

class LoggerI
{
  public:
    virtual void Log(
        LoggerMSGType                           t,
        wayround_i2p::ccutils::unicode::UString msg
    ) const
        = 0;
};

using LoggerI_shared = std::shared_ptr<LoggerI>;

class TerminalLogger : public LoggerI
{
  public:
    static std::shared_ptr<TerminalLogger> create();

    void Log(
        LoggerMSGType                           t,
        wayround_i2p::ccutils::unicode::UString msg
    ) const;

    ~TerminalLogger();

  protected:
    TerminalLogger();

  private:
    // std::weak_ptr<TerminalLogger> own_ptr;
};

wayround_i2p::ccutils::unicode::UString icon_by_type(LoggerMSGType);
wayround_i2p::ccutils::unicode::UString timestamp();

} // namespace wayround_i2p::ccutils::logger

#endif
