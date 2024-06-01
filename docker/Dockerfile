FROM ubuntu:latest

ARG CMAKE_VERSION=3.29.3
ARG ARM_NONE_EABI_TOOLCHAIN=12.2.mpacbti-rel1
ARG SONAR_SCANNER_VERSION=4.8.0.2856 


RUN apt-get update && apt-get install -y wget software-properties-common
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test

RUN apt-get install -y gcc-14 g++-14 build-essential
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 140 --slave /usr/bin/g++ g++ /usr/bin/g++-14 --slave /usr/bin/gcov gcov /usr/bin/gcov-14 

RUN apt-get install -y ninja-build

RUN apt-get install -y git python3 python3-pip python3-virtualenv 

RUN wget https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION-linux-x86_64.sh \
  && mkdir -p /opt/cmake \
  && sh ./cmake-$CMAKE_VERSION-linux-x86_64.sh --skip-license --prefix=/opt/cmake \
  && rm cmake-$CMAKE_VERSION-linux-x86_64.sh \
  && update-alternatives --install /usr/bin/cmake cmake /opt/cmake/bin/cmake 60 

RUN apt-get install -y clang-15 clang-tidy-15
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-15 150 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-15 --slave /usr/bin/clang-tidy clang-tidy-15 /usr/bin/clang-tidy-15 --slave /usr/bin/run-clang-tidy run-clang-tidy-15 /usr/bin/run-clang-tidy-15 

RUN cd /opt &&  wget https://github.com/matgla/YasldToolchain/releases/download/regular_gcc_14.0.dirty/yasld-gcc-14.tar.xz \
  && tar -xvf yasld-gcc-14.tar.xz \
  && update-alternatives --install /usr/bin/arm-none-eabi-gcc arm-none-eabi-gcc /opt/yasld-gcc-14/bin/arm-none-eabi-gcc 140 \
  && update-alternatives --install /usr/bin/arm-none-eabi-g++ arm-none-eabi-g++ /opt/yasld-gcc-14/bin/arm-none-eabi-g++ 140 \
  && update-alternatives --install /usr/bin/arm-none-eabi-size arm-none-eabi-size /opt/yasld-gcc-14/bin/arm-none-eabi-size 140 \
  && update-alternatives --install /usr/bin/arm-none-eabi-objcopy arm-none-eabi-objcopy /opt/yasld-gcc-14/bin/arm-none-eabi-objcopy 140 \
  && update-alternatives --install /usr/bin/arm-none-eabi-objdump arm-none-eabi-objdump /opt/yasld-gcc-14/bin/arm-none-eabi-objdump 140 \
  && update-alternatives --install /usr/bin/arm-none-eabi-ranlib arm-none-eabi-ranlib /opt/yasld-gcc-14/bin/arm-none-eabi-ranlib 140

RUN cd /opt rm -rf yasld-gcc-14.tar.xz

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get install -y unzip openjdk-21-jdk
RUN pip3 install gcovr --break-system-packages

RUN cd /opt \
  && wget https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-$SONAR_SCANNER_VERSION-linux.zip \
  && unzip sonar-scanner-cli-$SONAR_SCANNER_VERSION-linux.zip -d sonar_scanner \
  && rm -rf sonar-scanner-cli-$SONAR_SCANNER_VERSION-linux.zip 

ENV PATH=$PATH:/opt/sonar_scanner/sonar-scanner-$SONAR_SCANNER_VERSION-linux/bin

RUN cd /opt \
  && wget https://github.com/renode/renode/releases/download/v1.15.0/renode-1.15.0.linux-portable.tar.gz \
  && mkdir renode_portable \
  && tar xf renode-*.linux-portable.tar.gz -C renode_portable --strip-components=1 



ENV PATH="${PATH}:/opt/renode_portable"

RUN apt-get install -y mono-complete policykit-1 libgtk2.0-0 screen uml-utilities libc6-dev gcc python3 python3-pip

RUN cd /opt && wget https://github.com/matgla/YasldToolchain/releases/download/release_0.3/yasld-toolchain-0.3.tar.xz  \
  && tar -xvf yasld-toolchain-0.3.tar.xz 

RUN cd /opt rm -rf yasld-toolchain-0.3.tar.xz
RUN cd /opt/renode_portable/tests && sed -i -e 's/pyyaml==6.0/pyyaml==6.*/g' requirements.txt
RUN python3 -m pip install -r /opt/renode_portable/tests/requirements.txt --break-system-packages

RUN apt install -y libfl-dev libdebuginfod-dev

ENV YASLD_TOOLCHAIN_PATH="/opt/yasld-toolchain"

RUN python3 -m pip install --break-system-packages pymbr openocd humanfriendly littlefs-python

RUN apt-get clean

