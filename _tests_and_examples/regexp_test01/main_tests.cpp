#include <experimental/scope>

#include <wayround_i2p/ccutils/regexp/regexp.hpp>

wayround_i2p::ccutils::tst::TSTFuncResult main_001(
    const wayround_i2p::ccutils::tst::TSTFuncOpts &opts
)
{
    opts.Log(
        wayround_i2p::ccutils::tst::Status,
        "setting test subject to ingroup memory"
    );

    opts.iitm["test_subject_001"] = wayround_i2p::ccutils::unicode::UString(
        "test subject for testing regular expressions.\n\n"
        "it also contains some separate lines,\n"
        "which is cool for newline detection"
    );

    auto t00 = wayround_i2p::ccutils::unicode::UString(
        "-----( cut line )-----"
    );

    opts.Log(
        wayround_i2p::ccutils::tst::Status,
        std::format(
            "    test subject is:\n{1:}\n{0:}\n{1:}",
            std::any_cast<wayround_i2p::ccutils::unicode::UString>(
                opts.iitm["test_subject_001"]
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

wayround_i2p::ccutils::tst::TSTFuncResult main_002(
    const wayround_i2p::ccutils::tst::TSTFuncOpts &opts
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

        auto p = wayround_i2p::ccutils::regexp::Pattern::create();

        p->pattern_type = wayround_i2p::ccutils::regexp::PatternType::LineSplit;

        auto ts   = std::get<0>(subj01);
        auto ts_u = wayround_i2p::ccutils::unicode::UString(ts);

        auto missmatch_is_ok = std::get<1>(subj01);
        auto subj_check_cb   = std::get<2>(subj01);

        opts.Log(
            wayround_i2p::ccutils::tst::Status,
            std::format("testing subject is: {}", ts_u.repr_as_text())
        );

        auto ex01 = std::experimental::scope_exit(
            [&opts]()
            {
                opts.Log(
                    wayround_i2p::ccutils::tst::Status,
                    ""
                );
            }
        );

        opts.Log(
            wayround_i2p::ccutils::tst::Status,
            std::format("  missmatch is ok?: {}", missmatch_is_ok)
        );

        auto res = wayround_i2p::ccutils::regexp::match(p, ts_u);

        opts.Log(
            wayround_i2p::ccutils::tst::Status,
            std::format("   res->matched: {}", res->matched)
        );
        opts.Log(
            wayround_i2p::ccutils::tst::Status,
            std::format("   res->match_start: {}", res->match_start)
        );
        opts.Log(
            wayround_i2p::ccutils::tst::Status,
            std::format("   res->match_end: {}", res->match_end)
        );

        if (!res->matched)
        {
            opts.Log(wayround_i2p::ccutils::tst::Failure, "missmatched");
            goto fail_exit;
        }

        if (res->match_start != 0)
        {
            opts.Log(
                wayround_i2p::ccutils::tst::Failure,
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
                opts.Log(
                    wayround_i2p::ccutils::tst::Failure,
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
            opts.Log(
                wayround_i2p::ccutils::tst::Success,
                "  ok"
            );

            continue;

        fail_exit:
            if (std::get<1>(subj01) != true)
            {
                opts.Log(
                    wayround_i2p::ccutils::tst::Failure,
                    "  fail"
                );
                failed_count++;
            }
            else
            {
                opts.Log(
                    wayround_i2p::ccutils::tst::Success,
                    "  successfully failed"
                );
            }
        }
    }

    return {
        failed_count == 0
    };
}

wayround_i2p::ccutils::tst::TSTInfo main_002_i = {
    .group_name        = "main",
    .test_name         = "002",
    .description_short = "testing PatternType::LineSplit",
    .func              = main_002
};
