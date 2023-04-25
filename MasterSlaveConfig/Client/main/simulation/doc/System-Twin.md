# System-Twin #
Die Simulation des Festo-Transfersystems kann dazu verwendet werden, wie in der Realität, dass mehrere Anlagen hintereinander stehen. Hier wird beschrieben, wie man ein System aus zwei Anlagen, A und B, konfiguriert. 

Das System B steht dabei hinter dem System A und übernimmt somit die Werkstücke von A.

## Im Momentics-Projekt ##
Die beiden Simulationen kommunizieren mittels UDP. Es sind die Sources aus dem Ordner `simudp` mit zu compilieren. 

(Wird nur eine Instanz der Simulation betrieben, so können die Sources entfernt werden.)

## Im Make-File ##
Damit die Kommunikation zwischen den beiden Simulationen aufgebaut wird, muss im Make-File global das Macro `SIM\_TWIN` gesetzt sein, sowie die Socket-Lib mit eingebunden werden.

```
CC = qcc -Vgcc_nto$(PLATFORM)
CXX = qcc -lang-c++ -Vgcc_nto$(PLATFORM) -DSIM_TWIN 
LD = $(CXX)

#...

#LIBS += -L../mylib/$(OUTPUT_DIR) -lmylib
LIBS += -lsocket
```

Der Code von System B unterscheidet sich von A. Deshalb muss für die Simulation von System B zusätzlich das Macro `SIM\_TWIN\_B` gesetzt sein.

```
CC = qcc -Vgcc_nto$(PLATFORM)
CXX = qcc -lang-c++ -Vgcc_nto$(PLATFORM) -DSIM_TWIN -DSIM_TWIN_B
LD = $(CXX)
```

## Im Target-System ##
Die Simulationen synchronisieren sich über UDP. Dazu müssen entsprechend IP-Adressen und Portnummern der Systeme konfiguriert werden. Die Konfiguration ist eine Textdatei mit dem Namen `simudp.conf` und muss im Root-Ordner liegen.

Die folgenden Konfigurationsdateien gehen davon aus, dass 

- System A: IP=192.168.101.101
- System B: IP=192.168.101.111

haben. Die Port-Nummern können relativ frei gewählt werden, müssen nur beim Sender und Empfänger übereinstimmen.

![Übersicht Simconf](simudp_conf2.png)

System A `/simudp.conf`:

```
192.168.101.111    // System B-IP
51119
192.168.101.101    // Own-IP
41019

```

System B `/simudp.conf`:

```
192.168.101.101    // System A-IP
41019
192.168.101.111    // Own-IP
51119

```
Die letzte Zeile muss ein Newline haben!

(Die geübte Leser*in wird erkennen, dass die Simulationen sich gegenseitig referenzieren und somit Werkstücke im Kreis fahren könnten.)

## Start der Systeme ##
Die Simulation (das Programm) auf System B muss vor der Simulation auf System A gestartet werden, da nur Simulation A ein Startsignal an B sendet und damit die interne Simulationszeit synchronisert.

## Vordefinierte Aktionen ##
System A und System B werden unterschiedliche vordefinierte Aktionen durchführen. Entsprechend sind diese in den durch Macros ausgewählten Abschnitte im Source-Code (`simstarterqnx.cpp`) zu hinterlegen. Die Zeit wird beim Start synchronisiert.


