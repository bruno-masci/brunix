# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/phrack/osdev/workspace/brunix

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/phrack/osdev/workspace/brunix/build

# Include any dependencies generated for this target.
include CMakeFiles/brunix.elf.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/brunix.elf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/brunix.elf.dir/flags.make

CMakeFiles/brunix.elf.dir/kernel/multiboot_entry_point.S.o: CMakeFiles/brunix.elf.dir/flags.make
CMakeFiles/brunix.elf.dir/kernel/multiboot_entry_point.S.o: ../kernel/multiboot_entry_point.S
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/phrack/osdev/workspace/brunix/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building ASM object CMakeFiles/brunix.elf.dir/kernel/multiboot_entry_point.S.o"
	/home/phrack/osdev/opt/cross/bin/i686-elf-gcc $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -o CMakeFiles/brunix.elf.dir/kernel/multiboot_entry_point.S.o -c /home/phrack/osdev/workspace/brunix/kernel/multiboot_entry_point.S

# Object files for target brunix.elf
brunix_elf_OBJECTS = \
"CMakeFiles/brunix.elf.dir/kernel/multiboot_entry_point.S.o"

# External object files for target brunix.elf
brunix_elf_EXTERNAL_OBJECTS =

brunix.elf: CMakeFiles/brunix.elf.dir/kernel/multiboot_entry_point.S.o
brunix.elf: CMakeFiles/brunix.elf.dir/build.make
brunix.elf: CMakeFiles/brunix.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/phrack/osdev/workspace/brunix/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking ASM executable brunix.elf"
	@echo Preprocessing linker script
	/home/phrack/osdev/opt/cross/bin/i686-elf-gcc -ffreestanding -E -x c -isystem /home/phrack/osdev/workspace/brunix/include/ /home/phrack/osdev/workspace/brunix/linker.ld.pp | grep -v ^# > linker.ld
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/brunix.elf.dir/link.txt --verbose=$(VERBOSE)
	@echo Validating multiboot image...
	../multiboot/multiboot-checker.sh brunix.elf
	mkdir -p iso/boot/grub/
	cp /home/phrack/osdev/workspace/brunix/multiboot/grub.cfg iso/boot/grub/
	cp brunix.elf iso/boot/brunix.elf
	@echo Generating ISO image file...
	grub-mkrescue -d /home/phrack/osdev/workspace/brunix/multiboot/misc/grub/i386-pc -o brunix.iso iso/ 2>/dev/null
	objdump -D -S -l brunix.elf > brunix.asm
	/home/phrack/osdev/opt/cross/bin/i686-elf-objcopy --strip-debug brunix.elf brunix-nosym.elf

# Rule to build all files generated by this target.
CMakeFiles/brunix.elf.dir/build: brunix.elf

.PHONY : CMakeFiles/brunix.elf.dir/build

CMakeFiles/brunix.elf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/brunix.elf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/brunix.elf.dir/clean

CMakeFiles/brunix.elf.dir/depend:
	cd /home/phrack/osdev/workspace/brunix/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/phrack/osdev/workspace/brunix /home/phrack/osdev/workspace/brunix /home/phrack/osdev/workspace/brunix/build /home/phrack/osdev/workspace/brunix/build /home/phrack/osdev/workspace/brunix/build/CMakeFiles/brunix.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/brunix.elf.dir/depend

