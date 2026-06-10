#!/bin/sh

qemu-system-i386 -d int,cpu_reset -no-reboot -no-shutdown -cdrom ./isodir/retros.iso
# -D qemu.log
