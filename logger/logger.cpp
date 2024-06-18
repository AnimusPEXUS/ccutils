
#include <wayround_i2p/ccutils/logger/logger.hpp>

namespace wayround_i2p::ccutils::logger
{

wayround_i2p::ccutils::unicode::UString icon_by_type(LoggerMSGType t)
{
    wayround_i2p::ccutils::unicode::UString icon;

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

wayround_i2p::ccutils::unicode::UString timestamp()
{
    auto t = std::chrono::utc_clock::now();
    return std::format("{0:%F}T{0:%T}z", t);
}

/*
wayround_i2p::ccutils::unicode::UString FixMsg(
    LoggerMSGType                           t,
    wayround_i2p::ccutils::unicode::UString msg
)
{
    if (t == ToDo)
    {
        msg = wayround_i2p::ccutils::unicode::UString("todo: ") + msg;
    }

    if (t == FixMe)
    {
        msg = wayround_i2p::ccutils::unicode::UString("fixme: ") + msg;
    }
    return msg;
}
*/

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
    LoggerMSGType                                  t,
    const wayround_i2p::ccutils::unicode::UString &msg
) const
{
    // msg = FixMsg(t, msg);

    std::cout << std::format(
        "[{}] [{}] {}",
        icon_by_type(t),
        timestamp(),
        msg
    )
              << std::endl;
}

void TerminalLogger::Log(
    LoggerMSGType                                              t,
    const std::deque<wayround_i2p::ccutils::unicode::UString> &msgs
) const
{
    for (auto &msg : msgs)
    {
        this->Log(t, msg);
    }
}

void TerminalLogger::LogSplitLines(
    LoggerMSGType                                  t,
    const wayround_i2p::ccutils::unicode::UString &msg
) const
{
    std::deque<wayround_i2p::ccutils::unicode::UString> msgs;
    msgs = msg.lines(msgs);
    this->Log(t, msgs);
}

} // namespace wayround_i2p::ccutils::logger
