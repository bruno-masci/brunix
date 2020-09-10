# brunix
##### *** Small Unix-like 32-bits x86 OS for fun and learning (in progress) ***



This is a very simple operating system I have been working on for years. 
Through all these years I have been researching, learning, testing about different aspects of operating systems and also trying to understand how the building blocks play together.

My interest on OSs trace back to when I was young. I can say there are tons of information on OSs scattered
all around the Internet. In my humble opinion, the problem with that is we can't trust it or confuses us 
rather than it enlightens: there are countless 
projects/solutions half done, based on misconceptions, there are hidden assumptions, there is an erroneous 
usage of data types, incorrect setup of low-level structures, and so on. 
My hope is that this guide helps others to follow a logical path towards the building of an operating system,
learning about the important concepts and details in the process.

- -
We'll use [ELF](http://wiki.osdev.org/ELF) as the kernel image format, ELF-formatted kernel image,
and [GRUB](https://wiki.osdev.org/GRUB) [bootloader](https://wiki.osdev.org/Bootloader).
- -


### Goals

In this very first stage we are going to outline and depict the project structure. The idea is to do incremental 
developments (stage0, stage1, ...) to tackle all the complexities in an easier way.


## Project building

Is is highly recommended to build the project on a directory other than the source directory. We assume *build/* as the build directory, but we can use whichever we want.\
__Any command that appears below is assumed to be run from the *build/* directory.__

a) Build the *CMake* project:

	cmake ../

b) Build the *make* project from the generated *Makefile* file:

	make

After that, we should expect to see something like:

_[100%] Built target brunix.elf_


## Build target

### Supported build commands

	make
(this is the default) builds the kernel's image,

	make clean
removes the generated kernel's image,

	make clean-all
removes all generated files but *make*/*CMake*'s own files.


### Behind the scenes




## Image building and validation

From the *build/* directory, we must run:

	make

and expect to see something like:

[100%] Built target brunix.elf


## How do pieces play together?



##References:

* https://wiki.osdev.org
* http://www.cse.iitd.ernet.in/os-lectures
* http://wiki.osdev.org/Why_do_I_need_a_Cross_Compiler%3F
* https://css.csail.mit.edu/6.858/2014/readings/i386.pdf
* http://wiki.osdev.org/LD
* http://wiki.osdev.org/Multiboot
* https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
* http://os.phil-opp.com/multiboot-kernel.html
* http://www.jamesmolloy.co.uk/tutorial_html
* http://www.osdever.net/bkerndev/index.php