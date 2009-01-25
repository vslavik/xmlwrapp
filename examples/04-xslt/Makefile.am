
if WITH_XSLT

noinst_PROGRAMS = example

example_SOURCES = example.cxx
example_CPPFLAGS = -I$(top_srcdir)/include
example_LDADD = ../../src/libxsltwrapp.la ../../src/libxmlwrapp.la

endif

EXTRA_DIST = example.xml example.xsl
