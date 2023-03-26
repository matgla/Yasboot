FROM ubuntu:latest

ARG CMAKE_VERSION=3.26.1

RUN apt-get update && apt-get install -y wget software-properties-common
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test
RUN apt-get install -y gcc-12 g++-12 build-essential
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 120 --slave /usr/bin/g++ g++ /usr/bin/g++-12 --slave /usr/bin/gcov gcov /usr/bin/gcov-12

RUN echo gcc --version

RUN apt-get install -y ninja-build
RUN apt-get install -y git python3 python3-pip python3-virtualenv
RUN wget https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION-linux-x86_64.sh \ 
  && mkdir -p /opt/cmake \
  && sh ./cmake-$CMAKE_VERSION-linux-x86_64.sh --skip-license --prefix=/opt/cmake \
  && rm cmake-$CMAKE_VERSION-linux-x86_64.sh \
  && update-alternatives --install /usr/bin/cmake cmake /opt/cmake/bin/cmake 60

RUN apt-get install -y clang-15 clang-tidy-15
RUN update-alternatives --install /usr/bin/clang clang-15 /usr/bin/clang-15 150 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-15 --slave /usr/bin/clang-tidy clang-tidy-15 /usr/bin/clang-tidy-15 --slave /usr/bin/run-clang-tidy run-clang-tidy-15 /usr/bin/run-clang-tidy-15
RUN apt-get clean

