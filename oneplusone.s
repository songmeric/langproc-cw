.text
.globl f
f:
# Allocate space to save ra and caller's s0
addi sp,sp,-8
# Save caller's ra
sw ra,4(sp)
# Save caller's s0
sw s0,0(sp)
# Copy stack pointer into frame pointer
mv s0,sp
# Store parameter 0 ( a)
sw a0,18446744073709551612(s0)
# Store parameter 1 ( b)
sw a1,18446744073709551608(s0)
# Store parameter 2 ( a)
sw a2,18446744073709551604(s0)
# Store parameter 3 ( b)
sw a3,18446744073709551600(s0)
# Load int constant
li a0, 8
addi sp,sp,-4
sw a0,-20(s0)
# Evaluating left side for add
# Load variable "a"
lw a0,-4(s0)
# Pushing left side result for add
addi sp,sp,-4
sw a0,0(sp)
# Eva;iatomg side result for add
# Evaluating left side for mul
# Evaluating left side for mul
# Load variable "b"
lw a0,-8(s0)
# Pushing left side result for mul
addi sp,sp,-4
sw a0,0(sp)
# Eva;iatomg side result for mul
# Load variable "c"
lw a0,-20(s0)
# Popping saved left side result for mul
lw a1,0(sp)
addi sp,sp,4
# Executing operation for mul
mul a0,a1,a0
# Pushing left side result for mul
addi sp,sp,-4
sw a0,0(sp)
# Eva;iatomg side result for mul
# Load int constant
li a0, 5
# Popping saved left side result for mul
lw a1,0(sp)
addi sp,sp,4
# Executing operation for mul
mul a0,a1,a0
# Popping saved left side result for add
lw a1,0(sp)
addi sp,sp,4
# Executing operation for add
add a0,a1,a0
mv sp,s0
lw s0,(sp)
lw ra,4(sp)
addi sp,sp,8
ret
