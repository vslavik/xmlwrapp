
TESTS = test

AM_CPPFLAGS = -I$(top_srcdir)/include
LIBS = $(top_builddir)/src/libxmlwrapp.la \
	   $(BOOST_UNIT_TEST_FRAMEWORK_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS) \
	   $(BOOST_IOSTREAMS_LIBS) $(BOOST_IOSTREAMS_LDFLAGS)

noinst_PROGRAMS = test

test_SOURCES = \
		test.h \
		test_main.cxx \
		attributes/test_attributes.cxx \
		document/test_document.cxx \
		event/test_event.cxx \
		node/test_node.cxx \
		tree/test_tree.cxx \
		relaxng/test_relaxng.cxx \
		schema/test_schema.cxx \
        xpath/test_xpath.cxx

if WITH_XSLT
LIBS += $(top_builddir)/src/libxsltwrapp.la
test_SOURCES += xslt/test_xslt.cxx
endif

EXTRA_DIST = \
		$(srcdir)/*/data/*.out \
		$(srcdir)/*/data/*.rnc \
		$(srcdir)/*/data/*.rng \
		$(srcdir)/*/data/*.xml \
		$(srcdir)/*/data/*.xsl \
		$(srcdir)/*/data/*.xsd \
		$(srcdir)/*/data/*.dtd \
		$(srcdir)/*/data/output
