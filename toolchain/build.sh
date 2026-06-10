#!/bin/sh
set -ex

# [TODO] turn this in a makefile

# [TODO] change this
alias meson="python3 /workspaces/retros/meson-1.11.1/meson.py"

BINUTILS_VERSION=2.46.0
GCC_VERSION=16.1.0

BASE_PATH="$(cd "$(dirname "$0")" && pwd)"
PATCHES_PATH="${BASE_PATH}/patches"
SRC_PATH="${BASE_PATH}/src"
BUILD_PATH="${BASE_PATH}/build"
SYSROOT_PATH="${BASE_PATH}/sysroot"
TOOLCHAIN_PATH="${BASE_PATH}/toolchain"
MLIBC_PATH="${BASE_PATH}/mlibc"
MESON_CROSS_FILE_PATH="${BASE_PATH}/cross_file"

export PATH="${TOOLCHAIN_PATH}/usr/bin:${PATH}"

mkdir -p ${SRC_PATH} ${BUILD_PATH} ${SYSROOT_PATH} ${TOOLCHAIN_PATH}

# get and patch binutils if needed
if [ ! -f "${SRC_PATH}/.binutils_patched" ]; then
    wget -O "${SRC_PATH}/binutils-${BINUTILS_VERSION}.tar.xz" https://ftpmirror.gnu.org/binutils/binutils-${BINUTILS_VERSION}.tar.xz
    tar -C "${SRC_PATH}" -xf "${SRC_PATH}/binutils-${BINUTILS_VERSION}.tar.xz"
    rm "${SRC_PATH}/binutils-${BINUTILS_VERSION}.tar.xz"

    cd "${SRC_PATH}/binutils-${BINUTILS_VERSION}/ld"
    automake
    cd ${BASE_PATH}
    
    cat "$PATCHES_PATH/binutils.patch" | patch -p1 -d "${SRC_PATH}/binutils-${BINUTILS_VERSION}"
    touch "${SRC_PATH}/.binutils_patched"
fi

# get and patch gcc if needed
if [ ! -f "${SRC_PATH}/.gcc_patched" ]; then
	wget -O "${SRC_PATH}/gcc-${GCC_VERSION}.tar.xz" https://ftpmirror.gnu.org/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz
	tar  -C "${SRC_PATH}" -xf "${SRC_PATH}/gcc-${GCC_VERSION}.tar.xz"
	rm "${SRC_PATH}/gcc-${GCC_VERSION}.tar.xz"

    cd "${SRC_PATH}/gcc-${GCC_VERSION}/libstdc++-v3"
    autoconf
    cd ${BASE_PATH}

	cat "$PATCHES_PATH/gcc.patch" | patch -p1 -d "${SRC_PATH}/gcc-${GCC_VERSION}"
	touch "${SRC_PATH}/.gcc_patched"
fi

# prepare mlibc in the sysroot if needed
if [ ! -f "${BUILD_PATH}/.mlibc_headers_installed" ]; then
    mkdir -p "${BUILD_PATH}/mlibc-headers"
	cd "${BUILD_PATH}/mlibc-headers"
	meson setup --cross-file ${MESON_CROSS_FILE_PATH} --prefix=/usr -Dheaders_only=true "${MLIBC_PATH}"
	DESTDIR=${SYSROOT_PATH} ninja install
	cd "${BASE_PATH}"
	touch "${BUILD_PATH}/.mlibc_headers_installed"
fi

# build binutils if needed
if [ ! -f "${BUILD_PATH}/.binutils_built" ]; then
    mkdir -p "${BUILD_PATH}/binutils"
	cd "${BUILD_PATH}/binutils"

	"${SRC_PATH}/binutils-${BINUTILS_VERSION}/configure" --target=i386-retros --prefix=/usr --with-sysroot=${SYSROOT_PATH} --disable-werror --enable-default-execstack=no
	make -j $(nproc)
	make install DESTDIR="${TOOLCHAIN_PATH}"
	cd "${BASE_PATH}"
	touch "${BUILD_PATH}/.binutils_built"
fi

# build gcc if needed
if [ ! -f "${BUILD_PATH}/.gcc_built" ]; then
	mkdir -p "${BUILD_PATH}/gcc"
	cd "${BUILD_PATH}/gcc"

	CFLAGS_FOR_TARGET="-march=i386 -m32" CXXFLAGS_FOR_TARGET="-march=i386 -m32" "${SRC_PATH}/gcc-${GCC_VERSION}/configure" --target=i386-retros --prefix=/usr --with-sysroot=${SYSROOT_PATH} --enable-languages=c,c++ -enable-threads=posix --disable-multilib --enable-shared --enable-host-shared --with-pic
	make all-gcc all-target-libgcc -j $(nproc)
	make install-gcc install-target-libgcc DESTDIR="${TOOLCHAIN_PATH}"
	cd "${BASE_PATH}"
	touch "${BUILD_PATH}/.gcc_built"
fi

# build mlibc if needed
if [ ! -f "${BUILD_PATH}/.mlibc_built" ]; then
	mkdir -p "${BUILD_PATH}/mlibc"
	cd "${BUILD_PATH}/mlibc"
	meson setup -Ddefault_library=static --cross-file ${MESON_CROSS_FILE_PATH} --prefix=/usr -Dno_headers=true "${MLIBC_PATH}"
	DESTDIR="${SYSROOT_PATH}" ninja install
	cd "${BASE_PATH}"
	touch "${BUILD_PATH}/.mlibc_built"
fi
