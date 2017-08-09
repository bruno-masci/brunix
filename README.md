# brunix
Small Unix-like x86 OS for fun and learning (stage 0)

Note: We assume 386+ when we refer to x86 architecture.

Pre-requisites:
mtools
xorriso
grub
nasm
A proper cross compiler

here [see http://wiki.osdev.org/GCC_Cross-Compiler]

In this very first stage we are going to outline and depict the project structure. The idea is to do incremental developments (stage0, stage1, ...) to tackle all the complexities in an easier way.

For now, we'll just create a bare OS (if we even can call it that way) that includes:
* kernel loading (boot)
* basic console management

We'll use [ELF](http://wiki.osdev.org/ELF) as the kernel format, because it's a very well supported, portable and flexible format.

When the computer [boots](http://wiki.osdev.org/System_Initialization_(x86)),
the CPU starts in the so called [Real Mode](http://wiki.osdev.org/Real_Mode) for compatibility reasons. In order
to get all the power from an x86 CPU, we need to enable the so called [Protected Mode](http://wiki.osdev.org/Protected_Mode).

As the bootloader we will use GRUB (GRUB 2, in particular). GRUB save us all the pain of switching from Real Mode to Protected Mode, as it handles
all the unpleasant details and leaves the CPU in Protected Mode with a full 4 GiB addressing space (32 bits),
and paging and interrupts disabled.
We will take advantage of GRUB's native support for loading ELF files, and so we are going to use the flexible ELF format for our kernel.
For creating the kernel we are going to use the linker (ld) from the cross-compiler just built as the first step
to produce an ELF formatted kernel image.



Aside,
EBX contains a pointer to the Multiboot information structure
EAX contains the value 0x2BADB002
Note that GRUB configures a stack but we can't trust its location, so we need to define ours.



* kernel
** main.c: The C kernel's entry point.
** vga.c: Basic video routines.


.
├── bochsout.log
├── bochsrc.txt
├── brunix.elf
├── brunix-nosym.elf
├── brunix.sym
├── cmake-build-debug
│   └── CMakeFiles
│       └── clion-log.txt
├── include
│   ├── arch
│   │   └── x86
│   │       ├── io.h
│   │       ├── multiboot.h
│   │       ├── processor.h
│   │       └── vga.h
│   └── brunix
│       ├── screen.h
│       ├── stdio.h
│       └── stdlib.h
├── iso
│   └── boot
│       ├── brunix.elf
│       └── grub
│           └── grub.cfg
├── kernel
│   ├── main.c
│   ├── main.c.dep
│   ├── main.o
│   ├── Makefile
│   ├── multiboot_entry_point.asm
│   ├── multiboot_entry_point.o
│   ├── vga.c
│   ├── vga.c.dep
│   └── vga.o
├── libkern
│   ├── Makefile
│   ├── stdio.c
│   ├── stdio.c.dep
│   ├── stdio.o
│   ├── stdlib.c
│   ├── stdlib.c.dep
│   └── stdlib.o
├── linker.ld
├── Makefile
├── Makefile.inc
├── misc
│   └── grub
│       └── i386-pc
│           ├── 915resolution.mod
│           ├── acpi.mod
│           ├── adler32.mod
│           ├── affs.mod
│           ├── afs.mod
│           ├── ahci.mod
│           ├── all_video.mod
│           ├── aout.mod
│           ├── archelp.mod
│           ├── ata.mod
│           ├── at_keyboard.mod
│           ├── backtrace.mod
│           ├── bfs.mod
│           ├── biosdisk.mod
│           ├── bitmap.mod
│           ├── bitmap_scale.mod
│           ├── blocklist.mod
│           ├── boot_hybrid.img
│           ├── boot.img
│           ├── boot.mod
│           ├── bsd.mod
│           ├── btrfs.mod
│           ├── bufio.mod
│           ├── cat.mod
│           ├── cbfs.mod
│           ├── cbls.mod
│           ├── cbmemc.mod
│           ├── cbtable.mod
│           ├── cbtime.mod
│           ├── cdboot.img
│           ├── chain.mod
│           ├── cmdline_cat_test.mod
│           ├── cmosdump.mod
│           ├── cmostest.mod
│           ├── cmp.mod
│           ├── command.lst
│           ├── configfile.mod
│           ├── config.h
│           ├── cpio_be.mod
│           ├── cpio.mod
│           ├── cpuid.mod
│           ├── crc64.mod
│           ├── cryptodisk.mod
│           ├── crypto.lst
│           ├── crypto.mod
│           ├── cs5536.mod
│           ├── datehook.mod
│           ├── date.mod
│           ├── datetime.mod
│           ├── diskboot.img
│           ├── diskfilter.mod
│           ├── disk.mod
│           ├── div_test.mod
│           ├── dm_nv.mod
│           ├── drivemap.mod
│           ├── echo.mod
│           ├── efiemu32.o
│           ├── efiemu64.o
│           ├── efiemu.mod
│           ├── ehci.mod
│           ├── elf.mod
│           ├── eval.mod
│           ├── exfat.mod
│           ├── exfctest.mod
│           ├── ext2.mod
│           ├── extcmd.mod
│           ├── fat.mod
│           ├── file.mod
│           ├── font.mod
│           ├── freedos.mod
│           ├── fshelp.mod
│           ├── fs.lst
│           ├── functional_test.mod
│           ├── g2hdr.bin
│           ├── g2hdr.img
│           ├── g2ldr.img
│           ├── g2ldr.mbr
│           ├── gcry_arcfour.mod
│           ├── gcry_blowfish.mod
│           ├── gcry_camellia.mod
│           ├── gcry_cast5.mod
│           ├── gcry_crc.mod
│           ├── gcry_des.mod
│           ├── gcry_dsa.mod
│           ├── gcry_idea.mod
│           ├── gcry_md4.mod
│           ├── gcry_md5.mod
│           ├── gcry_rfc2268.mod
│           ├── gcry_rijndael.mod
│           ├── gcry_rmd160.mod
│           ├── gcry_rsa.mod
│           ├── gcry_seed.mod
│           ├── gcry_serpent.mod
│           ├── gcry_sha1.mod
│           ├── gcry_sha256.mod
│           ├── gcry_sha512.mod
│           ├── gcry_tiger.mod
│           ├── gcry_twofish.mod
│           ├── gcry_whirlpool.mod
│           ├── gdb.mod
│           ├── geli.mod
│           ├── gettext.mod
│           ├── gfxmenu.mod
│           ├── gfxterm_background.mod
│           ├── gfxterm_menu.mod
│           ├── gfxterm.mod
│           ├── gptsync.mod
│           ├── grldr.img
│           ├── grub-bios-setup
│           ├── grub-ntldr-img
│           ├── gzio.mod
│           ├── halt.mod
│           ├── hashsum.mod
│           ├── hdparm.mod
│           ├── hello.mod
│           ├── help.mod
│           ├── hexdump.mod
│           ├── hfs.mod
│           ├── hfspluscomp.mod
│           ├── hfsplus.mod
│           ├── http.mod
│           ├── hwmatch.mod
│           ├── iorw.mod
│           ├── iso9660.mod
│           ├── jfs.mod
│           ├── jpeg.mod
│           ├── kernel.img
│           ├── keylayouts.mod
│           ├── keystatus.mod
│           ├── ldm.mod
│           ├── legacycfg.mod
│           ├── legacy_password_test.mod
│           ├── linux16.mod
│           ├── linux.mod
│           ├── lnxboot.img
│           ├── loadenv.mod
│           ├── loopback.mod
│           ├── lsacpi.mod
│           ├── lsapm.mod
│           ├── lsmmap.mod
│           ├── ls.mod
│           ├── lspci.mod
│           ├── luks.mod
│           ├── lvm.mod
│           ├── lzma_decompress.img
│           ├── lzopio.mod
│           ├── macbless.mod
│           ├── macho.mod
│           ├── mda_text.mod
│           ├── mdraid09_be.mod
│           ├── mdraid09.mod
│           ├── mdraid1x.mod
│           ├── memdisk.mod
│           ├── memrw.mod
│           ├── minicmd.mod
│           ├── minix2_be.mod
│           ├── minix2.mod
│           ├── minix3_be.mod
│           ├── minix3.mod
│           ├── minix_be.mod
│           ├── minix.mod
│           ├── mmap.mod
│           ├── moddep.lst
│           ├── modinfo.sh
│           ├── morse.mod
│           ├── mpi.mod
│           ├── msdospart.mod
│           ├── multiboot2.mod
│           ├── multiboot.mod
│           ├── nativedisk.mod
│           ├── net.mod
│           ├── newc.mod
│           ├── nilfs2.mod
│           ├── normal.mod
│           ├── ntfscomp.mod
│           ├── ntfs.mod
│           ├── ntldr.mod
│           ├── odc.mod
│           ├── offsetio.mod
│           ├── ohci.mod
│           ├── part_acorn.mod
│           ├── part_amiga.mod
│           ├── part_apple.mod
│           ├── part_bsd.mod
│           ├── part_dfly.mod
│           ├── part_dvh.mod
│           ├── part_gpt.mod
│           ├── partmap.lst
│           ├── part_msdos.mod
│           ├── part_plan.mod
│           ├── part_sun.mod
│           ├── part_sunpc.mod
│           ├── parttool.lst
│           ├── parttool.mod
│           ├── password.mod
│           ├── password_pbkdf2.mod
│           ├── pata.mod
│           ├── pbkdf2.mod
│           ├── pbkdf2_test.mod
│           ├── pcidump.mod
│           ├── pci.mod
│           ├── plan9.mod
│           ├── play.mod
│           ├── png.mod
│           ├── priority_queue.mod
│           ├── probe.mod
│           ├── procfs.mod
│           ├── progress.mod
│           ├── pxeboot.img
│           ├── pxechain.mod
│           ├── pxe.mod
│           ├── raid5rec.mod
│           ├── raid6rec.mod
│           ├── read.mod
│           ├── reboot.mod
│           ├── regexp.mod
│           ├── reiserfs.mod
│           ├── relocator.mod
│           ├── romfs.mod
│           ├── scsi.mod
│           ├── search_fs_file.mod
│           ├── search_fs_uuid.mod
│           ├── search_label.mod
│           ├── search.mod
│           ├── sendkey.mod
│           ├── serial.mod
│           ├── setjmp.mod
│           ├── setjmp_test.mod
│           ├── setpci.mod
│           ├── sfs.mod
│           ├── signature_test.mod
│           ├── sleep.mod
│           ├── sleep_test.mod
│           ├── spkmodem.mod
│           ├── squash4.mod
│           ├── syslinuxcfg.mod
│           ├── tar.mod
│           ├── terminal.lst
│           ├── terminal.mod
│           ├── terminfo.mod
│           ├── test_blockarg.mod
│           ├── testload.mod
│           ├── test.mod
│           ├── testspeed.mod
│           ├── tftp.mod
│           ├── tga.mod
│           ├── time.mod
│           ├── trig.mod
│           ├── tr.mod
│           ├── truecrypt.mod
│           ├── true.mod
│           ├── udf.mod
│           ├── ufs1_be.mod
│           ├── ufs1.mod
│           ├── ufs2.mod
│           ├── uhci.mod
│           ├── usb_keyboard.mod
│           ├── usb.mod
│           ├── usbms.mod
│           ├── usbserial_common.mod
│           ├── usbserial_ftdi.mod
│           ├── usbserial_pl2303.mod
│           ├── usbserial_usbdebug.mod
│           ├── usbtest.mod
│           ├── vbe.mod
│           ├── verify.mod
│           ├── vga.mod
│           ├── vga_text.mod
│           ├── video_bochs.mod
│           ├── video_cirrus.mod
│           ├── video_colors.mod
│           ├── video_fb.mod
│           ├── videoinfo.mod
│           ├── video.lst
│           ├── video.mod
│           ├── videotest_checksum.mod
│           ├── videotest.mod
│           ├── xfs.mod
│           ├── xnu.mod
│           ├── xnu_uuid.mod
│           ├── xnu_uuid_test.mod
│           ├── xzio.mod
│           ├── zfscrypt.mod
│           ├── zfsinfo.mod
│           └── zfs.mod
├── os.iso
├── README.md
├── System.map
└── test
    ├── atoi.c
    └── atoi-test


References:

http://www.jamesmolloy.co.uk/tutorial_html
http://wiki.osdev.org
http://os.phil-opp.com/multiboot-kernel.html
http://www.osdever.net/bkerndev/index.php
http://www.jofre.de/?download=WritingAnOs.pdf

