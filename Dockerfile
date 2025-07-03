FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    wget \
    unzip \
    curl \
    coreutils \
    git \
    apt-transport-https \
    gnupg \
    lsb-release && \
    mkdir -p /etc/apt/keyrings && \
    curl -sSf https://librealsense.intel.com/Debian/librealsense.pgp | tee /etc/apt/keyrings/librealsense.pgp > /dev/null && \
    echo "deb [signed-by=/etc/apt/keyrings/librealsense.pgp] https://librealsense.intel.com/Debian/apt-repo $(lsb_release -cs) main" \
    | tee /etc/apt/sources.list.d/librealsense.list && \
    curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor >bazel-archive-keyring.gpg && \
    mv bazel-archive-keyring.gpg /usr/share/keyrings && \
    echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list && \
    apt-get update && apt-get install -y \
    librealsense2-utils \
    librealsense2-dev \
    librealsense2-dbg



WORKDIR /opt

RUN git clone https://github.com/opencv/opencv.git &&\
    git clone https://github.com/opencv/opencv_contrib.git &&\
    mkdir opencv/build && cd opencv/build &&\
    cmake -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules .. \
          -DCMAKE_INSTALL_PREFIX=/opencv &&\
    make -j$(nproc) && make DESTDIR=/opt/picoscan_tool/sysroot install &&\
    curl -o /home/bazelisk.deb -L https://github.com/bazelbuild/bazelisk/releases/download/v1.26.0/bazelisk-amd64.deb &&\
    dpkg -i /home/bazelisk.deb && \
    bazel --version


WORKDIR /workspace
COPY . .



#RUN apt-get update

#RUN apt-get install -y \
#    librealsense2-utils \
#    librealsense2-dev \
#    librealsense2-dbg



ENTRYPOINT ["/bin/bash"]
