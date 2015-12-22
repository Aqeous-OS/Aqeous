#!/bin/sh

rm -rfv isodir
rm -rfv objs
make all
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp objs/Aqeous isodir/boot/Aqeous
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Aqeous" {
	multiboot /boot/Aqeous
}
EOF
grub-mkrescue -o aqeous.iso isodir
