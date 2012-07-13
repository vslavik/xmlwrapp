
SUBDIRS = include src examples tests docs

pkgconfigdir=$(libdir)/pkgconfig

if WITH_XSLT
pkgconfig_DATA = xmlwrapp.pc xsltwrapp.pc
else
pkgconfig_DATA = xmlwrapp.pc
endif

bin_SCRIPTS = xmlwrapp-config

EXTRA_DIST = \
		LICENSE bootstrap \
		$(srcdir)/platform/Win32/README \
		$(srcdir)/platform/Win32/*.dsp \
		$(srcdir)/platform/Win32/*.dsw \
		$(srcdir)/platform/Win32/*.bkl \
		$(srcdir)/platform/Win32/*proj* \
		$(srcdir)/platform/Win32/*/*proj* \
		$(srcdir)/platform/Win32/*/*.sln
