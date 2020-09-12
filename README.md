# brunix
##### *** Small Unix-like 32-bits x86 OS for fun and learning (in progress) ***


## Overview

This is a very simple operating system I have been working on for years. 
Through all these years I have been researching, learning, testing about different aspects of operating systems and also
trying to understand how the building blocks play together.

My interest on OSs traces back to when I was young. I can say there are tons of information on OSs scattered
all around the Internet. In my humble opinion, the problem with that is we can't trust it or confuses us 
rather than it enlightens: there are countless 
projects/solutions half done, based on misconceptions, there are hidden assumptions, there are erroneous 
usage of data types, incorrect setup of low-level data structures, and so on. 
My hope is that this guide helps others to follow a logical path towards the building of a simple operating system,
learning about the important concepts and details in the process.

Once I finish the development of the OS, this page will contain general information about building and using it.
Apart from that, you can go step by step and grasp some concepts as needed.

## Step by step through the stages

I have split the development in different Git branches, starting from the branch *stage0* to some branch *stageN*, following
a logical path for that organization.
Each branch extends the previous one, adding functionalities and explanations. \
You can compare two adjacent branches to see exactly what changed. The idea here is that you complement both the source code and the README.md file.

* [stage0-multiboot](https://github.com/bruno-masci/brunix/tree/stage0-posta)
* [stage1-emulator_and_vga](https://github.com/bruno-masci/brunix/tree/stage0-multiboot)
* [stage2-kernel_debugging]() (in progress)


## References

* https://wiki.osdev.org
* http://www.cse.iitd.ernet.in/os-lectures
* http://www.jamesmolloy.co.uk/tutorial_html
* http://www.osdever.net/bkerndev/index.php

(a lot more on the different stages)