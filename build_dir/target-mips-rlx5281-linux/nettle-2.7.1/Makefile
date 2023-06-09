# Nettle Makefile



srcdir = .


LIBOBJS =  ${LIBOBJDIR}memxor$U.o

INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(INSTALL_PROGRAM) -s
MKDIR_P = /bin/mkdir -p

OPT_ASM_SOURCES = 

OPT_HOGWEED_SOURCES = 

OPT_HOGWEED_HEADERS = 

SUBDIRS = tools testsuite examples

include config.make

PRE_CPPFLAGS = -I.
# FIXME: Add configuration of LIBEXT?
LIBTARGETS =  libnettle.a  libhogweed.a
SHLIBTARGETS =  $(LIBNETTLE_FORLINK)  $(LIBHOGWEED_FORLINK)

getopt_SOURCES = getopt.c getopt1.c
getopt_TARGETS = $(getopt_SOURCES:.c=.$(OBJEXT))

internal_SOURCES = nettle-internal.c
internal_TARGETS = $(internal_SOURCES:.c=.$(OBJEXT))

TARGETS = aesdata$(EXEEXT_FOR_BUILD) desdata$(EXEEXT_FOR_BUILD) \
          twofishdata$(EXEEXT_FOR_BUILD) shadata$(EXEEXT_FOR_BUILD) \
          gcmdata$(EXEEXT_FOR_BUILD) \
	  $(getopt_TARGETS) $(internal_TARGETS) \
	  $(LIBTARGETS) $(SHLIBTARGETS)
IMPLICIT_TARGETS = # $(LIBNETTLE_FILE) $(LIBHOGWEED_FILE)

DOCTARGETS = # nettle.info nettle.html nettle.pdf

PKGCONFIG_FILES = nettle.pc  hogweed.pc
pkgconfigdir = $(libdir)/pkgconfig

all check install uninstall:
	$(MAKE) $@-here
	set -e; for d in $(SUBDIRS); do \
	  echo "Making $@ in $$d" ; (cd $$d && $(MAKE) $@); done

clean distclean mostlyclean maintainer-clean tags:
	set -e; for d in $(SUBDIRS); do \
	  echo "Making $@ in $$d" ; (cd $$d && $(MAKE) $@); done
	$(MAKE) $@-here

check-here:
	true

# FIXME: Remove. These targets aren't supported, but they are expected by the
# automake generated Makefiles in the lsh build.
dvi installcheck uninstallcheck:
	true

all-here: $(TARGETS) $(DOCTARGETS)

nettle_SOURCES = aes-decrypt-internal.c aes-decrypt.c \
		 aes-encrypt-internal.c aes-encrypt.c aes-encrypt-table.c \
		 aes-set-encrypt-key.c aes-set-decrypt-key.c aes-meta.c \
		 arcfour.c arcfour-crypt.c arcfour-meta.c \
		 arctwo.c arctwo-meta.c gosthash94-meta.c \
		 base16-encode.c base16-decode.c base16-meta.c \
                 base64-encode.c base64-decode.c base64-meta.c \
		 camellia-crypt.c camellia-crypt-internal.c \
		 camellia-set-encrypt-key.c camellia-set-decrypt-key.c \
		 camellia-table.c camellia-meta.c \
		 cast128.c cast128-meta.c \
		 blowfish.c \
		 cbc.c ctr.c gcm.c gcm-aes.c \
		 des.c \
		 des3.c des-compat.c \
		 hmac.c hmac-md5.c hmac-ripemd160.c hmac-sha1.c \
		 hmac-sha224.c hmac-sha256.c hmac-sha384.c hmac-sha512.c \
		 pbkdf2.c pbkdf2-hmac-sha1.c pbkdf2-hmac-sha256.c \
		 knuth-lfib.c \
		 md2.c md2-meta.c md4.c md4-meta.c \
		 md5.c md5-compress.c md5-compat.c md5-meta.c \
		 gosthash94.c \
		 ripemd160.c ripemd160-compress.c ripemd160-meta.c \
		 salsa20-core-internal.c \
		 salsa20-crypt.c salsa20r12-crypt.c salsa20-set-key.c \
		 sha1.c sha1-compress.c sha1-meta.c \
		 sha256.c sha256-compress.c sha224-meta.c sha256-meta.c \
		 sha512.c sha512-compress.c sha384-meta.c sha512-meta.c \
		 sha3.c sha3-permute.c \
		 sha3-224.c sha3-224-meta.c sha3-256.c sha3-256-meta.c \
		 sha3-384.c sha3-384-meta.c sha3-512.c sha3-512-meta.c\
		 serpent-set-key.c serpent-encrypt.c serpent-decrypt.c \
		 serpent-meta.c \
		 twofish.c twofish-meta.c \
		 umac-nh.c umac-nh-n.c umac-l2.c umac-l3.c \
		 umac-poly64.c umac-poly128.c umac-set-key.c \
		 umac32.c umac64.c umac96.c umac128.c \
		 yarrow256.c yarrow_key_event.c \
		 buffer.c buffer-init.c realloc.c \
		 nettle-meta-hashes.c nettle-meta-ciphers.c \
		 nettle-meta-armors.c \
		 write-be32.c write-le32.c write-le64.c

