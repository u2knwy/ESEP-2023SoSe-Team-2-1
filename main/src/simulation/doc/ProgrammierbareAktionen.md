# Programmierbare Aktionen #
Die Simulation der Festo-Transferanlage kann zeitgesteuert vorprogrammierte Aktionen durchführen, wie Werkstücke einlegen oder Tasten drücken. Für die Aktionen sind zwei Subsysteme zuständig, eins für Werkstück bezogene Aktionen und eins für Nutzeraktionen an den Tasten. Die Aktionen werden dabei durch spezielle Objekte (Command-Pattern) definiert, die beim Start der Simulation erzeugt und an das jeweilige Subsystem übergeben werden.

## Zeitsteuerung ##
Der Zeitpunkt jeder Aktion wird mit vorgegeben und wird in ganzen Millisekunden \[ms\] nach dem Simulationsstart angegeben. Die jeweiligen Aktionen werden dann in der nächsten Zeitscheibe ausgeführt, wenn die simulierte Zeit den Zeitpunkt genau erreicht oder überschritten hat. Somit kann ein Jitter von maximal einer Zeitscheibe beim Ausführen auftreten.

## Werkstück-Aktionen ##

### Semantik der Aktionsobjekte ###
Über Objekte der Klasse `SimItemHandlingActionKind` können werkstückbezogene Aktionen zeitlich vorab eingeplant werden. Dazu gehört das Hinzufügen von einzelnen Werkstücken sowie das Entfernen von Werkstücken. Beim Erzeugen des Objektes muss entschieden werden, ob ein neues Werkstück eines bestimmten Typs (definiert durch die Enum-Class `ItemKinds`) eingelegt wird oder in welchem Bereich Werkstücke entfernt werden (definiert durch die Enum-Class `SimItemHandlingActionKind`).

Derzeit können alle Typen von Werkstücken eingelegt werden. Die Einlegeposition ist fix (x=0, y=60). Das Koordinatensystem verläuft mit der x-Achse in Richtung des Bandes und beginnt mit x=0 links. Die y-Achse ist in Richtung Rutsche orientiert und beginnt mit y=0 an der von der Rutsche abgewandtem Führungsschiene des Bandes. Die Koordinate (x=0, y=60) bedeutet, dass das Werkstück am Anfang des Bandes liegt, so dass die Lichtschranke unterbrochen wird und das Werkstück die blauen Leitbleche beim Durchlauf nicht berührt. Neben der Art des Werkstücks kann für Testzwecke das Werkstück sich bei der Übergabe an das nächste System überschlagen (flip=true) oder bei der Weiche nur schwer rutschen (sticky=true).

Als Aktionen zum Entfernen der Werkstücke sind derzeit die Aktionen `removeall`, `removeslide`, `removeatend` (630<x<700) und `removeid` implementiert. Alle sich in der Anlage bzw. in den Bereichen befindlichen Werkstücke werden entfernt.

### Im Code ###
Aktionsobjekte mit Werkstück-Aktionen werden derzeit im Konstruktor der Klasse `SimulationStarterQNX` erzeugt und dort an das `handler`-Objekt übergeben.

## Tasten-Aktionen ##

### Semantik der Aktionsobjekte ###
Über Tastenaktionen (Objekte der Klasse `SimHCIAction`) können Tasten gedrückt und gehalten werden. Die Semantik ist die, dass ab dem angegebenen Zeitpunkt die durch das Objekt ausgewählten Tasten gedrückt sind. Somit muss zum Loslassen ein Objekt mit `releaseAll`  einplanen.

Ein Aktionsobjekt der Klasse `SimHCIAction` muss immer mit der Zeit der Aktion erzeugt werden. Soll nur ein Knopf ab dem Zeitpunkt gedrückt sein, so kann die Aktion direkt im Konstruktor mit einem gedrückten Knopf erzeugt werden. Sollen weitere Knöpfe zu diesem Zeitpunkt gedrückt sein, so müssen diese über Methodenaufrufe zusätzlich bei dem Objekt aktiviert werden. 

Beispiel für zwei Tasten:

```
SimHCIAction action(2000);
action.pressStart();
action.pressReset();
```

Das Objekt `action` repräsentiert das gleichzeitige Drücken (und halten) ab der Simulationszeit 2 s.	

### Im Code ###
Aktionsobjekte mit Tasten-Aktionen werden derzeit im Konstruktor der Klasse `SimulationStarterQNX` erzeugt und dort an das `hci`-Objekt übergeben.


