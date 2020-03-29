EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title "OpenSourceVentilator Controller"
Date "Mar. 28 2020"
Rev "2"
Comp "https://github.com/ermtl/Open-Source-Ventilator"
Comment1 "Drafting: Robbie Sharma"
Comment2 "Designer: Eric Vinter"
Comment3 "Design Credit: Eric Vinter - https://github.com/ermtl/Open-Source-Ventilator"
Comment4 "https://gitlab.com/open-source-ventilator/OpenLung"
$EndDescr
$Comp
L Device:R_POT_TRIM_US RV?
U 1 1 5E7D3FAC
P 6250 3000
AR Path="/5E7D3FAC" Ref="RV?"  Part="1" 
AR Path="/5E7CB03F/5E7D3FAC" Ref="RV201"  Part="1" 
F 0 "RV201" V 6045 3000 50  0000 C CNN
F 1 "10K" V 6136 3000 50  0000 C CNN
F 2 "" H 6250 3000 50  0001 C CNN
F 3 "~" H 6250 3000 50  0001 C CNN
	1    6250 3000
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5E7D3FB2
P 5900 3300
AR Path="/5E7D3FB2" Ref="R?"  Part="1" 
AR Path="/5E7CB03F/5E7D3FB2" Ref="R201"  Part="1" 
F 0 "R201" V 5695 3300 50  0000 C CNN
F 1 "270" V 5786 3300 50  0000 C CNN
F 2 "" H 5900 3300 50  0001 C CNN
F 3 "~" H 5900 3300 50  0001 C CNN
	1    5900 3300
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E7D3FBE
P 4400 4050
AR Path="/5E7D3FBE" Ref="C?"  Part="1" 
AR Path="/5E7CB03F/5E7D3FBE" Ref="C201"  Part="1" 
F 0 "C201" H 4150 4150 50  0000 L CNN
F 1 "0.1uF/50V" H 3900 4050 50  0000 L CNN
F 2 "" H 4400 4050 50  0001 C CNN
F 3 "~" H 4400 4050 50  0001 C CNN
	1    4400 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D?
U 1 1 5E7D3FC4
P 4000 3800
AR Path="/5E7D3FC4" Ref="D?"  Part="1" 
AR Path="/5E7CB03F/5E7D3FC4" Ref="D201"  Part="1" 
F 0 "D201" H 4000 3584 50  0000 C CNN
F 1 "SS36" H 4000 3675 50  0000 C CNN
F 2 "" H 4000 3800 50  0001 C CNN
F 3 "~" H 4000 3800 50  0001 C CNN
	1    4000 3800
	-1   0    0    1   
$EndComp
$Comp
L Device:D_Schottky D?
U 1 1 5E7D4AB0
P 6450 4150
AR Path="/5E7D4AB0" Ref="D?"  Part="1" 
AR Path="/5E7CB03F/5E7D4AB0" Ref="D202"  Part="1" 
F 0 "D202" V 6404 4229 50  0000 L CNN
F 1 "SS36" V 6495 4229 50  0000 L CNN
F 2 "" H 6450 4150 50  0001 C CNN
F 3 "~" H 6450 4150 50  0001 C CNN
	1    6450 4150
	0    1    1    0   
$EndComp
$Comp
L Device:CP1_Small C?
U 1 1 5E7D3FB8
P 4750 4050
AR Path="/5E7D3FB8" Ref="C?"  Part="1" 
AR Path="/5E7CB03F/5E7D3FB8" Ref="C202"  Part="1" 
F 0 "C202" H 4750 4150 50  0000 L CNN
F 1 "220uF/50V" H 4750 3950 50  0000 L CNN
F 2 "" H 4750 4050 50  0001 C CNN
F 3 "~" H 4750 4050 50  0001 C CNN
	1    4750 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E7D7E62
P 7700 4150
AR Path="/5E7D7E62" Ref="C?"  Part="1" 
AR Path="/5E7CB03F/5E7D7E62" Ref="C205"  Part="1" 
F 0 "C205" H 7792 4196 50  0000 L CNN
F 1 "0.1uF/50V" H 7792 4105 50  0000 L CNN
F 2 "" H 7700 4150 50  0001 C CNN
F 3 "~" H 7700 4150 50  0001 C CNN
	1    7700 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E7D84CA
P 6650 3300
AR Path="/5E7D84CA" Ref="C?"  Part="1" 
AR Path="/5E7CB03F/5E7D84CA" Ref="C203"  Part="1" 
F 0 "C203" V 6900 3400 50  0000 C CNN
F 1 "0.1uF/50V" V 6800 3450 50  0000 C CNN
F 2 "" H 6650 3300 50  0001 C CNN
F 3 "~" H 6650 3300 50  0001 C CNN
	1    6650 3300
	0    -1   -1   0   
$EndComp
$Comp
L Device:L L201
U 1 1 5E7D8EC4
P 6950 4000
F 0 "L201" V 7140 4000 50  0000 C CNN
F 1 "33uH" V 7049 4000 50  0000 C CNN
F 2 "" H 6950 4000 50  0001 C CNN
F 3 "~" H 6950 4000 50  0001 C CNN
	1    6950 4000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6400 3000 6750 3000
