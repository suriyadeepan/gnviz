#!/usr/bin/python

####
# Brace youself! Import statements are arriving
####
import sys
import os
import time
from pyglet.window import key
from pyglet import image
import pyglet
import resources
import csv

##
# create log handle
##
log = open(os.path.basename(__file__) + ".log",'w')

game_window = pyglet.window.Window(width=500,height=500, visible=True)
###
# initial x,y
sprite_x = 100
sprite_y = 100
#
###

#########################
# Read from trace files #
#########################
mob_list = list(csv.reader(open("Mobility.txt",'rb'),delimiter=' '))
nodes = list(csv.reader(open("Topology.txt",'rb'),delimiter=' '))
msgs = list(csv.reader(open("RadioCommunication.txt",'rb'),delimiter=' '))


frame_count = 0
time_per_frame = 0.1
msg_index = 1

###
# Set background
###
background = image.load('bg.png')#.get_texture()

@game_window.event
def on_draw():
    game_window.clear()
    background.blit(0,0)



    #pyglet.gl.glColor4f(1.0,0,0,1.0)                                               
    #glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)                             
    #glEnable (GL_BLEND)                                                            
    #glEnable (GL_LINE_SMOOTH);                                                     
    #glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE)                                     
    #glLineWidth (3)                                                                
    #pyglet.gl.glColor3f(1.0, 1.0, 1.0);
    pyglet.graphics.draw(2, pyglet.gl.GL_LINES,('v2i', (10, 15, 300, 305)) )  

    for sprite_elm in sprite:
        sprite_elm.draw()
        
    update_msgs()
    
@game_window.event
def on_key_press(symbol, modifiers):
    global time_per_frame
    if symbol == key.PLUS:
        time_per_frame /= 2
    elif symbol == key.MINUS:
        time_per_frame *= 2
    pyglet.clock.schedule_interval(update, time_per_frame)

def update(dt):

    global frame_count
    frame_count += 1

    if frame_count >= len(mob_list)-1:
        sys.exit()

    update_mob()

    log.write(str(time.time()) + " " + str(frame_count) +"\n")
    pass

def update_msgs():
    global msg_index
    ftime = float(msgs[msg_index][0]) + time_per_frame
    while float(msgs[msg_index][0]) < ftime:
        print "comparing", float(msgs[msg_index][0]), ftime
        print msgs[msg_index]
        src = { 'x': sprite[int(msgs[msg_index][2])].x, 'y' : sprite[int(msgs[msg_index][2])].y }
        dst = { 'x': sprite[int(msgs[msg_index][3])].x, 'y' : sprite[int(msgs[msg_index][3])].y }
        pyglet.graphics.draw(2, pyglet.gl.GL_LINES,('v2i', ( src['x'],src['y'] + 10,dst['x'],dst['y']+10 )) )
        msg_index += 1

def getLoca(axis):
    return int(mob_list[frame_count][axis])

def update_mob():
    sprite[int(mob_list[frame_count][1])].x = int(mob_list[frame_count][2])
    sprite[int(mob_list[frame_count][1])].y = int(mob_list[frame_count][3])


if __name__ == '__main__':
    ###
    # update function is called every "time_per_frame" seconds
    ###
    pyglet.clock.schedule_interval(update, time_per_frame)

    ###
    # setup
    ###
    sprite = list()
    for node in nodes:
        if len(node) > 0:
            sprite.append(pyglet.sprite.Sprite(resources.node, x=int(node[1]), y=int(node[2]) ))

    #########
    ## run ##
    pyglet.app.run()

    ###
    # cleanup
    log.close()

