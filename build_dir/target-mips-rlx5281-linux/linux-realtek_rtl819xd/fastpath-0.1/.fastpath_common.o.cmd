cmd_/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/fastpath-0.1/fastpath_common.o := rsdk-linux-gcc -Wp,-MD,/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/fastpath-0.1/.fastpath_common.o.d  -nostdinc -isystem /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/include -I/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include -Iarch/rlx/include/generated  -Iinclude -I/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi -Iarch/rlx/include/generated/uapi -I/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi -Iinclude/generated/uapi -include /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0x80000000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -fno-reorder-blocks -fno-tree-ch -fno-caller-saves -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -ffreestanding -EB -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEB -D_MIPSEB -D__MIPSEB -D__MIPSEB__ -I/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/ -I/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/soc-rtl819xd/ -Iinclude/asm-rlx -I/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -I /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/net -D__KERNEL__ -DCONFIG_RTK_IPTABLES_FAST_PATH -DCONFIG_FAST_PATH_MODULE  -DMODULE -mlong-calls  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(fastpath_common)"  -D"KBUILD_MODNAME=KBUILD_STR(fastpath)" -c -o /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/fastpath-0.1/fastpath_common.o /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/fastpath-0.1/fastpath_common.c

source_/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/fastpath-0.1/fastpath_common.o := /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/fastpath-0.1/fastpath_common.c

