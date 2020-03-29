EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title "OpenSourceVentilator Controller"
Date "Mar. 28 2020"
Rev "1b"
Comp "https://github.com/ermtl/Open-Source-Ventilator"
Comment1 "Drafting: Robbie Sharma"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 3050 2125 0    50   Input ~ 0
VDD
Text HLabel 3075 4625 0    50   Input ~ 0
GND
Text HLabel 3050 3850 0    50   Input ~ 0
_TRIGGER
$Comp
L Device:R_Small_US R304
U 1 1 5E873250
P 6525 2725
F 0 "R304" H 6593 2771 50  0000 L CNN
F 1 "10K" H 6593 2680 50  0000 L CNN
F 2 "" H 6525 2725 50  0001 C CNN
F 3 "~" H 6525 2725 50  0001 C CNN
	1    6525 2725
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C301
U 1 1 5E8736DB
P 5875 2575
F 0 "C301" H 5990 2621 50  0000 L CNN
F 1 "1uF" H 5990 2530 50  0000 L CNN
F 2 "" H 5875 2575 50  0001 C CNN
F 3 "~" H 5875 2575 50  0001 C CNN
	1    5875 2575
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC547 Q301
U 1 1 5E87402B
P 5325 3425
F 0 "Q301" H 5516 3471 50  0000 L CNN
F 1 "BC547" H 5516 3380 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 5525 3350 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 5325 3425 50  0001 L CNN
	1    5325 3425
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC557 Q302
U 1 1 5E8747BD
P 7075 3075
F 0 "Q302" H 7266 3029 50  0000 L CNN
F 1 "BC557" H 7266 3120 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 7275 3000 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC557.pdf" H 7075 3075 50  0001 L CNN
	1    7075 3075
	1    0    0    1   
$EndComp
$Comp
L Diode:1N4148 D301
U 1 1 5E87585C
P 7175 4175
F 0 "D301" V 7129 4254 50  0000 L CNN
F 1 "1N4148" V 7220 4254 50  0000 L CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 7175 4000 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 7175 4175 50  0001 C CNN
	1    7175 4175
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R303
U 1 1 5E877953
P 6175 3075
F 0 "R303" V 5970 3075 50  0000 C CNN
F 1 "10K" V 6061 3075 50  0000 C CNN
F 2 "" H 6175 3075 50  0001 C CNN
F 3 "~" H 6175 3075 50  0001 C CNN
	1    6175 3075
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R305
U 1 1 5E877E6F
P 8475 2675
F 0 "R305" H 8543 2721 50  0000 L CNN
F 1 "220" H 8543 2630 50  0000 L CNN
F 2 "" H 8475 2675 50  0001 C CNN
F 3 "~" H 8475 2675 50  0001 C CNN
	1    8475 2675
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D302
U 1 1 5E8784E9
P 8475 3175
F 0 "D302" V 8514 3058 50  0000 R CNN
F 1 "LED" V 8423 3058 50  0000 R CNN
F 2 "" H 8475 3175 50  0001 C CNN
F 3 "~" H 8475 3175 50  0001 C CNN
	1    8475 3175
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small_US R302
U 1 1 5E879970
P 6075 3875
F 0 "R302" V 5870 3875 50  0000 C CNN
F 1 "100K" V 5961 3875 50  0000 C CNN
F 2 "" H 6075 3875 50  0001 C CNN
F 3 "~" H 6075 3875 50  0001 C CNN
	1    6075 3875
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R301
U 1 1 5E87A070
P 4975 2975
F 0 "R301" H 4907 2929 50  0000 R CNN
F 1 "100K" H 4907 3020 50  0000 R CNN
F 2 "" H 4975 2975 50  0001 C CNN
F 3 "~" H 4975 2975 50  0001 C CNN
	1    4975 2975
	-1   0    0    1   
$EndComp
Wire Wire Line
	5875 2725 5875 3075
Wire Wire Line
	5875 3075 6075 3075
Wire Wire Line
	8475 2775 8475 3025
Wire Wire Line
	6525 2825 6525 3075
Wire Wire Line
	6525 3075 6275 3075
Wire Wire Line
	6525 3075 6875 3075
Connection ~ 6525 3075
Wire Wire Line
	5875 2425 5875 2125
Wire Wire Line
	7175 2875 7175 2125
Connection ~ 7175 2125
Wire Wire Line
	6525 2625 6525 2125
Connection ~ 6525 2125
Wire Wire Line
	5875 2125 6525 2125
Wire Wire Line
	5875 3075 5425 3075
Wire Wire Line
	5425 3075 5425 3225
Connection ~ 5875 3075
Wire Wire Line
	4975 3425 4975 3875
Wire Wire Line
	4975 3875 5975 3875
Connection ~ 4975 3425
Wire Wire Line
	4975 3425 5125 3425
Wire Wire Line
	7175 3275 7175 3875
Wire Wire Line
	7175 3875 6175 3875
Wire Wire Line
	7175 3875 7175 4025
Connection ~ 7175 3875
Wire Wire Line
	7175 3875 8075 3875
Wire Wire Line
	8075 3875 8075 3925
Wire Wire Line
	7175 4325 7175 4625
