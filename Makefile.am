
SUBDIRS = include src examples tests

pkgconfigdir=$(libdir)/pkgconfig

if WITH_XSLT
pkgconfig_DATA = xmlwrapp.pc xsltwrapp.pc
else
pkgconfig_DATA = xmlwrapp.pc
endif

bin_SCRIPTS = xmlwrapp-config
