#! python
"""
Visualisation for the Festo Transfer simulation.

2020-2023 (c) Thomas Lehmann

Contributors:
2021 Summer term: David Berschauer
     Type A/B-Support
     Injection of Work Pieces
     Clear Buttons
     Flip/Sticky Buttons
2022 Winter term: Niklas Kück
    Injection and Decoration of Binary Workpieces and Lego
    Improved config path for .bat or .sh script support
"""

import argparse
import json
import os
import sys
import pygame
from pygame.locals import *
import simulationreportreceiver
import simulationcommandinterface

VERSION = 13

from dataclasses import dataclass

@dataclass
class Arguments:
    target_IP: str = '127.0.0.1'
    target_port: int = 40000
    receive_port: int = 6001
    configfilename: str = os.path.abspath(os.path.dirname(__file__))+'/config.json'
    support_lego: bool = False

@dataclass
class SystemType:
    type_ID: str = 'u'   # u - unknown, A, B, C - Client

class Transform:
    """
    Class transforms Festo System coordinates to cooardinates on screen.
    """
    def __init__(self, base, scale=1):
        self.base = base    # x, y coordinates on screen for 0,0 of festo coordinate system
        self.scale_factor = scale
    def map(self, x, y):
        x_mapped = int(x + self.base[0])
        y_mapped = int(self.base[1] - y)
        return x_mapped, y_mapped
    def scale(self, r):
        return r*self.scale_factor

class CurrentSimTime(Transform):
    def __init__(self, fts, base):
        super().__init__(base)
        self.fts = fts
        self.font = pygame.font.Font(None, 18)
    def draw(self, surface):
        # Display some text
        current_sim_time = self.fts.get_current_simulation_time()
        text = self.font.render("Time: "+str(current_sim_time/1000), True, (10, 10, 10))
        surface.blit(text, (self.base[0], self.base[1]))

class Items(Transform):
    def __init__(self, fts, base, commandif=None):
        super().__init__(base)
        self.fts = fts
        self.ci = commandif
        self.itemBoxes = []
    def draw(self, surface):
        items = self.fts.get_items()
        #print(items)
        self.itemBoxes.clear()
        for _, item in items.items():
            x = item['x']
            y = item['y']
            kind = item['T']
            id = item['ID']
            pos = self.map(x, y)
            if kind[0] == 'l':  # Lego
                h = int(self.scale(32))
                w = int(self.scale(64))
                r = int(self.scale(20))
                rect_out = pygame.Rect(pos[0]-w/2, pos[1]-h/2, w, h)
                self.itemBoxes.append((id, rect_out))
                rect_out2 = pygame.Rect(pos[0]-w/2, pos[1]-h/2, w-16, h)
                rect_out3 = pygame.Rect(pos[0]-w/2, pos[1]-h/2, w-32, h)
                rect_out4 = pygame.Rect(pos[0]-w/2, pos[1]-h/2, w-48, h)
                pygame.draw.rect(surface, (100, 100, 100), rect_out, 1)
                if x > 260:
                    if kind == 'l1':
                        pygame.draw.rect(surface, (191, 191, 41), rect_out, 0)
                        pygame.draw.rect(surface, (100, 100, 0), rect_out2, 0)
                        pygame.draw.rect(surface, (191, 191, 41), rect_out3, 0)
                        pygame.draw.rect(surface, (235, 235, 0), rect_out4, 0)
                    elif kind == 'l2':
                        pygame.draw.rect(surface, (41, 191, 41), rect_out, 0)
                        pygame.draw.rect(surface, (0, 100, 0), rect_out2, 0)
                        pygame.draw.rect(surface, (0, 235, 0), rect_out3, 0)
                        pygame.draw.rect(surface, (41, 191, 41), rect_out4, 0)
                    elif kind == 'l3':
                        pygame.draw.rect(surface, (41, 191, 191), rect_out, 0)
                        pygame.draw.rect(surface, (0, 100, 100), rect_out2, 0)
                        pygame.draw.rect(surface, (41, 191, 191), rect_out3, 0)
                        pygame.draw.rect(surface, (41, 191, 191), rect_out4, 0)
                else:
                    pass
            else:
                r = int(self.scale(20))
                # create frame for filled circle
                rect_out = pygame.Rect(pos[0]-r, pos[1]-r, 2*r, 2*r)
                self.itemBoxes.append((id, rect_out))
                rect_in = pygame.Rect(pos[0]-r/2, pos[1]-r/2, r, r)
                pygame.draw.circle(surface, (100, 100, 100), pos, r, 1)
                if x > 260:
                    # draw depending on type
                    if kind == 'f':
                        pygame.draw.ellipse(surface, (0, 0, 200), rect_out, 0)
                    elif kind == 'HD':  # Hole down
                        pygame.draw.ellipse(surface, (180, 0, 0), rect_out, 0)
                    elif kind == 'HU':  # Hole up
                        pygame.draw.ellipse(surface, (180, 0, 0), rect_out, 0)
                        pygame.draw.ellipse(surface, (100, 100, 100), rect_in, 0)
                    elif kind == 'MD':  # Metal down
                        pygame.draw.ellipse(surface, (220, 220, 220), rect_out, 0)
                    elif kind == 'MU':  # Metal up
                        pygame.draw.ellipse(surface, (220, 220, 220), rect_out, 0)
                        if x > 390:
                            pygame.draw.ellipse(surface, (100, 100, 100), rect_in, 0)
                    elif kind[0] == 'c':  # Coded
                        pygame.draw.ellipse(surface, (100, 100, 100), rect_out, 0)
                        rect_1 = pygame.Rect(pos[0]-r/1.2, pos[1]-r/1.2, r/0.6, r/0.6)
                        rect_2 = pygame.Rect(pos[0]-r/1.5, pos[1]-r/1.5, r/0.75, r/0.75)
                        rect_3 = pygame.Rect(pos[0]-r/2.1, pos[1]-r/2.1, r/1.05, r/1.05)
                        rect_4 = pygame.Rect(pos[0]-r/2.8, pos[1]-r/2.8, r/1.4, r/1.4)
                        rect_5 = pygame.Rect(pos[0]-r/4.8, pos[1]-r/4.8, r/2.4, r/2.4)
                        color_ring_1 = (200, 200, 200)
                        color_ring_3 = (200, 200, 200)
                        color_ring_5 = (200, 200, 200)
                        if kind == 'cA':  # Coded 000
                            pass
                        elif kind == 'cB':  # Coded 001
                            color_ring_5 = (40, 40, 40)
                        elif kind == 'cC':  # Coded 010
                            color_ring_3 = (40, 40, 40)
                        elif kind == 'cD':  # Coded 011
                            color_ring_3 = (40, 40, 40)
                            color_ring_5 = (40, 40, 40)
                        elif kind == 'cE':  # Coded 100
                            color_ring_1 = (40, 40, 40)
                        elif kind == 'cF':  # Coded 101
                            color_ring_1 = (40, 40, 40)
                            color_ring_5 = (40, 40, 40)
                        elif kind == 'cG':  # Coded 110
                            color_ring_1 = (40, 40, 40)
                            color_ring_3 = (40, 40, 40)
                        elif kind == 'cH':  # Coded 111
                            color_ring_1 = (40, 40, 40)
                            color_ring_3 = (40, 40, 40)
                            color_ring_5 = (40, 40, 40)
                        pygame.draw.ellipse(surface, color_ring_1, rect_1, 0)
                        pygame.draw.ellipse(surface, (100, 100, 100), rect_2, 0)
                        pygame.draw.ellipse(surface, color_ring_3, rect_3, 0)
                        pygame.draw.ellipse(surface, (100, 100, 100), rect_4, 0)
                        pygame.draw.ellipse(surface, color_ring_5, rect_5, 0)
                    else:
                        pass
    def mousebuttondown(self, pos):
        if self.ci is not None :
            for id, itembox in self.itemBoxes:
                if itembox.collidepoint(pos):
                    self.ci.remove_id(item_id=id)
    def mousebuttonup(self, pos):
        pass

