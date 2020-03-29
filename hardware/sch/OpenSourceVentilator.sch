EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title "OpenSourceVentilator Controller"
Date "Mar. 28 2020"
Rev "2b"
Comp "https://github.com/ermtl/Open-Source-Ventilator"
Comment1 "Drafting: Robbie Sharma"
Comment2 "Designer: Eric Vinter"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Driver_Motor:Pololu_Breakout_A4988 A101
U 1 1 5E7BFF5C
P 8650 5125
F 0 "A101" H 9300 5725 50  0000 C CNN
F 1 "Pololu_Breakout_A4988" H 9400 5825 50  0000 C CNN
F 2 "Module:Pololu_Breakout-16_15.2x20.3mm" H 8925 4375 50  0001 L CNN
F 3 "https://www.pololu.com/product/2980/pictures" H 8750 4825 50  0001 C CNN
	1    8650 5125
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5E81E84B
P 4000 4175
F 0 "#PWR0106" H 4000 3925 50  0001 C CNN
F 1 "GND" H 4005 4002 50  0000 C CNN
F 2 "" H 4000 4175 50  0001 C CNN
F 3 "" H 4000 4175 50  0001 C CNN
	1    4000 4175
	1    0    0    -1  
$EndComp
Wire Wire Line
	4575 3750 4575 3900
Wire Wire Line
	8250 4725 8100 4725
Wire Wire Line
	8100 4825 8250 4825
Text Label 3525 2850 0    50   ~ 0
STEP_STP
Text Label 6825 5125 0    50   ~ 0
STEP_DIR
Text Label 6825 5225 0    50   ~ 0
STEP_STP
$Comp
L power:GND #PWR0105
U 1 1 5E7FC7A7
P 8650 6075
F 0 "#PWR0105" H 8650 5825 50  0001 C CNN
F 1 "GND" H 8655 5902 50  0000 C CNN
F 2 "" H 8650 6075 50  0001 C CNN
F 3 "" H 8650 6075 50  0001 C CNN
	1    8650 6075
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J103
U 1 1 5E8080EB
P 9600 5125
F 0 "J103" H 9500 4675 50  0000 L CNN
F 1 "Conn_Stepper_Motor" H 9300 4800 50  0000 L CNN
F 2 "" H 9600 5125 50  0001 C CNN
F 3 "~" H 9600 5125 50  0001 C CNN
	1    9600 5125
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 5025 9150 5025
Wire Wire Line
	9150 5125 9400 5125
Wire Wire Line
	9400 5225 9150 5225
Wire Wire Line
	9150 5325 9400 5325
$Comp
L Device:CP1 C101
U 1 1 5E82A24D
P 2150 6300
F 0 "C101" V 1898 6300 50  0000 C CNN
F 1 "4700uF/50V" V 1989 6300 50  0000 C CNN
F 2 "" H 2150 6300 50  0001 C CNN
F 3 "~" H 2150 6300 50  0001 C CNN
	1    2150 6300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2150 6450 2150 6550
Wire Wire Line
	2150 6050 2150 6150
Connection ~ 2150 6050
Connection ~ 2150 6550
Wire Wire Line
	8650 5925 8650 5950
Text Label 4150 6150 2    50   ~ 0
VPWR_8V
Text Label 6075 1200 2    50   ~ 0
VPWR_8V
Wire Wire Line
	8100 4725 8100 4825
NoConn ~ 8050 5525
NoConn ~ 8050 5625
Wire Wire Line
	8050 5525 8250 5525
Wire Wire Line
	8050 5625 8250 5625
$Comp
L power:+5V #PWR0114
U 1 1 5E8C8AC8
P 4775 750
F 0 "#PWR0114" H 4775 600 50  0001 C CNN
F 1 "+5V" H 4825 900 50  0000 C CNN
F 2 "" H 4775 750 50  0001 C CNN
F 3 "" H 4775 750 50  0001 C CNN
	1    4775 750 
	1    0    0    -1  
$EndComp
Text Label 3525 2750 0    50   ~ 0
STEP_DIR
Text Label 3550 2950 0    50   ~ 0
STROBE_TM
Text Label 3625 2450 0    50   ~ 0
CLK_TM
Text Label 3625 2550 0    50   ~ 0
DIO_TM
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5E900544
P 2150 6050
F 0 "#FLG0101" H 2150 6125 50  0001 C CNN
F 1 "PWR_FLAG" H 2150 6223 50  0000 C CNN
F 2 "" H 2150 6050 50  0001 C CNN
F 3 "~" H 2150 6050 50  0001 C CNN
	1    2150 6050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 5E86CE08
