#include <experimental/scope>

#include <wayround_i2p/ccutils/logger/logger.hpp>
#include <wayround_i2p/ccutils/regexp/regexp.hpp>

wayround_i2p::ccutils::tst::TSTFuncResult main_001(
    const wayround_i2p::ccutils::tst::TSTInfo                &func_info,
    std::map<std::string, std::any>                          &iitm,
    const wayround_i2p::ccutils::tst::LOGGER_CB_FUNCTION_TYPE logger
)
{
    logger(
        wayround_i2p::ccutils::logger::Status,
        "setting test subjects to ingroup memory"
    );

    iitm["test_subject_001"] = wayround_i2p::ccutils::unicode::UString(
        "test subject for testing regular expressions.\n\n"
        "it also contains some separate lines,\n"
        "which is cool for newline detection"
    );

    iitm["test_subject_002"] = "// todo: move here from match/search/findAll test below";

    iitm["test_subject_003"] = wayround_i2p::ccutils::unicode::UString(
        "aaaaaaaaaa"
    );

    auto t00 = wayround_i2p::ccutils::unicode::UString(
        "-----( cut here )-----"
    );

    logger(
        wayround_i2p::ccutils::logger::Status,
        std::format(
            "    test subject is:\n{1:}\n{0:}\n{1:}",
            std::any_cast<wayround_i2p::ccutils::unicode::UString>(
                iitm["test_subject_001"]
            ),
            t00
        )
    );

    return {
        true
    };
}

wayround_i2p::ccutils::tst::TSTInfo main_001_i = {
    .group_name = "main",
    .test_name  = "001",
    .func       = main_001
};

// -------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_002(
    const wayround_i2p::ccutils::tst::TSTInfo                &func_info,
    std::map<std::string, std::any>                          &iitm,
    const wayround_i2p::ccutils::tst::LOGGER_CB_FUNCTION_TYPE logger
)
{
    int failed_count = 0;

    for (
        auto &subj01 :
        std::vector<
            std::tuple<
                std::string,  // test subject
                bool,         // failure is good?
                std::function<
                    std::tuple<
                        bool, // return true if not error
                        std::string>(
                        std::string subj,
                        size_t      end
                    )> // match_end check_cb
                >>{
            {"\n",
             false,
             [](
                 std::string subj,
             size_t      end
             ) -> std::tuple<bool, std::string>
             { return std::tuple(end == 1, "must be 1"); }
            },

            {"\r\n",
             false,
             [](std::string subj,
             size_t      end
             ) -> std::tuple<bool, std::string>
             { return std::tuple(end == 2, "must be 2"); }
            },
            {"\n\r",
             false,
             [](std::string subj,
             size_t      end
             ) -> std::tuple<bool, std::string>
             { return std::tuple(end == 2, "must be 2"); }
            },
            {"\r",
             true,
             [](std::string subj,
             size_t      end
             ) -> std::tuple<bool, std::string>
             { return std::tuple(end == 0, "must be 0"); }
            },
            {"\na",
             false,
             [](std::string subj,
             size_t      end
             ) -> std::tuple<bool, std::string>
             { return std::tuple(end == 1, "must be 1"); }
            },
            {"\nb",
             false,
             [](std::string subj,
             size_t      end
             ) -> std::tuple<bool, std::string>
             { return std::tuple(end == 1, "must be 1"); }
            },
            {"\n12345",
             false,
             [](std::string subj,
             size_t      end
             ) -> std::tuple<bool, std::string>
             { return std::tuple(end == 1, "must be 1"); }
            },
            {"",
             true,
             [](std::string subj,
             size_t      end
             ) -> std::tuple<bool, std::string>
             { return std::tuple(end == 0, "must be 0"); }
            },
            {"  ",
             true,
             [](std::string subj,
             size_t      end
             ) -> std::tuple<bool, std::string>
             { return std::tuple(end == 0, "must be 0"); }
            },
    }
    )
    {

        auto p = wayround_i2p::ccutils::regexp::Pattern::create(
            wayround_i2p::ccutils::regexp::PatternType::LineSplit
        );

        auto ts   = std::get<0>(subj01);
        auto ts_u = wayround_i2p::ccutils::unicode::UString(ts);

        auto missmatch_is_ok = std::get<1>(subj01);
        auto subj_check_cb   = std::get<2>(subj01);

        logger(
            wayround_i2p::ccutils::logger::Status,
            std::format("testing subject is: {}", ts_u.repr_as_text())
        );

        auto ex01 = std::experimental::scope_exit(
            [&logger]()
            {
                logger(
                    wayround_i2p::ccutils::logger::Status,
                    ""
                );
            }
        );

        logger(
            wayround_i2p::ccutils::logger::Status,
            std::format("  missmatch is ok?: {}", missmatch_is_ok)
        );

        auto res = wayround_i2p::ccutils::regexp::match(p, ts_u);
        if (res->error)
        {
            logger(
                wayround_i2p::ccutils::logger::Error,
                std::format("   error: {}", res->error->Error())
            );
            goto error_exit;
        }

        logger(
            wayround_i2p::ccutils::logger::Status,
            std::format("   res->matched: {}", res->matched)
        );
        logger(
            wayround_i2p::ccutils::logger::Status,
            std::format("   res->match_start: {}", res->match_start)
        );
        logger(
            wayround_i2p::ccutils::logger::Status,
            std::format("   res->match_end: {}", res->match_end)
        );

        if (!res->matched)
        {
            logger(wayround_i2p::ccutils::logger::Failure, "missmatched");
            goto fail_exit;
        }

        if (res->match_start != 0)
        {
            logger(
                wayround_i2p::ccutils::logger::Failure,
                std::format(
                    "invalid match_start (must be 0): {}",
                    res->match_start
                )
            );
            goto fail_exit;
        }

        {
            auto res2 = subj_check_cb(ts, res->match_end);

            if (!std::get<0>(res2))
            {
                logger(
                    wayround_i2p::ccutils::logger::Failure,
                    std::format(
                        "invalid match_end ({}): {}",
                        std::get<1>(res2),
                        res->match_end
                    )
                );
                goto fail_exit;
            }
        }

        {
            logger(
                wayround_i2p::ccutils::logger::Success,
                "  ok"
            );

            continue;

        fail_exit:
            if (std::get<1>(subj01) != true)
            {
                logger(
                    wayround_i2p::ccutils::logger::Failure,
                    "  fail"
                );
                failed_count++;
            }
            else
            {
                logger(
                    wayround_i2p::ccutils::logger::Success,
                    "  successfully failed"
                );
            }
            continue;
        error_exit:
        }
    }

    return {
        failed_count == 0
    };
}

