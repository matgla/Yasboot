#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# flash.py
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

from openocd import OpenOcd 
from argparse import ArgumentParser

import os 
from pathlib import Path

parser = ArgumentParser(description="Script to flash via openOCD")

parser.add_argument("--file", "-f", dest="filename", action="store", 
                    help="Path to file to be flashed", required=True)

args, rest = parser.parse_known_args()

args.filename = Path(args.filename).absolute()

if not os.path.exists(args.filename):
  raise RuntimeError("File not exists: " + str(args.filename))

with OpenOcd() as oocd:
  oocd.halt()
  print("Writing", args.filename, ", to: 0x10000000")
  oocd.execute("flash write_image erase " + str(args.filename) + " 0x10000000")
  oocd.execute("flash verify_image " + str(args.filename) + " 0x10000000")
  oocd.execute("reset init")
  oocd.resume()

