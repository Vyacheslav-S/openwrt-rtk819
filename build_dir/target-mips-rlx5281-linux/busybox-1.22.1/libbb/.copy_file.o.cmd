cmd_libbb/copy_file.o := rsdk-linux-gcc -Wp,-MD,libbb/.copy_file.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.22.1)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wno-format-security -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -fno-unwind-tables -fno-asynchronous-unwind-tables -Os  -Os -pipe -march=5281 -fno-caller-saves   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(copy_file)"  -D"KBUILD_MODNAME=KBUILD_STR(copy_file)" -c -o libbb/copy_file.o libbb/copy_file.c

deps_libbb/copy_file.o := \
  libbb/copy_file.c \
    $(wildcard include/config/feature/non/posix/cp.h) \
    $(wildcard include/config/feature/verbose/cp/message.h) \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/feature/preserve/hardlinks.h) \
  include/libbb.h \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/use/bb/shadow.h) \
    $(wildcard include/config/feature/utmp.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/use/bb/pwd/grp.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/seamless/xz.h) \
    $(wildcard include/config/feature/seamless/lzma.h) \
    $(wildcard include/config/feature/seamless/bz2.h) \
    $(wildcard include/config/feature/seamless/gz.h) \
    $(wildcard include/config/feature/seamless/z.h) \
    $(wildcard include/config/feature/check/names.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/long/opts.h) \
    $(wildcard include/config/feature/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/feature/individual.h) \
    $(wildcard include/config/echo.h) \
    $(wildcard include/config/printf.h) \
    $(wildcard include/config/test.h) \
    $(wildcard include/config/kill.h) \
    $(wildcard include/config/chown.h) \
    $(wildcard include/config/ls.h) \
    $(wildcard include/config/xxx.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/feature/hwib.h) \
    $(wildcard include/config/desktop.h) \
    $(wildcard include/config/feature/crond/d.h) \
    $(wildcard include/config/use/bb/crypt.h) \
    $(wildcard include/config/feature/adduser/to/group.h) \
    $(wildcard include/config/feature/del/user/from/group.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/feature/editing/save/on/exit.h) \
    $(wildcard include/config/pmap.h) \
    $(wildcard include/config/feature/show/threads.h) \
    $(wildcard include/config/feature/ps/additional/columns.h) \
    $(wildcard include/config/feature/topmem.h) \
    $(wildcard include/config/feature/top/smp/process.h) \
    $(wildcard include/config/killall.h) \
    $(wildcard include/config/pgrep.h) \
    $(wildcard include/config/pkill.h) \
    $(wildcard include/config/pidof.h) \
    $(wildcard include/config/sestatus.h) \
    $(wildcard include/config/unicode/support.h) \
    $(wildcard include/config/feature/mtab/support.h) \
    $(wildcard include/config/feature/clean/up.h) \
    $(wildcard include/config/feature/devfs.h) \
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
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/ctype.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/uClibc_touplow.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/dirent.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/dirent.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/errno.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/errno.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/linux/errno.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/asm/errno.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/asm-generic/errno-base.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/syscall.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/sysnum.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/fcntl.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/fcntl.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sgidefs.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/types.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/time.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/select.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/select.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/sigset.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/time.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/sysmacros.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/pthreadtypes.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/uio.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/stat.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/stat.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/inttypes.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/netdb.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/netinet/in.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/socket.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/uio.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/socket.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/socket_type.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/sockaddr.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/asm/socket.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/asm/sockios.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/asm/ioctl.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/asm-generic/ioctl.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/in.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/rpc/netdb.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/siginfo.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/netdb.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/setjmp.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/setjmp.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/signal.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/signum.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/sigaction.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/sigcontext.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/sigstack.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/ucontext.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/sigthread.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/stdio.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/uClibc_stdio.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/wchar.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/include/stdarg.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/stdlib.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/waitflags.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/waitstatus.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/alloca.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/string.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/libgen.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/poll.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/poll.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/poll.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/ioctl.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/ioctls.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/asm/ioctls.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/ioctl-types.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/ttydefaults.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/mman.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/mman.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/resource.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/resource.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/time.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/wait.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/termios.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/termios.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/uClibc_clk_tck.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/param.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/linux/param.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/asm/param.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/asm-generic/param.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/pwd.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/grp.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/shadow.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/paths.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/mntent.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/sys/statfs.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/bits/statfs.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/../../../../mips-linux-uclibc/include/arpa/inet.h \
  include/xatonum.h \

libbb/copy_file.o: $(deps_libbb/copy_file.o)

$(deps_libbb/copy_file.o):
