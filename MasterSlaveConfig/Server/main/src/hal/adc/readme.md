# ADC

## Hardware

Der BeagleboneBlack besitzt einen Analog-Digital-Konverter mit 8 Kanälen und 12 bit Auflösung. Davon sind die Kanäle 1 bis 7 (AN0-AN6) über die Pinheader nach außen geführt. Der ADC ist Bestandteil des Touchscreen Controller.
**Der ADC verträgt eine maximale Spannung von 1.8 V!**
Auf der Adapterplatine für die Festo-Anlage sind daher Schutzdioden vorhanden, die die Spannung pro Kanal auf diese 1.8V begrenzen. Um höhere Spannungsbereiche messen zu können, sind für jeden Kanal Spannungsteiler vorgesehen. 
Der Analog-Ausgang des Höhenmessers der Festo-Anlage ist auf Kanal 1 mittels Spannungsteiler angeschlossen. 
![Spannungsteiler für den Höhensensor](7_ADC/SchaltungADC.PNG)
Der Spannungsteiler sorgt dafür, dass die max. 10V des Höhensensors auf maximal 1.8V begrenzt werden.

$`U_{ADC} = \frac{U_{Höhensensor}\cdot R_{25}}{R_{25}+R_{26}}`$