P 4225 6450
F 0 "#PWR0109" H 4225 6200 50  0001 C CNN
F 1 "GND" H 4230 6277 50  0000 C CNN
F 2 "" H 4225 6450 50  0001 C CNN
F 3 "" H 4225 6450 50  0001 C CNN
	1    4225 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4225 6400 4225 6450
Wire Wire Line
	3450 6400 3625 6400
Wire Wire Line
	3450 6150 3625 6150
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5E90BC7A
P 2150 6550
F 0 "#FLG0102" H 2150 6625 50  0001 C CNN
F 1 "PWR_FLAG" H 2150 6723 50  0000 C CNN
F 2 "" H 2150 6550 50  0001 C CNN
F 3 "~" H 2150 6550 50  0001 C CNN
	1    2150 6550
	1    0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 5E90CFC8
P 3625 6150
F 0 "#FLG0103" H 3625 6225 50  0001 C CNN
F 1 "PWR_FLAG" H 3700 6325 50  0000 C CNN
F 2 "" H 3625 6150 50  0001 C CNN
F 3 "~" H 3625 6150 50  0001 C CNN
	1    3625 6150
	1    0    0    -1  
$EndComp
Connection ~ 3625 6150
$Comp
L power:PWR_FLAG #FLG0104
U 1 1 5E90DEB5
P 3625 6400
F 0 "#FLG0104" H 3625 6475 50  0001 C CNN
F 1 "PWR_FLAG" H 3550 6575 50  0000 C CNN
F 2 "" H 3625 6400 50  0001 C CNN
F 3 "~" H 3625 6400 50  0001 C CNN
	1    3625 6400
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J101
U 1 1 5E8D5C5A
P 2150 2350
F 0 "J101" H 1900 2875 50  0000 C CNN
F 1 "Conn_TM1638_Disp" H 1850 2750 50  0000 C CNN
F 2 "" H 2150 2350 50  0001 C CNN
F 3 "~" H 2150 2350 50  0001 C CNN
	1    2150 2350
	-1   0    0    -1  
$EndComp
Connection ~ 3625 6400
Wire Wire Line
	3625 6400 4225 6400
$Comp
L power:+3.3V #PWR0107
U 1 1 5E962B4A
P 5000 1650
F 0 "#PWR0107" H 5000 1500 50  0001 C CNN
F 1 "+3.3V" H 5000 1800 50  0000 C CNN
F 2 "" H 5000 1650 50  0001 C CNN
F 3 "" H 5000 1650 50  0001 C CNN
	1    5000 1650
	1    0    0    -1  
$EndComp
$Comp
L power:+24V #PWR0101
U 1 1 5E7D126E
P 2375 5650
F 0 "#PWR0101" H 2375 5500 50  0001 C CNN
F 1 "+24V" H 2390 5823 50  0000 C CNN
F 2 "" H 2375 5650 50  0001 C CNN
F 3 "" H 2375 5650 50  0001 C CNN
	1    2375 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 6050 2375 6050
Wire Wire Line
	2150 6550 2375 6550
Connection ~ 2375 6050
Wire Wire Line
	2375 6050 2550 6050
Connection ~ 2375 6550
Wire Wire Line
	2375 6550 2800 6550
Wire Wire Line
	2350 2550 4075 2550
Wire Wire Line
	2350 2450 4075 2450
Wire Wire Line
	2350 2250 2875 2250
Wire Wire Line
	2350 2150 2875 2150
$Comp
L Connector_Generic:Conn_01x04 J109
U 1 1 5E7F3A60
P 7325 3250
F 0 "J109" H 7405 3242 50  0000 L CNN
F 1 "Conn_BME280_1" H 7405 3151 50  0000 L CNN
F 2 "" H 7325 3250 50  0001 C CNN
F 3 "~" H 7325 3250 50  0001 C CNN
	1    7325 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7125 3350 6800 3350
Text Label 5125 3150 0    50   ~ 0
SDA_I2C
Text Label 5125 3250 0    50   ~ 0
SCL_I2C
Wire Wire Line
	4675 1675 4675 1750
Wire Wire Line
	5550 3450 6950 3450
Wire Wire Line
	2875 2250 2875 2950
