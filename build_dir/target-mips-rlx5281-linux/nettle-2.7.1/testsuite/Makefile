

srcdir = .


top_srcdir = ..

include ../config.make

PRE_CPPFLAGS = -I.. -I$(top_srcdir)
PRE_LDFLAGS = -L..

TS_NETTLE_SOURCES = aes-test.c arcfour-test.c arctwo-test.c \
		    blowfish-test.c cast128-test.c \
	            base16-test.c base64-test.c \
		    camellia-test.c \
		    des-test.c des3-test.c des-compat-test.c \
		    md2-test.c md4-test.c md5-test.c md5-compat-test.c \
		    memxor-test.c gosthash94-test.c \
		    ripemd160-test.c \
		    salsa20-test.c \
		    sha1-test.c sha224-test.c sha256-test.c \
		    sha384-test.c sha512-test.c \
		    sha3-permute-test.c sha3-224-test.c sha3-256-test.c \
		    sha3-384-test.c sha3-512-test.c \
		    serpent-test.c twofish-test.c \
		    knuth-lfib-test.c \
		    cbc-test.c ctr-test.c gcm-test.c hmac-test.c umac-test.c \
		    meta-hash-test.c meta-cipher-test.c meta-armor-test.c \
		    buffer-test.c yarrow-test.c pbkdf2-test.c

TS_HOGWEED_SOURCES = sexp-test.c sexp-format-test.c \
		     rsa2sexp-test.c sexp2rsa-test.c \
		     bignum-test.c random-prime-test.c \
		     pkcs1-test.c \
		     rsa-test.c rsa-encrypt-test.c rsa-keygen-test.c \
		     dsa-test.c dsa-keygen-test.c \
		     ecc-mod-test.c ecc-modinv-test.c ecc-redc-test.c \
		     ecc-mul-g-test.c ecc-mul-a-test.c \
		     ecdsa-sign-test.c ecdsa-verify-test.c ecdsa-keygen-test.c

TS_SOURCES = $(TS_NETTLE_SOURCES) $(TS_HOGWEED_SOURCES)
CXX_SOURCES = cxx-test.cxx

TS_NETTLE = $(TS_NETTLE_SOURCES:.c=$(EXEEXT))
TS_HOGWEED = $(TS_HOGWEED_SOURCES:.c=$(EXEEXT))
TS_C = $(TS_NETTLE)  $(TS_HOGWEED)
TS_CXX =  $(CXX_SOURCES:.cxx=$(EXEEXT))
TARGETS = $(TS_C) $(TS_CXX)
TS_SH = sexp-conv-test pkcs1-conv-test  symbols-test
TS_ALL = $(TARGETS) $(TS_SH)
EXTRA_SOURCES = sha1-huge-test.c
EXTRA_TARGETS = $(EXTRA_SOURCES:.c=$(EXEEXT))

# Includes all C source files, regardless of configuration
SOURCES = $(TS_SOURCES) $(EXTRA_SOURCES) testutils.c

DISTFILES = $(SOURCES) $(CXX_SOURCES) Makefile.in .test-rules.make \
	    $(TS_SH) setup-env teardown-env \
	    gold-bug.txt testutils.h sha3.awk

all: $(TARGETS) $(EXTRA_TARGETS)

.c.$(OBJEXT):
	$(COMPILE) -c $< && $(DEP_PROCESS)

.SUFFIXES: .cxx
.cxx.$(OBJEXT):
	$(COMPILE_CXX) -c $< && $(DEP_PROCESS)

# BSD (and Solaris) make doesn't allow extra dependencies together one
# single-suffix rules, which makes it impossible or almost impossible
# to use suffix rules to build the test executables. So we use an
# explicit rule for each and every executable.

LIB_HOGWEED =  -lhogweed
TEST_OBJS = testutils.$(OBJEXT) ../nettle-internal.$(OBJEXT) \
	    $(LIB_HOGWEED) -lnettle $(LIBS) $(TESTSUITE_LIBS)

../nettle-internal.$(OBJEXT):
	( cd .. && $(MAKE) nettle-internal.$(OBJEXT) )

.PHONY: test-rules
test-rules:
	(for f in $(TS_NETTLE) $(TS_HOGWEED) $(EXTRA_TARGETS) ; do \
	  echo $$f'$$(EXEEXT): '$$f'.$$(OBJEXT)' ; \
	  echo '	$$(LINK) '$$f'.$$(OBJEXT) $$(TEST_OBJS) -o '$$f'$$(EXEEXT)' ; \
	  echo ; \
	done ; \
	for f in $(TS_CXX) ; do \
	  echo $$f'$$(EXEEXT): '$$f'.$$(OBJEXT)' ; \
	  echo '	$$(LINK_CXX) '$$f'.$$(OBJEXT) $$(TEST_OBJS) -o '$$f'$$(EXEEXT)' ; \
	  echo ; \
	done) > $(srcdir)/.test-rules.make

include $(srcdir)/.test-rules.make

$(TARGETS) $(EXTRA_TARGETS): testutils.$(OBJEXT) ../nettle-internal.$(OBJEXT) \
	../libnettle.a  ../libhogweed.a

# For use as, e.g.,
#
#   make check EMULATOR='$(VALGRIND)'
#   make check EMULATOR='$(VALGRIND) --log-fd=3' 3>valgrind.log

VALGRIND = valgrind --error-exitcode=1 --leak-check=full --show-reachable=yes

# The PATH update is for locating dlls on w*ndows.
check: $(TS_ALL)
	LD_LIBRARY_PATH=../.lib PATH="../.lib:$$PATH" srcdir="$(srcdir)" \
	  EMULATOR="$(EMULATOR)" NM="$(NM)" EXEEXT="$(EXEEXT)" \
          $(top_srcdir)/run-tests $(TS_ALL)


Makefile: $(srcdir)/Makefile.in ../config.status
	cd .. && $(SHELL) ./config.status testsuite/$@

install uninstall:
	true

distdir: $(DISTFILES)
	cp $? $(distdir)

clean:
	-rm -f $(TARGETS) $(EXTRA_TARGETS) *.o test.in test1.out test2.out

distclean: clean
	-rm -f Makefile *.d

tags:
	etags -o $(srcdir)/TAGS --include $(top_srcdir) $(srcdir)/*.c $(srcdir)/*.h

# Includes dependency files for everything, including objects which
# the current configuration will not build.
DEP_FILES = $(SOURCES:.c=.$(OBJEXT).d) $(CXX_SOURCES:.cxx=.$(OBJEXT).d)
include  $(DEP_FILES)
