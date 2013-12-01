loadI 1 => r1
loadI 2 => r2
nop
add r1, r2 => r3
i2i r3 => r4
addI r4, 4 => r5
add r5, r4 => r6
add r2, r1 => r4
i2i r4 => r3
addI r3, 5 => r5
L1: nop
addI r3, 5 => r2
addI r3, 5 => r3
jumpI -> L1