Wire Wire Line
	4775 1075 2875 1075
Connection ~ 4775 1075
Wire Wire Line
	4775 1075 4775 1750
$Comp
L Connector_Generic:Conn_01x03 J104
U 1 1 5E833250
P 800 3125
F 0 "J104" H 825 2750 50  0000 C CNN
F 1 "BUZZER" H 825 2875 50  0000 C CNN
F 2 "" H 800 3125 50  0001 C CNN
F 3 "~" H 800 3125 50  0001 C CNN
	1    800  3125
	-1   0    0    1   
$EndComp
Text Label 3525 2650 0    50   ~ 0
STEP_DISABLE
Text Label 6825 5025 0    50   ~ 0
STEP_DISABLE
$Comp
L Connector_Generic:Conn_01x05 J110
U 1 1 5E8521D0
P 6600 5125
F 0 "J110" H 6518 4700 50  0000 C CNN
F 1 "Conn_A4988_Stepper" H 6518 4791 50  0000 C CNN
F 2 "" H 6600 5125 50  0001 C CNN
F 3 "~" H 6600 5125 50  0001 C CNN
	1    6600 5125
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J105
U 1 1 5E852BC2
P 1675 2750
F 0 "J105" H 1975 2250 50  0000 C CNN
F 1 "Conn_A4988_Stepper" H 1875 2375 50  0000 C CNN
F 2 "" H 1675 2750 50  0001 C CNN
F 3 "~" H 1675 2750 50  0001 C CNN
	1    1675 2750
	-1   0    0    1   
$EndComp
Wire Wire Line
	1875 2650 4075 2650
Wire Wire Line
	1875 2750 4075 2750
Wire Wire Line
	1875 2850 4075 2850
Wire Wire Line
	1875 2950 2875 2950
Connection ~ 2875 2950
Wire Wire Line
	2875 2950 2875 3225
Wire Wire Line
	2875 1075 2875 2150
Wire Wire Line
	2875 1075 1875 1075
Wire Wire Line
	1875 1075 1875 2550
Connection ~ 2875 1075
Wire Wire Line
	1875 1075 1050 1075
Connection ~ 1875 1075
Connection ~ 2875 3225
Wire Wire Line
	2875 3225 2875 3900
Wire Wire Line
	6800 5125 7775 5125
Wire Wire Line
	7775 5125 7775 5225
Wire Wire Line
	7775 5225 8250 5225
Wire Wire Line
	7900 5125 7900 5175
Wire Wire Line
	7900 5175 7675 5175
Wire Wire Line
	7675 5175 7675 5225
Wire Wire Line
	7675 5225 6800 5225
Wire Wire Line
	7900 5125 8250 5125
Wire Wire Line
	8000 4925 7700 4925
Wire Wire Line
	8650 4425 8650 4325
Wire Wire Line
	8650 4325 8475 4325
Wire Wire Line
	8000 4325 8000 4925
Wire Wire Line
	8000 4925 8000 5425
Wire Wire Line
	8000 5425 8250 5425
Connection ~ 8000 4925
Wire Wire Line
	7125 5950 7500 5950
Connection ~ 8650 5950
Wire Wire Line
	8650 5950 8650 6075
$Comp
L Connector_Generic:Conn_01x02 J108
U 1 1 5E8C2AF7
P 6300 1200
F 0 "J108" H 6380 1192 50  0000 L CNN
F 1 "Conn_Nano_PWR" H 6380 1101 50  0000 L CNN
F 2 "" H 6300 1200 50  0001 C CNN
F 3 "~" H 6300 1200 50  0001 C CNN
	1    6300 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4675 1675 5000 1675
Wire Wire Line
	5000 1650 5000 1675
Connection ~ 5000 1675
Wire Wire Line
	5000 1675 5550 1675
Wire Wire Line
	4475 1200 4475 1750
Wire Wire Line
	5650 3350 5650 3675
Wire Wire Line
	5650 3900 5250 3900
$Comp
L power:GND #PWR0103
U 1 1 5E8E6464
P 5250 4175
F 0 "#PWR0103" H 5250 3925 50  0001 C CNN
F 1 "GND" H 5255 4002 50  0000 C CNN
F 2 "" H 5250 4175 50  0001 C CNN
F 3 "" H 5250 4175 50  0001 C CNN
	1    5250 4175
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3900 5250 4175
$Comp
L MCU_Module:Arduino_Nano_v3.x A102
U 1 1 5E80A9CC
P 4575 2750
F 0 "A102" H 4075 4100 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 4100 4000 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 4575 2750 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 4575 2750 50  0001 C CNN
	1    4575 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4675 3750 4675 3900
