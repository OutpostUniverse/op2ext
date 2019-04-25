# For building on CircleCI with Google Test

# Build from the root project folder with:
#   docker build .circleci/ --tag outpostuniverse/circleci-gcc-clang-googletest
# Push image to DockerHub with:
#   docker login
#   docker push outpostuniverse/circleci-gcc-clang-googletest

# Run locally using the CircleCI command line interface:
#   circleci build
# Validate .circleci/config.yml file with:
#   circleci config validate

FROM ubuntu:18.04

# Install base development tools
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
  && rm -rf /var/lib/apt/lists/*

# Install mingw compiler
RUN apt-get update && apt-get install -y --no-install-recommends \
    mingw-w64 \
  && rm -rf /var/lib/apt/lists/*

# Install Google Test source package
RUN apt-get update && apt-get install -y --no-install-recommends \
    googletest \
  && rm -rf /var/lib/apt/lists/*

# Compile and install Google Test
WORKDIR /tmp/gtest/
RUN cmake -DCMAKE_CXX_FLAGS="-std=c++14" /usr/src/googletest/ && \
  make && \
  cp googlemock/gtest/lib*.a /usr/lib && \
  cp googlemock/lib*.a /usr/lib && \
  rm -rf /tmp/gtest/
WORKDIR /

# Install CircleCI tools needed for primary containers
RUN apt-get update && apt-get install -y --no-install-recommends \
    git \
    ssh \
    tar \
    gzip \
    ca-certificates \
  && rm -rf /var/lib/apt/lists/*
