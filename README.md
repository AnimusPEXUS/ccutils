# ccutils

## about

ccutils is collection of C++ tools. I'm going to use it in my projects.

## provides following tools

<table>

<tr>
	<th>Item</th>
	<th>Short Description</th>
	<th>State/Status/Usability</th>
</tr>

<tr>
	<td>errors</td>
	<td>
		Defines error (and error_ptr) type, which is like in Go. I decided to use it in
		all ccutils.
	</td>
	<td>ok, but needs thinking.</td>
</tr>

<tr>
	<td>unicode</td>
	<td>
		Unicode support. Unified interface to some unicode libraries.
		currently only ICU supported
	</td>
	<td>ok more or less. probably some more functionality needed.</td>
</tr>

<tr>
	<td>regexp</td>
	<td>
		regexp engine to work with unicode strings.
	</td>
	<td>ok. needs testing, upgrades and some more advanced stuff.</td>
</tr>

<tr>
	<td>ip</td>
	<td>
		It's the tool for parsing and formatting IP (v4/v6)
		address strings and working
		with their values.
	</td>
	<td>todo</td>
</tr>

<tr>
	<td>logger</td>
	<td>
		currently is lightweight logging facility.
		in future it should be pluggable
		and should be able to integrate with modern logging systems.
	</td>
	<td><em>probably ok</em>, but needs upgrades and thinking</td>

</tr>

<tr>
	<td>posix_tools</td>
	<td>
		this is just a shortcut for POSIX utilities.
	</td>
	<td>not ready. working on it by need. not finished. don't use it.</td>
</tr>

<tr>
	<td>repr</td>
	<td>
		Interface for requesting human-readable data representation.
	</td>
	<td>ok</td>
</tr>

<tr>
	<td>worker01</td>
	<td>
		This is tool for more comfy creation and handling of tasks/routines.
	</td>
	<td>stable for now. testing needed. probably reworks needed.</td>
</tr>

<tr>
	<td>test_suite_tool</td>
	<td>
		This tool is for creating test suites.
		You can see it in action under
		[testing subdirectory](./_tests_and_examples).
	</td>
	<td>ok. stable for now</td>
</tr>

<tr>
	<td>fint</td>
	<td>
		Flexible Int manipulation utilities. provides new 'smart' integer type
		to work with integers of arbitrary size.
	</td>
	<td>wip. requires thinking</td>
</tr>

<tr>
	<td><span style="color:red;">akigo</span></td>
	<td>
        <p>
		Some Golang vibe to C++. As I migrate from Golang,
		I missing some convenient
		parts of it in C++.
        </p>
        <p>
        This is the main idea-package of ccutils. Most other ccutils packages done to supply functionality to akigo.
        </p>
	</td>
	<td>work in progress. see separate table to know it's state.</td>
</tr>

<!--

<tr>
<td></td>
<td>
</td>
<td></td>
</tr>

-->

</table>

## <span style="color:red;">akigo</span> state

akigo devided on two ideological parts:

1. packages like in go
1. utility packages for akigo, which doesn't deserv to climb up to overal ccutils

<table>

<tr>
	<th>Item</th>
	<th>Short Description</th>
	<th>State/Status/Usability</th>
</tr>

<!--

<tr>
    <td></td>
    <td>
    </td>
    <td></td>
</tr>

-->

</table>
