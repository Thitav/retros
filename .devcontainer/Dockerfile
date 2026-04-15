# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/master/containers/ubuntu/.devcontainer/base.Dockerfile
# [Choice] Ubuntu version (use jammy or bionic on local arm64/Apple Silicon): jammy, focal, bionic
ARG VARIANT="jammy"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}

ARG PREFIX="/opt/cross"
ARG TARGET="i386-elf"
ENV PATH="$PREFIX/bin:$PATH"

WORKDIR $PREFIX

ARG APT_DEPS="build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev \
  texinfo cmake xorriso grub-common grub-pc-bin qemu-system qemu-system-gui"

RUN set -ex; \
  apt-get update; \
  DEBIAN_FRONTEND=noninteractive \
  apt-get install -y --no-install-recommends $APT_DEPS; \
  apt-get clean;

RUN set -ex; \
  wget https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.xz; \
  tar -xvf binutils-2.42.tar.xz && rm -rf binutils-2.42.tar.xz; \
  mkdir build-binutils && cd build-binutils; \
  ../binutils-2.42/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror; \
  make && make install;

RUN set -ex; \
  wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.xz; \
  tar -xvf gcc-13.2.0.tar.xz && rm -rf gcc-13.2.0.tar.xz; \
  mkdir build-gcc && cd build-gcc; \
  ../gcc-13.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers; \
  make all-gcc && make all-target-libgcc && make install-gcc && make install-target-libgcc;
