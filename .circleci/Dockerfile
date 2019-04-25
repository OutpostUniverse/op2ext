# For building and running unit tests on CircleCI
# See makefile from project root for local usage

FROM ubuntu:18.04

# Install base development tools
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential=12.4ubuntu1 \
    cmake=3.10.2-1ubuntu2 \
  && rm -rf /var/lib/apt/lists/*

# Install mingw compiler
RUN apt-get update && apt-get install -y --no-install-recommends \
    mingw-w64=5.0.3-1 \
  && rm -rf /var/lib/apt/lists/*

# Install wine so resulting unit test binaries can be run
RUN dpkg --add-architecture i386 && \
  apt-get update && apt-get install -y --no-install-recommends \
    wine-stable=3.0-1ubuntu1 \
    wine32=3.0-1ubuntu1 \
  && rm -rf /var/lib/apt/lists/*

# Install Google Test source package
RUN apt-get update && apt-get install -y --no-install-recommends \
    googletest=1.8.0-6 \
  && rm -rf /var/lib/apt/lists/*

# Set mingw as default compiler
ENV CXX=i686-w64-mingw32-g++
ENV  CC=i686-w64-mingw32-gcc

# Compile and install Google Test
WORKDIR /tmp/gtest/
RUN cmake -DCMAKE_CXX_FLAGS="-std=c++17" -DCMAKE_SYSTEM_NAME="Windows" -Dgtest_disable_pthreads=ON /usr/src/googletest/ && \
  make && \
  cp googlemock/gtest/lib*.a /usr/i686-w64-mingw32/lib/ && \
  cp googlemock/lib*.a /usr/i686-w64-mingw32/lib/ && \
  rm -rf /tmp/gtest/
WORKDIR /

# Install CircleCI tools needed for primary containers
RUN apt-get update && apt-get install -y --no-install-recommends \
    git=1:2.17.1-1ubuntu0.4 \
    ssh=1:7.6p1-4ubuntu0.3 \
    tar=1.29b-2ubuntu0.1 \
    gzip=1.6-5ubuntu1 \
    ca-certificates=20180409 \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /code
VOLUME /code

CMD ["make", "--keep-going", "check"]
