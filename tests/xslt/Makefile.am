
AM_CPPFLAGS = -I$(top_srcdir)/include
LIBS = ../../src/libxsltwrapp.la ../../src/libxmlwrapp.la

if WITH_XSLT

TESTS = runtest.pl

noinst_PROGRAMS = \
		test_xslt-01 \
		test_xslt-02 \
		test_xslt-03 \
		test_xslt-04 \
		test_xslt-05

test_xslt_01_SOURCES = test_xslt-01.cxx
test_xslt_02_SOURCES = test_xslt-02.cxx
test_xslt_03_SOURCES = test_xslt-03.cxx
test_xslt_04_SOURCES = test_xslt-04.cxx
test_xslt_05_SOURCES = test_xslt-05.cxx

endif

EXTRA_DIST = data/*.xml data/*.xsl data/*.out runtest.pl
