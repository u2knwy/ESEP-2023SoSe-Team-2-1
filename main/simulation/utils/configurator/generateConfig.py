#! python
"""
Config generator for Festo Transfer simulation.

2023 (c) Niklas Kück

"""
import pygame
from pygame.locals import *
import os

VERSION = 1

def captionWrite(font, background, screen):
    pygame.display.set_caption('Festo Simulation Config Generator')
    text = font.render('Festo Simulation Config Generator', True, (10, 10, 10))
    textpos = text.get_rect()
    textpos.centerx = background.get_rect().centerx
    screen.blit(text, textpos)

def generateFiles(festoA, festoB, festoPC, legoActive, manualControlActive):
    visuStrA = '{'
    visuStrA += '"targetip": "' + festoA[0].selectSpecific() + '", '
    visuStrA += '"targetport": "' + festoB[2].selectSpecific()  + '", '
    visuStrA += '"receiveport": "' + festoPC[1].selectSpecific() + '", '
    visuStrA += '"lego": ' + ('true' if legoActive else 'false') + '", '
    visuStrA += '"manualcontrol": ' + ('true' if manualControlActive else 'false') + "}"
    # In der Main werden die Verzeichnisse 'irgendwo' erzeugt, somit gleichen Pfad verwenden.
    # Ansonsten ergibt sich bei __file__ == '' der Zugriff auf root! Oder
    # abspath() verwenden.
    with open("A/config.json", "w") as f:
        f.write(visuStrA)
    #f = open(os.path.dirname(__file__) + "/A/config.json", "w")
    #f.write(visuStrA)
    #f.close()

    visuStrB = "{\"targetip\": \""+ festoB[0].selectSpecific()
    visuStrB += "\", \"targetport\": " + festoA[2].selectSpecific()
    visuStrB += ", \"receiveport\": " + festoPC[2].selectSpecific()
    visuStrB += ", \"lego\": " + ('true' if legoActive else 'false')
    visuStrB += ", \"manualcontrol\": " + ('true' if manualControlActive else 'false') + "}"
    
    with open("B/config.json", "w") as f:
        f.write(visuStrB)
    #f = open(os.path.dirname(__file__) + "/B/config.json", "w")
    #f.write(visuStrB)
    #f.close()
    
    simStrA = festoB[0].selectSpecific() + "\t\t// System-B IP\n"
    simStrA += festoB[1].selectSpecific() + "\t\t\t// System-B receive port\n"
    simStrA += festoA[0].selectSpecific() + "\t\t// Local IP\n"
    simStrA += festoA[1].selectSpecific() + "\t\t\t// Local receive port\n"
    simStrA += festoPC[0].selectSpecific() + "\t\t// Reporting Host IP\n"
    simStrA += festoPC[1].selectSpecific() + "\t\t\t// Reporting port\n<EOF>\n"
    with open("A/simudp.conf", "w") as f:
        f.write(simStrA)
    #f = open(os.path.dirname(__file__) + "/A/simudp.conf", "w")
    #f.write(simStrA)
    #f.close()

    simStrB = festoA[0].selectSpecific() + "\t\t// System-B IP\n"
    simStrB += festoA[1].selectSpecific() + "\t\t\t// System-B receive port\n"
    simStrB += festoB[0].selectSpecific() + "\t\t// Local IP\n"
    simStrB += festoB[1].selectSpecific() + "\t\t\t// Local receive port\n"
    simStrB += festoPC[0].selectSpecific() + "\t\t// Reporting Host IP\n"
    simStrB += festoPC[2].selectSpecific() + "\t\t\t// Reporting port\n<EOF>\n"
    with open("B/simudp.conf", "w") as f:
        f.write(simStrB)
    #f = open(os.path.dirname(__file__) + "/B/simudp.conf", "w")
    #f.write(simStrB)
    #f.close()