hogweed_SOURCES = $(OPT_HOGWEED_SOURCES) sexp.c sexp-format.c \
		  sexp-transport.c sexp-transport-format.c \
		  bignum.c bignum-next-prime.c \
		  bignum-random.c bignum-random-prime.c \
		  sexp2bignum.c \
		  pkcs1.c pkcs1-encrypt.c pkcs1-decrypt.c \
		  pkcs1-rsa-digest.c pkcs1-rsa-md5.c pkcs1-rsa-sha1.c \
		  pkcs1-rsa-sha256.c pkcs1-rsa-sha512.c \
		  rsa.c rsa-sign.c rsa-verify.c \
		  rsa-pkcs1-sign.c rsa-pkcs1-sign-tr.c rsa-pkcs1-verify.c \
		  rsa-md5-sign.c rsa-md5-verify.c \
		  rsa-sha1-sign.c rsa-sha1-verify.c \
		  rsa-sha256-sign.c rsa-sha256-verify.c \
		  rsa-sha512-sign.c rsa-sha512-verify.c \
		  rsa-encrypt.c rsa-decrypt.c rsa-decrypt-tr.c \
		  rsa-keygen.c rsa-compat.c rsa-blind.c \
		  rsa2sexp.c sexp2rsa.c \
		  dsa.c dsa-sign.c dsa-verify.c dsa-keygen.c \
		  dsa-sha1-sign.c dsa-sha1-verify.c \
		  dsa-sha256-sign.c dsa-sha256-verify.c  \
		  dsa2sexp.c sexp2dsa.c \
		  pgp-encode.c rsa2openpgp.c \
		  der-iterator.c der2rsa.c der2dsa.c \
		  sec-add-1.c sec-sub-1.c sec-modinv.c sec-tabselect.c \
		  gmp-glue.c cnd-copy.c \
		  ecc-mod.c ecc-generic-modp.c ecc-generic-modq.c \
		  ecc-modp.c ecc-modq.c ecc-generic-redc.c \
		  ecc-192.c ecc-224.c ecc-256.c ecc-384.c ecc-521.c \
		  ecc-size.c ecc-j-to-a.c ecc-a-to-j.c \
		  ecc-dup-jj.c ecc-add-jja.c ecc-add-jjj.c \
		  ecc-mul-g.c ecc-mul-a.c ecc-hash.c ecc-random.c \
		  ecc-point.c ecc-scalar.c ecc-point-mul.c ecc-point-mul-g.c \
		  ecc-ecdsa-sign.c ecdsa-sign.c \
		  ecc-ecdsa-verify.c ecdsa-verify.c ecdsa-keygen.c

