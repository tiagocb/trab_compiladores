loadI 1 => r1
loadI 2 => r2
loadI 2 => r2
loadI 2 => r2
loadI 1 => r1
add r1, r2 => r3
add r2, r1 => r3
mult r3, r2 => r4
mult r2, r3 => r4
loadI 6 => r4
addI r1, 5 => r4
nop
nop
nop
nop
nop
nop
rsubI r3, 19 => r5
addI r4, 10 => r5
store r4 => r5