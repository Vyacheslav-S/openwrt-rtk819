/*
 *
 * Automatically generated file; DO NOT EDIT.
 * Linux/rlx 3.10.49 Kernel Configuration
 *
 */
/*
 * Helper macros to use CONFIG_ options in C/CPP expressions. Note that
 * these only work with boolean and tristate options.
 */
/*
 * Getting something that works in C and CPP for an arg that may or may
 * not be defined is tricky.  Here, if we have "#define CONFIG_BOOGER 1"
 * we match on the placeholder define, insert the "0," for arg1 and generate
 * the triplet (0, 1, 0).  Then the last step cherry picks the 2nd arg (a one).
 * When CONFIG_BOOGER is not defined, we generate a (... 1, 0) pair, and when
 * the last step cherry picks the 2nd arg, we get a zero.
 */






/*
 * IS_ENABLED(CONFIG_FOO) evaluates to 1 if CONFIG_FOO is set to 'y' or 'm',
 * 0 otherwise.
 *
 */



/*
 * IS_BUILTIN(CONFIG_FOO) evaluates to 1 if CONFIG_FOO is set to 'y', 0
 * otherwise. For boolean options, this is equivalent to
 * IS_ENABLED(CONFIG_FOO).
 */


/*
 * IS_MODULE(CONFIG_FOO) evaluates to 1 if CONFIG_FOO is set to 'm', 0
 * otherwise.
 */
/*
 * DO NOT MODIFY.
 *
 * This file was generated by Kbuild
 *
 */
/* MIPS pt_regs offsets. */
/* MIPS task_struct offsets. */







/* MIPS thread_info offsets. */
/* MIPS specific thread_struct offsets. */
/* Size of struct page */
/* Linux mm_struct offsets. */
/* Linux sigcontext offsets. */
/* Linux signal numbers. */
/* thread_info.h: MIPS low-level thread information
 *
 * Copyright (C) 2002  David Howells (dhowells@redhat.com)
 * - Incorporating suggestions made by Linus Torvalds and Dave Miller
 */
/* thread information allocation */
/*
 * thread information flags
 * - these are process state flags that various assembly files may need to
 *   access
 * - pending work-to-be-done flags are in LSW
 * - other flags in MSW
 */
/* work to do in syscall_trace_leave() */
/* work to do on interrupt/exception return */
/* work to do on any return to u-space */
/*
 * Put .bss..swapper_pg_dir as the first thing in .bss. This will
 * ensure that it has .bss alignment (64K).
 */
/*
 * Helper macros to support writing architecture specific
 * linker scripts.
 *
 * A minimal linker scripts has following content:
 * [This is a sample, architectures may have special requiriements]
 *
 * OUTPUT_FORMAT(...)
 * OUTPUT_ARCH(...)
 * ENTRY(...)
 * SECTIONS
 * {
 *	. = START;
 *	__init_begin = .;
 *	HEAD_TEXT_SECTION
 *	INIT_TEXT_SECTION(PAGE_SIZE)
 *	INIT_DATA_SECTION(...)
 *	PERCPU_SECTION(CACHELINE_SIZE)
 *	__init_end = .;
 *
 *	_stext = .;
 *	TEXT_SECTION = 0
 *	_etext = .;
 *
 *      _sdata = .;
 *	RO_DATA_SECTION(PAGE_SIZE)
 *	RW_DATA_SECTION(...)
 *	_edata = .;
 *
 *	EXCEPTION_TABLE(...)
 *	NOTES
 *
 *	BSS_SECTION(0, 0, 0)
 *	_end = .;
 *
 *	STABS_DEBUG
 *	DWARF_DEBUG
 *
 *	DISCARDS		// must be the last
 * }
 *
 * [__init_begin, __init_end] is the init section that may be freed after init
 * [_stext, _etext] is the text section
 * [_sdata, _edata] is the data section
 *
 * Some of the included output section have their own set of constants.
 * Examples are: [__initramfs_start, __initramfs_end] for initramfs and
 *               [__nosave_begin, __nosave_end] for the nosave data
 */
/*
 * Export symbols from the kernel to modules.  Forked from module.h
 * to reduce the amount of pointless cruft we feed to gcc when only
 * exporting a simple symbol or two.
 *
 * Try not to add #includes here.  It slows compilation and makes kernel
 * hackers place grumpy comments in header files.
 */