HEADERS = aes.h arcfour.h arctwo.h asn1.h bignum.h blowfish.h \
	  base16.h base64.h buffer.h camellia.h cast128.h \
	  cbc.h ctr.h \
	  des.h des-compat.h dsa.h ecc-curve.h ecc.h ecdsa.h \
	  gcm.h gosthash94.h hmac.h \
	  knuth-lfib.h \
	  macros.h nettle-config.h \
	  md2.h md4.h \
	  md5.h md5-compat.h \
	  memxor.h \
	  nettle-meta.h nettle-types.h \
	  pbkdf2.h \
	  pgp.h pkcs1.h realloc.h ripemd160.h rsa.h rsa-compat.h \
	  salsa20.h sexp.h \
	  serpent.h sha.h sha1.h sha2.h sha3.h twofish.h \
	  umac.h yarrow.h $(OPT_HOGWEED_HEADERS)

INSTALL_HEADERS = $(HEADERS) nettle-stdint.h

SOURCES = $(nettle_SOURCES) $(hogweed_SOURCES) \
	  $(getopt_SOURCES) $(internal_SOURCES) \
	  aesdata.c desdata.c twofishdata.c shadata.c gcmdata.c eccdata.c

DISTFILES = $(SOURCES) $(HEADERS) getopt.h .bootstrap run-tests \
	aclocal.m4 configure.ac \
	configure stamp-h.in \
	config.guess config.sub install-sh texinfo.tex \
	config.h.in config.m4.in config.make.in	Makefile.in \
	README AUTHORS COPYING.LIB INSTALL NEWS TODO ChangeLog \
	nettle.pc.in hogweed.pc.in nettle-config.h.in \
	memxor.c $(des_headers) descore.README \
	aes-internal.h camellia-internal.h serpent-internal.h \
	cast128_sboxes.h desinfo.h desCode.h \
	nettle-internal.h nettle-write.h prime-list.h \
	gmp-glue.h ecc-internal.h \
	mini-gmp.h mini-gmp.c asm.m4 \
	nettle.texinfo nettle.info nettle.html nettle.pdf sha-example.c

# Rules building static libraries
nettle_OBJS = $(nettle_SOURCES:.c=.$(OBJEXT)) $(LIBOBJS)
nettle_PURE_OBJS = $(nettle_OBJS:.$(OBJEXT)=.p$(OBJEXT))

hogweed_OBJS = $(hogweed_SOURCES:.c=.$(OBJEXT)) $(OPT_ASM_SOURCES:.asm=.$(OBJEXT))
hogweed_PURE_OBJS = $(hogweed_OBJS:.$(OBJEXT)=.p$(OBJEXT))

libnettle.a: $(nettle_OBJS)
	-rm -f $@
	$(AR) $(ARFLAGS) $@ $(nettle_OBJS)
	$(RANLIB) $@

libhogweed.a: $(hogweed_OBJS)
	-rm -f $@
	$(AR) $(ARFLAGS) $@ $(hogweed_OBJS)
	$(RANLIB) $@

.c.$(OBJEXT):
	$(COMPILE) $(CCPIC_MAYBE) -c $< \
	&& $(DEP_PROCESS)

# Rules building shared libraries.
$(LIBNETTLE_FORLINK): $(nettle_PURE_OBJS)
	$(LIBNETTLE_LINK) $(nettle_PURE_OBJS) -o $@ $(LIBNETTLE_LIBS)
	-mkdir .lib 2>/dev/null
	(cd .lib \
          && rm -f $(LIBNETTLE_FORLINK) \
	  && $(LN_S) ../$(LIBNETTLE_FORLINK) $(LIBNETTLE_FORLINK) \
	  && [ -z "$(LIBNETTLE_SONAME)" ] \
	  || { rm -f $(LIBNETTLE_SONAME) \
	       && $(LN_S) $(LIBNETTLE_FORLINK) $(LIBNETTLE_SONAME) ; } )