Wire Wire Line
	4675 3900 5250 3900
Connection ~ 5250 3900
Wire Wire Line
	4000 3900 4000 4175
Connection ~ 4000 3900
Wire Wire Line
	4000 3900 4575 3900
$Comp
L Connector_Generic:Conn_01x02 J107
U 1 1 5E90491A
P 4700 6300
F 0 "J107" H 4780 6292 50  0000 L CNN
F 1 "Conn_Nano_PWR" H 4780 6201 50  0000 L CNN
F 2 "" H 4700 6300 50  0001 C CNN
F 3 "~" H 4700 6300 50  0001 C CNN
	1    4700 6300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J106
U 1 1 5E90594A
P 2800 6925
F 0 "J106" H 2880 6917 50  0000 L CNN
F 1 "Conn_Motor_PWR" H 2880 6826 50  0000 L CNN
F 2 "" H 2800 6925 50  0001 C CNN
F 3 "~" H 2800 6925 50  0001 C CNN
	1    2800 6925
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J111
U 1 1 5E906111
P 10275 4625
F 0 "J111" H 10355 4617 50  0000 L CNN
F 1 "Conn_Motor_PWR" H 10355 4526 50  0000 L CNN
F 2 "" H 10275 4625 50  0001 C CNN
F 3 "~" H 10275 4625 50  0001 C CNN
	1    10275 4625
	1    0    0    -1  
$EndComp
$Comp
L power:+24V #PWR0108
U 1 1 5E906ED9
P 8850 4250
F 0 "#PWR0108" H 8850 4100 50  0001 C CNN
F 1 "+24V" H 8865 4423 50  0000 C CNN
F 2 "" H 8850 4250 50  0001 C CNN
F 3 "" H 8850 4250 50  0001 C CNN
	1    8850 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 4250 8850 4300
Wire Wire Line
	8850 4300 10075 4300
Wire Wire Line
	10075 4300 10075 4625
Connection ~ 8850 4300
Wire Wire Line
	8850 4300 8850 4425
Wire Wire Line
	10075 5925 8850 5925
Connection ~ 8850 5925
Wire Wire Line
	8850 5925 8850 6075
Wire Wire Line
	10075 4725 10075 5925
Wire Wire Line
	4500 6150 4500 6300
Wire Wire Line
	3625 6150 4500 6150
Connection ~ 4225 6400
Wire Notes Line
	6025 7800 475  7800
Wire Notes Line
	475  7800 475  4975
Wire Notes Line
	475  4975 6025 4975
Wire Wire Line
	4225 6400 4500 6400
$Sheet
S 2800 5950 650  700 
U 5E7CB03F
F0 "LM2596HVS Step Down PSU 4.5V- 60V To 3-35V" 50
F1 "LM2596HVS_Adjustable_Regulator.sch" 50
F2 "Vin+" I L 2800 6050 50 
F3 "Vin-" I L 2800 6550 50 
F4 "Vout+" O R 3450 6150 50 
F5 "Vout-" O R 3450 6400 50 
$EndSheet
Text Label 4175 6400 2    50   ~ 0
VNANO_GND
$Comp
L power:+5V #PWR0104
U 1 1 5E941759
P 8475 4225
F 0 "#PWR0104" H 8475 4075 50  0001 C CNN
F 1 "+5V" H 8525 4375 50  0000 C CNN
F 2 "" H 8475 4225 50  0001 C CNN
F 3 "" H 8475 4225 50  0001 C CNN
	1    8475 4225
	1    0    0    -1  
$EndComp
Wire Wire Line
	8475 4225 8475 4325
Connection ~ 8475 4325
Wire Wire Line
	8475 4325 8000 4325
Text Label 6075 1300 2    50   ~ 0
VNANO_GND
Wire Wire Line
	2550 6925 2600 6925
Wire Wire Line
	2375 5650 2375 6050
Wire Wire Line
	2550 6925 2550 6050
Connection ~ 2550 6050
Wire Wire Line
	2550 6050 2800 6050
Wire Wire Line
	2600 7025 2375 7025
Wire Wire Line
	2375 7025 2375 6550
