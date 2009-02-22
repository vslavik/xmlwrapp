
AM_CPPFLAGS = -I$(top_srcdir)/include
LIBS = ../../src/libxmlwrapp.la

TESTS = runtest.pl

noinst_PROGRAMS = \
		test_attr-01 \
		test_attr-02 \
		test_attr-03 \
		test_attr-04 \
		test_attr-05 \
		test_attr-06 \
		test_attr-07 \
		test_attr-08 \
		test_attr-09 \
		test_attr-10

test_attr_01_SOURCES = test_attr-01.cxx
test_attr_02_SOURCES = test_attr-02.cxx
test_attr_03_SOURCES = test_attr-03.cxx
test_attr_04_SOURCES = test_attr-04.cxx
test_attr_05_SOURCES = test_attr-05.cxx
test_attr_06_SOURCES = test_attr-06.cxx
test_attr_07_SOURCES = test_attr-07.cxx
test_attr_08_SOURCES = test_attr-08.cxx
test_attr_09_SOURCES = test_attr-09.cxx
test_attr_10_SOURCES = test_attr-10.cxx

EXTRA_DIST = $(srcdir)/data/*.out $(srcdir)/data/*.xml $(srcdir)/data/*.dtd runtest.pl
