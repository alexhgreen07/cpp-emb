# cpp-emb
C++ embedded helper library

This project uses the `scons` build system and `cpputest` test framework. See:

* Scons: http://scons.org/
* CppUTest: https://cpputest.github.io/

To clone repo with submodules run: 

`git clone --recursive <repo_url>`

To install `scons` in Debian based systems such as Ubuntu use:

`sudo apt install scons`

To build the tests run:

`scons`

To run the built tests, use:

`./bin/test.exe`

## Formatting

To run the formatter, first install clang format:

`sudo apt install clang-format-3.8`

Then to run the formatter use:

`./tools/format.sh`