Wire Wire Line
	5550 1675 5550 3450
Wire Notes Line
	6025 3750 11200 3750
Wire Notes Line
	6025 3750 6025 7800
Text Notes 2150 5275 2    157  ~ 31
Power Supply
Text Notes 8050 4000 2    157  ~ 31
Motor Controller
Text Notes 2425 800  2    157  ~ 31
Main Controller
Wire Wire Line
	1050 1075 1050 3025
Wire Wire Line
	1000 3025 1050 3025
Wire Wire Line
	1000 3225 2875 3225
$Comp
L Connector:Barrel_Jack J102
U 1 1 5E82ADEB
P 875 6275
F 0 "J102" H 950 6575 50  0000 R CNN
F 1 "24V Barrel Jack" H 1175 6500 50  0000 R CNN
F 2 "" H 925 6235 50  0001 C CNN
F 3 "~" H 925 6235 50  0001 C CNN
	1    875  6275
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5E7FF736
P 2375 7200
F 0 "#PWR0102" H 2375 6950 50  0001 C CNN
F 1 "GND" H 2380 7027 50  0000 C CNN
F 2 "" H 2375 7200 50  0001 C CNN
F 3 "" H 2375 7200 50  0001 C CNN
	1    2375 7200
	1    0    0    -1  
$EndComp
Connection ~ 2375 7025
Wire Wire Line
	2375 7025 2375 7200
Wire Wire Line
	2550 5725 2550 6050
Text Label 3125 5725 2    50   ~ 0
VPWR_24V
Wire Wire Line
	2550 5725 3125 5725
Text Label 7725 1150 2    50   ~ 0
VPWR_24V
Wire Wire Line
	6100 1300 5650 1300
Connection ~ 5650 3350
$Comp
L Device:CP1_Small C104
U 1 1 5E8292E8
P 7625 1625
F 0 "C104" H 7775 1700 50  0000 C CNN
F 1 "10nF" H 7800 1625 50  0000 C CNN
F 2 "" H 7625 1625 50  0001 C CNN
F 3 "~" H 7625 1625 50  0001 C CNN
	1    7625 1625
	1    0    0    -1  
$EndComp
Wire Wire Line
	5075 2850 5425 2850
$Comp
L Device:R_Small_US R104
U 1 1 5E82FA3D
P 7225 1350
F 0 "R104" H 7157 1304 50  0000 R CNN
F 1 "68K" H 7157 1395 50  0000 R CNN
F 2 "" H 7225 1350 50  0001 C CNN
F 3 "~" H 7225 1350 50  0001 C CNN
	1    7225 1350
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R105
U 1 1 5E85B73C
P 7225 1675
F 0 "R105" H 7157 1629 50  0000 R CNN
F 1 "10K" H 7157 1720 50  0000 R CNN
F 2 "" H 7225 1675 50  0001 C CNN
F 3 "~" H 7225 1675 50  0001 C CNN
	1    7225 1675
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 5E85CBBC
P 7225 2150
F 0 "#PWR0112" H 7225 1900 50  0001 C CNN
F 1 "GND" H 7230 1977 50  0000 C CNN
F 2 "" H 7225 2150 50  0001 C CNN
F 3 "" H 7225 2150 50  0001 C CNN
	1    7225 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7725 1150 7225 1150
Wire Wire Line
	7225 1150 7225 1250
Wire Wire Line
	7225 1450 7225 1500
Wire Wire Line
	7225 1775 7225 1825
Connection ~ 7225 1500
Wire Wire Line
	7225 1500 7225 1575
$Comp
L power:GND #PWR0113
U 1 1 5E8B8AF2
P 8850 6075
F 0 "#PWR0113" H 8850 5825 50  0001 C CNN
F 1 "GND" H 8855 5902 50  0000 C CNN
F 2 "" H 8850 6075 50  0001 C CNN
F 3 "" H 8850 6075 50  0001 C CNN
	1    8850 6075
	1    0    0    -1  
$EndComp
Wire Wire Line
	7125 5325 7125 5950
Wire Wire Line
	6800 5325 7125 5325
Text Label 4650 1075 2    55   ~ 0
VNANO_5V
Text Label 6825 4925 0    55   ~ 0
VNANO_5V
Text Label 6825 5325 0    50   ~ 0
VNANO_GND
Text Label 1725 7025 0    50   ~ 0
VMOTOR_GND
Wire Wire Line
	1725 7025 2375 7025
