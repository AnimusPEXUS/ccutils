# ccutils

## about

ccutils is collection of C++ tools. I'm going to use it in my projects.

Mainly those are shortcuts and life enchancers.

### namely

#### errors

Defines error (and error_ptr) type, which is like in Go. I decided to use it in 
all ccutils, so it's out of akigo.

### unicode

This is unification layer for working with various Unicode backends. 
So it's pluggable. Currently only ICU supported. But I plan to write my own
set of tools for Unicode, includingly codepoint database.

#### regexp

regexp engine to work with unicode strings. 
ccutils/unicode and ccutils/unicode depends on each other.

#### ip

It's the tool for parsing and formatting IP (v4/v6) address strings and working
with their values.

#### logger

WIP. currently is lightweight logging facility. in future it should be pluggable 
and should be able to log into modern logging services.

#### posix_tools

WIP. this is just a shortcut for POSIX utilities.

#### repr

This is supposed to be like in PHP

#### akigo

WIP. Should add some Golang vibe to C++.

#### worker01

This is tool for more comfy creation and handling of tasks/routines.

#### test_suite_tool

Is tool for creating test suites. At this point, while I'm developing regexp
module, you can see how test_suite_tool is used in 
[subdirectory](_tests_and_examples/regexp_test01) directory.
