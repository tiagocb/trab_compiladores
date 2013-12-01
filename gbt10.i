loadI 1 => r1
loadI 1 => r1
loadI 2 => r2
add r1, r2 => r3
subI r3, 1 => r3
add r2, r3 => r4
add r3, r4 => r5
add r4, r5 => r6
addI r6, 1 => r6
add r5, r6 => r7
jumpI -> L1
L1: jumpI -> L2
L2: jumpI -> L3
L3: jumpI -> L4
L4: jumpI -> L0
L0: add r9, r10 => r11
add r5, r6 => r7
nop
add r5, r6 => r7
nop
store r1 => r7