#!/usr/bin/sh

gcc -g main.c csv_reader.c csv_reader.h -o main.out

gdb main.out