class box():
    def __init__(self, boxconfig):
        super().__init__()
        self.activeField = False
        self.text = ''
        self.frame = boxconfig[3]
        self.input_Box = boxconfig
        self.font = pygame.font.Font(None, 18)
        self.input_inactive = (100, 100, 130)
        self.input_active = (100, 100, 250)
        self.textcolor = (10,10,10)

    def createTextBox(self, screen):
        color = self.input_inactive
        if self.activeField:
            color = self.input_active
        if len(self.text) == 0:
           text_surface = self.font.render(self.input_Box[1], True, color)
        else:
            text_surface = self.font.render(self.text, True, color)
        screen.blit(text_surface, (self.input_Box[0].x+5, self.input_Box[0].y+6))
        if self.frame:
            pygame.draw.rect(screen, color, self.input_Box[0], 2)

    def drawText(self, screen):
        text_surface = self.font.render(self.input_Box[2][1], True, self.textcolor)
        screen.blit(text_surface, (self.input_Box[2][0].x, self.input_Box[2][0].y))

    def draw(self, surface):
        self.createTextBox(surface)
        self.drawText(surface)

    def changeConfig(self, activeInput):
        if not activeInput:
            self.activeField = True
            return True
        elif self.activeField:
            self.activeField = False
            return False
        else:
            return activeInput

    def getText(self, event, text):
        if self.activeField:
            if event.key == pygame.K_RETURN:
                pass
            elif event.key == pygame.K_BACKSPACE:
                text = text[:-1]
            else:
                text += event.unicode
        return text

    def ipCheck(self, event):
        self.text = self.getText(event, self.text)
        if len(self.text) > 15:
            self.text = self.text[0:15]

    def portCheck(self, event):
        self.text = self.getText(event, self.text)
        if len(self.text) > 5:
            self.text = self.text[0:5]
        
    def isEqual(self, toCheck):
        if not self.activeField and not toCheck.activeField and self.text == toCheck.text:
            self.text = ''

    def selectSpecific(self):
        return self.text if len(self.text) > 0 else self.input_Box[1]