$(LIBHOGWEED_FORLINK): $(hogweed_PURE_OBJS) $(LIBNETTLE_FORLINK)
	$(LIBHOGWEED_LINK) $(hogweed_PURE_OBJS) -o $@ $(LIBHOGWEED_LIBS)
	-mkdir .lib 2>/dev/null
	(cd .lib \
          && rm -f $(LIBHOGWEED_FORLINK) \
	  && $(LN_S) ../$(LIBHOGWEED_FORLINK) $(LIBHOGWEED_FORLINK) \
	  && [ -z "$(LIBHOGWEED_SONAME)" ] \
	  || { rm -f $(LIBHOGWEED_SONAME) \
	       && $(LN_S) $(LIBHOGWEED_FORLINK) $(LIBHOGWEED_SONAME) ; } )

.c.p$(OBJEXT):
	$(COMPILE) $(SHLIBCFLAGS) -c $< -o $@ \
	&& $(DEP_PROCESS)

# For Solaris and BSD make, we have to use an explicit rule for each
# executable. Avoid object file targets to make it easy to run the
# right compiler.
aesdata$(EXEEXT_FOR_BUILD): aesdata.c
	$(CC_FOR_BUILD) `test -f aesdata.c || echo '$(srcdir)/'`aesdata.c \
	-o aesdata$(EXEEXT_FOR_BUILD)

desdata$(EXEEXT_FOR_BUILD): desdata.c
	$(CC_FOR_BUILD) `test -f desdata.c || echo '$(srcdir)/'`desdata.c \
	-o desdata$(EXEEXT_FOR_BUILD)

twofishdata$(EXEEXT_FOR_BUILD): twofishdata.c
	$(CC_FOR_BUILD) `test -f twofishdata.c || echo '$(srcdir)/'`twofishdata.c \
	-o twofishdata$(EXEEXT_FOR_BUILD)

shadata$(EXEEXT_FOR_BUILD): shadata.c
	$(CC_FOR_BUILD) `test -f shadata.c || echo '$(srcdir)/'`shadata.c -lm \
	-o shadata$(EXEEXT_FOR_BUILD)

gcmdata$(EXEEXT_FOR_BUILD): gcmdata.c
	$(CC_FOR_BUILD) `test -f gcmdata.c || echo '$(srcdir)/'`gcmdata.c \
	-o gcmdata$(EXEEXT_FOR_BUILD)

eccdata$(EXEEXT_FOR_BUILD): eccdata.c mini-gmp.c mini-gmp.h
	$(CC_FOR_BUILD) `test -f eccdata.c || echo '$(srcdir)/'`eccdata.c \
	-o eccdata$(EXEEXT_FOR_BUILD)

# desCore rules
# It seems using $(srcdir)/ doesn't work with GNU make 3.79.1
# des_headers = $(srcdir)/rotors.h $(srcdir)/keymap.h 
des_headers = rotors.h keymap.h 

# Generate DES headers.
$(des_headers): desdata.c
	$(MAKE) desdata$(EXEEXT)
	f="$(srcdir)/`basename $@`"; \
	  ./desdata$(EXEEXT) $(@F) > $${f}T; \
	  test -s $${f}T && mv -f $${f}T $$f

des.$(OBJEXT): des.c des.h $(des_headers)

# Generate ECC files.
# Some possible choices for 192:
#	k = 15, c = 4,  64 entries,  ~3 KB
#	k = 20, c = 6, 128 entries,  ~6 KB
#	k = 10, c = 6, 256 entries, ~12 KB
#	k =  7, c = 6, 320 entries, ~15 KB
#	k =  9, c = 7, 512 entries, ~24 KB
ecc-192.h: eccdata.stamp
	./eccdata$(EXEEXT_FOR_BUILD) 192 7 6 $(GMP_NUMB_BITS) > $@T && mv $@T $@
