.text
.globl f
f:
# Allocate space to save s0
addi sp,sp,-4
# Save caller's s0
sw s0,0(sp)
# Copy stack pointer into frame pointer
mv s0,sp
li a0, 12345