def main():
    pygame.init()

    screen = pygame.display.set_mode((550, 250))

    background = pygame.Surface(screen.get_size())
    background = background.convert()
    background.fill((250, 250, 250))

    active = True

    clock = pygame.time.Clock()

    font = pygame.font.Font(None, 18)

    activeInput = False

    festoAIp = box((pygame.Rect(100, 50, 105, 24), '192.168.224.128', (pygame.Rect(10, 55, 30, 24), 'IP-Festo A:'), True))
    festoAPort1 = box((pygame.Rect(310, 50, 50, 24), '41010', (pygame.Rect(210, 55, 60, 24), 'Port Receive:'), True))
    festoAPort2 = box((pygame.Rect(470, 50, 50, 24), '51010', (pygame.Rect(370, 55, 60, 24), 'Port Send:'), False))
    festoA = (festoAIp, festoAPort1, festoAPort2)

    festoBIp = box((pygame.Rect(100, 90, 105, 24), '192.168.224.129', (pygame.Rect(10, 95, 30, 24), 'IP-Festo B:'), True))
    festoBPort1 = box((pygame.Rect(310, 90, 50, 24), '51010', (pygame.Rect(210, 95, 60, 24), 'Port Receive:'), True))
    festoBPort2 = box((pygame.Rect(470, 90, 50, 24), '41010', (pygame.Rect(370, 95, 60, 24), 'Port Send:'), False))
    festoB = (festoBIp, festoBPort1, festoBPort2)

    festoPCIp = box((pygame.Rect(100, 125, 105, 24), '192.168.224.1', (pygame.Rect(10, 130, 30, 24), 'IP-PC:'), True))
    festoPCPort1 = box((pygame.Rect(310, 125, 50, 24), '60000', (pygame.Rect(210, 130, 60, 24), 'Port Receive A:'), True))
    festoPCPort2 = box((pygame.Rect(470, 125, 50, 24), '50000', (pygame.Rect(370, 130, 60, 24), 'Port Receive B:'), True))
    festoPC = (festoPCIp, festoPCPort1, festoPCPort2)

    generateBox = box((pygame.Rect(210, 220, 60, 24), 'Generate', (0,0), True))
    legoBox = box((pygame.Rect(10, 170, 115, 24), 'Lego Visualisation', (0, 0), True))
    manualControlBox = box((pygame.Rect(130, 170, 115, 24), 'Manual Control', (0, 0), True))

    while active:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                active = False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if festoAIp.input_Box[0].collidepoint(event.pos):
                    activeInput = festoAIp.changeConfig(activeInput)
                elif festoBIp.input_Box[0].collidepoint(event.pos):
                    activeInput = festoBIp.changeConfig(activeInput)
                elif festoAPort1.input_Box[0].collidepoint(event.pos):
                    activeInput = festoAPort1.changeConfig(activeInput)
                elif festoBPort1.input_Box[0].collidepoint(event.pos):
                    activeInput = festoBPort1.changeConfig(activeInput)
                elif festoPCIp.input_Box[0].collidepoint(event.pos):
                    activeInput = festoPCIp.changeConfig(activeInput)
                elif festoPCPort1.input_Box[0].collidepoint(event.pos):
                    activeInput = festoPCPort1.changeConfig(activeInput)
                elif festoPCPort2.input_Box[0].collidepoint(event.pos):
                    activeInput = festoPCPort2.changeConfig(activeInput)
                elif legoBox.input_Box[0].collidepoint(event.pos):
                    legoBox.activeField = not legoBox.activeField
                elif manualControlBox.input_Box[0].collidepoint(event.pos):
                    manualControlBox.activeField = not manualControlBox.activeField
                elif generateBox.input_Box[0].collidepoint(event.pos):
                    generateFiles(festoA, festoB, festoPC, legoBox.activeField, manualControlBox.activeField)
            #elif event.type == pygame.KEYDOWN and pygame.K_ESCAPE:
                #active = False
            elif event.type == pygame.KEYDOWN:
                festoAIp.ipCheck(event)
                festoBIp.ipCheck(event)
                festoPCIp.ipCheck(event)
                festoAPort1.portCheck(event)
                festoBPort1.portCheck(event)
                festoPCPort1.portCheck(event)
                festoPCPort2.portCheck(event)
        
        #logic
        festoAIp.isEqual(festoBIp)
        festoBIp.isEqual(festoAIp)
        festoAIp.isEqual(festoPCIp)
        festoPCIp.isEqual(festoAIp)
        festoBIp.isEqual(festoPCIp)
        festoPCIp.isEqual(festoBIp)
        festoAPort1.isEqual(festoBPort1)
        festoBPort1.isEqual(festoAPort1)
        festoPCPort1.isEqual(festoPCPort2)
        festoPCPort2.isEqual(festoPCPort1)
        festoAPort1.isEqual(festoPCPort1)
        festoPCPort1.isEqual(festoAPort1)
        festoBPort1.isEqual(festoPCPort1)
        festoPCPort1.isEqual(festoBPort1)
        festoAPort1.isEqual(festoPCPort2)
        festoPCPort2.isEqual(festoAPort1)
        festoBPort1.isEqual(festoPCPort2)
        festoPCPort2.isEqual(festoBPort1)

        festoAPort2.text = festoBPort1.text
        festoBPort2.text = festoAPort1.text

        #delete
        screen.fill((255, 255, 255))

        #write
        captionWrite(font, background,screen)
        festoA[0].draw(screen)
        festoA[1].draw(screen)
        festoA[2].draw(screen)

        festoB[0].draw(screen)
        festoB[1].draw(screen)
        festoB[2].draw(screen)

        festoPC[0].draw(screen)
        festoPC[1].draw(screen)
        festoPC[2].draw(screen)

        legoBox.createTextBox(screen)
        manualControlBox.createTextBox(screen)
        generateBox.createTextBox(screen)

        #refresh
        pygame.display.flip()

        #refreshtime
        clock.tick(30)

    pygame.quit()

if __name__ == '__main__': 
    os.makedirs('A', exist_ok=True)
    os.makedirs('B', exist_ok=True)
    #if not os.path.exists('A'):
    #    os.makedirs('A')
    #if not os.path.exists('B'):
    #    os.makedirs('B')

    print("Version:", VERSION)

    main()