# Some possible choices for 224:
#	k = 18, c = 4,  64 entries,  ~4 KB
#	k = 24, c = 6, 128 entries,  ~8 KB
#	k = 12, c = 6, 256 entries, ~16 KB
#	k =  8, c = 6, 320 entries, ~20 KB
#	k = 10, c = 7, 512 entries, ~32 KB
ecc-224.h: eccdata.stamp
	./eccdata$(EXEEXT_FOR_BUILD) 224 12 6 $(GMP_NUMB_BITS) > $@T && mv $@T $@
# Some possible choices for 256:
#	k = 20, c = 4,  64 entries,  ~4 KB
#	k = 27, c = 6, 128 entries,  ~8 KB
#	k = 14, c = 6, 256 entries, ~16 KB
#	k =  9, c = 6, 320 entries, ~20 KB
#	k = 12, c = 7, 512 entries, ~32 KB
ecc-256.h: eccdata.stamp
	./eccdata$(EXEEXT_FOR_BUILD) 256 14 6 $(GMP_NUMB_BITS) > $@T && mv $@T $@
# Some possible choices for 384:
#	k = 31, c = 4,  64 entries,  ~6 KB
#	k = 41, c = 6, 128 entries, ~12 KB
#	k = 20, c = 6, 256 entries, ~24 KB
#	k = 14, c = 6, 320 entries, ~30 KB
#	k = 18, c = 7, 512 entries, ~48 KB
ecc-384.h: eccdata.stamp
	./eccdata$(EXEEXT_FOR_BUILD) 384 41 6 $(GMP_NUMB_BITS) > $@T && mv $@T $@
# Some possible choices for 521:
#	k = 42, c = 4,  64 entries,  ~9 KB
#	k = 56, c = 6, 128 entries, ~18 KB
#	k = 28, c = 6, 256 entries, ~35 KB
#	k = 19, c = 6, 320 entries, ~44 KB
#	k = 24, c = 7, 512 entries, ~70 KB
ecc-521.h: eccdata.stamp
	./eccdata$(EXEEXT_FOR_BUILD) 521 56 6 $(GMP_NUMB_BITS) > $@T && mv $@T $@

eccdata.stamp: eccdata.c
	$(MAKE) eccdata$(EXEEXT_FOR_BUILD)
	echo stamp > eccdata.stamp

ecc-192.$(OBJEXT): ecc-192.h
ecc-224.$(OBJEXT): ecc-224.h
ecc-256.$(OBJEXT): ecc-256.h
ecc-384.$(OBJEXT): ecc-384.h
ecc-521.$(OBJEXT): ecc-521.h

ecc-192.p$(OBJEXT): ecc-192.h
ecc-224.p$(OBJEXT): ecc-224.h
ecc-256.p$(OBJEXT): ecc-256.h
ecc-384.p$(OBJEXT): ecc-384.h
ecc-521.p$(OBJEXT): ecc-521.h

.asm.s:
	$(M4) $(srcdir)/asm.m4 machine.m4 config.m4 $< >$@T \
	&& test -s $@T && mv -f $@T $@

.s.$(OBJEXT):
	$(COMPILE) $(CCPIC_MAYBE) -c $<

.s.p$(OBJEXT):
	$(COMPILE) $(SHLIBCFLAGS) -c $< -o $@

# Texinfo rules
.texinfo.info:
	cd $(srcdir) && $(MAKEINFO) --output $@ `basename "$<"`

.texinfo.html:
	cd $(srcdir) && $(MAKEINFO) --html --no-split \
	  --output $@T `basename "$<"` \
	  && test -s $@T && mv -f $@T $@

.texinfo.dvi:
	cd $(srcdir) && texi2dvi -b `basename "$<"`

.dvi.ps:
	cd $(srcdir) && dvips -Ppdf -G0 -o `basename "$<" .dvi`.ps `basename "$<"`

# Avoid rebuilding .dvi and .ps files when the .texinfo source is unchanged.
PS2PDFFLAGS=-dCompatibilityLevel=1.3 -dMAxSubsetPct=100 -dSubsetFonts=true -dEmbedAllFonts=true
.texinfo.pdf:
	$(MAKE) `basename "$<" .texinfo`.ps
	cd $(srcdir) && ps2pdf $(PS2PDFFLAGS) `basename "$<" .texinfo`.ps

