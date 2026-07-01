#!/bin/sh

set -ex

qemu-system-i386 -m 3G -d int,cpu_reset -no-reboot -no-shutdown -cdrom ./isodir/retros.iso
# -D qemu.log
