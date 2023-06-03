#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# create_disk.py
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
import os 
import sys
from pathlib import Path
import json 
import parted 
import humanfriendly

parser = argparse.ArgumentParser(description="Script for empty disk creation")
parser.add_argument("--output", "-o", dest="output", action="store", help="filename of output file", required=True)
parser.add_argument("--layout", "-l", dest="layout", action="store", help="JSON file with disk layout description", required=True)

args, rest = parser.parse_known_args()

def get_int(value):
  try:
    return int(value)
  except ValueError:
    try: 
      return int(value, 16)
    except ValueError:
      return int(humanfriendly.parse_size(value, binary=True))

filepath = Path(args.output)
if os.path.exists(filepath.absolute()):
  print("File already exists:", filepath.absolute())
  os.remove(filepath.absolute())

print("Creating partitions")

with open(args.layout, "r") as file:
  layout = json.load(file)

size = get_int(layout["size"]) 
print("Disk size is: ", hex(size))

with open(filepath, "wb") as file:
  file.write(bytearray(size))

device = parted.getDevice(str(filepath.absolute()))
disk = parted.freshDisk(device, "msdos")

for key, value in sorted(layout["partitions"].items()):
  g = parted.Geometry(
      device=device, 
      start=int(get_int(value["start"]) / 512),
      length=int(get_int(value["length"]) / 512)
      ) 
  p = parted.Partition(
      disk=disk, 
      type=parted.PARTITION_NORMAL,
      geometry=g
      )
  disk.addPartition(partition=p)
  if value["bootable"]:
    p.setFlag(parted.PARTITION_BOOT)

disk.commit()

