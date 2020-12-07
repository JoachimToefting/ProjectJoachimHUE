# ProjectJoachimHUE
 Projekt til Embedded 2

## Projektets opbygning
Det går ud på at i det smarte hus kan man styrer lysets farve med en keypad, keypadden skal pladseres på MEGA2560 sammen med en display, når man er færdig med at indtaste lysinformation (RGB) bliver den sent over seriel til et uno board.

Elementer der skal implamenteres:
- LCD til information der bliver indtastet
- Keypad der skal være bruger indtast
- RGB LED der skal outputte lyset med informationen

Ekstra:
- Kører informationen over seriel til et uno board der så for led'en til at lyse
- Et menu system til LCD panelet

## Opsætning af Hardware
---
### LCD
Nogle tegninger
| LCD | Board | PORT  |
| :-: | :---: | :---: |
| VSS |  GND  |
| VDD |  5V   |
| V0  | Pot*  |
| RS  |   6   |  PH3  |
| RW  |   7   |  PH4  |
| E   |   8   |  PH5  |
| D4  |  22   |  PA0  |
| D5  |  23   |  PA1  |
| D6  |  24   |  PA2  |
| D7  |  25   |  PA3  |
| A   |  5v   |
| K   |  5v   |

*til det tredjeben på potentiometeret

```RS``` til ```D7``` kan indstilles til ønskede porte og pin i filen lcd.h

### Keypad
En tegning
| Keypad | Board | PORT  |
| :----: | :---: | :---: |
| 1      |  A8   |  PK0  |
| 2      |  A9   |  PK1  |
| 3      |  A10  |  PK2  |
| 4      |  A11  |  PK3  |
| 5      |  A12  |  PK4  |
| 6      |  A13  |  PK5  |
| 7      |  A14  |  PK6  |
| 8      |  A15  |  PK7  |


## Sådan bruges interfacet:
---
Der vil på displayet blive vist:
```
Color:
Red:
```
Her vælges hvor stærk den røde farve skal blive fra 000 til 255 der skal indtastet tre cifre, dette gøres også for blå: ```Blue``` og grøn: ```green```.

## Forstå Errors

Efter indtastning af farver kan der ses:
```
No color set,
Error: X
```
Hvis X er ```-1``` er det fordi en af indtastningerne ikke er et tal.

Hvis X er ```-2``` er det fordi en af indtastningerne ikke er inden for 0 til 255.