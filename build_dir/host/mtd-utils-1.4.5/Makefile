
# -*- sh -*-

CPPFLAGS += -I./include $(ZLIBCPPFLAGS) $(LZOCPPFLAGS) $(XZCPPFLAGS) -I./include/linux/lzma

ifeq ($(WITHOUT_XATTR), 1)
  CPPFLAGS += -DWITHOUT_XATTR
endif
ifeq ($(WITHOUT_LZO), 1)
  CPPFLAGS += -DWITHOUT_LZO
else
  LZOLDLIBS = -llzo2
endif

ifeq ($(shell uname -o),Cygwin)
CPPFLAGS += -I./include/cygwin
endif

ifneq ($(shell uname -o),Cygwin)
SUBDIRS = lib ubi-utils mkfs.ubifs
TESTS = tests

TARGETS = ftl_format flash_erase nanddump doc_loadbios \
	ftl_check mkfs.jffs2 flash_lock flash_unlock flash_info \
	flash_otp_info flash_otp_dump mtd_debug flashcp nandwrite nandtest \
	jffs2dump \
	nftldump nftl_format docfdisk \
	rfddump rfdformat \
	serve_image recv_image \
	sumtool #jffs2reader
else
SUBDIRS =
TARGETS = mkfs.jffs2
endif
SCRIPTS = flash_eraseall

SYMLINKS =

LDLIBS = -L$(BUILDDIR)/lib -lmtd
LDDEPS = $(BUILDDIR)/lib/libmtd.a

include common.mk

# mkfs.ubifs needs -lubi which is in ubi-utils/
subdirs_mkfs.ubifs_all: subdirs_ubi-utils_all

clean::
	-rm -f $(SYMLINKS)
ifneq ($(BUILDDIR)/.git,)
ifneq ($(BUILDDIR),.)
ifneq ($(BUILDDIR),$(CURDIR))
	rm -rf $(BUILDDIR)
endif
endif
endif
	$(MAKE) -C $(TESTS) clean

$(SYMLINKS):
	ln -sf ../fs/jffs2/$@ $@

$(BUILDDIR)/mkfs.jffs2: $(addprefix $(BUILDDIR)/,\
	compr_rtime.o mkfs.jffs2.o compr_zlib.o \
	$(if $(WITHOUT_LZO),,compr_lzo.o) \
	compr_lzma.o lzma/LzFind.o lzma/LzmaEnc.o lzma/LzmaDec.o \
	compr.o rbtree.o)
LDFLAGS_mkfs.jffs2 = $(ZLIBLDFLAGS) $(LZOLDFLAGS)
LDLIBS_mkfs.jffs2  = -lz $(LZOLDLIBS)

$(BUILDDIR)/jffs2reader: $(BUILDDIR)/jffs2reader.o
LDFLAGS_jffs2reader = $(ZLIBLDFLAGS) $(LZOLDFLAGS)
LDLIBS_jffs2reader  = -lz $(LZOLDLIBS)

$(BUILDDIR)/lib/libmtd.a: subdirs_lib_all ;

install:: ${TARGETS} ${SCRIPTS}
	mkdir -p ${DESTDIR}/${SBINDIR}
	install -m 0755 ${TARGETS} ${SCRIPTS} ${DESTDIR}/${SBINDIR}/
	mkdir -p ${DESTDIR}/${MANDIR}/man1
	gzip -9c mkfs.jffs2.1 > ${DESTDIR}/${MANDIR}/man1/mkfs.jffs2.1.gz

tests::
	$(MAKE) -C $(TESTS)

cscope:
	cscope -bR
