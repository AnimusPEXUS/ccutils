#include <wayround_i2p/ccutils/logger/logger.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

using UString = wayround_i2p::ccutils::unicode::UString;

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_001(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    wayround_i2p::ccutils::unicode::UString x = "тест";

    logger->Log(
        wayround_i2p::ccutils::logger::Status,
        std::format("test string is \"{}\": it's length is {}", x, x.length())
    );

    return {
        x.length() == 4
    };
}

wayround_i2p::ccutils::tst::TSTInfo main_001_i = {
    .group_name = "main",
    .test_name  = "001",
    .func       = main_001
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_002(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    bool ok = false;

    const auto ex_res1 = UString("--------------------------------------тест--------------------------------------");
    const auto ex_res2 = UString("-------------------------------------тест2--------------------------------------");

    auto res_str = UString("тест").center(80, '-');

    ok = res_str == ex_res1;

    logger->LogSplitLines(
        (ok ? wayround_i2p::ccutils::logger::Success : wayround_i2p::ccutils::logger::Failure),
        std::format("must be  : {}\nactually : {}", ex_res1, res_str)
    );

    res_str = UString("тест2").center(80, '-');

    ok = (ok && res_str == ex_res2);

    logger->LogSplitLines(
        (ok ? wayround_i2p::ccutils::logger::Success : wayround_i2p::ccutils::logger::Failure),
        std::format("must be  : {}\nactually : {}", ex_res2, res_str)
    );

    return {ok};
}

wayround_i2p::ccutils::tst::TSTInfo main_002_i = {
    .group_name = "main",
    .test_name  = "002",
    .func       = main_002
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_003(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{

    std::size_t error_count = 0;

    const auto ex_res1 = UString("abcdef12345test");

    if (ex_res1.index("test") == 11)
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res1.index("testq") == -1)
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res1.index("test", 12) == -1)
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res1.rindex("test") == 11)
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res1.rindex("abc") == 0)
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res1.rindex("abc", 1) == -1)
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    return {error_count == 0};
}

wayround_i2p::ccutils::tst::TSTInfo main_003_i = {
    .group_name  = "main",
    .test_name   = "003",
    .description = "UString index",
    .func        = main_003
};