Text Label 9425 5925 0    50   ~ 0
VMOTOR_GND
Text Label 10000 4300 2    50   ~ 0
VPWR_24V
Wire Wire Line
	6500 2675 6500 3150
Connection ~ 6500 3150
Wire Wire Line
	6500 3150 7125 3150
Wire Wire Line
	6650 2775 6650 3250
Connection ~ 6650 3250
Wire Wire Line
	6650 3250 7125 3250
Wire Wire Line
	6800 2875 6800 3350
Connection ~ 6800 3350
Wire Wire Line
	6800 3350 5650 3350
Wire Wire Line
	6950 2975 6950 3450
Connection ~ 6950 3450
Wire Wire Line
	6950 3450 7125 3450
$Comp
L Device:CP1_Small C105
U 1 1 5E93BC17
P 7500 5525
F 0 "C105" H 7650 5600 50  0000 C CNN
F 1 "100uF" H 7700 5525 50  0000 C CNN
F 2 "" H 7500 5525 50  0001 C CNN
F 3 "~" H 7500 5525 50  0001 C CNN
	1    7500 5525
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 4925 7500 5425
Connection ~ 7500 4925
Wire Wire Line
	7500 4925 6800 4925
Wire Wire Line
	7500 5625 7500 5950
Connection ~ 7500 5950
Wire Wire Line
	7500 5950 8650 5950
Wire Wire Line
	2350 2350 3225 2350
Wire Wire Line
	3225 2350 3225 2950
Wire Wire Line
	3225 2950 4075 2950
Text Label 3850 3050 2    50   ~ 0
BUZZER
$Comp
L Device:Fuse_Polarized_Small F101
U 1 1 5E96C0EF
P 1350 6175
F 0 "F101" H 1350 6360 50  0000 C CNN
F 1 "Fuse" H 1350 6269 50  0000 C CNN
F 2 "" H 1350 6175 50  0001 C CNN
F 3 "~" H 1350 6175 50  0001 C CNN
	1    1350 6175
	1    0    0    -1  
$EndComp
Wire Wire Line
	1175 6175 1250 6175
Wire Wire Line
	7125 2975 6950 2975
Wire Wire Line
	7125 2875 6800 2875
Wire Wire Line
	7125 2775 6650 2775
Wire Wire Line
	7125 2675 6500 2675
$Comp
L Connector_Generic:Conn_01x04 J112
U 1 1 5E8EFC12
P 7325 2775
F 0 "J112" H 7405 2767 50  0000 L CNN
F 1 "Conn_BME280_2" H 7405 2676 50  0000 L CNN
F 2 "" H 7325 2775 50  0001 C CNN
F 3 "~" H 7325 2775 50  0001 C CNN
	1    7325 2775
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0111
U 1 1 5E98B924
P 5750 1925
F 0 "#PWR0111" H 5750 1775 50  0001 C CNN
F 1 "+5V" H 5800 2075 50  0000 C CNN
F 2 "" H 5750 1925 50  0001 C CNN
F 3 "" H 5750 1925 50  0001 C CNN
	1    5750 1925
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R102
U 1 1 5E98BDE8
P 5750 2400
F 0 "R102" H 5682 2354 50  0000 R CNN
F 1 "10K" H 5682 2445 50  0000 R CNN
F 2 "" H 5750 2400 50  0001 C CNN
F 3 "~" H 5750 2400 50  0001 C CNN
	1    5750 2400
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R103
U 1 1 5E9ACD4E
P 6050 2400
F 0 "R103" H 5982 2354 50  0000 R CNN
F 1 "10K" H 5982 2445 50  0000 R CNN
F 2 "" H 6050 2400 50  0001 C CNN
F 3 "~" H 6050 2400 50  0001 C CNN
	1    6050 2400
	-1   0    0    1   
$EndComp
Wire Wire Line
	5750 1925 5750 2200
Wire Wire Line
	5750 2200 6050 2200
Wire Wire Line
	6050 2200 6050 2300
Connection ~ 5750 2200
Wire Wire Line
	5750 2200 5750 2300
$Comp
L Device:R_Small_US R106
U 1 1 5EA0D211
P 7700 4575
F 0 "R106" H 7632 4529 50  0000 R CNN
F 1 "10K" H 7632 4620 50  0000 R CNN
F 2 "" H 7700 4575 50  0001 C CNN
F 3 "~" H 7700 4575 50  0001 C CNN
	1    7700 4575
	-1   0    0    1   
