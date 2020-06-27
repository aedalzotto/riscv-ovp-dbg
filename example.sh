#!/bin/sh

# Select CrossCompiler/Processor Type
# for example RISCV32/riscv or IMG_MIPS32R2/mips32 or ARM7TDMI/arm
CROSS=RISCV32
PROCTYPE=riscv

# Check Installation supports this example and installed processor model
checkinstall.exe -group ${PROCTYPE} -p install.pkg --nobanner \
                 -message " If selected processor ${PROCTYPE} is not installed please select an alternative in the example script." || exit

# Build Application
make -C application CROSS=${CROSS}

# Build Platform
make -C module 

platform/platform.${IMPERAS_ARCH}.exe 
