
SUBDIRS = include src examples tests docs

pkgconfigdir=$(libdir)/pkgconfig

if WITH_XSLT
pkgconfig_DATA = xmlwrapp.pc xsltwrapp.pc
else
pkgconfig_DATA = xmlwrapp.pc
endif

bin_SCRIPTS = xmlwrapp-config

EXTRA_DIST = LICENSE bootstrap \
			 platform/Win32/*.dsp platform/Win32/*.dsw
