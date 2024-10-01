#include <wayround_i2p/ccutils/logger/logger.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

using UString = wayround_i2p::ccutils::unicode::UString;

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_std_format(
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

wayround_i2p::ccutils::tst::TSTInfo main_std_format_i = {
    .group_name = "main",
    .test_name  = "std::format",
    .func       = main_std_format
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_center(
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

wayround_i2p::ccutils::tst::TSTInfo main_string_center_i = {
    .group_name = "main",
    .test_name  = "string::center",
    .func       = main_string_center
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_count(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    std::size_t error_count = 0;

    const auto ex_res1 = UString("abcdef12345test");

    if (ex_res1.count("e") == 2)
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

wayround_i2p::ccutils::tst::TSTInfo main_string_count_i = {
    .group_name = "main",
    .test_name  = "string::count",
    .func       = main_string_count
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_startswith(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    std::size_t error_count = 0;

    const auto ex_res1 = UString("abcdef12345test");

    if (ex_res1.startswith("abcd"))
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res1.startswith("bcde", 1))
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res1.startswith("test", 11))
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (!ex_res1.startswith("test", 11, 12))
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

wayround_i2p::ccutils::tst::TSTInfo main_string_startswith_i = {
    .group_name = "main",
    .test_name  = "string::startswith",
    .func       = main_string_startswith
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_endswith(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    std::size_t error_count = 0;

    const auto ex_res1 = UString("abcdef12345test");

    if (ex_res1.endswith("test"))
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res1.endswith("tes", -1, 14))
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

wayround_i2p::ccutils::tst::TSTInfo main_string_endswith_i = {
    .group_name = "main",
    .test_name  = "string::endswith",
    .func       = main_string_endswith
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_index(
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

wayround_i2p::ccutils::tst::TSTInfo main_string_index_i = {
    .group_name = "main",
    .test_name  = "string::index",
    .func       = main_string_index
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_expandtabs(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    std::size_t error_count = 0;

    const auto ex_res1 = UString("\tabcde\tf12345test\t");

    if (ex_res1.expandtabs() == "	abcde	f12345test	")
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

wayround_i2p::ccutils::tst::TSTInfo main_string_expandtabs_i = {
    .group_name = "main",
    .test_name  = "string::expandtabs",
    .func       = main_string_expandtabs
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_upper_isUpper(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    std::size_t error_count = 0;

    const auto ex_res1 = UString("Some simple text");
    const auto ex_res2 = UString("ALL_UPPER");

    if (!ex_res1.isUpper())
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res2.isUpper())
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res1.upper().isUpper())
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

wayround_i2p::ccutils::tst::TSTInfo main_string_upper_isUpper_i = {
    .group_name = "main",
    .test_name  = "string::upper and isUpper",
    .func       = main_string_upper_isUpper
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_capitalize(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    std::size_t error_count = 0;

    const auto ex_res1 = UString("some simple text");
    const auto ex_res2 = UString(" some simple text");

    if (ex_res1.capitalize() == "Some simple text")
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res2.capitalize() == ex_res2)
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

wayround_i2p::ccutils::tst::TSTInfo main_string_capitalize_i = {
    .group_name = "main",
    .test_name  = "string::capitalize",
    .func       = main_string_capitalize
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_title(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    std::size_t error_count = 0;

    const auto ex_res1 = UString(" some simple text");

    if (ex_res1.title() == " Some Simple Text")
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

wayround_i2p::ccutils::tst::TSTInfo main_string_title_i = {
    .group_name = "main",
    .test_name  = "string::title",
    .func       = main_string_title
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_strip(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    std::size_t error_count = 0;

    const auto ex_res1 = UString(" Some Simple Text ");

    if (ex_res1.strip() == "Some Simple Text")
    {
        logger->Log(wayround_i2p::ccutils::logger::Success);
    }
    else
    {
        logger->Log(wayround_i2p::ccutils::logger::Failure);
        error_count++;
    }

    if (ex_res1.strip({' ', 'S', 't'}) == "ome Simple Tex")
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

wayround_i2p::ccutils::tst::TSTInfo main_string_strip_i = {
    .group_name = "main",
    .test_name  = "string::strip",
    .func       = main_string_strip
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_partition(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    std::size_t error_count = 0;

    const auto ex_res1 = UString("Some Simple:Text");

    if (auto t = ex_res1.partition(":");
        std::get<0>(t) == "Some Simple"
        && std::get<1>(t) == ":"
        && std::get<2>(t) == "Text")
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

wayround_i2p::ccutils::tst::TSTInfo main_string_partition_i = {
    .group_name = "main",
    .test_name  = "string::partition",
    .func       = main_string_partition
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_removeprefix(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    std::size_t error_count = 0;

    const auto ex_res1 = UString("Some Simple Text");
    const auto ex_res2 = UString("aaa") + ex_res1;
    const auto res     = ex_res2.removeprefix("aaa");

    logger->LogSplitLines(
        wayround_i2p::ccutils::logger::Status,
        std::format(
            R"--(
ex_res1             =  {}
ex_res2             =  {}
removeprefix("aaa") == {}
)--",
            ex_res1,
            ex_res2,
            res
        )
    );

    if (res == ex_res1)
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

wayround_i2p::ccutils::tst::TSTInfo main_string_removeprefix_i = {
    .group_name = "main",
    .test_name  = "string::removeprefix",
    .func       = main_string_removeprefix
};

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_string_replace(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    std::size_t error_count = 0;

    const auto ex_res1 = UString("Some Simple Text");
    const auto ex_res2 = UString("Some Complex Text");
    const auto res     = ex_res1.replace("Simple", "Complex");

    logger->LogSplitLines(
        wayround_i2p::ccutils::logger::Status,
        std::format(
            R"--(
ex_res1                      =  {}
ex_res2                      =  {}
replace("Simple", "Complex") == {}
)--",
            ex_res1,
            ex_res2,
            res
        )
    );

    if (res == ex_res2)
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

wayround_i2p::ccutils::tst::TSTInfo main_string_replace_i = {
    .group_name = "main",
    .test_name  = "string::replace",
    .func       = main_string_replace
};
