# arduino-licht
An Arduino Light with PiR and LDR

### Pinout
```
        +--_--+
   RST -|1   8|- 5V
(A3) 3 -|2   7|- 2 (A1)
(A2) 4 -|3   6|- 1 (PWM)
   GND -|4   5|- 0 (PWM)
        +-----+
```
### Circuit
```
LDR:
5V---LDR-+-R2---GND
         |
         A2
LED:
0---LED---R1---GND

R1 = ~220ohm
R2 = ~2.2Kohm

```