class TrafficLight(Transform):
    def __init__(self, fts, base):
        super().__init__(base)
        self.fts = fts
    def draw(self, surface):
        actuators = self.fts.get_actuators_raw()
        #print(sensors, sensors & self.bitmask)
        height = self.scale(20)
        width = self.scale(60)
        red_box = pygame.Rect(self.base[0], self.base[1]-2*height, width, height)
        yellow_box = pygame.Rect(self.base[0], self.base[1]-height, width, height)
        green_box = pygame.Rect(self.base[0], self.base[1], width, height)
        if actuators & 0x0010:
            pygame.draw.rect(surface, (255, 0, 0), red_box)
        else:
            pygame.draw.rect(surface, (250, 200, 200), red_box)
        if actuators & 0x0020:
            pygame.draw.rect(surface, (255, 255, 0), yellow_box)
        else:
            pygame.draw.rect(surface, (255, 255, 180), yellow_box)
        if actuators & 0x0040:
            pygame.draw.rect(surface, (0, 255, 0), green_box)
        else:
            pygame.draw.rect(surface, (200, 250, 200), green_box)

class Belt(Transform):
    def __init__(self, fts, base):
        super().__init__(base)
        self.fts = fts
    def draw(self, surface):
        actuators = self.fts.get_actuators_raw()
        #print(sensors, sensors & self.bitmask)
        height = self.scale(10)
        if actuators & 0x0004:
            # slow
            width = self.scale(25)
        else:
            # fast
            width = self.scale(60)

        x, y = self.map(560, 0)

        if actuators & 0x0008:
            # Stop so select color
            color = (255, 0, 0)
        else:
            color = (255, 255, 255)

        if actuators & 0x000F:
            arrow_box = pygame.Rect(x, y - 2 * height, width, height)
            pygame.draw.rect(surface, color, arrow_box)
        if actuators & 0x0001:
            # belt right
            pygame.draw.polygon(surface, color, [[x+width+height, y-1.5*height],[x+width,y-0.5*height],[x+width,y-2.5*height]], 0)
        if actuators & 0x0002:
            # belt left
            pygame.draw.polygon(surface, color, [[x-height, y-1.5*height],[x,y-0.5*height],[x,y-2.5*height]], 0)
    
