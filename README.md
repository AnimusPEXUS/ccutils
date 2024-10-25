# ccutils

## about

ccutils is collection of C++ tools. I'm going to use it in my projects.

## namely

<table>

<tr>
<td>errors</td>
<td>
Defines error (and error_ptr) type, which is like in Go. I decided to use it in 
all ccutils.
</td>
</tr>

<tr>
<td>unicode</td>
<td>
Unicode support. Unified interface to some unicode libraries. currently only ICU supported
</td>
</tr>

<tr>
<td>regexp</td>
<td>
regexp engine to work with unicode strings.
</td>
</tr>

<tr>
<td>ip</td>
<td>
It's the tool for parsing and formatting IP (v4/v6) address strings and working
with their values.
</td>
</tr>

<tr>
<td>logger</td>
<td>
WIP. currently is lightweight logging facility. in future it should be pluggable 
and should be able to integrate with modern logging systems.
</td>
</tr>

<tr>
<td>posix_tools</td>
<td>
WIP. this is just a shortcut for POSIX utilities.
</td>
</tr>

<tr>
<td>repr</td>
<td>
Interface for requesting human-readable data representation.
</td>
</tr>

<tr>
<td>akigo</td>
<td>
Some Golang vibe to C++. As I migrate from Golang, I missing some convinient
parts of it in C++.
</td>
</tr>

<tr>
<td>worker01</td>
<td>
This is tool for more comfy creation and handling of tasks/routines.
</td>
</tr>

<tr>
<td>test_suite_tool</td>
<td>
This tool is for creating test suites. 
You can see it in action under [testing subdirectory](./_tests_and_examples).
</td>
</tr>

<!--


<tr>
<td></td>
<td>
</td>
</tr>

-->

</table>
