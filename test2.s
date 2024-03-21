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
# Store parameter 0 ( x)
sw a0,-4(s0)
# Store parameter 1 ( y)
sw a1,-8(s0)
# Store parameter 2 ( x)
sw a2,-12(s0)
# Store parameter 3 ( y)
sw a3,-16(s0)
# Evaluating left side for and
# Load variable "x"
lw a0,-4(s0)
# Pushing left side result for and
addi sp,sp,-4
sw a0,0(sp)
# Eva;iatomg side result for and
# Load variable "y"
lw a0,-8(s0)
# Popping saved left side result for and
lw a1,0(sp)
addi sp,sp,4
# Executing operation for and
and a0,a1,a0
mv sp,s0
lw s0,(sp)
lw ra,4(sp)
addi sp,sp,8
ret