class HCI(Transform):
    def __init__(self, fts, base, commandif=None):
        super().__init__(base)
        self.fts = fts
        self.ci = commandif
        height = self.scale(40)
        width = self.scale(40)
        # Raster ist 1.5 * Size
        self.start_box = pygame.Rect(self.base[0], self.base[1]-3*height, width, height)
        self.stop_box = pygame.Rect(self.base[0]+1.5*width, self.base[1]-3*height, width, height)
        self.reset_box = pygame.Rect(self.base[0], self.base[1]-1.5*height, width, height)
        self.Q1_box = pygame.Rect(self.base[0], self.base[1], width, height)
        self.Q2_box = pygame.Rect(self.base[0]+1.5*width, self.base[1], width, height)
        self.E_Stop_box = pygame.Rect(self.base[0]+3*width, self.base[1], width, height)
        self.E_Stop_box_inner = pygame.Rect(self.base[0]+3*width, self.base[1], width*2/3, height*2/3)
        self.hci_box = self.start_box.union(self.E_Stop_box)
        self.E_Stop_timer = 10
        self.E_Stop_state = False
        self.release_on_up = False
        self.button_active_start = False
        self.button_active_stop = False
        self.button_active_resest = False
        self.button_active_e_stop = False
    def draw(self, surface):
        actuators = self.fts.get_actuators_raw()
        sensors = self.fts.get_sensors_raw()
        #print(sensors, sensors & 0x0800)
        #start
        pygame.draw.rect(surface, (0, 100, 50), self.start_box)
        if sensors & 0x0100:
            pygame.draw.circle(surface, (0, 150, 70), (self.start_box.centerx, self.start_box.centery), int(self.start_box.width/3), 0)
        if actuators & 0x0100:
            pygame.draw.circle(surface, (255, 255, 255), (int(self.start_box.centerx-self.start_box.width/3), int(self.start_box.centery-self.start_box.height/3)), 3, 0)
        else:
            pygame.draw.circle(surface, (0, 0, 0), (int(self.start_box.centerx-self.start_box.width/3), int(self.start_box.centery-self.start_box.height/3)), 3, 1)
        #stop (inverted)
        pygame.draw.rect(surface, (200, 0, 40), self.stop_box)
        if (sensors & 0x0200) == 0:
            pygame.draw.circle(surface, (240, 0, 60), (self.stop_box.centerx, self.stop_box.centery), int(self.stop_box.width/3), 0) 
        # reset
        pygame.draw.rect(surface, (100, 100, 100), self.reset_box)
        if sensors & 0x0400:
            pygame.draw.circle(surface, (130, 130, 130), (self.reset_box.centerx, self.reset_box.centery), int(self.reset_box.width/3), 0) 
        if actuators & 0x0200:
            pygame.draw.circle(surface, (255, 255, 255), (int(self.reset_box.centerx-self.reset_box.width/3), int(self.reset_box.centery-self.reset_box.height/3)), 3, 0) 
        else:
            pygame.draw.circle(surface, (0, 0, 0), (int(self.reset_box.centerx-self.reset_box.width/3), int(self.reset_box.centery-self.reset_box.height/3)), 3, 1) 
        # Q1
        pygame.draw.rect(surface, (100, 100, 100), self.Q1_box)
        if actuators & 0x0400:
            pygame.draw.circle(surface, (255, 255, 255), (int(self.Q1_box.centerx), int(self.Q1_box.centery)), int(self.Q1_box.width*.4), 0) 
        # Q2
        pygame.draw.rect(surface, (100, 100, 100), self.Q2_box)
        if actuators & 0x0800:
            pygame.draw.circle(surface, (255, 255, 255), (int(self.Q2_box.centerx), int(self.Q2_box.centery)), int(self.Q2_box.width*.4), 0) 
        # E-Stop
        if (sensors & 0x0800) == 0:
            if self.E_Stop_timer > 3:
                self.E_Stop_timer = 0
                self.E_Stop_state = not self.E_Stop_state
            else:
                self.E_Stop_timer = self.E_Stop_timer + 1
            if self.E_Stop_state:
                pygame.draw.circle(surface, (255, 50, 50), (self.E_Stop_box.centerx, self.E_Stop_box.centery), int(self.start_box.width/2), 0)
            else:
                pygame.draw.circle(surface, (100, 0, 10), (self.E_Stop_box.centerx, self.E_Stop_box.centery), int(self.start_box.width/2), 0)
        else:   # inactive
            pygame.draw.circle(surface, (250, 0, 50), (self.E_Stop_box.centerx, self.E_Stop_box.centery), int(self.start_box.width/2), 0)
            pygame.draw.circle(surface, (200, 0, 40), (self.E_Stop_box.centerx, self.E_Stop_box.centery), int(self.start_box.width/3), 0)
            
    def mousebuttondown(self, pos):
        update = False
        if self.hci_box.collidepoint(pos) and self.ci is not None:
            if self.start_box.collidepoint(pos):
                self.button_active_start = not self.button_active_start
                update = True
                #self.ci.press_start_only()
                #self.release_on_up = True
                #print("hit start")
        if self.stop_box.collidepoint(pos):
            self.button_active_stop = not self.button_active_stop
            update = True
            #self.ci.press_stop_only()
            #self.release_on_up = True
            #print("hit stop")
        if self.reset_box.collidepoint(pos):
            self.button_active_resest = not self.button_active_resest
            update = True
            #self.ci.press_reset_only()
            #self.release_on_up = True
            #print("hit reset")
        if self.E_Stop_box.collidepoint(pos):
            self.button_active_e_stop = not self.button_active_e_stop
            update = True
            #self.ci.press_emergency_stop_only()
            #self.release_on_up = True
            #print("E-Stop")
        if update:
            value = 0
            value = value | (0x01 if self.button_active_start else 0)
            value = value | (0x02 if self.button_active_stop else 0)
            value = value | (0x04 if self.button_active_resest else 0)
            value = value | (0x08 if self.button_active_e_stop else 0)
            #print(value)
            self.ci.press_code(value)
    def mousebuttonup(self, pos):
        if self.release_on_up:
            self.ci.release_all()
            self.release_on_up = False

