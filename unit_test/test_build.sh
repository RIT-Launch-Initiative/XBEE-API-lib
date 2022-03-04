#!/bin/bash

gcc ../../net/*.c ../../spinlock/*.c ../*.c *.c -I.. -I../../net -I../../spinlock -ggdb -o unit_test