/* Some toolchains use a `_' prefix for all user symbols. */
/* Indirect, so macros are expanded before pasting. */
/* Align . to a 8 byte boundary equals to maximum function alignment. */
/*
 * Align to a 32 byte boundary equal to the
 * alignment gcc 4.5 uses for a struct
 */
/* The actual configuration determine if the init/exit sections
 * are handled as text/data or they can be discarded (which
 * often happens at runtime)
 */
/* .data section */
/*
 * Data section helpers
 */
/*
 * Read only Data
 */
/* RODATA & RO_DATA provided for backward compatibility.
 * All archs are supposed to use RO_DATA() */
/* .text section. Map to function alignment to avoid address changes
 * during second ld run in second ld pass when generating System.map */
/* sched.text is aling to function alignment to secure we have same
 * address even at second ld pass when generating System.map */
/* spinlock.text is aling to function alignment to secure we have same
 * address even at second ld pass when generating System.map */
/* Section used for early init (in .S files) */
/*
 * Exception table
 */
/*
 * Init task
 */
/* init and exit section handling */
/*
 * bss (Block Started by Symbol) - uninitialized data
 * zeroed during startup
 */
/*
 * Allow archectures to redefine BSS_FIRST_SECTIONS to add extra
 * sections to the front of bss.
 */
/*
 * DWARF debug sections.
 * Symbols in the DWARF debugging sections are relative to
 * the beginning of the section so we begin them at 0.
 */
  /* Stabs debugging sections.  */
/*
 * Default discarded sections.
 *
 * Some archs want to discard exit text/data at runtime rather than
 * link time due to cross-section references such as alt instructions,
 * bug table, eh_frame, etc.  DISCARDS must be the last of output
 * section definitions so that such archs put those in earlier section
 * definitions.
 */
/**
 * PERCPU_INPUT - the percpu input sections
 * @cacheline: cacheline size
 *
 * The core percpu section names and core symbols which do not rely
 * directly upon load addresses.
 *
 * @cacheline is used to align subsections to avoid false cacheline
 * sharing between subsections for different purposes.
 */
/**
 * PERCPU_VADDR - define output section for percpu area
 * @cacheline: cacheline size
 * @vaddr: explicit base address (optional)
 * @phdr: destination PHDR (optional)
 *
 * Macro which expands to output section for percpu area.
 *
 * @cacheline is used to align subsections to avoid false cacheline
 * sharing between subsections for different purposes.
 *
 * If @vaddr is not blank, it specifies explicit base address and all
 * percpu symbols will be offset from the given address.  If blank,
 * @vaddr always equals @laddr + LOAD_OFFSET.
 *
 * @phdr defines the output PHDR to use if not blank.  Be warned that
 * output PHDR is sticky.  If @phdr is specified, the next output
 * section in the linker script will go there too.  @phdr should have
 * a leading colon.
 *
 * Note that this macros defines __per_cpu_load as an absolute symbol.
 * If there is no need to put the percpu section at a predetermined
 * address, use PERCPU_SECTION.
 */
/**
 * PERCPU_SECTION - define output section for percpu area, simple version
 * @cacheline: cacheline size
 *
 * Align to PAGE_SIZE and outputs output section for percpu area.  This
 * macro doesn't manipulate @vaddr or @phdr and __per_cpu_load and
 * __per_cpu_start will be identical.
 *
 * This macro is equivalent to ALIGN(PAGE_SIZE); PERCPU_VADDR(@cacheline,,)
 * except that __per_cpu_load is defined as a relative symbol against
 * .data..percpu which is required for relocatable x86_32 configuration.
 */
/*
 * Definition of the high level *_SECTION macros
 * They will fit only a subset of the architectures
 */
/*
 * Writeable data.
 * All sections are combined in a single .data section.
 * The sections following CONSTRUCTORS are arranged so their
 * typical alignment matches.
 * A cacheline is typical/always less than a PAGE_SIZE so
 * the sections that has this restriction (or similar)
 * is located before the ones requiring PAGE_SIZE alignment.
 * NOSAVE_DATA starts and ends with a PAGE_SIZE alignment which
 * matches the requirement of PAGE_ALIGNED_DATA.
 *
 * use 0 as page_align if page_aligned data is not used */
OUTPUT_ARCH(mips)
ENTRY(kernel_entry)
PHDRS {
 text PT_LOAD FLAGS(7); /* RWX */
 note PT_NOTE FLAGS(4); /* R__ */
}
  jiffies = jiffies_64 + 4;
