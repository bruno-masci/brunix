cmake_minimum_required(VERSION 3.7)

project(brunix LANGUAGES ASM C)

#########################################################################
# Compiler & Assembler config
#########################################################################
terset(CROSS_COMPILER_BIN_PATH /home/osdev/opt/cross/bin)
set(CMAKE_C_COMPILER        ${CROSS_COMPILER_BIN_PATH}/i686-elf-gcc)
set(CMAKE_ASM_COMPILER      ${CMAKE_C_COMPILER})
set(CMAKE_LINKER            ${CROSS_COMPILER_BIN_PATH}/i686-elf-ld)

set(CMAKE_C_STANDARD 99)

set(C_PREPROCESSOR_FLAGS -E -x c)

# Do not optimise for now with "-O1", "-O2"... and so on, so we can do some validations.
# Note also the use of "-fno-omit-frame-pointer" option below (for debugging purposes).
set(CMAKE_C_FLAGS "-ffreestanding -c -fno-omit-frame-pointer \
    -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wredundant-decls -Wnested-externs \
    -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wconversion")


#########################################################################
# Source files and other input files
#########################################################################
file(GLOB_RECURSE   ASM_SRCS                    "kernel/*.S")

file(GLOB           UNPROCESSED_LINKER_SCRIPT   "linker.ld.pp")
file(GLOB           KERNEL_INCLUDE_DIR          "include")
file(GLOB           MULTIBOOT_DIR               "multiboot")


#########################################################################
# Intermediate and output files
#########################################################################
set(OUTPUT_NAME brunix)
set(BIN_NAME ${OUTPUT_NAME}.elf)
set(ISO_NAME ${OUTPUT_NAME}.iso)
set(DISASSEMBLED_BIN_NAME ${OUTPUT_NAME}.asm)
set(SYSTEM_MAP_NAME System.map)

set(PREPROCESSED_LINKER_SCRIPT linker.ld)


#########################################################################
# Linking config
#########################################################################
set(LDFLAGS "-ffreestanding -nostdlib -n -T ${PREPROCESSED_LINKER_SCRIPT}")
set(CMAKE_EXE_LINKER_FLAGS "${LDFLAGS} -Wl,-Map,${SYSTEM_MAP_NAME}")
set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> <OBJECTS> -lgcc")


#########################################################################
# Executable creation
#########################################################################
add_executable(${BIN_NAME} ${ASM_SRCS})
target_include_directories(${BIN_NAME} PUBLIC include)

# ------------------------------ PRE BUILD ------------------------------

# prepare linker script file
add_custom_command(TARGET ${BIN_NAME} PRE_BUILD
        COMMENT "Preprocessing linker script..."
        COMMAND ${CMAKE_C_COMPILER} ${C_PREPROCESSOR_FLAGS} -isystem ${KERNEL_INCLUDE_DIR} ${UNPROCESSED_LINKER_SCRIPT} |  grep -v "^#" > ${PREPROCESSED_LINKER_SCRIPT})

# ------------------------------ POST BUILD -----------------------------

# validate whether the generated image is Multiboot-complaint
add_custom_command(TARGET ${BIN_NAME} POST_BUILD
        COMMENT "Validating Multiboot image..."
        COMMAND ${MULTIBOOT_DIR}/multiboot-checker.sh ${BIN_NAME}
        MAIN_DEPENDENCY ${BIN_NAME})

# create kernel's bootable ISO image
add_custom_command(TARGET ${BIN_NAME} POST_BUILD
        COMMENT "Generating ISO image file..."
        COMMAND mkdir -p iso/boot/grub/
        COMMAND cp ${MULTIBOOT_DIR}/grub.cfg iso/boot/grub/
        COMMAND cp ${BIN_NAME} iso/boot/${BIN_NAME}
        COMMAND grub-mkrescue -d ${MULTIBOOT_DIR}/misc/grub/i386-pc -o ${ISO_NAME} iso/ 2>/dev/null
        MAIN_DEPENDENCY ${BIN_NAME})

# disassemble the kernel image
add_custom_command(TARGET ${BIN_NAME} POST_BUILD
        COMMENT "Disassembling kernel image..."
        COMMAND objdump -D -S -l ${BIN_NAME} > ${DISASSEMBLED_BIN_NAME}
        MAIN_DEPENDENCY ${BIN_NAME})


#########################################################################
# CMake's custom targets
#########################################################################
add_custom_target(clean-all
        COMMENT "Cleaning all (but make/Cmake's own files)..."
        COMMAND rm -rf ${BIN_NAME} ${SYSTEM_MAP_NAME} ${PREPROCESSED_LINKER_SCRIPT}
        COMMAND rm -rf ${ISO_NAME} iso ${DISASSEMBLED_BIN_NAME})