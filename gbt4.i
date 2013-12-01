loadI 1 => r1
loadI 1 => r2
add r1, r2 => r3
jumpI -> L0
add r2, r3 => r4
jumpI -> L4
add r3, r4 => r5
jumpI -> L1
add r5, r6 => r7
L1: jumpI -> L2
add r6, r7 => r8
L2: jumpI -> L3
add r8, r9 => r10
L3: jumpI -> L4
L4: jumpI -> L0
L0: add r9, r10 => r11