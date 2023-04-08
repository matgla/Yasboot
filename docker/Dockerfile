FROM ubuntu:latest

ARG CMAKE_VERSION=3.26.1
ARG ARM_NONE_EABI_TOOLCHAIN=12.2.mpacbti-rel1
ARG SONAR_SCANNER_VERSION=4.8.0.2856 


RUN apt-get update && apt-get install -y wget software-properties-common
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test

RUN apt-get install -y gcc-12 g++-12 build-essential
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 120 --slave /usr/bin/g++ g++ /usr/bin/g++-12 --slave /usr/bin/gcov gcov /usr/bin/gcov-12 

RUN apt-get install -y ninja-build

RUN apt-get install -y git python3 python3-pip python3-virtualenv 

RUN wget https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION-linux-x86_64.sh \
  && mkdir -p /opt/cmake \
  && sh ./cmake-$CMAKE_VERSION-linux-x86_64.sh --skip-license --prefix=/opt/cmake \
  && rm cmake-$CMAKE_VERSION-linux-x86_64.sh \
  && update-alternatives --install /usr/bin/cmake cmake /opt/cmake/bin/cmake 60 

RUN apt-get install -y clang-15 clang-tidy-15
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-15 150 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-15 --slave /usr/bin/clang-tidy clang-tidy-15 /usr/bin/clang-tidy-15 --slave /usr/bin/run-clang-tidy run-clang-tidy-15 /usr/bin/run-clang-tidy-15 
RUN cd /opt && wget https://developer.arm.com/-/media/Files/downloads/gnu/$ARM_NONE_EABI_TOOLCHAIN/binrel/arm-gnu-toolchain-$ARM_NONE_EABI_TOOLCHAIN-x86_64-arm-none-eabi.tar.xz \
  && tar -xvf arm-gnu-toolchain-$ARM_NONE_EABI_TOOLCHAIN-x86_64-arm-none-eabi.tar.xz \
  && update-alternatives --install /usr/bin/arm-none-eabi-gcc arm-none-eabi-gcc /opt/arm-gnu-toolchain-$ARM_NONE_EABI_TOOLCHAIN-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc 120 \
  && update-alternatives --install /usr/bin/arm-none-eabi-g++ arm-none-eabi-g++ /opt/arm-gnu-toolchain-$ARM_NONE_EABI_TOOLCHAIN-x86_64-arm-none-eabi/bin/arm-none-eabi-g++ 120 \
  && update-alternatives --install /usr/bin/arm-none-eabi-gdb arm-none-eabi-gdb /opt/arm-gnu-toolchain-$ARM_NONE_EABI_TOOLCHAIN-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb 120 \
  && update-alternatives --install /usr/bin/arm-none-eabi-size arm-none-eabi-size /opt/arm-gnu-toolchain-$ARM_NONE_EABI_TOOLCHAIN-x86_64-arm-none-eabi/bin/arm-none-eabi-size 120 \
  && update-alternatives --install /usr/bin/arm-none-eabi-objcopy arm-none-eabi-objcopy /opt/arm-gnu-toolchain-$ARM_NONE_EABI_TOOLCHAIN-x86_64-arm-none-eabi/bin/arm-none-eabi-objcopy 120 \
  && update-alternatives --install /usr/bin/arm-none-eabi-objdump arm-none-eabi-objdump /opt/arm-gnu-toolchain-$ARM_NONE_EABI_TOOLCHAIN-x86_64-arm-none-eabi/bin/arm-none-eabi-objdump 120 \
  && update-alternatives --install /usr/bin/arm-none-eabi-ranlib arm-none-eabi-ranlib /opt/arm-gnu-toolchain-$ARM_NONE_EABI_TOOLCHAIN-x86_64-arm-none-eabi/bin/arm-none-eabi-ranlib 120

RUN cd /opt rm -rf arm-gnu-toolchain-$ARM_NONE_EABI_TOOLCHAIN-x86_64-arm-none-eabi.tar.xz

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get install -y unzip gcovr openjdk-19-jdk

RUN cd /opt \
  && wget https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-$SONAR_SCANNER_VERSION-linux.zip \
  && unzip sonar-scanner-cli-$SONAR_SCANNER_VERSION-linux.zip -d sonar_scanner \
  && rm -rf sonar-scanner-cli-$SONAR_SCANNER_VERSION-linux.zip 

ENV PATH=$PATH:/opt/sonar_scanner/sonar-scanner-$SONAR_SCANNER_VERSION-linux/bin

RUN apt-get clean

