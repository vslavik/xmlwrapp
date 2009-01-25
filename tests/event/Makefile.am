
AM_CPPFLAGS = -I$(top_srcdir)/include
LIBS = ../../src/libxmlwrapp.la

TESTS = runtest.pl

noinst_PROGRAMS = \
		test_event-01 \
		test_event-02 \
		test_event-03

test_event_01_SOURCES = test_event-01.cxx
test_event_02_SOURCES = test_event-02.cxx
test_event_03_SOURCES = test_event-03.cxx

EXTRA_DIST = data/*.out data/*.xml runtest.pl
