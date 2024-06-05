#include <wayround_i2p/ccutils/regexp/regexp.hpp>

wayround_i2p::ccutils::tst::TSTFuncResult main_001(
    const wayround_i2p::ccutils::tst::TSTFuncOpts &opts
)
{

    opts.iitm["test_subject_001"] = wayround_i2p::ccutils::unicode::UString(
        "test subject for testing regular expressions.\n\n"
        "it also contains some separate lines,\n"
        "which is cool for newline detection"
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

    for (auto &subj01 : std::vector<std::tuple<
             std::string, // test subject
             bool         // failure is good?
             >>{
             {R"xx(\n)xx",   false},
             {R"xx(\r\n)xx", false},
             {R"xx(\n\r)xx", true },
             {R"xx(\r)xx",   true },
             {R"xx()xx",     true },
             {R"xx(  )xx",   true },
    })
    {

        auto p = wayround_i2p::ccutils::regexp::Pattern::create();

        p->pattern_type = wayround_i2p::ccutils::regexp::PatternType::LineSplit;

        auto ts   = std::get<0>(subj01);
        auto ts_u = wayround_i2p::ccutils::unicode::UString(ts);

        opts.Log(
            wayround_i2p::ccutils::tst::Status,
            std::format("testing subject is: {}", ts)
        );

        auto res = wayround_i2p::ccutils::regexp::match(p, ts_u);

        if (!res->matched)
        {
            opts.Log(wayround_i2p::ccutils::tst::Failure, "missmatched");
            goto fail_exit;
        }

        if (res->match_start != 0)
        {
            opts.Log(
                wayround_i2p::ccutils::tst::Failure,
                std::format("invalid match_start (must be 0): {}", res->match_start)
            );
            goto fail_exit;
        }

        if (res->match_end != 1)
        {
            opts.Log(
                wayround_i2p::ccutils::tst::Failure,
                std::format("invalid match_end (must be 1): {}", res->match_end)
            );
            goto fail_exit;
        }

        {
            opts.Log(
                wayround_i2p::ccutils::tst::Status,
                "  succeeded"
            );

            opts.Log(
                wayround_i2p::ccutils::tst::Success,
                "  good"
            );

            continue;

        fail_exit:
            if (std::get<1>(subj01) != true)
            {
                opts.Log(
                    wayround_i2p::ccutils::tst::Failure,
                    "  failed"
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
