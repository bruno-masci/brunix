#!/bin/bash

if ! grub-file --is-x86-multiboot $1; then
	echo "$1": Invalid Multiboot image! Aborting...
	exit 1
fi