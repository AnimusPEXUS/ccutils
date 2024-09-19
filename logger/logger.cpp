
#include <wayround_i2p/ccutils/logger/logger.hpp>

namespace wayround_i2p::ccutils::logger
{

wayround_i2p::ccutils::unicode::UString icon_by_type(LoggerMSGType t)
{
    UString icon;

    switch (t)
    {
        default:
            icon = " ? ";
            break;
        case Text:
            icon = "   ";
            break;
        case Status:
            icon = "---";
            break;
        case Info:
            icon = " i ";
            break;
        case Warning:
            icon = " ! ";
            break;
        case Error:
            icon = " e ";
            break;
        case Failure:
            icon = " f ";
            break;
        case Success:
            icon = " s ";
            break;
        case ExpectedFailure:
            icon = " / ";
            break;
        case UnexpectedSuccess:
            icon = " u ";
            break;
        case ToDo:
            icon = "Do!";
            break;
        case FixMe:
            icon = "Fix";
            break;
    }

    return icon;
}

UString string_for_type(LoggerMSGType t)
{
    UString ret;

    switch (t)
    {
        default:
            ret = "undefined";
            break;
        case Text:
            ret = "";
            break;
        case Status:
            ret = "status";
            break;
        case Info:
            ret = "info";
            break;
        case Warning:
            ret = "!warning!";
            break;
        case Error:
            ret = "!!error!!";
            break;
        case Failure:
            ret = "failure";
            break;
        case Success:
            ret = "success";
            break;
        case ExpectedFailure:
            ret = "expected failure";
            break;
        case UnexpectedSuccess:
            ret = "unexpected failure";
            break;
        case ToDo:
            ret = "todo";
            break;
        case FixMe:
            ret = "fixme";
            break;
    }

    return ret;
}

wayround_i2p::ccutils::unicode::UString timestamp()
{
    auto t = std::chrono::utc_clock::now();
    return std::format("{0:%F}T{0:%T}z", t);
}

std::shared_ptr<TerminalLogger> TerminalLogger::create()
{
    auto ret = std::shared_ptr<TerminalLogger>(new TerminalLogger());
    return ret;
}

TerminalLogger::TerminalLogger()
{
}

TerminalLogger::~TerminalLogger()
{
}

void TerminalLogger::Log(
    LoggerMSGType t
) const
{
    std::cout << std::format(
        "[{}] [{}] {}",
        icon_by_type(t),
        timestamp(),
        string_for_type(t)
    )
              << std::endl;
}

void TerminalLogger::Log(
    LoggerMSGType  t,
    const UString &msg
) const
{
    std::cout << std::format(
        "[{}] [{}] {}",
        icon_by_type(t),
        timestamp(),
        msg
    )
              << std::endl;
}

void TerminalLogger::Log(
    LoggerMSGType              t,
    const std::deque<UString> &msgs
) const
{
    for (auto &msg : msgs)
    {
        this->Log(t, msg);
    }
}

void TerminalLogger::LogSplitLines(
    LoggerMSGType  t,
    const UString &msg
) const
{
    std::deque<UString> msgs;
    msgs = msg.splitlines(msgs);
    this->Log(t, msgs);
}

} // namespace wayround_i2p::ccutils::logger
