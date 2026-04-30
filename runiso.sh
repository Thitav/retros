#!/bin/sh

qemu-system-i386 -d int,cpu_reset -no-reboot -no-shutdown -D qemu.log -cdrom ./isodir/retros.iso