# Configure-related rules, mostly copied from the autoconf manual. No
# $(srcdir) prefixes on the targets, though.

configure: configure.ac aclocal.m4
	cd $(srcdir) && $(AUTOCONF)

# autoheader might not change config.h.in, so touch a stamp file.
config.h.in: stamp-h.in
stamp-h.in: configure.ac aclocal.m4
	cd $(srcdir) && $(AUTOHEADER)
	echo timestamp > $(srcdir)/stamp-h.in

config.status: configure
	./config.status --recheck

config.h: stamp-h
stamp-h: config.h.in config.status
	./config.status config.h
	echo timestamp > stamp-h

Makefile: Makefile.in config.status
	./config.status $@

config.make: config.make.in config.status
	./config.status $@

config.m4: config.m4.in config.status
	./config.status $@

nettle.pc: nettle.pc.in config.status
	./config.status $@

hogweed.pc: hogweed.pc.in config.status
	./config.status $@

# Installation
install-doc: # install-info
install-here: install-doc install-headers install-static install-pkgconfig \
	 install-shared-nettle  install-shared-hogweed

install-static: $(LIBTARGETS)
	$(MKDIR_P) $(DESTDIR)$(libdir)
	for f in $(LIBTARGETS); do \
	  $(INSTALL_DATA) $$f $(DESTDIR)$(libdir) ; \
	done

install-dll-nettle:
	$(MKDIR_P) $(DESTDIR)$(bindir)
	$(INSTALL_DATA) $(LIBNETTLE_FORLINK) $(DESTDIR)$(bindir)/$(LIBNETTLE_FORLINK)

install-shared-nettle: $(LIBNETTLE_FORLINK) # install-dll-nettle
	$(MKDIR_P) $(DESTDIR)$(libdir)
	$(INSTALL_DATA) $(LIBNETTLE_FILE_SRC) $(DESTDIR)$(libdir)/$(LIBNETTLE_FILE)
	[ -z "$(LIBNETTLE_SONAME)" ] \
                || (cd $(DESTDIR)$(libdir) \
		&& rm -f $(LIBNETTLE_SONAME) $(LIBNETTLE_FORLINK) \
		&& $(LN_S) $(LIBNETTLE_FILE) $(LIBNETTLE_SONAME) \
		&& $(LN_S) $(LIBNETTLE_FILE) $(LIBNETTLE_FORLINK))

install-dll-hogweed:
	$(MKDIR_P) $(DESTDIR)$(bindir)
	$(INSTALL_DATA) $(LIBHOGWEED_FORLINK) $(DESTDIR)$(bindir)/$(LIBHOGWEED_FORLINK)

install-shared-hogweed: $(LIBHOGWEED_FORLINK) # install-dll-hogweed
	$(MKDIR_P) $(DESTDIR)$(libdir)
	$(INSTALL_DATA) $(LIBHOGWEED_FILE_SRC) $(DESTDIR)$(libdir)/$(LIBHOGWEED_FILE)
	[ -z "$(LIBHOGWEED_SONAME)" ] \
                || (cd $(DESTDIR)$(libdir) \
		&& rm -f $(LIBHOGWEED_SONAME) $(LIBHOGWEED_FORLINK) \
		&& $(LN_S) $(LIBHOGWEED_FILE) $(LIBHOGWEED_SONAME) \
		&& $(LN_S) $(LIBHOGWEED_FILE) $(LIBHOGWEED_FORLINK))

# I'd like to use makes VPATH search to locate the files to be
# installed. But it seems most make programs don't set $<, $^, $? and
# friends for ordinary explicit rules.

install-info: nettle.info
	$(MKDIR_P) $(DESTDIR)$(infodir)
	f=nettle.info ; \
	[ -f $$f ] || f="$(srcdir)/$$f" ; \
	$(INSTALL_DATA) "$$f" $(DESTDIR)$(infodir) ; \
	if (install-info --version && \
	    install-info --version 2>&1 | sed 1q | grep -i -v debian) >/dev/null 2>&1; then \
	  install-info --info-dir="$(DESTDIR)$(infodir)" "$$f" ; \
	else : ; fi

