#!/bin/bash
# This is a convinient tool to generate runtime for single asm file
nasm -felf64 $1 -o out.o
ld -o out out.o
chmod u+x out