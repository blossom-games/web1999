#!/usr/bin/env python
#
# Generates a version of the 216-color web-safe palette as a PPM file
# which can be easily converted to other formats and transformed manually.

import itertools

print('P3 216 1')
print('# Web-safe 216-color palette')
print('# This is a PPM image file')
print(216, 1)
print(255)

for (r, g, b) in itertools.product((0x00, 0x33, 0x66, 0x99, 0xcc, 0xff), repeat=3):
    print(r, g, b)