deps_/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/fastpath-0.1/fastpath_common.o := \
    $(wildcard include/config/rtk/vlan/support.h) \
    $(wildcard include/config/rtl/fast/ipv6.h) \
    $(wildcard include/config/bridge.h) \
    $(wildcard include/config/net/sched.h) \
    $(wildcard include/config/rtl/hw/qos/support.h) \
    $(wildcard include/config/rtl/nf/conntrack/garbage/new.h) \
    $(wildcard include/config/rtl/eth/driver/refine.h) \
    $(wildcard include/config/rtl/fast/gre.h) \
    $(wildcard include/config/rtl/speed/monitor.h) \
    $(wildcard include/config/rtl/pppoe/direct/reply.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/rtl/layered/driver/l4.h) \
    $(wildcard include/config/rtl/8198.h) \
    $(wildcard include/config/rtl/8196ct.h) \
    $(wildcard include/config/rtl/qos/patch.h) \
    $(wildcard include/config/rtk/voip/qos.h) \
    $(wildcard include/config/rtk/vlan/wan/tag/support.h) \
    $(wildcard include/config/rtl/819x.h) \
    $(wildcard include/config/netfilter/xt/target/trace.h) \
    $(wildcard include/config/ip/vs.h) \
    $(wildcard include/config/openwrt/sdk.h) \
    $(wildcard include/config/rtl/fast/pppoe.h) \
    $(wildcard include/config/rtl/dscp/iptable/check.h) \
    $(wildcard include/config/rtl/layered/driver/acl.h) \
    $(wildcard include/config/rtl/netif/mapping.h) \
    $(wildcard include/config/rtl/iptables/rule/2/acl.h) \
    $(wildcard include/config/rtl/local/public.h) \
    $(wildcard include/config/rtl/public/ssid.h) \
    $(wildcard include/config/bridge/vlan/filtering.h) \
    $(wildcard include/config/rtl/fast/filter.h) \
    $(wildcard include/config/support/russia/features.h) \
    $(wildcard include/config/rtl/l2tp/direct/ppp/reply.h) \
    $(wildcard include/config/fast/path.h) \
    $(wildcard include/config/udp/frag/cache.h) \
  include/net/rtl/rtl_types.h \
    $(wildcard include/config/rtl865x/nicdrv2.h) \
    $(wildcard include/config/rtl/8198c.h) \
    $(wildcard include/config/rtl/8197f.h) \
    $(wildcard include/config/rtl/dynamic/iram/mapping/for/wapi.h) \
    $(wildcard include/config/rtl/8196c.h) \
    $(wildcard include/config/rtl/disable/eth/mips16.h) \
    $(wildcard include/config/rlx.h) \
    $(wildcard include/config/rtl/ulinker/brsc.h) \
    $(wildcard include/config/rtl/eth/priv/skb.h) \
    $(wildcard include/config/rtl8196c/eth/iot.h) \
    $(wildcard include/config/mp/psd/support.h) \
    $(wildcard include/config/rtl8196c/green/ethernet.h) \
    $(wildcard include/config/rtl/8196c/esd.h) \
    $(wildcard include/config/rtl/819xd.h) \
    $(wildcard include/config/rtl/8198/esd.h) \
    $(wildcard include/config/rtl/8196e.h) \
    $(wildcard include/config/rtl/8197d/dyn/thr.h) \
    $(wildcard include/config/rtl/8881a.h) \
    $(wildcard include/config/rtl/819xdt.h) \
    $(wildcard include/config/rtl/log/debug.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/rtl819x/spi/flash.h) \
    $(wildcard include/config/rtl/proc/new.h) \
    $(wildcard include/config/cpu/little/endian.h) \
    $(wildcard include/config/rtl/switch/new/descriptor.h) \
    $(wildcard include/config/rtl/ap/package.h) \
    $(wildcard include/config/rtl/check/switch/tx/hangup.h) \
    $(wildcard include/config/rtl/gso.h) \
    $(wildcard include/config/rtl/97f/hw/tx/csum.h) \
    $(wildcard include/config/rtl/hw/tx/csum.h) \
    $(wildcard include/config/rtl/eth/tx/sg.h) \
    $(wildcard include/config/rtl/tso.h) \
    $(wildcard include/config/rtl/8367r/support.h) \
    $(wildcard include/config/rtl/adaptable/tso.h) \
    $(wildcard include/config/rtl/wtdog.h) \
  include/generated/uapi/linux/version.h \
  include/linux/module.h \
    $(wildcard include/config/module/stripped.h) \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/module/sig.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/debug/set/module/ronx.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/types.h \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/types.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/posix_types.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/sgidefs.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/asm-generic/posix_types.h \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/const.h \
  include/linux/stat.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/stat.h \
  include/uapi/linux/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141001/bin/../lib/gcc/mips-linux-uclibc/4.6.4/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/linkage.h \
  include/linux/bitops.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
    $(wildcard include/config/cpu/has/cls.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/kvm/guest.h) \
    $(wildcard include/config/dma/noncoherent.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/byteorder.h \
  include/linux/byteorder/big_endian.h \
  include/uapi/linux/byteorder/big_endian.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/cpu-features.h \
    $(wildcard include/config/cpu/has/fpu.h) \
    $(wildcard include/config/cpu/has/wmpu.h) \
    $(wildcard include/config/cpu/has/llsc.h) \
    $(wildcard include/config/cpu/has/tls.h) \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/cpu.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/cpu-info.h \
    $(wildcard include/config/mips/mt/smp.h) \
    $(wildcard include/config/mips/mt/smtc.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/soc-rtl819xd/bspcpu.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  include/asm-generic/bitops/sched.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/arch_hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/le.h \
  include/asm-generic/bitops/ext2-atomic.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/typecheck.h \
  include/linux/printk.h \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
  include/linux/kern_levels.h \
  include/linux/dynamic_debug.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/uapi/linux/string.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/string.h \
    $(wildcard include/config/cpu/rlx4181.h) \
    $(wildcard include/config/cpu/rlx5181.h) \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/errno.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/errno.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/asm-generic/errno-base.h \
  include/uapi/linux/kernel.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/sysinfo.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/context/tracking.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  include/linux/bug.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/break.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/break.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/thread_info.h \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/processor.h \
    $(wildcard include/config/cpu/has/radiax.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/cachectl.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/rlxregs.h \
    $(wildcard include/config/cpu/rlx5281.h) \
    $(wildcard include/config/cpu/rlx4281.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/hazards.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/prefetch.h \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/irqflags.h \
    $(wildcard include/config/irq/cpu.h) \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  include/linux/spinlock_up.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_up.h \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/atomic.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/cmpxchg.h \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/atomic-long.h \
  include/asm-generic/atomic64.h \
  include/linux/math64.h \
  include/uapi/linux/time.h \
  include/linux/uidgid.h \
    $(wildcard include/config/uidgid/strict/type/checks.h) \
    $(wildcard include/config/user/ns.h) \
  include/linux/highuid.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/cma.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/wait.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/current.h \
  include/asm-generic/current.h \
  include/uapi/linux/wait.h \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/nodemask.h \
    $(wildcard include/config/movable/node.h) \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/page-flags-layout.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/generated/bounds.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/page.h \
    $(wildcard include/config/mips/huge/tlb/support.h) \
    $(wildcard include/config/cpu/mips32.h) \
  include/linux/pfn.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/io.h \
    $(wildcard include/config/pci.h) \
  include/asm-generic/iomap.h \
    $(wildcard include/config/has/ioport.h) \
    $(wildcard include/config/generic/iomap.h) \
  include/asm-generic/pci_iomap.h \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/pgtable-bits.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/mach-generic/ioremap.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/mach-generic/mangle-port.h \
    $(wildcard include/config/swap/io/space.h) \
  include/asm-generic/memory_model.h \
  include/asm-generic/getorder.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/mutex/spin/on/owner.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/rcu/user/qs.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tiny/preempt/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
  include/linux/completion.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/rcutiny.h \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/timex.h \
  include/uapi/linux/timex.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/param.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/uapi/asm-generic/param.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/timex.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/sched/book.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/topology.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/sysctl.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/rbtree.h \
  include/uapi/linux/sysctl.h \
  include/linux/elf.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/elf.h \
    $(wildcard include/config/mips32/n32.h) \
    $(wildcard include/config/mips32/o32.h) \
    $(wildcard include/config/mips32/compat.h) \
  include/uapi/linux/elf.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/elf-em.h \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kobject_ns.h \
  include/linux/kref.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/tracepoint.h \
  include/linux/static_key.h \
  include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/module.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/uaccess.h \
  include/net/netfilter/nf_conntrack.h \
    $(wildcard include/config/netfilter/debug.h) \
    $(wildcard include/config/nf/conntrack/mark.h) \
    $(wildcard include/config/nf/conntrack/secmark.h) \
    $(wildcard include/config/net/ns.h) \
    $(wildcard include/config/netfilter/xt/match/layer7.h) \
  include/linux/netfilter/nf_conntrack_common.h \
  include/uapi/linux/netfilter/nf_conntrack_common.h \
  include/linux/netfilter/nf_conntrack_tcp.h \
  include/uapi/linux/netfilter/nf_conntrack_tcp.h \
  include/linux/netfilter/nf_conntrack_dccp.h \
  include/net/netfilter/nf_conntrack_tuple.h \
  include/linux/netfilter/x_tables.h \
  include/linux/netdevice.h \
    $(wildcard include/config/dcb.h) \
    $(wildcard include/config/wlan.h) \
    $(wildcard include/config/ax25.h) \
    $(wildcard include/config/mac80211/mesh.h) \
    $(wildcard include/config/net/ipip.h) \
    $(wildcard include/config/net/ipgre.h) \
    $(wildcard include/config/ipv6/sit.h) \
    $(wildcard include/config/ipv6/tunnel.h) \
    $(wildcard include/config/rps.h) \
    $(wildcard include/config/netpoll.h) \
    $(wildcard include/config/xps.h) \
    $(wildcard include/config/bql.h) \
    $(wildcard include/config/rfs/accel.h) \
    $(wildcard include/config/fcoe.h) \
    $(wildcard include/config/net/poll/controller.h) \
    $(wildcard include/config/libfcoe.h) \
    $(wildcard include/config/wireless/ext.h) \
    $(wildcard include/config/ethernet/packet/mangle.h) \
    $(wildcard include/config/vlan/8021q.h) \
    $(wildcard include/config/net/dsa.h) \
    $(wildcard include/config/netprio/cgroup.h) \
    $(wildcard include/config/net/dsa/tag/dsa.h) \
    $(wildcard include/config/net/dsa/tag/trailer.h) \
    $(wildcard include/config/netpoll/trap.h) \
  include/linux/pm_qos.h \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/pm/runtime.h) \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/miscdevice.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/major.h \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/acpi.h) \
    $(wildcard include/config/pinctrl.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  include/linux/ioport.h \
  include/linux/klist.h \
  include/linux/pinctrl/devinfo.h \
  include/linux/pm.h \
    $(wildcard include/config/vt/console/sleep.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  include/linux/ratelimit.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/device.h \
  include/linux/pm_wakeup.h \
  include/linux/delay.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/delay.h \
  include/linux/rculist.h \
  include/linux/dmaengine.h \
    $(wildcard include/config/async/tx/enable/channel/switch.h) \
    $(wildcard include/config/rapidio/dma/engine.h) \
    $(wildcard include/config/dma/engine.h) \
    $(wildcard include/config/net/dma.h) \
    $(wildcard include/config/async/tx/dma.h) \
  include/linux/uio.h \
  include/uapi/linux/uio.h \
  include/linux/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
  include/linux/mm.h \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/ppc.h) \
    $(wildcard include/config/parisc.h) \
    $(wildcard include/config/metag.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/debug/vm/rb.h) \
    $(wildcard include/config/arch/uses/numa/prot/none.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/have/cmpxchg/double.h) \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mm/owner.h) \
    $(wildcard include/config/mmu/notifier.h) \
  include/linux/auxvec.h \
  include/uapi/linux/auxvec.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/auxvec.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/guard.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  include/linux/uprobes.h \
    $(wildcard include/config/arch/supports/uprobes.h) \
    $(wildcard include/config/uprobes.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/mmu.h \
  include/linux/range.h \
  include/linux/bit_spinlock.h \
  include/linux/shrinker.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/pgtable.h \
    $(wildcard include/config/cpu/supports/uncached/accelerated.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/pgtable-32.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/fixmap.h \
  include/asm-generic/pgtable-nopmd.h \
  include/asm-generic/pgtable-nopud.h \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/swap.h) \
  include/linux/huge_mm.h \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  include/linux/vm_event_item.h \
    $(wildcard include/config/migration.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/scatterlist.h \
  include/asm-generic/scatterlist.h \
    $(wildcard include/config/need/sg/dma/length.h) \
  include/linux/dynamic_queue_limits.h \
  include/linux/ethtool.h \
  include/linux/compat.h \
    $(wildcard include/config/compat/old/sigaction.h) \
    $(wildcard include/config/odd/rt/sigaction.h) \
  include/uapi/linux/ethtool.h \
  include/linux/if_ether.h \
  include/linux/skbuff.h \
    $(wildcard include/config/nf/conntrack.h) \
    $(wildcard include/config/bridge/netfilter.h) \
    $(wildcard include/config/xfrm.h) \
    $(wildcard include/config/net/cls/act.h) \
    $(wildcard include/config/ipv6/ndisc/nodetype.h) \
    $(wildcard include/config/network/secmark.h) \
    $(wildcard include/config/rtl/hardware/multicast.h) \
    $(wildcard include/config/rtl865x/lanport/restriction.h) \
    $(wildcard include/config/rtl/igmp/proxy.h) \
    $(wildcard include/config/rtl/sendfile/patch.h) \
    $(wildcard include/config/network/phy/timestamping.h) \
  include/linux/kmemcheck.h \
  include/linux/net.h \
  include/linux/random.h \
    $(wildcard include/config/arch/random.h) \
  include/uapi/linux/random.h \
    $(wildcard include/config/fips/rng.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/ioctl.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/uapi/asm-generic/ioctl.h \
  include/linux/irqnr.h \
    $(wildcard include/config/generic/hardirqs.h) \
  include/uapi/linux/irqnr.h \
  include/linux/fcntl.h \
  include/uapi/linux/fcntl.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/fcntl.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/asm-generic/fcntl.h \
  include/uapi/linux/net.h \
  include/linux/socket.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/socket.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/socket.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/sockios.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/sockios.h \
  include/uapi/linux/socket.h \
  include/linux/textsearch.h \
  include/linux/err.h \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/slab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/slub_def.h \
    $(wildcard include/config/slub/stats.h) \
    $(wildcard include/config/memcg/kmem.h) \
    $(wildcard include/config/slub/debug.h) \
  include/linux/kmemleak.h \
    $(wildcard include/config/debug/kmemleak.h) \
  include/net/checksum.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/checksum.h \
  include/linux/in6.h \
  include/uapi/linux/in6.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  include/linux/timerqueue.h \
  include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/arch/has/dma/set/coherent/mask.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  include/linux/dma-attrs.h \
  include/linux/dma-direction.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/dma-mapping.h \
    $(wildcard include/config/sgi/ip27.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/dma-coherence.h \
  include/asm-generic/dma-coherent.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/mach-generic/dma-coherence.h \
  include/asm-generic/dma-mapping-common.h \
  include/linux/dma-debug.h \
    $(wildcard include/config/dma/api/debug.h) \
  include/linux/netdev_features.h \
  include/net/flow_keys.h \
  include/uapi/linux/if_ether.h \
  include/net/net_namespace.h \
    $(wildcard include/config/ipv6.h) \
    $(wildcard include/config/ip/sctp.h) \
    $(wildcard include/config/ip/dccp.h) \
    $(wildcard include/config/netfilter.h) \
    $(wildcard include/config/nf/defrag/ipv6.h) \
    $(wildcard include/config/wext/core.h) \
  include/net/netns/core.h \
  include/net/netns/mib.h \
    $(wildcard include/config/xfrm/statistics.h) \
  include/net/snmp.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/snmp.h \
  include/linux/u64_stats_sync.h \
  include/net/netns/unix.h \
  include/net/netns/packet.h \
  include/net/netns/ipv4.h \
    $(wildcard include/config/ip/multiple/tables.h) \
    $(wildcard include/config/ip/route/classid.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/ip/mroute.h) \
    $(wildcard include/config/ip/mroute/multiple/tables.h) \
  include/net/inet_frag.h \
  include/linux/percpu_counter.h \
  include/net/netns/ipv6.h \
    $(wildcard include/config/ipv6/multiple/tables.h) \
    $(wildcard include/config/ipv6/mroute.h) \
    $(wildcard include/config/ipv6/mroute/multiple/tables.h) \
  include/net/dst_ops.h \
  include/net/netns/sctp.h \
  include/net/netns/dccp.h \
  include/net/netns/netfilter.h \
  include/linux/proc_fs.h \
  include/linux/fs.h \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/direct/io.h) \
  include/linux/kdev_t.h \
  include/uapi/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/rculist_bl.h \
  include/linux/list_bl.h \
  include/linux/path.h \
  include/linux/radix-tree.h \
  include/linux/pid.h \
  include/linux/capability.h \
  include/uapi/linux/capability.h \
  include/linux/semaphore.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/fiemap.h \
  include/linux/migrate_mode.h \
  include/linux/percpu-rwsem.h \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/uapi/linux/fs.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/limits.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/projid.h \
  include/uapi/linux/quota.h \
  include/linux/nfs_fs_i.h \
  include/linux/netfilter.h \
    $(wildcard include/config/nf/nat/needed.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/if.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/hdlc/ioctl.h \
  include/linux/in.h \
  include/uapi/linux/in.h \
  include/uapi/linux/netfilter.h \
  include/net/flow.h \
  include/net/netns/x_tables.h \
    $(wildcard include/config/bridge/nf/ebtables.h) \
  include/net/netns/conntrack.h \
    $(wildcard include/config/nf/conntrack/proc/compat.h) \
    $(wildcard include/config/nf/conntrack/labels.h) \
  include/linux/list_nulls.h \
  include/net/netns/xfrm.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/xfrm.h \
  include/linux/seq_file_net.h \
  include/linux/seq_file.h \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/no/hz/common.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/virt/cpu/accounting/gen.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/bcache.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
    $(wildcard include/config/no/hz/full.h) \
  include/uapi/linux/sched.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/ptrace.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/isadep.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/ptrace.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/cputime.h \
  include/asm-generic/cputime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/asm-generic/cputime_jiffies.h \
  include/linux/sem.h \
  include/uapi/linux/sem.h \
  include/linux/ipc.h \
  include/uapi/linux/ipc.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/ipcbuf.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/asm-generic/ipcbuf.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/sembuf.h \
  include/linux/signal.h \
    $(wildcard include/config/old/sigaction.h) \
  include/uapi/linux/signal.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/signal.h \
    $(wildcard include/config/trad/signals.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/signal.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/asm-generic/signal-defs.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/sigcontext.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/sigcontext.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/siginfo.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/uapi/asm-generic/siginfo.h \
  include/linux/proportions.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/seccomp/filter.h) \
  include/uapi/linux/seccomp.h \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/resource.h \
  include/uapi/linux/resource.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/resource.h \
  include/asm-generic/resource.h \
  include/uapi/asm-generic/resource.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
  include/linux/key.h \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  include/linux/nsproxy.h \
  include/net/dsa.h \
  include/net/netprio_cgroup.h \
  include/linux/cgroup.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/cgroupstats.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/taskstats.h \
  include/linux/prio_heap.h \
  include/linux/idr.h \
  include/linux/xattr.h \
  include/uapi/linux/xattr.h \
  include/linux/hardirq.h \
  include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  include/linux/vtime.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/hardirq.h \
  include/asm-generic/hardirq.h \
  include/linux/irq_cpustat.h \
  include/linux/irq.h \
    $(wildcard include/config/generic/pending/irq.h) \
    $(wildcard include/config/hardirqs/sw/resend.h) \
  include/linux/irqreturn.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/irq.h \
  include/linux/irqdomain.h \
    $(wildcard include/config/irq/domain.h) \
    $(wildcard include/config/of/irq.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/mach-generic/irq.h \
    $(wildcard include/config/i8259.h) \
    $(wildcard include/config/irq/cpu/rm7k.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/irq_regs.h \
  include/linux/irqdesc.h \
    $(wildcard include/config/irq/preflow/fasteoi.h) \
    $(wildcard include/config/sparse/irq.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/hw_irq.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/neighbour.h \
  include/linux/netlink.h \
  include/net/scm.h \
    $(wildcard include/config/security/network.h) \
  include/linux/security.h \
    $(wildcard include/config/security/path.h) \
    $(wildcard include/config/security/network/xfrm.h) \
    $(wildcard include/config/securityfs.h) \
    $(wildcard include/config/security/yama.h) \
  include/uapi/linux/netlink.h \
  include/uapi/linux/netdevice.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/if_packet.h \
  include/linux/if_link.h \
  include/uapi/linux/if_link.h \
  include/uapi/linux/netfilter/x_tables.h \
  include/linux/netfilter_ipv4.h \
  include/uapi/linux/netfilter_ipv4.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/netfilter/nf_conntrack_tuple_common.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/netfilter/nf_conntrack_sctp.h \
  include/linux/netfilter/nf_conntrack_proto_gre.h \
  include/net/netfilter/ipv6/nf_conntrack_icmpv6.h \
  include/net/rtl/features/rtl_private_struct.h \
    $(wildcard include/config/rtl/hardware/nat.h) \
    $(wildcard include/config/rtl/hw/nat/bypass/pkt.h) \
  include/net/netfilter/ipv4/nf_conntrack_ipv4.h \
  include/net/netfilter/ipv6/nf_conntrack_ipv6.h \
  include/net/netfilter/nf_conntrack_core.h \
  include/net/netfilter/nf_conntrack_l3proto.h \
  include/net/netlink.h \
  include/net/netfilter/nf_conntrack_l4proto.h \
    $(wildcard include/config/nf/ct/netlink/timeout.h) \
  include/net/netns/generic.h \
  include/net/netfilter/nf_conntrack_ecache.h \
    $(wildcard include/config/nf/conntrack/events.h) \
  include/net/netfilter/nf_conntrack_expect.h \
  include/net/netfilter/nf_conntrack_extend.h \
    $(wildcard include/config/nf/nat.h) \
    $(wildcard include/config/nf/conntrack/zones.h) \
    $(wildcard include/config/nf/conntrack/timestamp.h) \
    $(wildcard include/config/nf/conntrack/timeout.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/net/bridge/br_private.h \
    $(wildcard include/config/bridge/igmp/snooping.h) \
    $(wildcard include/config/rtl/igmp/snooping.h) \
    $(wildcard include/config/atm/lane.h) \
  include/linux/if_bridge.h \
  include/uapi/linux/if_bridge.h \
  include/linux/netpoll.h \
  include/linux/interrupt.h \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/generic/irq/probe.h) \
  include/net/route.h \
  include/net/dst.h \
  include/linux/rtnetlink.h \
  include/uapi/linux/rtnetlink.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/if_addr.h \
  include/net/neighbour.h \
  include/net/rtnetlink.h \
  include/net/inetpeer.h \
  include/net/ipv6.h \
    $(wildcard include/config/have/efficient/unaligned/access.h) \
  include/linux/ipv6.h \
    $(wildcard include/config/ipv6/privacy.h) \
    $(wildcard include/config/ipv6/router/pref.h) \
    $(wildcard include/config/ipv6/route/info.h) \
    $(wildcard include/config/ipv6/optimistic/dad.h) \
    $(wildcard include/config/ipv6/mip6.h) \
    $(wildcard include/config/ipv6/subtrees.h) \
  include/uapi/linux/ipv6.h \
  include/linux/icmpv6.h \
  include/uapi/linux/icmpv6.h \
  include/linux/tcp.h \
    $(wildcard include/config/tcp/md5sig.h) \
  include/net/sock.h \
    $(wildcard include/config/net.h) \
  include/linux/uaccess.h \
  include/linux/memcontrol.h \
    $(wildcard include/config/memcg/swap.h) \
    $(wildcard include/config/inet.h) \
  include/linux/res_counter.h \
  include/linux/aio.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/aio_abi.h \
  include/linux/filter.h \
    $(wildcard include/config/bpf/jit.h) \
  include/uapi/linux/filter.h \
  include/linux/rculist_nulls.h \
  include/linux/poll.h \
  include/uapi/linux/poll.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/uapi/asm/poll.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/asm-generic/poll.h \
  include/net/inet_connection_sock.h \
  include/net/inet_sock.h \
  include/linux/jhash.h \
  include/linux/unaligned/packed_struct.h \
  include/net/request_sock.h \
  include/net/netns/hash.h \
  include/net/inet_timewait_sock.h \
  include/net/tcp_states.h \
  include/net/timewait_sock.h \
  include/uapi/linux/tcp.h \
  include/linux/udp.h \
  include/uapi/linux/udp.h \
  include/net/if_inet6.h \
  include/net/ndisc.h \
  include/linux/if_arp.h \
    $(wildcard include/config/firewire/net.h) \
  include/uapi/linux/if_arp.h \
  include/linux/hash.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/in_route.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/route.h \
  include/linux/ip.h \
  include/uapi/linux/ip.h \
  include/linux/if_vlan.h \
  include/linux/etherdevice.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/arch/rlx/include/asm/unaligned.h \
  include/linux/unaligned/be_struct.h \
  include/linux/unaligned/le_byteshift.h \
  include/linux/unaligned/generic.h \
  include/uapi/linux/if_vlan.h \
  include/linux/netfilter_ipv4/ip_tables.h \
  include/uapi/linux/netfilter_ipv4/ip_tables.h \
  include/net/ip_fib.h \
    $(wildcard include/config/ip/route/multipath.h) \
  include/net/fib_rules.h \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/fib_rules.h \
  include/net/ip_vs.h \
    $(wildcard include/config/ip/vs/ipv6.h) \
    $(wildcard include/config/ip/vs/debug.h) \
    $(wildcard include/config/ip/vs/proto/tcp.h) \
    $(wildcard include/config/ip/vs/proto/udp.h) \
    $(wildcard include/config/ip/vs/proto/sctp.h) \
    $(wildcard include/config/ip/vs/nfct.h) \
  /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/linux-3.10.49/include/uapi/linux/ip_vs.h \
  include/net/rtl/features/rtl_features.h \
    $(wildcard include/config/rtl/iptables/fast/path.h) \
    $(wildcard include/config/rtl/multiple/wan.h) \
    $(wildcard include/config/rtl/redirect/acl/support/for/isp/multi/wan.h) \
    $(wildcard include/config/rtl/ext/port/support.h) \
    $(wildcard include/config/rtl/avoid/adding/wlan/pkt/to/hw/nat.h) \
    $(wildcard include/config/rtl/fast/bridge.h) \
    $(wildcard include/config/hardware/nat/debug.h) \
    $(wildcard include/config/rtl/isp/multi/wan/support.h) \
    $(wildcard include/config/rtl/hardware/ipv6/support.h) \
    $(wildcard include/config/rtl/gc/independence/on/kernel.h) \
    $(wildcard include/config/rtl/url/patch.h) \
    $(wildcard include/config/rtl/hw/napt/refine/kernel/hooks.h) \
  include/linux/inetdevice.h \
  include/net/rtl/fastpath/fastpath_core.h \
    $(wildcard include/config/rtl/fast/pppoe/debug.h) \
    $(wildcard include/config/v6/udp/frag/cache.h) \
    $(wildcard include/config/ppp/mppe/mppc.h) \
    $(wildcard include/config/ppp/multilink.h) \
    $(wildcard include/config/ppp/filter.h) \
    $(wildcard include/config/rtl/pppoe/hwacc.h) \
    $(wildcard include/config/rtl/92d/support.h) \
    $(wildcard include/config/rtl/sdram/ge/32m.h) \
    $(wildcard include/config/rtl/shrink/memory/size.h) \
    $(wildcard include/config/url/filter/user/mode/support.h) \
    $(wildcard include/config/rtl/ip/policy/routing/support.h) \
    $(wildcard include/config/rtl/vlanid/iptable/check.h) \
    $(wildcard include/config/rtl/vlanpri/iptable/check.h) \
  include/net/rtl/rtl_queue.h \
  include/uapi/linux/netfilter/nf_conntrack_tuple_common.h \
  include/net/rtl/rtl865x_nat.h \
    $(wildcard include/config/rtl865x/nat/add/fail/check.h) \
    $(wildcard include/config/rtl/inbound/collision/avoidance.h) \
    $(wildcard include/config/rtl/half/napt/acceleration.h) \
  include/net/rtl/features/rtl_ps_log.h \
  include/net/rtl/rtl_nic.h \
    $(wildcard include/config/rtl/stp.h) \
    $(wildcard include/config/rtl/phy/patch.h) \
    $(wildcard include/config/net/wireless/agn.h) \
    $(wildcard include/config/rtl8192se.h) \
    $(wildcard include/config/rtl8192cd.h) \
    $(wildcard include/config/rtl/no/br/shortcut.h) \
    $(wildcard include/config/rtl/fastbridge.h) \
    $(wildcard include/config/rtl/multi/lan/dev.h) \
    $(wildcard include/config/rtl/eth/napi/support.h) \
    $(wildcard include/config/pocket/ap/support.h) \
    $(wildcard include/config/pocket/router/support.h) \
    $(wildcard include/config/rtl/custom/passthru.h) \
    $(wildcard include/config/rtl8196c/revision/b.h) \
    $(wildcard include/config/rtk/voip/ethernet/dsp/is/host.h) \
    $(wildcard include/config/rtl/unkown/unicast/control.h) \
    $(wildcard include/config/rtl/inband/ctl/api.h) \
    $(wildcard include/config/rtl/layered/driver.h) \
    $(wildcard include/config/end.h) \
    $(wildcard include/config/check.h) \
    $(wildcard include/config/rtl8686/gmac.h) \
    $(wildcard include/config/rtk/vlan/for/cable/modem.h) \
    $(wildcard include/config/rtl/nic/hwstats.h) \
    $(wildcard include/config/rtl/hw/vlan/support.h) \
    $(wildcard include/config/rtl/hw/vlan/support/hw/nat.h) \
    $(wildcard include/config/rtl/custom/passthru/pppoe.h) \
    $(wildcard include/config/rtl/process/pppoe/igmp/for/bridge/forward.h) \
  include/net/rtl/rtl865x_netif.h \
    $(wildcard include/config/rtl/inband/ctl/acl.h) \
    $(wildcard include/config/rtl/dns/trap.h) \
    $(wildcard include/config/rtl/http/redirect/local.h) \
    $(wildcard include/config/rtl/qos/8021p/support.h) \
    $(wildcard include/config/rtk/bridge/vlan/support.h) \
    $(wildcard include/config/rtl/mac/based/netif.h) \
    $(wildcard include/config/rtl/vlan/based/netif.h) \
    $(wildcard include/config/rtl8196/rtl8366.h) \
    $(wildcard include/config/rtl/exchange/portmask.h) \
    $(wildcard include/config/auto/dhcp/check.h) \
    $(wildcard include/config/rtk/wlan/event/indicate.h) \
    $(wildcard include/config/rtl/link/aggregation.h) \
    $(wildcard include/config/8198/port5/rgmii.h) \
    $(wildcard include/config/rtl/8196c/inic.h) \
    $(wildcard include/config/rtk/inband/host/hack.h) \
    $(wildcard include/config/8198/port5/gmii.h) \
    $(wildcard include/config/rtl/89xxd.h) \
    $(wildcard include/config/8198c/8211fs.h) \
    $(wildcard include/config/rtl/8198/nfbi/board.h) \
    $(wildcard include/config/rtl/8198c/8367rb.h) \
    $(wildcard include/config/rtl/8198c/8211f.h) \
    $(wildcard include/config/rtl/8211f/support.h) \
    $(wildcard include/config/rtl/avoid/adding/wan/subnet/pkt/to/hw/nat.h) \
    $(wildcard include/config/rtl/iptables2acl/patch.h) \
    $(wildcard include/config/swconfig.h) \
    $(wildcard include/config/swconfig/support/hw/nat.h) \
    $(wildcard include/config/support/hw/nat.h) \
    $(wildcard include/config/drv/ppp/netif/name.h) \
  include/net/rtl/rtl_types.h \
  include/net/arp.h \
  include/net/rtl/rtl865x_fdb_api.h \
    $(wildcard include/config/rtl/ivl/support.h) \
    $(wildcard include/config/rtl865x/sync/l2.h) \
  include/net/rtl/rtl865x_netif.h \

/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/fastpath-0.1/fastpath_common.o: $(deps_/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/fastpath-0.1/fastpath_common.o)

$(deps_/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/target-mips-rlx5281-linux/linux-realtek_rtl819xd/fastpath-0.1/fastpath_common.o):
