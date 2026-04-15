#!/bin/sh

rm -r ./isodir
mkdir -p ./isodir/boot/grub
cp ./build/sysroot/usr/boot/kernel.bin ./isodir/boot/retros.bin
cp ./grub.cfg ./isodir/boot/grub/grub.cfg
grub-mkrescue -o ./isodir/retros.iso ./isodir