$EndComp
Wire Wire Line
	8000 4325 7700 4325
Connection ~ 8000 4325
Wire Wire Line
	6800 5025 8250 5025
Wire Wire Line
	7700 4325 7700 4475
Wire Wire Line
	7700 4675 7700 4925
Connection ~ 7700 4925
Wire Wire Line
	7700 4925 7500 4925
$Comp
L Switch:SW_SPST SW101
U 1 1 5E80751E
P 1700 5700
F 0 "SW101" H 1700 5935 50  0000 C CNN
F 1 "PWR_SW" H 1700 5844 50  0000 C CNN
F 2 "" H 1700 5700 50  0001 C CNN
F 3 "~" H 1700 5700 50  0001 C CNN
	1    1700 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 5700 1500 6175
Wire Wire Line
	1500 6175 1450 6175
Wire Wire Line
	1900 5700 1900 6050
Wire Wire Line
	1900 6050 2150 6050
Wire Wire Line
	1175 6375 1500 6375
Wire Wire Line
	1500 6375 1500 6550
Wire Wire Line
	1500 6550 2150 6550
$Comp
L power:GND #PWR0110
U 1 1 5E833262
P 3100 1975
F 0 "#PWR0110" H 3100 1725 50  0001 C CNN
F 1 "GND" H 3105 1802 50  0000 C CNN
F 2 "" H 3100 1975 50  0001 C CNN
F 3 "" H 3100 1975 50  0001 C CNN
	1    3100 1975
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW102
U 1 1 5E8335F9
P 3500 1700
F 0 "SW102" H 3500 1935 50  0000 C CNN
F 1 "INT_LIMIT" H 3500 1844 50  0000 C CNN
F 2 "" H 3500 1700 50  0001 C CNN
F 3 "~" H 3500 1700 50  0001 C CNN
	1    3500 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 1700 3100 1975
$Sheet
S 9125 1050 1000 900 
U 5E86E19B
F0 "Power Loss Latch" 50
F1 "nano_pwr_loss_alarm.sch" 50
F2 "VDD" I L 9125 1450 50 
F3 "GND" I L 9125 1600 50 
F4 "_TRIGGER" I L 9125 1300 50 
F5 "ALARM+" O R 10125 1250 50 
F6 "ALARM-" O R 10125 1550 50 
$EndSheet
Text Label 8675 1450 0    50   ~ 0
VPWR_8V
Wire Wire Line
	8675 1450 9125 1450
Text Label 8650 1600 0    50   ~ 0
VNANO_GND
Wire Wire Line
	8650 1600 9125 1600
$Comp
L Device:Speaker LS101
U 1 1 5E8E91AF
P 10700 1325
F 0 "LS101" H 10870 1321 50  0000 L CNN
F 1 "ALARM" H 10870 1230 50  0000 L CNN
F 2 "" H 10700 1125 50  0001 C CNN
F 3 "~" H 10690 1275 50  0001 C CNN
	1    10700 1325
	1    0    0    -1  
$EndComp
Wire Wire Line
	10125 1250 10500 1250
Wire Wire Line
	10500 1250 10500 1325
Wire Wire Line
	10125 1550 10500 1550
Wire Wire Line
	10500 1550 10500 1425
Wire Wire Line
	4775 750  4775 850 
Wire Wire Line
	8650 1300 9125 1300
Text Label 8650 1300 0    55   ~ 0
VNANO_5V
Wire Notes Line
	8275 475  8275 2250
Wire Notes Line
	8275 2250 11225 2250
Text Notes 9100 750  2    157  ~ 31
Alarm
Wire Wire Line
	3700 1700 3950 1700
Wire Wire Line
	3100 1700 3300 1700
Wire Wire Line
	3950 2350 4075 2350
Wire Wire Line
	3950 1700 3950 2350
Wire Wire Line
	5650 1300 5650 3350
Wire Wire Line
	4475 1200 6100 1200
Wire Wire Line
	7225 1500 7625 1500
Wire Wire Line
	7625 1500 7625 1525
Wire Wire Line
	7225 1825 7625 1825
Wire Wire Line
	7625 1825 7625 1725
Connection ~ 7225 1825
Wire Wire Line
	7225 1825 7225 2000
