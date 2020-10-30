doxygen_tagfile = $(abs_builddir)/xmlwrapp.doxytag

# We use this target just to allow having a rule generating the documentation.
noinst_DATA = $(doxygen_tagfile)

html_DATA = $(abs_builddir)/html/*.*

$(doxygen_tagfile):
	cd $(srcdir) && DOXYGEN_OUTPUT_DIRECTORY_SLASH="$(abs_builddir)/" doxygen

EXTRA_DIST = Doxyfile $(srcdir)/manual/*.doxygen

dist-hook:
	cd $(distdir) && doxygen