Wire Wire Line
	6750 3000 6750 3300
Wire Wire Line
	6250 3150 6250 3300
Wire Wire Line
	6250 3800 6150 3800
Wire Wire Line
	6000 3300 6250 3300
Connection ~ 6250 3300
Wire Wire Line
	6250 3300 6250 3800
$Comp
L power:GND #PWR0202
U 1 1 5E7DE582
P 5650 4550
F 0 "#PWR0202" H 5650 4300 50  0001 C CNN
F 1 "GND" H 5655 4377 50  0000 C CNN
F 2 "" H 5650 4550 50  0001 C CNN
F 3 "" H 5650 4550 50  0001 C CNN
	1    5650 4550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0201
U 1 1 5E7DEC9C
P 5500 3300
F 0 "#PWR0201" H 5500 3050 50  0001 C CNN
F 1 "GND" V 5505 3172 50  0000 R CNN
F 2 "" H 5500 3300 50  0001 C CNN
F 3 "" H 5500 3300 50  0001 C CNN
	1    5500 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	5500 3300 5800 3300
Wire Wire Line
	5650 4550 5650 4300
$Comp
L Regulator_Switching:LM2576HVS-5 U?
U 1 1 5E7D3FCA
P 5650 3900
AR Path="/5E7D3FCA" Ref="U?"  Part="1" 
AR Path="/5E7CB03F/5E7D3FCA" Ref="U201"  Part="1" 
F 0 "U201" H 5650 4267 50  0000 C CNN
F 1 "LM2576HVS-5" H 5650 4176 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-263-5_TabPin3" H 5650 3650 50  0001 L CIN
F 3 "http://www.ti.com/lit/ds/symlink/lm2576.pdf" H 5650 3900 50  0001 C CNN
	1    5650 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 4300 5650 4300
Connection ~ 5650 4300
Wire Wire Line
	5650 4300 5650 4200
Wire Wire Line
	7250 4250 7250 4300
Connection ~ 6450 4300
Wire Wire Line
	6450 4300 7250 4300
Wire Wire Line
	4400 4300 4750 4300
Wire Wire Line
	4400 4150 4400 4300
Wire Wire Line
	5150 4000 5150 4300
Connection ~ 5150 4300
Wire Wire Line
	5150 4300 5650 4300
Wire Wire Line
	4750 4150 4750 4300
Connection ~ 4750 4300
Wire Wire Line
	4750 4300 5150 4300
Wire Wire Line
	6150 4000 6450 4000
Wire Wire Line
	6450 4000 6800 4000
Connection ~ 6450 4000
Wire Wire Line
	7250 4000 7250 4050
Wire Wire Line
	7700 4000 7700 4050
Wire Wire Line
	7100 4000 7250 4000
Connection ~ 7250 4000
Wire Wire Line
	7250 4000 7700 4000
Wire Wire Line
	7700 4250 7700 4300
Wire Wire Line
	7700 4300 7250 4300
Connection ~ 7250 4300
Wire Wire Line
	6250 3300 6550 3300
Wire Wire Line
	6750 3000 7250 3000
Wire Wire Line
	7250 3000 7250 4000
Connection ~ 6750 3000
Connection ~ 7700 4000
Connection ~ 7700 4300
Connection ~ 4400 4300
Wire Wire Line
	4400 3950 4400 3800
Connection ~ 4400 3800
Wire Wire Line
	4400 3800 4150 3800
Wire Wire Line
	4750 3950 4750 3800
Wire Wire Line
	4750 3800 4400 3800
Wire Wire Line
	7700 4000 8350 4000
Wire Wire Line
	7700 4300 8350 4300
$Comp
L Device:CP1_Small C?
U 1 1 5E7D6AC0
P 7250 4150
AR Path="/5E7D6AC0" Ref="C?"  Part="1" 
AR Path="/5E7CB03F/5E7D6AC0" Ref="C204"  Part="1" 
F 0 "C204" H 7341 4196 50  0000 L CNN
F 1 "220uF/50V" H 7250 4050 50  0000 L CNN
F 2 "" H 7250 4150 50  0001 C CNN
F 3 "~" H 7250 4150 50  0001 C CNN
	1    7250 4150
	1    0    0    -1  
$EndComp
NoConn ~ 6000 3000
Wire Wire Line
	6000 3000 6100 3000
Text Notes 7400 4900 0    50   ~ 0
Ref: https://www.sunrom.com/p/lm2596hvs-adj-to263\nTODO: Verify C203 is supposed to be 0.1uF/50V
Text HLabel 8350 4000 2    50   Output ~ 0
Vout+
Text HLabel 8350 4300 2    50   Output ~ 0
Vout-
Text HLabel 3250 3800 0    50   Input ~ 0
Vin+
Text HLabel 3250 4300 0    50   Input ~ 0
Vin-
Wire Wire Line
	3250 4300 4400 4300
Wire Wire Line
	3250 3800 3850 3800
Wire Wire Line
	5150 3800 4750 3800
Connection ~ 4750 3800
$EndSCHEMATC