# NOTE: I'd like to use $^, but that's a GNU extension. $? should be
# more portable, and equivalent for phony targets.
install-headers: $(INSTALL_HEADERS)
	$(MKDIR_P) $(DESTDIR)$(includedir)/nettle
	for f in $(INSTALL_HEADERS) ; do \
	  if [ -f "$$f" ] ; then \
	    $(INSTALL_DATA) "$$f" $(DESTDIR)$(includedir)/nettle ; \
	  else \
	    $(INSTALL_DATA) "$(srcdir)/$$f" $(DESTDIR)$(includedir)/nettle ; \
	  fi ; done

install-pkgconfig: $(PKGCONFIG_FILES)
	$(MKDIR_P) $(DESTDIR)$(pkgconfigdir)
	for f in $(PKGCONFIG_FILES) ; do \
	  $(INSTALL_DATA) "$$f" $(DESTDIR)$(pkgconfigdir) ; \
	done

# Uninstall
uninstall-here: uninstall-info uninstall-headers uninstall-static \
		uninstall-pkgconfig  uninstall-shared

uninstall-static:
	for f in $(LIBTARGETS) ; do \
	  rm -f $(DESTDIR)$(libdir)/$$f ; \
	done

uninstall-headers:
	for f in $(INSTALL_HEADERS) ; do \
	  rm -f $(DESTDIR)$(includedir)/nettle/$$f ; \
	done

uninstall-info:
	if (install-info --version && \
	    install-info --version 2>&1 | sed 1q | grep -i -v debian) >/dev/null 2>&1; then \
	  install-info --info-dir="$(DESTDIR)$(infodir)" --remove "$(DESTDIR)$(infodir)"/nettle.info ; \
	else : ; fi
	-rm -f $(DESTDIR)$(infodir)/nettle.info

# FIXME: Leaves the links around
uninstall-shared: uninstall-shared-nettle  uninstall-shared-hogweed

uninstall-dll-nettle:
	rm -f $(DESTDIR)$(bindir)/$(LIBNETTLE_FORLINK)

uninstall-shared-nettle: # uninstall-dll-nettle
	rm -f $(DESTDIR)$(libdir)/$(LIBNETTLE_FILE)
	[ -z "$(LIBNETTLE_SONAME)" ] \
		|| rm -f $(LIBNETTLE_SONAME) $(LIBNETTLE_FORLINK)

uninstall-dll-hogweed:
	rm -f $(DESTDIR)$(bindir)/$(LIBHOGWEED_FORLINK)

uninstall-shared-hogweed: # uninstall-dll-hogweed
	rm -f $(DESTDIR)$(libdir)/$(LIBHOGWEED_FILE)
	[ -z "$(LIBHOGWEED_SONAME)" ] \
		|| rm -f $(LIBHOGWEED_SONAME) $(LIBHOGWEED_FORLINK)

uninstall-pkgconfig:
	for f in $(PKGCONFIG_FILES) ; do \
	  rm -f $(DESTDIR)$(pkgconfigdir)/$$f ; \
	done

# Distribution
distdir = $(PACKAGE_NAME)-$(PACKAGE_VERSION)
top_distdir = $(distdir)

# NOTE: We should handle both absolute and relative $destdir.

distdir: $(DISTFILES)
	rm -rf "$(distdir)"
	mkdir "$(distdir)"
	set -e; for f in $(DISTFILES) ; do \
	  if [ -f "$$f" ] ; then cp "$$f" "$(distdir)" ; \
	  else cp "$(srcdir)/$$f" "$(distdir)" ; \
	  fi ; \
	done
	set -e; for d in sparc32 sparc64 x86 x86_64 arm arm/neon arm/v6; do \
	  mkdir "$(distdir)/$$d" ; \
	  find "$(srcdir)/$$d" -maxdepth 1 '(' -name '*.asm' -o -name '*.m4' ')' \
	    -exec cp '{}' "$(distdir)/$$d" ';' ; \
	done
	set -e; for d in $(SUBDIRS); do \
	  sd="$(distdir)/$$d" ; \
	  mkdir "$$sd" && $(MAKE) -C $$d distdir="`cd $$sd && pwd`" $@ ; \
	done