class ClearWorkpieces(Transform):
    def __init__(self, fts, base, commandif=None, sys_type=SystemType()):
        super().__init__(base)
        self.fts = fts
        self.ci = commandif
        self.sys_type = sys_type
        # Raster ist 1.5 * Size
        font = pygame.font.Font(None, 20)
        self.clear = font.render("Clear", True, (10, 10, 10))
        self.clear_textpos = self.clear.get_rect()
        self.clear_textpos.centerx = self.base[0]
        self.clear_textpos.centery = self.base[1]
    def draw(self, surface):
        if self.sys_type.type_ID == 'A' or self.sys_type.type_ID == 'B':
            pygame.draw.rect(surface, (100, 100, 100), self.clear_textpos.inflate(8,8))
            surface.blit(self.clear, self.clear_textpos)
    def mousebuttondown(self, pos):
        if self.ci is not None and (self.sys_type.type_ID == 'A' or self.sys_type.type_ID == 'B'):
            if self.clear_textpos.collidepoint(pos):
                pass
    def mousebuttonup(self, pos):
        pass

class ClearWorkpiecesSlide(ClearWorkpieces):
    def __init__(self, fts, base, commandif=None, sys_type=SystemType()):
        super().__init__(fts, base, commandif, sys_type)
    def mousebuttondown(self, pos):
        if self.ci is not None and (self.sys_type.type_ID == 'A' or self.sys_type.type_ID == 'B'):
            if self.clear_textpos.collidepoint(pos):
                self.ci.remove_all_slide()
    def mousebuttonup(self, pos):
        pass

class ClearWorkpiecesAll(ClearWorkpieces):
    def __init__(self, fts, base, commandif=None, sys_type=SystemType()):
        super().__init__(fts, base, commandif, sys_type)
    def mousebuttondown(self, pos):
        if self.ci is not None and (self.sys_type.type_ID == 'A' or self.sys_type.type_ID == 'B'):
            if self.clear_textpos.collidepoint(pos):
                self.ci.remove_all()
    def mousebuttonup(self, pos):
        pass
class ClearWorkpiecesEnd(ClearWorkpieces):
    def __init__(self, fts, base, commandif=None, sys_type=SystemType()):
        super().__init__(fts, base, commandif, sys_type)
    def draw(self, surface):
        if self.sys_type.type_ID == 'B':
            pygame.draw.rect(surface, (100, 100, 100), self.clear_textpos.inflate(8,8))
            surface.blit(self.clear, self.clear_textpos)
    def mousebuttondown(self, pos):
        if self.ci is not None and self.sys_type.type_ID == 'B':
            if self.clear_textpos.collidepoint(pos):
                self.ci.remove_all_end()
    def mousebuttonup(self, pos):
        pass

