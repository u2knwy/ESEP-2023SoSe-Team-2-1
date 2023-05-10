# ESEP

ESEP Gruppe 2 SS23

## Projektstruktur

Das Repository ist unterteilt in folgende Ordner:

- [helloworld](/helloworld/): Momentics-Projekt zum ausführen von Test-Code
- [main](/main/): Momentics-Projekt, das den Code zum ausführen der entwickelten Software auf dem Beaglebone enthält
- [tests](/tests/): Momentics-Projekt, das Unit- und Modultests ausführt. Die Quelldateien aus dem `main`-Projekt sind durch das Makefile inkludiert.
- [docs](/docs/): Enthält die Dokumentation des Projektes (RDT, Code-Doku, Diagramme etc...)

## Benutzung

Das Programm kann wie folgt gestartet werden:

```shell
sorting [mode] [-p,--pusher] [-s,--sensors] [-a,--actuators] [-m,--measure]
```

Der positionale Parameter `mode` ist dabei obligatorisch und muss einer der folgenden Werte haben:

- `master`: Starten des Programms im "Master" Betrieb
- `slave`: Starten des Programms im "Slave" Betrieb
- `tests`: Ausführen der Tests mit GoogleTest Suite
- `demo`: Starten des "Demo-Mode"

Die optionalen Parameter haben folgende Bedeutung:

- `-p,--pusher`: An der Hardware ist ein Auswerfer anstatt einer Weiche montiert. Wenn nicht angegeben, wird angenommen dass eine Weiche montiert ist.

Folgende Parameter werden nur im Demo-Modus verwendet und werden sonst ignoriert. Standardmäßig sind sie auf false.

- `-s,--sensors`: Ausführen der Sensor Demo-Funktion
- `-a,--actuators`: Ausführen der Aktor Demo-Funktion
- `-m,--measure`: Ausführen der Höhensensor Demo-Funktion