dist: distdir
	tar cf - $(distdir) | gzip -c >$(distdir).tar.gz	
	rm -rf $(distdir)

rm_distcheck = test ! -d distcheck-tmp \
	    || { find distcheck-tmp -type d ! -perm -200 -exec chmod u+w {} ';' \
                 && rm -fr distcheck-tmp; }; 

distcheck: dist
	$(rm_distcheck)
	mkdir distcheck-tmp
	gzip -d < $(distdir).tar.gz \
	  | { cd distcheck-tmp && tar xf - && chmod -R a-w $(distdir) ; }
	mkdir distcheck-tmp/build
	mkdir distcheck-tmp/install
	cd distcheck-tmp/build && ../$(distdir)/configure --prefix="`cd ../install && pwd`"
	cd distcheck-tmp/build && $(MAKE)
	cd distcheck-tmp/build && $(MAKE) check
	cd distcheck-tmp/build && $(MAKE) install
	cd distcheck-tmp/build && $(MAKE) uninstall
	cd distcheck-tmp && find install -type f -print > leftover-install-files
	@test `cat distcheck-tmp/leftover-install-files | wc -l` -le 1  \
	  || { echo "ERROR: files left after uninstall:" ; \
	       cat distcheck-tmp/leftover-install-files ; \
	       exit 1; }
	chmod -R a-w distcheck-tmp/install
	mkdir distcheck-tmp/destdir
	destdir="`cd distcheck-tmp/destdir && pwd`" \
	  && cd distcheck-tmp/build \
	  && $(MAKE) install DESTDIR="$$destdir" \
	  && $(MAKE) uninstall DESTDIR="$$destdir"
	cd distcheck-tmp && find destdir -type f -print > leftover-destdir-files
	@test `cat distcheck-tmp/leftover-destdir-files | wc -l` -le 1  \
	  || { echo "ERROR: destdir files left after uninstall:" ; \
	       cat distcheck-tmp/leftover-destdir-files ; \
	       exit 1; }	
	cd distcheck-tmp/build && $(MAKE) dist
	cd distcheck-tmp/build && rm *.gz
	cd distcheck-tmp/build && $(MAKE) distclean
	cd distcheck-tmp && find build -type f -print > leftover-build-files
	@test `cat distcheck-tmp/leftover-build-files | wc -l` -eq 0  \
	  || { echo "ERROR: files left in build directory after distclean:" ; \
	       cat distcheck-tmp/leftover-build-files ; \
	       exit 1; }
	$(rm_distcheck)

clean-here:
	-rm -f $(TARGETS) $(IMPLICIT_TARGETS) *.$(OBJEXT) *.p$(OBJEXT) *.s \
		ecc-192.h ecc-224.h ecc-256.h ecc-384.h ecc-521.h \
		eccdata$(EXEEXT_FOR_BUILD) eccdata.stamp
	-rm -rf .lib

distclean-here: clean-here
	-rm -f config.h stamp-h config.log config.status machine.m4 \
	config.make config.m4 Makefile nettle-stdint.h \
	nettle.pc hogweed.pc \
	*.asm *.d

maintainer-clean-here:
	-rm -f $(DOCTARGETS) *.dvi *.ps

tags-here:	
	etags -o $(srcdir)/TAGS $(srcdir)/*.c $(srcdir)/*.h

DEP_FILES = $(SOURCES:.c=.$(OBJEXT).d) $(SOURCES:.c=.p$(OBJEXT).d) asm.d
include  $(DEP_FILES)