class WorkPieceInjection(Transform):
    def __init__(self, fts, base, commandif=None, sys_type=SystemType(), support_lego=False):
        super().__init__(base)
        self.fts = fts
        self.ci = commandif
        self.sys_type = sys_type
        height = self.scale(30)
        width = self.scale(30)
        self.flip = False
        self.sticky = False
        self.support_lego = support_lego
        # Raster ist 1.5 * Size
        buttons_origin_x = self.base[0]-3.3*width
        buttons_origin_y = self.base[1]
        self.wp_fl_box = pygame.Rect(buttons_origin_x, buttons_origin_y, width, height)
        self.wp_hd_box = pygame.Rect(buttons_origin_x+1.5*width, buttons_origin_y, width, height)
        self.wp_hu_box = pygame.Rect(buttons_origin_x+3.0*width, buttons_origin_y, width, height)
        self.wp_hu_box_inner = pygame.Rect(buttons_origin_x+3.25*width, buttons_origin_y+height/4, width/2, height/2)
        self.wp_md_box = pygame.Rect(buttons_origin_x+4.5*width, buttons_origin_y, width, height)
        self.wp_mu_box = pygame.Rect(buttons_origin_x+6.0*width, buttons_origin_y, width, height)
        self.wp_mu_box_inner = pygame.Rect(buttons_origin_x+6.25*width, buttons_origin_y+height/4, width/2, height/2)
        buttons_2_origin_y = self.base[1] + 1.5 * height
        self.wp_a_box = pygame.Rect(buttons_origin_x, buttons_2_origin_y, width, height)
        self.wp_b_box = pygame.Rect(buttons_origin_x+1.5*width, buttons_2_origin_y, width, height)
        self.wp_c_box = pygame.Rect(buttons_origin_x+3.0*width, buttons_2_origin_y, width, height)
        self.wp_d_box = pygame.Rect(buttons_origin_x+4.5*width, buttons_2_origin_y, width, height)
        self.wp_e_box = pygame.Rect(buttons_origin_x+6.0*width, buttons_2_origin_y, width, height)
        self.wp_f_box = pygame.Rect(buttons_origin_x+7.5*width, buttons_2_origin_y, width, height)
        self.wp_g_box = pygame.Rect(buttons_origin_x+9.0*width, buttons_2_origin_y, width, height)
        self.wp_h_box = pygame.Rect(buttons_origin_x+10.5*width, buttons_2_origin_y, width, height)
        self.wp_lone_box = pygame.Rect(buttons_origin_x+12*width, buttons_2_origin_y, width+14, height)
        self.wp_ltwo_box = pygame.Rect(buttons_origin_x+14*width, buttons_2_origin_y, width+14, height)
        self.wp_lthree_box = pygame.Rect(buttons_origin_x+16*width, buttons_2_origin_y, width+14, height)
        self.wpi_box = self.wp_fl_box.union(self.wp_mu_box)
        
        font = pygame.font.Font(None, 20)
        self.texta = font.render("000", True, (255, 255, 255))
        self.a_textpos = self.texta.get_rect()
        self.a_textpos.centerx = self.wp_a_box.centerx
        self.a_textpos.centery = self.wp_a_box.centery
        self.textb = font.render("001", True, (255, 255, 255))
        self.b_textpos = self.textb.get_rect()
        self.b_textpos.centerx = self.wp_b_box.centerx
        self.b_textpos.centery = self.wp_b_box.centery
        self.textc = font.render("010", True, (255, 255, 255))
        self.c_textpos = self.textc.get_rect()
        self.c_textpos.centerx = self.wp_c_box.centerx
        self.c_textpos.centery = self.wp_c_box.centery
        self.textd = font.render("011", True, (255, 255, 255))
        self.d_textpos = self.textd.get_rect()
        self.d_textpos.centerx = self.wp_d_box.centerx
        self.d_textpos.centery = self.wp_d_box.centery
        self.texte = font.render("100", True, (255, 255, 255))
        self.e_textpos = self.texte.get_rect()
        self.e_textpos.centerx = self.wp_e_box.centerx
        self.e_textpos.centery = self.wp_e_box.centery
        self.textf = font.render("101", True, (255, 255, 255))
        self.f_textpos = self.textf.get_rect()
        self.f_textpos.centerx = self.wp_f_box.centerx
        self.f_textpos.centery = self.wp_f_box.centery
        self.textg = font.render("110", True, (255, 255, 255))
        self.g_textpos = self.textg.get_rect()
        self.g_textpos.centerx = self.wp_g_box.centerx
        self.g_textpos.centery = self.wp_g_box.centery
        self.texth = font.render("111", True, (255, 255, 255))
        self.h_textpos = self.texth.get_rect()
        self.h_textpos.centerx = self.wp_h_box.centerx
        self.h_textpos.centery = self.wp_h_box.centery
        self.textlone = font.render("Lego1", True, (255, 255, 255))
        self.lone_textpos = self.textlone.get_rect()
        self.lone_textpos.centerx = self.wp_lone_box.centerx
        self.lone_textpos.centery = self.wp_lone_box.centery
        self.textltwo = font.render("Lego2", True, (255, 255, 255))
        self.ltwo_textpos = self.textltwo.get_rect()
        self.ltwo_textpos.centerx = self.wp_ltwo_box.centerx
        self.ltwo_textpos.centery = self.wp_ltwo_box.centery
        self.textlthree = font.render("Lego3", True, (255, 255, 255))
        self.lthree_textpos = self.textlthree.get_rect()
        self.lthree_textpos.centerx = self.wp_lthree_box.centerx
        self.lthree_textpos.centery = self.wp_lthree_box.centery
        self.flip_text = font.render("FLIP", True, (0, 0, 0))
        self.no_flip_text = font.render("Flip", True, (180, 180, 180))
        self.sticky_text = font.render("STICKY", True, (0, 0, 0))
        self.no_sticky_text = font.render("Sticky", True, (180, 180, 180))
        self.flip_textpos = self.flip_text.get_rect()
        self.sticky_textpos = self.sticky_text.get_rect()
        self.flip_textpos.union_ip(self.sticky_textpos)
        self.sticky_textpos = self.flip_textpos.copy() 
        self.flip_textpos.centerx = self.base[0] + 8.5 * width -3.3*width
        self.flip_textpos.centery = self.wp_fl_box.centery - self.flip_textpos.height*0.7
        self.sticky_textpos.centerx = self.base[0] + 8.5 * width -3.3*width
        self.sticky_textpos.centery = self.wp_mu_box.centery + self.sticky_textpos.height*0.7
    def draw(self, surface):
        if self.sys_type.type_ID == 'A':
            # only system at front (A) can inject workpices
            #binary
            pygame.draw.rect(surface, (100, 100, 100), self.wp_a_box.inflate(4,4))
            pygame.draw.ellipse(surface, (130, 130, 130), self.wp_a_box, 0)
            surface.blit(self.texta, self.a_textpos)

            pygame.draw.rect(surface, (100, 100, 100), self.wp_b_box.inflate(4,4))
            pygame.draw.ellipse(surface, (130, 130, 130), self.wp_b_box, 0)
            surface.blit(self.textb, self.b_textpos)
            pygame.draw.rect(surface, (100, 100, 100), self.wp_c_box.inflate(4,4))
            pygame.draw.ellipse(surface, (130, 130, 130), self.wp_c_box, 0)
            surface.blit(self.textc, self.c_textpos)

            pygame.draw.rect(surface, (100, 100, 100), self.wp_d_box.inflate(4,4))
            pygame.draw.ellipse(surface, (130, 130, 130), self.wp_d_box, 0)
            surface.blit(self.textd, self.d_textpos)

            pygame.draw.rect(surface, (100, 100, 100), self.wp_e_box.inflate(4,4))
            pygame.draw.ellipse(surface, (130, 130, 130), self.wp_e_box, 0)
            surface.blit(self.texte, self.e_textpos)

            pygame.draw.rect(surface, (100, 100, 100), self.wp_f_box.inflate(4,4))
            pygame.draw.ellipse(surface, (130, 130, 130), self.wp_f_box, 0)
            surface.blit(self.textf, self.f_textpos)

            pygame.draw.rect(surface, (100, 100, 100), self.wp_g_box.inflate(4,4))
            pygame.draw.ellipse(surface, (130, 130, 130), self.wp_g_box, 0)
            surface.blit(self.textg, self.g_textpos)

            pygame.draw.rect(surface, (100, 100, 100), self.wp_h_box.inflate(4,4))
            pygame.draw.ellipse(surface, (130, 130, 130), self.wp_h_box, 0)
            surface.blit(self.texth, self.h_textpos)

            # lego
            if self.support_lego:
                pygame.draw.rect(surface, (171, 171, 41), self.wp_lone_box.inflate(4,4))
                surface.blit(self.textlone, self.lone_textpos)

                pygame.draw.rect(surface, (41, 171, 41), self.wp_ltwo_box.inflate(4,4))
                surface.blit(self.textltwo, self.ltwo_textpos)

                pygame.draw.rect(surface, (41, 171, 171), self.wp_lthree_box.inflate(4,4))
                surface.blit(self.textlthree, self.lthree_textpos)

            # flat
            pygame.draw.rect(surface, (100, 100, 100), self.wp_fl_box.inflate(4,4))
            pygame.draw.ellipse(surface, (0, 0, 200), self.wp_fl_box, 0)
            # hole down
            pygame.draw.rect(surface, (100, 100, 100), self.wp_hd_box.inflate(4,4))
            pygame.draw.ellipse(surface, (180, 0, 0), self.wp_hd_box, 0)
            # hole up
            pygame.draw.rect(surface, (100, 100, 100), self.wp_hu_box.inflate(4,4))
            pygame.draw.ellipse(surface, (180, 0, 0), self.wp_hu_box, 0)
            pygame.draw.ellipse(surface, (100, 100, 100), self.wp_hu_box_inner, 0)
            # metal down
            pygame.draw.rect(surface, (100, 100, 100), self.wp_md_box.inflate(4,4))
            pygame.draw.ellipse(surface, (220, 220, 220), self.wp_md_box, 0)
            # metal up
            pygame.draw.rect(surface, (100, 100, 100), self.wp_mu_box.inflate(4,4))
            pygame.draw.ellipse(surface, (220, 220, 220), self.wp_mu_box, 0)
            pygame.draw.ellipse(surface, (100, 100, 100), self.wp_mu_box_inner, 0)
            if self.flip:
                pygame.draw.rect(surface, (220, 220, 220), self.flip_textpos.inflate(5,5))
                surface.blit(self.flip_text, self.flip_textpos)
            else:
                pygame.draw.rect(surface, (100, 100, 100), self.flip_textpos.inflate(5,5))
                surface.blit(self.no_flip_text, self.flip_textpos)
            if self.sticky:
                pygame.draw.rect(surface, (220, 220, 220), self.sticky_textpos.inflate(5,5))
                surface.blit(self.sticky_text, self.sticky_textpos)
            else:
                pygame.draw.rect(surface, (100,100, 100), self.sticky_textpos.inflate(5,5))
                surface.blit(self.no_sticky_text, self.sticky_textpos)

    def mousebuttondown(self, pos):
        clear = False
        if self.ci is not None:
            if self.sys_type.type_ID == 'A':
                if self.wp_fl_box.collidepoint(pos):
                    self.ci.add_item(kind='flat', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_hd_box.collidepoint(pos):
                    self.ci.add_item(kind='holedown', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_hu_box.collidepoint(pos):
                    self.ci.add_item(kind='holeup', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_md_box.collidepoint(pos):
                    self.ci.add_item(kind='metaldown', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_mu_box.collidepoint(pos):
                    self.ci.add_item(kind='metalup', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_a_box.collidepoint(pos):
                    self.ci.add_item(kind='code0', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_b_box.collidepoint(pos):
                    self.ci.add_item(kind='code1', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_c_box.collidepoint(pos):
                    self.ci.add_item(kind='code2', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_d_box.collidepoint(pos):
                    self.ci.add_item(kind='code3', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_e_box.collidepoint(pos):
                    self.ci.add_item(kind='code4', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_f_box.collidepoint(pos):
                    self.ci.add_item(kind='code5', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_g_box.collidepoint(pos):
                    self.ci.add_item(kind='code6', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_h_box.collidepoint(pos):
                    self.ci.add_item(kind='code7', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_lone_box.collidepoint(pos):
                    self.ci.add_item(kind='lego1', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_ltwo_box.collidepoint(pos):
                    self.ci.add_item(kind='lego2', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.wp_lthree_box.collidepoint(pos):
                    self.ci.add_item(kind='lego3', flip=self.flip, sticky=self.sticky, at=0)
                    clear = True
                if self.flip_textpos.collidepoint(pos):
                    self.flip = not self.flip
                if self.sticky_textpos.collidepoint(pos):
                    self.sticky = not self.sticky
        if clear:
            self.sticky = False
            self.flip = False
    def mousebuttonup(self, pos):
        pass

class SimControl(Transform):
    def __init__(self, fts, base, commandif=None):
        super().__init__(base)
        self.fts = fts
        self.ci = commandif
        height = self.scale(40)
        width = self.scale(80)
        self.start_box = pygame.Rect(self.base[0], self.base[1], width, height)
        self.font = pygame.font.Font(None, 18)
    def draw(self, surface):
        # Display text
        pygame.draw.rect(surface, (0, 150, 100), self.start_box)
        text = self.font.render("Start", True, (10, 10, 10))
        surface.blit(text, (self.base[0]+10, self.base[1]+10))
    def mousebuttondown(self, pos):
        if self.start_box.collidepoint(pos) and self.ci is not None:
            self.ci.start_simulation()
            #print("hit sim start")
    def mousebuttonup(self, pos):
        pass

class LightBarrier(Transform):
    def __init__(self, fts, base, bitmask, start, end):
        super().__init__(base)
        self.fts = fts
        self.bitmask = bitmask
        self.start = start
        self.end = end
    def draw(self, surface):
        sensors = self.fts.get_sensors_raw()
        #print(sensors, sensors & self.bitmask)
        if sensors & self.bitmask:
            start = self.map(self.start[0], self.start[1])
            end = self.map(self.end[0], self.end[1])
            pygame.draw.line(surface, (255, 0, 0), start, end, 2)

class MetalSensor(Transform):
    def __init__(self, fts, base):
        super().__init__(base)
        self.fts = fts
    def draw(self, surface):
        sensors = self.fts.get_sensors_raw()
        pos = self.map(395, 60)
        r = self.scale(5)
        if sensors & 0x0010:
            pygame.draw.circle(surface, (0, 255, 0), pos, r, 0)
        else:
            pygame.draw.circle(surface, (180, 0, 0), pos, r, 0)

class HeightSensor(Transform):
    def __init__(self, fts, base):
        super().__init__(base)
        self.fts = fts
    def draw(self, surface):
        sensors = self.fts.get_sensors_raw()
        pos = self.map(270, 60)
        r = self.scale(5)
        if sensors & 0x0004:
            pygame.draw.circle(surface, (0, 255, 0), pos, r, 0)
        else:
            pygame.draw.circle(surface, (180, 0, 0), pos, r, 0)

class Seperator(Transform):
    def __init__(self, fts, base):
        super().__init__(base)
        self.fts = fts
        self.pusher = False
    def draw(self, surface):
        if self.fts.get_config_is_ejector() and not self.fts.get_config_is_feed_separator():
            self.pusher = True
        if not self.fts.get_config_is_ejector() and self.fts.get_config_is_feed_separator():
            self.pusher = False

        sensors = self.fts.get_sensors_raw()
        if self.pusher:
            start = self.map(395, 30)
            w = self.scale(40)
            if sensors & 0x0020:
                end = self.map(395, 40)
            else:
                end = self.map(395, 80)
            pygame.draw.line(surface, (120, 120, 120), start, end, w)
        else:
            start = self.map(420, 30)
            w = self.scale(10)
            if sensors & 0x0020:
                end = self.map(470, 30)
            else:
                end = self.map(420, 80)
            pygame.draw.line(surface, (120, 120, 120), start, end, w)

def main(target_ip='127.0.0.1', target_port=40000, receive_port=6001, support_lego=False):
    receiver_ip = ''
    fts = simulationreportreceiver.SimulationStatusFacadeUDP(recv_ip=receiver_ip, recv_port=receive_port)
    ci = simulationcommandinterface.SimulationCommandInterface(target=target_ip, port=target_port)
    
    sys_type = SystemType()

    # Initialise screen
    pygame.init()
    screen = pygame.display.set_mode((800, 400))#350
    # Fill background
    background = pygame.Surface(screen.get_size())
    background = background.convert()
    background.fill((250, 250, 250))
    
    base = (50, 300)

    # draw conveyor belt & slide
    t = Transform(base)
    w = t.scale(700)
    h = t.scale(80)
    x, y = t.map(0, 0)
    r = pygame.Rect(x, y-h, w, h)
    pygame.draw.rect(background, (0, 0, 0), r, 0)
    # Slide
    x, y = t.map(395-60/2, 80+140)
    w = t.scale(60)
    h = t.scale(140)
    r = pygame.Rect(x, y, w, h)
    pygame.draw.rect(background, (20, 20, 20), r, 0)
    # Blech
    x, y = t.map(200+38, 38)
    w = t.scale(300)
    h = t.scale(38)
    r = pygame.Rect(x, y, w, h)
    pygame.draw.rect(background, (0, 0, 200), r, 0)
    x, y = t.map(200, 0)
    off = t.scale(2)
    pygame.draw.polygon(background, (0,0,200), [(x, y-off), (x+h,y-off), (x+h, y-h)])
 
    clock = pygame.time.Clock()

    # Blit everything to the screen
    screen.blit(background, (0, 0))
    pygame.display.flip()

    # Add lightbarrier system
    lb = []
    lb.append(LightBarrier(fts, base, 0x0001, (20, 0), (20, 80)))   # begin
    lb.append(LightBarrier(fts, base, 0x0002, (290, 0), (290, 80)))   # height
    lb.append(LightBarrier(fts, base, 0x0008, (415, 0), (415, 80)))   # feed
    lb.append(LightBarrier(fts, base, 0x0040, (425, 110), (365, 110)))   # slide ???  (middle 395+-10)
    lb.append(LightBarrier(fts, base, 0x0080, (655, 0), (655, 80)))   # end

    itemdisplay = Items(fts, base, ci)
    sim_clock = CurrentSimTime(fts, (600, 100))
    sim_ctrl = SimControl(fts, (600, 50), ci)
    hci = HCI(fts, (120, 150), ci)
    wpi = WorkPieceInjection(fts, (150, 310), ci, sys_type, support_lego)
    cwp_s = ClearWorkpiecesSlide(fts, (442, 50), ci, sys_type)
    cwp_a = ClearWorkpiecesAll(fts, (440, 320), ci, sys_type)
    cwp_e = ClearWorkpiecesEnd(fts, (700, 190), ci, sys_type)
    status_lights = TrafficLight(fts, (500, 100))
    feedsep = Seperator(fts, base)
    metal = MetalSensor(fts, base)
    height = HeightSensor(fts, base)
    belt = Belt(fts, base)
     
    # Event loop
    try:
        while 1:
            clock.tick(30)

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    fts.close()
                    return
                if event.type == pygame.MOUSEBUTTONDOWN:
                    #print(dir(pygame.mouse))
                    pos = pygame.mouse.get_pos()
                    #print(pos)
                    hci.mousebuttondown(pos)
                    sim_ctrl.mousebuttondown(pos)
                    wpi.mousebuttondown(pos)
                    cwp_s.mousebuttondown(pos)
                    cwp_a.mousebuttondown(pos)
                    cwp_e.mousebuttondown(pos)
                    itemdisplay.mousebuttondown(pos)
                if event.type == pygame.MOUSEBUTTONUP:
                    #print(dir(pygame.mouse))
                    pos = pygame.mouse.get_pos()
                    #print(pos)
                    hci.mousebuttonup(pos)

            screen.blit(background, (0, 0))

            # Display System name
            if fts.get_config_is_system_b():
                sys_type.type_ID = 'B'
            else:
                if fts.get_config_is_system_client():
                    sys_type.type_ID = 'C'
                else:
                    sys_type.type_ID = 'A'

            font = pygame.font.Font(None, 36)
            if sys_type.type_ID == 'B':
                pygame.display.set_caption('Festo Simulation B')
                text = font.render("Festo Transfer System B", True, (10, 10, 10))
            if sys_type.type_ID == 'C':
                pygame.display.set_caption('Festo-Anlage')
                text = font.render("Festo Transfer Client", True, (10, 10, 10))
            if sys_type.type_ID == 'A':
                pygame.display.set_caption('Festo Simulation A')
                text = font.render("Festo Transfer System A", True, (10, 10, 10))
            textpos = text.get_rect()
            textpos.centerx = background.get_rect().centerx
            screen.blit(text, textpos)

            belt.draw(screen)
            feedsep.draw(screen)
            for e in lb:
                e.draw(screen)
            itemdisplay.draw(screen)
            metal.draw(screen)
            height.draw(screen)
            status_lights.draw(screen)
            hci.draw(screen)
            wpi.draw(screen)
            cwp_s.draw(screen)
            cwp_a.draw(screen)
            cwp_e.draw(screen)
            sim_clock.draw(screen)
            sim_ctrl.draw(screen)
            pygame.display.flip()
    except KeyboardInterrupt:
        print("\nTry to end script, if I'm still stuck, press Ctrl-C again.")
        fts.close()

if __name__ == '__main__': 
    # Argparse
    # - IP
    # - ports
    # - Lego(TM)
    parser = argparse.ArgumentParser(description='Show state of Fest Transfer System Simulaiton.')
    parser.add_argument('-tip', '--targetip', 
                        dest='target_IP', type=str,
                        help='IP-adress of target system (simulation).')
    parser.add_argument('-tp', '--targetport', 
                        dest='target_port', type=int,
                        help='UDP-port to send comannds on target to.')
    parser.add_argument('-lp', '--localport', 
                        dest='receive_port', type=int,
                        help='UDP-port to receive report status information from simulation.')
    parser.add_argument('-cf', '--configfile', 
                        dest='configfilename', type=str,
                        help='File name with configuration information.')
    parser.add_argument('-l', '--lego', 
                        dest='support_lego', action='store_true', default=None,
                        help='Show injection option for Lego(TM) bricks.')

    args = parser.parse_args()
    arguments = Arguments()
    #print(arguments.configfilename)

    # use given configfile, if provided
    if args.configfilename is not None:
        arguments.configfilename = args.configfilename
    # load config file 
    if os.path.isfile(arguments.configfilename):
        with open(arguments.configfilename) as configfile:
            configuration = configfile.read()
            try:
                configuration_dir = json.loads(configuration)
            except json.decoder.JSONDecodeError as err:
                print(f"Error in config file {arguments.configfilename}")
                print(err)
            else:
                if 'targetip' in configuration_dir:
                    arguments.target_IP = configuration_dir['targetip']
                else:
                    print("Configfile error, targetip-entry not found!")
                if 'targetport' in configuration_dir:
                    arguments.target_port = configuration_dir['targetport']
                else:
                    print("Configfile error, targeport-entry not found!")
                if 'receiveport' in configuration_dir:
                    arguments.receive_port = configuration_dir['receiveport']
                else:
                    print("Configfile error, receiveport-entry not found!")
                if 'lego' in configuration_dir:
                    arguments.support_lego = configuration_dir['lego']
    arguments.target_IP = args.target_IP if args.target_IP is not None else arguments.target_IP
    arguments.target_port = args.target_port if args.target_port is not None else arguments.target_port
    arguments.receive_port = args.receive_port if args.receive_port is not None else arguments.receive_port
    arguments.support_lego = args.support_lego if args.support_lego is not None else arguments.support_lego
    
    print("Version:", VERSION)
    print(f"Talk to {arguments.target_IP} on port {arguments.target_port}")
    print(f"Listen on port {arguments.receive_port}")

    main(target_ip=arguments.target_IP, target_port=arguments.target_port, receive_port=arguments.receive_port, support_lego=arguments.support_lego)
