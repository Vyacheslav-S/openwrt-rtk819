cmd_libbb/perror_nomsg.o := rsdk-linux-gcc -Wp,-MD,libbb/.perror_nomsg.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.22.1)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wno-format-security -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -fno-unwind-tables -fno-asynchronous-unwind-tables -Os  -Os -pipe -march=5281 -fno-caller-saves   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(perror_nomsg)"  -D"KBUILD_MODNAME=KBUILD_STR(perror_nomsg)" -c -o libbb/perror_nomsg.o libbb/perror_nomsg.c

deps_libbb/perror_nomsg.o := \
  libbb/perror_nomsg.c \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config/big/endian.h) \
    $(wildcard include/config/little/endian.h) \
    $(wildcard include/config/nommu.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/include-fixed/limits.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/include-fixed/syslimits.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/limits.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/features.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/uClibc_config.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/cdefs.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/posix1_lim.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/local_lim.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/linux/limits.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/uClibc_local_lim.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/posix2_lim.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/xopen_lim.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/stdio_lim.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/byteswap.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/byteswap.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/byteswap-common.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/endian.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/endian.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/include/stdint.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/stdint.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/wchar.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/wordsize.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/include/stdbool.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/unistd.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/posix_opt.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/uClibc_posix_opt.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/environments.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/types.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/typesizes.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/include/stddef.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/confname.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/getopt.h \

libbb/perror_nomsg.o: $(deps_libbb/perror_nomsg.o)

$(deps_libbb/perror_nomsg.o):