SECTIONS
{
 . = 0x80000000;
 /* read-only */
 _text = .; /* Text and read-only data */
 .text : {
  . = ALIGN(8); *(.text.hot) *(.text) *(.ref.text) *(.devinit.text) *(.devexit.text) *(.text.unlikely)
  . = ALIGN(8); __sched_text_start = .; *(.sched.text) __sched_text_end = .;
  . = ALIGN(8); __lock_text_start = .; *(.spinlock.text) __lock_text_end = .;
  . = ALIGN(8); __kprobes_text_start = .; *(.kprobes.text) __kprobes_text_end = .;
 
  *(.text.*)
  *(.fixup)
  *(.gnu.warning)
/*#ifdef CONFIG_RTL8192SE*/
  /* start of wlan f/w definition */
  __fw_start = . ;
  *(.rtl8192sfw.bin)
  __fw_end = . ;
  __AGC_TAB_start = . ;
  *(.AGC_TAB.txt)
  __AGC_TAB_end = . ;
  __phy_reg_start = . ;
  *(.phy_reg.txt)
  __phy_reg_end = . ;
  __phy_reg_MP_start = . ;
  *(.phy_reg_MP.txt)
  __phy_reg_MP_end = . ;
  __MACPHY_REG_start = . ;
  *(.MACPHY_REG.txt)
  __MACPHY_REG_end = . ;
  __radio_a_start = . ;
  *(.radio_a.txt)
  __radio_a_end = . ;
  __radio_b_start = . ;
  *(.radio_b.txt)
  __radio_b_end = . ;
  __PHY_REG_PG_start = .;
  *(.PHY_REG_PG.txt)
  __PHY_REG_PG_end = .;
  __PHY_to1T2R_start = .;
  *(.PHY_to1T2R.txt)
  __PHY_to1T2R_end = .;
  __PHY_to1T2R_b_start = .;
  *(.PHY_to1T2R_b.txt)
  __PHY_to1T2R_b_end = .;
  __PHY_to1T1R_start = .;
  *(.PHY_to1T1R.txt)
  __PHY_to1T1R_end = .;
  __PHY_to1T1R_b_start = .;
  *(.PHY_to1T1R_b.txt)
  __PHY_to1T1R_b_end = .;
  __radio_a_hp_start = . ;
  *(.radio_a_hp.txt)
  __radio_a_hp_end = . ;
  /* end of wlan f/w definition */
 } :text = 0
 _etext = .; /* End of text section */
 . = ALIGN(16); __ex_table : AT(ADDR(__ex_table) - 0) { __start___ex_table = .; KEEP(*(__ex_table)) __stop___ex_table = .; }
 /* Exception table for data bus errors */
 __dbe_table : {
  __start___dbe_table = .;
  KEEP(*(__dbe_table))
  __stop___dbe_table = .;
 }
    . = ALIGN(16384);
    __iram = . ;
    __iram_start = . ;
    .iram :
    {
  *(.iram-gen) /* general usage (essential) */
  *(.iram-fwd) /* general packet forwarding used */
  *(.iram-rtkwlan) /* realtek wlan driver */
  *(.iram-l2-fwd) /* L2 packet forwarding */
  *(.iram-l34-fwd) /* L34 packet forwarding */
  *(.iram-tx) /* protocol stack TX */
  *(.iram-extdev) /* ROMEDRV extension device fwd */
  *(.iram-crypto) /* authetication / crypto-engine */
  *(.iram-rtkusb) /* usb */
  *(.iram-voip) /* voip */
  *(.iram) /* other usage */
  *(.iram.1)
    }
    __iram_tail = . ;
      . = ALIGN(16384);
    __iram_end = . ;
    . = ALIGN(8192);
    __dram = . ;
    __dram_start = . ;
    .dram :
    {
  *(.dram-wapi) /*wapi encryption/decryption used*/
     *(.dram-gen) /* general usage (essential) */
  *(.dram-fwd) /* general packet forwarding used */
     *(.dram-l2-fwd) /* L2 packet forwarding */
  *(.dram-l34-fwd) /* L34 packet forwarding */
     *(.dram-extdev) /* ROMEDRV extension device fwd */
     *(.dram-rtkwlan) /* realtek wlan driver */
  *(.dram-crypto) /* authetication / crypto-engine */
     *(.dram-voip) /* voip */
  *(.dram-tx) /* protocol stack TX */
  *(.dram) /* other usage */
  *(.dram.1)
    }
    __dram_tail = . ;
    . = ALIGN(8192);
    __dram_end = . ;
 .notes : AT(ADDR(.notes) - 0) { __start_notes = .; *(.note.*) __stop_notes = .; } :text :note
 .dummy : { *(.dummy) } :text
 _sdata = .; /* Start of data section */
 . = ALIGN((4096)); .rodata : AT(ADDR(.rodata) - 0) { __start_rodata = .; *(.rodata) *(.rodata.*) *(__vermagic) . = ALIGN(8); __start___tracepoints_ptrs = .; *(__tracepoints_ptrs) __stop___tracepoints_ptrs = .; *(__tracepoints_strings) } .rodata1 : AT(ADDR(.rodata1) - 0) { *(.rodata1) } .pci_fixup : AT(ADDR(.pci_fixup) - 0) { __start_pci_fixups_early = .; KEEP(*(.pci_fixup_early)) __end_pci_fixups_early = .; __start_pci_fixups_header = .; KEEP(*(.pci_fixup_header)) __end_pci_fixups_header = .; __start_pci_fixups_final = .; KEEP(*(.pci_fixup_final)) __end_pci_fixups_final = .; __start_pci_fixups_enable = .; KEEP(*(.pci_fixup_enable)) __end_pci_fixups_enable = .; __start_pci_fixups_resume = .; KEEP(*(.pci_fixup_resume)) __end_pci_fixups_resume = .; __start_pci_fixups_resume_early = .; KEEP(*(.pci_fixup_resume_early)) __end_pci_fixups_resume_early = .; __start_pci_fixups_suspend = .; KEEP(*(.pci_fixup_suspend)) __end_pci_fixups_suspend = .; } .builtin_fw : AT(ADDR(.builtin_fw) - 0) { __start_builtin_fw = .; KEEP(*(.builtin_fw)) __end_builtin_fw = .; } .rio_ops : AT(ADDR(.rio_ops) - 0) { __start_rio_switch_ops = .; KEEP(*(.rio_switch_ops)) __end_rio_switch_ops = .; } __ksymtab : AT(ADDR(__ksymtab) - 0) { __start___ksymtab = .; KEEP(*(SORT(___ksymtab+*))) __stop___ksymtab = .; } __ksymtab_gpl : AT(ADDR(__ksymtab_gpl) - 0) { __start___ksymtab_gpl = .; KEEP(*(SORT(___ksymtab_gpl+*))) __stop___ksymtab_gpl = .; } __ksymtab_unused : AT(ADDR(__ksymtab_unused) - 0) { __start___ksymtab_unused = .; KEEP(*(SORT(___ksymtab_unused+*))) __stop___ksymtab_unused = .; } __ksymtab_unused_gpl : AT(ADDR(__ksymtab_unused_gpl) - 0) { __start___ksymtab_unused_gpl = .; KEEP(*(SORT(___ksymtab_unused_gpl+*))) __stop___ksymtab_unused_gpl = .; } __ksymtab_gpl_future : AT(ADDR(__ksymtab_gpl_future) - 0) { __start___ksymtab_gpl_future = .; KEEP(*(SORT(___ksymtab_gpl_future+*))) __stop___ksymtab_gpl_future = .; } __kcrctab : AT(ADDR(__kcrctab) - 0) { __start___kcrctab = .; KEEP(*(SORT(___kcrctab+*))) __stop___kcrctab = .; } __kcrctab_gpl : AT(ADDR(__kcrctab_gpl) - 0) { __start___kcrctab_gpl = .; KEEP(*(SORT(___kcrctab_gpl+*))) __stop___kcrctab_gpl = .; } __kcrctab_unused : AT(ADDR(__kcrctab_unused) - 0) { __start___kcrctab_unused = .; *(SORT(___kcrctab_unused+*)) __stop___kcrctab_unused = .; } __kcrctab_unused_gpl : AT(ADDR(__kcrctab_unused_gpl) - 0) { __start___kcrctab_unused_gpl = .; KEEP(*(SORT(___kcrctab_unused_gpl+*))) __stop___kcrctab_unused_gpl = .; } __kcrctab_gpl_future : AT(ADDR(__kcrctab_gpl_future) - 0) { __start___kcrctab_gpl_future = .; KEEP(*(SORT(___kcrctab_gpl_future+*))) __stop___kcrctab_gpl_future = .; } __ksymtab_strings : AT(ADDR(__ksymtab_strings) - 0) { *(__ksymtab_strings+*) } __init_rodata : AT(ADDR(__init_rodata) - 0) { *(.ref.rodata) *(.devinit.rodata) *(.devexit.rodata) } __param : AT(ADDR(__param) - 0) { __start___param = .; KEEP(*(__param)) __stop___param = .; } __modver : AT(ADDR(__modver) - 0) { __start___modver = .; KEEP(*(__modver)) __stop___modver = .; . = ALIGN((4096)); __end_rodata = .; } . = ALIGN((4096));
 /* writeable */
 .data : { /* Data */
  . = . + 0; /* for CONFIG_MAPPED_KERNEL */
  . = ALIGN((4096 << (1))); *(.data..init_task)
  . = ALIGN(4096); __nosave_begin = .; *(.data..nosave) . = ALIGN(4096); __nosave_end = .;
  . = ALIGN(1 << 5); *(.data..cacheline_aligned)
  . = ALIGN(1 << 5); *(.data..read_mostly) . = ALIGN(1 << 5);
  *(.data) *(.ref.data) *(.data..shared_aligned) *(.devinit.data) *(.devexit.data) *(.data.unlikely) . = ALIGN(32); *(__tracepoints) . = ALIGN(8); __start___jump_table = .; KEEP(*(__jump_table)) __stop___jump_table = .; . = ALIGN(8); __start___verbose = .; KEEP(*(__verbose)) __stop___verbose = .; *(.data.[a-zA-Z_]*)
  CONSTRUCTORS
 }
 _gp = . + 0x8000;
 .lit8 : {
  *(.lit8)
 }
 .lit4 : {
  *(.lit4)
 }
 /* We want the small data sections together, so single-instruction offsets
	   can access them all, and initialized data all before uninitialized, so
	   we can shorten the on-disk segment size.  */
 .sdata : {
  *(.sdata)
 }
 _edata = .; /* End of data section */
 /* will be freed after init */
 . = ALIGN(4096); /* Init code and data */
 __init_begin = .;
 . = ALIGN(4096); .init.text : AT(ADDR(.init.text) - 0) { _sinittext = .; *(.init.text) *(.cpuinit.text) *(.meminit.text) _einittext = .; }
 .init.data : AT(ADDR(.init.data) - 0) { *(.init.data) *(.cpuinit.data) *(.meminit.data) *(.init.rodata) *(.cpuinit.rodata) *(.meminit.rodata) . = ALIGN(32); __dtb_start = .; KEEP(*(.dtb.init.rodata)) __dtb_end = .; . = ALIGN(16); __setup_start = .; KEEP(*(.init.setup)) __setup_end = .; __initcall_start = .; KEEP(*(.initcallearly.init)) __initcall0_start = .; KEEP(*(.initcall0.init)) KEEP(*(.initcall0s.init)) __initcall1_start = .; KEEP(*(.initcall1.init)) KEEP(*(.initcall1s.init)) __initcall2_start = .; KEEP(*(.initcall2.init)) KEEP(*(.initcall2s.init)) __initcall3_start = .; KEEP(*(.initcall3.init)) KEEP(*(.initcall3s.init)) __initcall4_start = .; KEEP(*(.initcall4.init)) KEEP(*(.initcall4s.init)) __initcall5_start = .; KEEP(*(.initcall5.init)) KEEP(*(.initcall5s.init)) __initcallrootfs_start = .; KEEP(*(.initcallrootfs.init)) KEEP(*(.initcallrootfss.init)) __initcall6_start = .; KEEP(*(.initcall6.init)) KEEP(*(.initcall6s.init)) __initcall7_start = .; KEEP(*(.initcall7.init)) KEEP(*(.initcall7s.init)) __initcall_end = .; __con_initcall_start = .; KEEP(*(.con_initcall.init)) __con_initcall_end = .; __security_initcall_start = .; KEEP(*(.security_initcall.init)) __security_initcall_end = .; . = ALIGN(4); __initramfs_start = .; KEEP(*(.init.ramfs)) . = ALIGN(8); KEEP(*(.init.ramfs.info)) }
 . = ALIGN(4);
 .mips.machines.init : AT(ADDR(.mips.machines.init) - 0) {
  __mips_machines_start = .;
  KEEP(*(.mips.machines.init))
  __mips_machines_end = .;
 }
 /* .exit.text is discarded at runtime, not link time, to deal with
	 * references from .rodata
	 */
 .exit.text : {
  *(.exit.text) *(.cpuexit.text) *(.memexit.text)
 }
 .exit.data : {
  *(.exit.data) *(.cpuexit.data) *(.cpuexit.rodata) *(.memexit.data) *(.memexit.rodata)
 }
 . = ALIGN(4096); .data..percpu : AT(ADDR(.data..percpu) - 0) { __per_cpu_load = .; __per_cpu_start = .; *(.data..percpu..first) . = ALIGN(4096); *(.data..percpu..page_aligned) . = ALIGN(1 << 5); *(.data..percpu..readmostly) . = ALIGN(1 << 5); *(.data..percpu) *(.data..percpu..shared_aligned) __per_cpu_end = .; }
 /*
	 * Align to 64K in attempt to eliminate holes before the
	 * .bss..swapper_pg_dir section at the start of .bss.  This
	 * also satisfies PAGE_SIZE alignment as the largest page size
	 * allowed is 64K.
	 */
 . = ALIGN(0x10000);
 __init_end = .;
 /* freed after init ends here */
 /*
	 * Force .bss to 64K alignment so that .bss..swapper_pg_dir
	 * gets that alignment.	 .sbss should be empty, so there will be
	 * no holes after __init_end. */
 . = ALIGN(0); __bss_start = .; . = ALIGN(0); .sbss : AT(ADDR(.sbss) - 0) { *(.sbss .sbss.*) *(.scommon) } . = ALIGN(0x10000); .bss : AT(ADDR(.bss) - 0) { *(.bss..swapper_pg_dir) *(.bss..page_aligned) *(.dynbss) *(.bss .bss.*) *(COMMON) } . = ALIGN(0); __bss_stop = .;
 _end = . ;
 /* These mark the ABI of the kernel for debuggers.  */
 .mdebug.abi32 : {
  KEEP(*(.mdebug.abi32))
 }
 .mdebug.abi64 : {
  KEEP(*(.mdebug.abi64))
 }
 /* This is the MIPS specific mdebug section.  */
 .mdebug : {
  *(.mdebug)
 }
 .stab 0 : { *(.stab) } .stabstr 0 : { *(.stabstr) } .stab.excl 0 : { *(.stab.excl) } .stab.exclstr 0 : { *(.stab.exclstr) } .stab.index 0 : { *(.stab.index) } .stab.indexstr 0 : { *(.stab.indexstr) } .comment 0 : { *(.comment) }
 .debug 0 : { *(.debug) } .line 0 : { *(.line) } .debug_srcinfo 0 : { *(.debug_srcinfo) } .debug_sfnames 0 : { *(.debug_sfnames) } .debug_aranges 0 : { *(.debug_aranges) } .debug_pubnames 0 : { *(.debug_pubnames) } .debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) } .debug_abbrev 0 : { *(.debug_abbrev) } .debug_line 0 : { *(.debug_line) } .debug_frame 0 : { *(.debug_frame) } .debug_str 0 : { *(.debug_str) } .debug_loc 0 : { *(.debug_loc) } .debug_macinfo 0 : { *(.debug_macinfo) } .debug_weaknames 0 : { *(.debug_weaknames) } .debug_funcnames 0 : { *(.debug_funcnames) } .debug_typenames 0 : { *(.debug_typenames) } .debug_varnames 0 : { *(.debug_varnames) }
 /* These must appear regardless of  .  */
 .gptab.sdata : {
  *(.gptab.data)
  *(.gptab.sdata)
 }
 .gptab.sbss : {
  *(.gptab.bss)
  *(.gptab.sbss)
 }
 /* Sections to be discarded */
 /DISCARD/ : { *(.exit.text) *(.cpuexit.text) *(.memexit.text) *(.exit.data) *(.cpuexit.data) *(.cpuexit.rodata) *(.memexit.data) *(.memexit.rodata) *(.exitcall.exit) *(.discard) *(.discard.*) }
 /DISCARD/ : {
  /* ABI crap starts here */
  *(.MIPS.options)
  *(.options)
  *(.pdr)
  *(.reginfo)
 }
}
