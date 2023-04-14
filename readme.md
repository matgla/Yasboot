[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=matgla_Yasboot&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=matgla_Yasboot)
[![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=matgla_Yasboot&metric=ncloc)](https://sonarcloud.io/summary/new_code?id=matgla_Yasboot)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=matgla_Yasboot&metric=coverage)](https://sonarcloud.io/summary/new_code?id=matgla_Yasboot)
[![Vulnerabilities](https://sonarcloud.io/api/project_badges/measure?project=matgla_Yasboot&metric=vulnerabilities)](https://sonarcloud.io/summary/new_code?id=matgla_Yasboot)

[![SystemTests](https://github.com/matgla/Yasboot/actions/workflows/st.yml/badge.svg?branch=main)](https://github.com/matgla/Yasboot/actions/workflows/st.yml)
[![SystemTests](https://github.com/matgla/Yasboot/actions/workflows/clang-tidy.yml/badge.svg?branch=main)](https://github.com/matgla/Yasboot/actions/workflows/clang-tidy.yml)

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

<p align="center"> <img src="docs/logo_yasboot_small.png" width=50%></p>

# Readme

**[WIP]** Project is not implemented yet, work just started.

**Yasboot** is a simple bootloader for microcontrollers. 

# How to build 
## Prerequisites
If you want to build project you need to install: 

```shell
cmake
g++
arm-none-eabi toolchain
python3
python3-virtualenv
make
```

## Project setup 

```shell
git clone https://github.com/matgla/Yasboot.git
cd Yasboot 
mkdir build 
cmake ..
make menuconfig (select configuration of your board)
make 
```

# Supported devices 
## Boards
* MSPC [mspc](https://github.com/matgla/mspc)
* HOST simulated hardware on any device that can be possible to compile and execute

## MCUs
* RP2040 

# Contributions
If you want to contribute, you are always welcome! 

# License 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>