$Sheet
S 9000 2625 1225 650 
U 5E885EB6
F0 "Analog User Controls" 55
F1 "AnalogUserControls.sch" 55
F2 "VIN" I L 9000 2700 55 
F3 "BUTTONS" O R 10225 2775 55 
F4 "PEEP" O R 10225 2925 55 
F5 "TIDAL_VOL" O R 10225 3075 55 
F6 "RESP_RATE" O R 10225 3200 55 
F7 "GND" B L 9000 3175 55 
$EndSheet
Wire Wire Line
	5075 3450 5475 3450
Text Label 5500 2950 2    55   ~ 0
BUTTONS
Wire Wire Line
	5075 2950 5500 2950
Text Label 5500 3050 2    55   ~ 0
PEEP_CTL
Text Label 5475 3350 2    55   ~ 0
RR_CTL
Wire Wire Line
	5075 3050 5500 3050
Text Label 5475 3450 2    55   ~ 0
TV_CTL
Wire Wire Line
	5075 3350 5475 3350
Text Label 10575 3075 2    55   ~ 0
TV_CTL
Text Label 10675 2775 2    55   ~ 0
BUTTONS
Text Label 10700 2925 2    55   ~ 0
PEEP_CTL
Text Label 10600 3200 2    55   ~ 0
RR_CTL
Wire Wire Line
	10600 3200 10225 3200
Wire Wire Line
	10575 3075 10225 3075
Wire Wire Line
	10700 2925 10225 2925
Wire Wire Line
	10675 2775 10225 2775
Wire Wire Line
	4775 850  7950 850 
Wire Wire Line
	7950 850  7950 2700
Wire Wire Line
	7950 2700 9000 2700
Connection ~ 4775 850 
Wire Wire Line
	4775 850  4775 1075
Wire Wire Line
	5650 3675 8675 3675
Wire Wire Line
	8675 3675 8675 3175
Wire Wire Line
	8675 3175 9000 3175
Connection ~ 5650 3675
Wire Wire Line
	5650 3675 5650 3900
Wire Wire Line
	5075 3150 5750 3150
Wire Wire Line
	5750 2500 5750 3150
Connection ~ 5750 3150
Wire Wire Line
	5750 3150 6275 3150
Wire Wire Line
	6050 2500 6050 3250
Wire Wire Line
	5075 3250 6050 3250
Connection ~ 6050 3250
Wire Wire Line
	6050 3250 6375 3250
Wire Wire Line
	7225 1500 5425 1500
Wire Wire Line
	5425 1500 5425 2850
$Comp
L Connector_Generic:Conn_01x04 J113
U 1 1 5EA301ED
P 6825 2350
F 0 "J113" H 6905 2342 50  0000 L CNN
F 1 "LCD_PCF8574" H 6905 2251 50  0000 L CNN
F 2 "" H 6825 2350 50  0001 C CNN
F 3 "~" H 6825 2350 50  0001 C CNN
	1    6825 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6625 2450 6275 2450
Wire Wire Line
	6275 2450 6275 3150
Connection ~ 6275 3150
Wire Wire Line
	6275 3150 6500 3150
Wire Wire Line
	6625 2550 6375 2550
Wire Wire Line
	6375 2550 6375 3250
Connection ~ 6375 3250
Wire Wire Line
	6375 3250 6650 3250
Wire Wire Line
	7225 2000 6525 2000
Wire Wire Line
	6525 2000 6525 2250
Wire Wire Line
	6525 2250 6625 2250
Connection ~ 7225 2000
Wire Wire Line
	7225 2000 7225 2150
Wire Wire Line
	6375 2200 6375 2350
Wire Wire Line
	6375 2350 6625 2350
Wire Notes Line
	2000 2625 2825 2625
Wire Notes Line
	2825 2625 2825 1725
Wire Notes Line
	2825 1725 2000 1725
Wire Notes Line
	2000 1725 2000 2625
Text Notes 2700 1675 2    55   ~ 11
**Replacable by\nLCD PCF8574 \non I2C Pins\nA4/A5
Text Notes 11000 675  2    55   ~ 11
*Not necessary for controller prototyping
Connection ~ 6050 2200
Wire Wire Line
	6050 2200 6375 2200
Wire Wire Line
	2875 3900 4000 3900
Wire Wire Line
	4075 3050 3850 3050
Wire Wire Line
	3850 3050 3850 3125
Wire Wire Line
	1000 3125 3850 3125
$EndSCHEMATC
