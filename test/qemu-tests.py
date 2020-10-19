#!/usr/bin/env python

import re
from gradelib import *

r = Runner(save("jos.out"))
# r = Runner(save("jos.out"),
#            stop_breakpoint("101000"))

@test(0, "running JOS")
def test_jos():
    r.run_qemu()

@test(20, parent=test_jos)
def test_printf_octal():
    r.match("6828 decimal is 15254 octal!")

@test(20, parent=test_jos)
def test_ram_size():
    r.match("RAM memory: 512 MiB")


run_tests()
