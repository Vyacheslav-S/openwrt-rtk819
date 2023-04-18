# noinst_PROGRAMS = testconnect testdownload testswap

lib_LTLIBRARIES = libmms.la

libmms_la_SOURCES = 		\
	mmsio.h			\
	mms.c			\
	mms.h			\
	mms-common.h		\
	mms-common-funcs.h	\
	mms_config.h		\
	mmsh.c			\
	mmsh.h			\
	mmsx.c			\
	mmsx.h			\
	bswap.h			\
	uri.c			\
	uri.h			\
	utf.c			\
	utf.h			\
	asfheader.h		\
	mmsh.h

# noinst_HEADERS = 

libmms_la_LDFLAGS = -version-info 0:2:0 -no-undefined -export-dynamic
libmms_la_LIBADD = $(MMS_LIBS)

libmmsincdir = $(includedir)/libmms

libmmsinc_HEADERS = 	\
	mms.h		\
	mmsio.h		\
	mmsx.h		\
	mmsh.h

nodist_libmmsinc_HEADERS = 	\
	mms_config.h

INCLUDES =				\
	-I$(top_srcdir) 		\
	-I$(top_builddir) 		\
	$(MMS_CFLAGS)			\
	$(MMS_PKG_CFLAGS)

DEPS = $(top_builddir)/src/libmms.la
LDADD = $(top_builddir)/src/libmms.la

# libmms_test_SOURCES = testconnect.c testdownload.c testswap.c
# libmms_test_LDADD = $(LDADD)
