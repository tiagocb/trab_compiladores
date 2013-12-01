loadI 1 => r1
loadI 1 => r2
loadI 1 => r2
loadI 1 => r2
jumpI -> L0
loadI 1 => r1
add r1, r2 => r3
add r1, r2 => r3
jumpI -> L4
add r2, r1 => r3
addI r3, 1 => r3
jumpI -> L1
mult r3, r2 => r4
mult r2, r3 => r4
L1: jumpI -> L2
subI r4, 1 => r4
div r3, r4 => r5
div r4, r3 => r5
L2: jumpI -> L3
nop
nop
L3: jumpI -> L4
nop
nop
nop
L4: jumpI -> L0
addI r1, 0 => r3
subI r1, 0 => r3
addI r3, 1 => r3
L0: add r9, r10 => r11