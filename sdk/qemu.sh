#!/bin/bash
qemu-system-x86_64 -m 1024 -s -S -drive file=./disk.img,index=0,media=disk,format=raw
