## === === === === ===  Base image with lined deps === === === === ===
ARG BASE_IMAGE=ubuntu:18.04
FROM ${BASE_IMAGE} as base

ARG DEBIAN_FRONTEND=noninteractive

# System Deps for VIAME
# Fletch system dependencies
RUN apt-get update -qq &&\
    apt-get install -qq \
        curl \
    &&\
    apt-get install -qq --no-install-recommends \
        apt-transport-https \
        ca-certificates \
        git \
        openssl \
        software-properties-common \
        gpg-agent \
    && rm -rf /var/lib/apt/lists/*

## === === === === ===  Compile-time dependencies === === === === ===
FROM base as compiler

# Install latest CMAKE
RUN curl -LsS https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - &&\
    apt-add-repository 'deb https://apt.kitware.com/ubuntu/ xenial main' &&\
    apt-get update -qq &&\
    apt-get install -qq \
        cmake \
    &&\
    apt-get install -qq --no-install-recommends \
        build-essential\
        ninja-build \
        pkg-config \
        ccache \
    && rm -rf /var/lib/apt/lists/*

## === === === === ===  Building ITK  === === === === ===
FROM compiler as itk

# Get VIAME
WORKDIR /root/itk

COPY . .

RUN mkdir -p build && \
    cd build &&\
    cmake "${FLAGS}" \
        -G Ninja \
        .. &&\
    ninja &&\
    DESTDIR=/root/diff ninja install

## === === === === ===  Copy in build artifacts === === === === ===

FROM base

COPY --from=itk /root/diff /
WORKDIR /root/
