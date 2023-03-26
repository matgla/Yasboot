#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# append_checksum.py
#
# Copyright (C) 2023 Mateusz Stadnik <matgla@live.com>
#
# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General
# Public License along with this program. If not, see
# <https://www.gnu.org/licenses/>.
#

import argparse 
import sys 

parser = argparse.ArgumentParser(description = "Script to resize image")

parser.add_argument("--input", dest="input", action="store", help="Path to input file", required = True)
parser.add_argument("--output", dest="output", action="store", help="Path to output file", required = True)
parser.add_argument("--size", dest="size", action="store", help="Output binary size", required = True)
args, rest = parser.parse_known_args()

print("Resize file: ", args.input)
with open(args.input, "rb") as file:
    input = file.read() 
    if (len(input) > int(args.size)):
        sys.exit("File is too large")

    size_to_append = int(args.size) - len(input)
    input_padded = input + bytes(size_to_append)

    with open(args.output, "wb") as output:
        output.write(input_padded)

