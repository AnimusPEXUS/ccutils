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

wayround_i2p::ccutils::tst::TSTFuncResult main_001(
    const wayround_i2p::ccutils::tst::TSTFuncOpts &opts
)
{
    wayround_i2p::ccutils::unicode::UString x = "тест";

    opts.iitm["test_subject_001"] = wayround_i2p::ccutils::unicode::UString(
        "test subject for testing regular expressions"
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