Wire Wire Line
	7175 4625 7475 4625
Wire Wire Line
	8075 4625 8075 4525
$Comp
L power:GND #PWR0301
U 1 1 5E88298D
P 7475 4825
F 0 "#PWR0301" H 7475 4575 50  0001 C CNN
F 1 "GND" H 7480 4652 50  0000 C CNN
F 2 "" H 7475 4825 50  0001 C CNN
F 3 "" H 7475 4825 50  0001 C CNN
	1    7475 4825
	1    0    0    -1  
$EndComp
Text HLabel 8875 3625 2    50   Output ~ 0
ALARM+
Text HLabel 8875 4625 2    50   Output ~ 0
ALARM-
Wire Wire Line
	8475 2125 8475 2575
Wire Wire Line
	8575 4525 8575 4625
$Comp
L power:GND #PWR0302
U 1 1 5E8867CF
P 8575 4825
F 0 "#PWR0302" H 8575 4575 50  0001 C CNN
F 1 "GND" H 8580 4652 50  0000 C CNN
F 2 "" H 8575 4825 50  0001 C CNN
F 3 "" H 8575 4825 50  0001 C CNN
	1    8575 4825
	1    0    0    -1  
$EndComp
Wire Wire Line
	5425 3625 5425 4625
Wire Wire Line
	5425 4625 7175 4625
Connection ~ 7175 4625
Wire Wire Line
	7475 4625 7475 4825
Connection ~ 7475 4625
Wire Wire Line
	7475 4625 8075 4625
Wire Wire Line
	8575 4625 8575 4825
Connection ~ 8575 4625
Wire Wire Line
	8575 4625 8875 4625
Connection ~ 5425 4625
$Comp
L Transistor_BJT:BC547 Q303
U 1 1 5E889D3C
P 4025 3850
F 0 "Q303" H 4216 3896 50  0000 L CNN
F 1 "BC547" H 4216 3805 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 4225 3775 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 4025 3850 50  0001 L CNN
	1    4025 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8475 3325 8475 3625
Wire Wire Line
	8875 3625 8475 3625
Connection ~ 8475 3625
Wire Wire Line
	8475 3625 8475 3925
Connection ~ 5875 2125
Wire Wire Line
	4975 3075 4975 3425
$Comp
L Device:R_Small_US R306
U 1 1 5E920447
P 3400 3850
F 0 "R306" V 3195 3850 50  0000 C CNN
F 1 "100K" V 3286 3850 50  0000 C CNN
F 2 "" H 3400 3850 50  0001 C CNN
F 3 "~" H 3400 3850 50  0001 C CNN
	1    3400 3850
	0    -1   1    0   
$EndComp
Wire Wire Line
	3500 3850 3825 3850
Wire Wire Line
	3050 3850 3300 3850
$Comp
L Device:R_Small_US R307
U 1 1 5E926BC7
P 4125 2325
F 0 "R307" H 4058 2279 50  0000 R CNN
F 1 "10K" H 4058 2370 50  0000 R CNN
F 2 "" H 4125 2325 50  0001 C CNN
F 3 "~" H 4125 2325 50  0001 C CNN
	1    4125 2325
	1    0    0    1   
$EndComp
Wire Wire Line
	3075 4625 4125 4625
Wire Wire Line
	4125 4050 4125 4625
Connection ~ 4125 4625
Wire Wire Line
	4125 4625 5425 4625
Wire Wire Line
	4675 2525 4125 2525
$Comp
L Transistor_FET:SiA453EDJ Q304
U 1 1 5E8BB916
P 4875 2525
F 0 "Q304" H 5250 2525 50  0000 C CNN
F 1 "SiA453EDJ" H 5250 2425 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:Vishay_PowerPAK_SC70-6L_Single" H 5075 2450 50  0001 L CIN
F 3 "http://www.vishay.com/docs/62864/sia453edj.pdf" V 4875 2525 50  0001 L CNN
	1    4875 2525
	1    0    0    1   
$EndComp
Wire Wire Line
	4975 2725 4975 2875
Wire Wire Line
	4975 2125 4975 2325
Connection ~ 4975 2125
Wire Wire Line
	4975 2125 5875 2125
Wire Wire Line
	3050 2125 4125 2125
Wire Wire Line
	4125 2125 4125 2225
Connection ~ 4125 2125
Wire Wire Line
	4125 2125 4975 2125
Wire Wire Line
	4125 2425 4125 2525
Connection ~ 4125 2525
Wire Wire Line
	4125 2525 4125 3650
$Comp
L Relay:G6EU K301
U 1 1 5EA9DD6B
P 8275 4225
F 0 "K301" H 7846 4179 50  0000 R CNN
F 1 "G6EU" H 7846 4270 50  0000 R CNN
F 2 "Relay_THT:Relay_SPDT_Omron_G6E" H 9405 4195 50  0001 C CNN
F 3 "https://www.omron.com/ecb/products/pdf/en-g6e.pdf" H 8275 4225 50  0001 C CNN
	1    8275 4225
	1    0    0    1   
$EndComp
Wire Wire Line
	7175 2125 8475 2125
Wire Wire Line
	6525 2125 7175 2125
$EndSCHEMATC
