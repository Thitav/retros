#!/bin/sh

rm -r ./isodir
mkdir -p ./isodir/boot/grub
cp ./build/sysroot/usr/boot/kernel.bin ./isodir/boot/retros.bin
cp ./grub.cfg ./isodir/boot/grub/grub.cfg

i386-retros-gcc ./user/sysproc.c -o ./user/sysproc -fno-pie -nostdlib -ffreestanding
cp ./user/sysproc ./isodir/boot/

grub-mkrescue -o ./isodir/retros.iso ./isodir
