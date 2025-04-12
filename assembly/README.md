# Assembly

With RISC-V ISA

## Getting Started

GCC
- Assembler (Assembly to Object Code)
- Linker (Object Code to Binary)
QEMU - RISC-V Emulator
GDB - Debugger

Assembly files will end with the extension ".s"
Linked files will end with the extension ".ld"
Executable and Linkable files end with the extension ".elf"

The below command create an executable,

```
riscv64-unknown-elf-gcc -O0 -ggdb -nostdlib -march=rv32im -mabi=ilp32 -Wl,-Tmain.ld main.s -o main.elf
```

RISC-V Emulator,

```
qemu-system-riscv32 -S -M virt -nographic -bios none -kernel main.elf -gdb tcp::1234
```

To quit QEMU, ^A and X

GDB,

```
gdb-multiarch main.elf -ex "target remote localhost:1234" -ex "break _start" -ex "continue" -q
```

GDB Commands,

```
info reg
```

Type "q" to quit.

To strip off debugging information from the main.elf file,

```
riscv64-unknown-elf-objcopy -O binary main.elf main.bin
```

To pretty-print,

```
xxd -e -c 4 -g 4 main.bin
``

> Note: The x0 register always points to 0, which can be used to assign numbers to assign values to other registers using the instruction addi, which means add immediate.