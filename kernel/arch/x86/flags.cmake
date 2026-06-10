set(ISA_C_FLAGS "-std=gnu11" "-g" "-ffreestanding" "-O2" "-Wall" "-Wextra")
set(ISA_LINKER_FLAGS "-ffreestanding" "-O2" "-nostdlib" "-lgcc" "-z max-page-size=0x1000")
