
html_DATA = $(srcdir)/html/*.*

EXTRA_DIST = Doxyfile $(srcdir)/manual/*.doxygen

dist-hook:
	cd $(distdir) && doxygen
