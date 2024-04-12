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
import pymbr
# import parted 
import humanfriendly
from littlefs import LittleFS 

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

def write_mbr(filepath):
    mbr = pymbr.MBR()
    mbr.bootcode = pymbr.Bootcode.ZERO
    partition_table = pymbr.PartitionTable()
    mbr.signature = 0xaa55
    # disk = parted.freshDisk(device, "msdos")

    for key, value in sorted(layout["partitions"].items()):
        p = pymbr.PartitionEntry(lba = int(get_int(value["start"]) / 512), 
                                 size = int(get_int(value["length"]) / 512))
        p.bootflag = value["bootable"]
        p.startCHS = pymbr.CHSTuple(0, 0, 0)
        p.endCHS = pymbr.CHSTuple(0, 0, 0)
        p.type = 0

        partition_table.partitions[key] = p

    mbr.partitionTable = partition_table
    bin = mbr.compose()
    with open(filepath, "wb") as file: 
        file.write(bin)
    

def create_littlefs_partition(block_size, block_count, config, working_directory):
    print("Creating littleFS partition with", block_count, "-", block_size, "bytes blocks") 
    fs = LittleFS(block_size=block_size, block_count=block_count)
    print("Working directory:", working_directory)
    if "content" in config:
        root_dir = working_directory / config["content"]
        for root, dirs, files in os.walk(root_dir):
            for dir in dirs: 
                target_path = Path("/") / Path(root).relative_to(root_dir) / dir
                host_path = Path(root) / dir
                print(host_path, "->", target_path) 
                fs.mkdir(str(target_path))
            for file in files: 
                target_path = Path("/") / Path(root).relative_to(root_dir) / file 
                host_path = Path(root) / file 
                print(host_path, "->", target_path) 
                with open(host_path, "rb") as source:
                    with fs.open(str(target_path), "wb") as out:
                        out.write(source.read())
    return fs 

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

write_mbr(filepath)

print("Creating filesystems")
for key, value in sorted(layout["partitions"].items()):
    if value["format"] == "littlefs":
        length = get_int(value["length"])
        block_size = get_int(layout["eraseSize"])
        if (length % block_size != 0):
            raise RuntimeError("LittleFS partition size must be multiple of erase size")
        working_directory = Path(args.layout).parent.absolute()
        fs = create_littlefs_partition(block_size, int(length / block_size), value, working_directory)
        with open(filepath, "r+b") as img:
            print("Writing partition to", hex(get_int(value["start"])))
            img.seek(get_int(value["start"]))
            img.write(fs.context.buffer)