wayround_i2p::ccutils::tst::TSTInfo main_002_i = {
    .group_name        = "main",
    .test_name         = "002",
    .description_short = "testing PatternType::LineSplit match",
    .func              = main_002
};

// -------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_003(
    const wayround_i2p::ccutils::tst::TSTInfo                &func_info,
    std::map<std::string, std::any>                          &iitm,
    const wayround_i2p::ccutils::tst::LOGGER_CB_FUNCTION_TYPE logger
)
{
    auto p = wayround_i2p::ccutils::regexp::Pattern::create(
        wayround_i2p::ccutils::regexp::PatternType::LineSplit
    );

    auto ts = std::any_cast<wayround_i2p::ccutils::unicode::UString>(
        iitm["test_subject_001"]
    );

    auto res = wayround_i2p::ccutils::regexp::findAll(p, ts);

    logger(
        wayround_i2p::ccutils::logger::Status,
        "indexes of line splists in test_subject_001"
    );

    logger(
        wayround_i2p::ccutils::logger::ToDo,
        "check output, make constant, check against constant"
    );

    auto res_err = std::get<1>(res);

    if (res_err)
    {
        logger(
            wayround_i2p::ccutils::logger::Failure,
            res_err->Error()
        );
        return {false};
    }

    for (auto &i : std::get<0>(res))
    {
        logger(
            wayround_i2p::ccutils::logger::Status,
            std::format("  {}:{}", i->match_start, i->match_end)
        );
    }

    return {true};
}

wayround_i2p::ccutils::tst::TSTInfo main_003_i = {
    .group_name        = "main",
    .test_name         = "003",
    .description_short = "testing PatternType::LineSplit findAll",
    .func              = main_003
};

