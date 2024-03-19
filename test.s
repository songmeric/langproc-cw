.text
.globl f
f:
# Allocate space to save s0
addi sp,sp,-4
# Save caller's s0
sw s0,0(sp)
# Copy stack pointer into frame pointer
mv s0,sp
# Load int constant
li a0, 5
addi sp,sp,-4
sw a0,-4(s0)
# Load int constant
li a0, 3
addi sp,sp,-4
sw a0,-8(s0)
# Load variable "y"
lw a0,-8(s0)
mv sp,s0
lw s0,(sp)
addi sp,sp,4
ret
