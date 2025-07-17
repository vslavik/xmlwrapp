0. Introduction
---------------

xmlwrapp is a modern style C++ library for working with XML data, built atop
the venerable libxml2 C library.

Additional resources, bug reports, latest sources etc. can be found on the
[project page](https://vslavik.github.io/xmlwrapp/).

Packages tarballs can be downloaded from the same location or from the
[release page](https://github.com/vslavik/xmlwrapp/releases).

If you need help or want to discuss xmlwrapp, feel free to join the discussion
group hosted at [Google Groups](https://groups.google.com/group/xmlwrapp)
or email directly to xmlwrapp@googlegroups.com.


1. Requirements
---------------

In order to build xmlwrapp, you need libxml2 version 2.4.28 or newer. When
building with XSLT support, libxslt 1.1.6 or newer is required. Both libraries
are available from [libxml2 home page](https://xmlsoft.org).


2. Building on Unix
-------------------

On Unix, either autotools-based build system or CMake can be used.

## Building using Autotools

Building xmlwrapp is usually as simple as running the following three commands:

    $ ./configure
    $ make
    $ make install

See the output of `./configure --help` for additional settings and options.

Cross-compiling, notably for Windows, is also supported using the usual
`--host` option, e.g. `--host=x86_64-w64-mingw32`.

Note that if you obtained the library sources from Git, and not from the
release archives, `configure` script won't exist and you will need to run

    $ autoreconf -i

to create it and other build files.

## Building using CMake

Building xmlwrapp using CMake is also straightforward. Just do the following:

    $ cmake -S . -B build-dir
    $ cmake --build build-dir
    $ cmake --install build-dir

The usual CMake `BUILD_SHARED_LIBS` option is supported for selecting whether
shared (default) or static libraries should be built, use `cmake -L` to see the
other available options.


3. Building on Windows
----------------------

The library can be built either with Microsoft Visual C++ compiler, using the
project files from located in platform/Win32 directory or in the same way as
under Unix, i.e. with configure and make, if you have Cygwin installed (note
that MinGW can be used as a cross-compiler under Cygwin, producing native
libraries without any Cygwin dependencies). In either case, you will need to
either build libxml and libxslt for Windows yourself or download the already
built versions from http://www.zlatkovic.com/libxml.en.html or elsewhere.


4. Using xmlwrapp
-----------------

On Unix, you should use pkg-config to get compiler flags for xmlwrapp or
xsltwrapp libraries:

    $ c++ -c `pkg-config --cflags xmlwrapp` ...
    $ c++ -o ... `pkg-config --libs xmlwrapp`

On Windows, you need to link against xmlwrapp libraries and add the include/
directory to compiler's headers search path.