// -------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_004(
    const wayround_i2p::ccutils::tst::TSTInfo                &func_info,
    std::map<std::string, std::any>                          &iitm,
    const wayround_i2p::ccutils::tst::LOGGER_CB_FUNCTION_TYPE logger
)
{
    auto p = wayround_i2p::ccutils::regexp::Pattern::create(
        wayround_i2p::ccutils::regexp::PatternType::LineStart
    );

    auto ts = std::any_cast<wayround_i2p::ccutils::unicode::UString>(
        iitm["test_subject_001"]
    );

    auto res = wayround_i2p::ccutils::regexp::findAll(p, ts);

    logger(
        wayround_i2p::ccutils::logger::Status,
        "indexes of line starts in test_subject_001"
    );

    logger(
        wayround_i2p::ccutils::logger::ToDo,
        "check output, make constant, check against constant"
    );

    auto res_err = std::get<1>(res);

    if (res_err)
    {
        logger(
            wayround_i2p::ccutils::logger::Failure,
            res_err->Error()
        );
        return {false};
    }

    for (auto &i : std::get<0>(res))
    {
        logger(
            wayround_i2p::ccutils::logger::Status,
            std::format("  {}:{}", i->match_start, i->match_end)
        );
    }

    return {true};
}

wayround_i2p::ccutils::tst::TSTInfo main_004_i = {
    .group_name        = "main",
    .test_name         = "004",
    .description_short = "testing PatternType::LineStart findAll",
    .func              = main_004
};

// -------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_005(
    const wayround_i2p::ccutils::tst::TSTInfo                &func_info,
    std::map<std::string, std::any>                          &iitm,
    const wayround_i2p::ccutils::tst::LOGGER_CB_FUNCTION_TYPE logger
)
{
    auto p = wayround_i2p::ccutils::regexp::Pattern::create(
        wayround_i2p::ccutils::regexp::PatternType::LineEnd
    );

    auto ts = std::any_cast<wayround_i2p::ccutils::unicode::UString>(
        iitm["test_subject_001"]
    );

    auto res = wayround_i2p::ccutils::regexp::findAll(p, ts);

    logger(
        wayround_i2p::ccutils::logger::Status,
        "indexes of line ends in test_subject_001"
    );

    logger(
        wayround_i2p::ccutils::logger::ToDo,
        "check output, make constant, check against constant"
    );

    auto res_err = std::get<1>(res);

    if (res_err)
    {
        logger(
            wayround_i2p::ccutils::logger::Failure,
            res_err->Error()
        );
        return {false};
    }

    for (auto &i : std::get<0>(res))
    {
        logger(
            wayround_i2p::ccutils::logger::Status,
            std::format("  {}:{}", i->match_start, i->match_end)
        );
    }

    return {true};
}

wayround_i2p::ccutils::tst::TSTInfo main_005_i = {
    .group_name        = "main",
    .test_name         = "005",
    .description_short = "testing PatternType::LineEnd findAll",
    .func              = main_005
};

// -------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_006(
    const wayround_i2p::ccutils::tst::TSTInfo                &func_info,
    std::map<std::string, std::any>                          &iitm,
    const wayround_i2p::ccutils::tst::LOGGER_CB_FUNCTION_TYPE logger
)
{
    bool ret = false;
    auto ts  = std::any_cast<wayround_i2p::ccutils::unicode::UString>(
        iitm["test_subject_003"]
    );

    auto pattern = wayround_i2p::ccutils::regexp::Pattern::create(
        wayround_i2p::ccutils::regexp::PatternType::ExactChar
    );
    pattern->setMinMaxCount(6, 6);
    pattern->values = {'a'};

    auto res = wayround_i2p::ccutils::regexp::match(pattern, ts);

    if (res->error)
    {
        logger(
            wayround_i2p::ccutils::logger::Error,
            std::format("error: {}", res->error->Error())
        );
    }

    logger(
        wayround_i2p::ccutils::logger::Status,
        std::format("matched?: {}", res->matched)
    );

    return {ret};
}

wayround_i2p::ccutils::tst::TSTInfo main_006_i = {
    .group_name        = "main",
    .test_name         = "006",
    .description_short = "repetitions test 01",
    .func              = main_006
};
