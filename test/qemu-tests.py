#!/usr/bin/env python

from gradelib import *

r = Runner(save("test.out"))

@test(0, "Running tests...")
def test_jos():
    r.run_qemu()

##
## libkern testing
##


# @test(1, parent=test_jos)
# def test_printf_octal():
#     r.match("6828 decimal is 15254 octal!")
#
#
# @test(1, parent=test_jos)
# def test_printf_binary():
#     r.match("131 decimal is 10000011 binary!")

# 6828 decimal is 15254 octal!
# 131 decimal is 10000011 binary!
# 132 decimal is 132 signed integer!
# -133 decimal is -133 signed integer!
# 134 decimal is 134 unsigned integer!
# Null string is (null)
# String brunix is brunix



@test(1, parent=test_jos)
def test_ram_size():
    r.match("RAM memory: 512 MiB")


# Function defined in gradelib.py
run_tests()
