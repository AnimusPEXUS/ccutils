# Test Suite Tool

<em>(by WayRound.I2P project)</em>

## About

Test Suite Tool - is tool for creating test suites for C++ code.

## How to Use

1. Write your C++ code in form of CMake project.
1. Create a separate CMake project for your code's testsuite.
1. Use `ccutils/test_suite_tool` cmake in your project's test suite ([examples](#examples) will follow).
1. Build your project's test suite.
1. Run your project's test suite.

## Groups {#groups}

Tests are separated by groups. You place test in some group by assigning
name of group to `group_name` field of test's `TSTInfo` struct.

Groups are used for visual separation of tests.

Also, in time of testing run, group items (tests) share runtime memory storage 
via `iitm` field of `TSTFuncOpts` passed to test function. your tests can share 
some data, classes or whatever via this `iitm` field.

## Examples {#examples}

### Example

For this example we take test suite inside `ccmake/tests/test_suite_tool_test01`.

Here we see **t.cpp**, which contains `main()` function for test suite and some 
`main_xxx.cpp` files, which contains actual tests.

`t.cpp` `#include`s those `main_xxx.cpp` files, so tests become 
available before `main()` function declaration.

Order of `main_xxx.cpp` files inclusion is not important. also `main_xxx.cpp` 
files can be separated in separate dirs if needed 
(maybe if there will be a huge amount of them).

Also note 'main' part in `main_xxx.cpp` file names is just for obviousity of 
their belonging to 'main' tests [group](#groups).

Inside of `main()`, you create `wayround_i2p::ccutils::tst::run_tests_Parameters` 
structure as you like and add tests into it.
To add tests into `wayround_i2p::ccutils::tst::run_tests_Parameters `
structure you can:

- manually construct values for `group_order` and `groups` 
fields
- use `AddTest()` function shortcut, which automatically creates groups and 
  ordering lists. <span style="color:maroon;">note:</span> order or callings to 
 `AddTest()` is important, as this will influence order of groups and tests in 
 them. Of-course, ordering of groups and tests can be redefined, by redefining
 corresponding fields under `groups` field's value.

### More Examples

Probably I will use this my test suite for many of my projects, so you may 
check those out in my [GitHub C++ repos](https://github.com/AnimusPEXUS?tab=repositories&q=&type=&language=c%2B%2B&sort=)