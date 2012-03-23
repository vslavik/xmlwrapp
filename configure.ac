dnl configure.ac script for xmlwrapp, process with autoconf to create configure
dnl
dnl Copyright (C) 2008 Vaclav Slavik <vslavik@gmail.com>,
dnl               2008 Vadim Zeitlin <vz-xmlwrapp@zeitlins.org>
dnl All Rights Reserved
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions
dnl are met:
dnl
dnl 1. Redistributions of source code must retain the above copyright
dnl    notice, this list of conditions and the following disclaimer.
dnl 2. Redistributions in binary form must reproduce the above copyright
dnl    notice, this list of conditions and the following disclaimer in
dnl    the documentation and/or other materials provided with the
dnl    distribution.
dnl 3. Neither the name of the Author nor the names of its contributors
dnl    may be used to endorse or promote products derived from this software
dnl    without specific prior written permission.
dnl
dnl THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
dnl AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
dnl TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
dnl PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
dnl OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
dnl SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
dnl LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
dnl USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
dnl AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
dnl OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
dnl OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
dnl SUCH DAMAGE.

dnl === Prologue ===

AC_REVISION($Id$)dnl

AC_PREREQ(2.61)
AC_INIT(xmlwrapp, 0.6.3, [xmlwrapp@googlegroups.com])

AC_CONFIG_SRCDIR([xmlwrapp.pc.in])
AC_CONFIG_AUX_DIR([admin])

AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

dnl remember, "build" is where we compile, "host" is where the resulting
dnl program runs (which may be different from "build" for cross-compilation)
AC_CANONICAL_HOST


dnl === Command line options ===

AC_ARG_ENABLE(xslt,
             [AC_HELP_STRING([--disable-xslt],
                             [don't build libxsltwrapp library])],
             [case "x${enableval}" in
                   x) build_xslt=no ;;
                xyes) build_xslt=yes ;;
                 xno) build_xslt=no ;;
                   *) AC_MSG_ERROR(bad value ${enableval} for --enable-xslt) ;;
             esac],
             [build_xslt=yes])


dnl === Program checks ===

AC_PROG_CXX
AC_PROG_LD
AC_PROG_LIBTOOL


dnl === Library checks ===

dnl use C++ compiler as we're checking for C++ libraries/headers from now on
dnl (we couldn't do it before as libtool tests must be done with C compiler)
AC_LANG(C++)

PKG_CHECK_MODULES(LIBXML, [libxml-2.0 >= 2.4.28])

if test "x$build_xslt" = "xyes" ; then
    PKG_CHECK_MODULES(LIBXSLT, [libxslt >= 1.1.6])
    PKG_CHECK_MODULES(LIBEXSLT, [libexslt])
fi


AC_HEADER_ASSERT

BOOST_REQUIRE
BOOST_FIND_HEADER([boost/pool/singleton_pool.hpp])
BOOST_IOSTREAMS
BOOST_TEST

dnl === Compiler-specific stuff ===

if test "x$GCC" == "xyes"; then
    dnl Enable some useful warnings that GCC supports:
    CXXFLAGS="$CXXFLAGS -W -Wall -Wcast-align -Wwrite-strings"
fi


dnl === Generate output files ===

dnl This is support for the legacy xmlwrapp-config script, which has to behave
dnl as it did in 0.5.x, i.e. output flags for both libxmlwrapp and
dnl libxsltwrapp and also include dependency libs' flags for static linking
LEGACY_LINK_FLAGS="-lxmlwrapp $LIBXML_LIBS"
if test "x$build_xslt" = "xyes" ; then
    LEGACY_LINK_FLAGS="-lxsltwrapp $LIBEXSLT_LIBS $LIBXSLT_LIBS $LEGACY_LINK_FLAGS"
fi
AC_SUBST(LEGACY_LINK_FLAGS)

AM_CONDITIONAL(WITH_XSLT, [ test "x$build_xslt" = "xyes" ])

AC_CONFIG_FILES([
    xmlwrapp.pc
    xsltwrapp.pc
    xmlwrapp-config
    Makefile
    include/Makefile
    src/Makefile
    docs/Makefile
    examples/Makefile
    examples/01-tree_parsing/Makefile
    examples/02-event_parsing/Makefile
    examples/03-xml_generation/Makefile
    examples/04-xslt/Makefile
    tests/Makefile
])
AC_OUTPUT
