# Fibonacci Series, RISC-V

_start:
    addi x1, x0, 0
    addi x2, x0, 1
    addi x3, x0, 5 # loop value

loop:
    # The loop part
    add x4, x1, x2
    addi x3, x0, -1 # Decrementing loop value
    add x1, x0, x2
    add x2, x0, x4

    bne, x0, x3, loop

j.

