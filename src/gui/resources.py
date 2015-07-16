import pyglet
import os

pyglet.resource.path = [os.environ['GNVIZ'] + "/res"]
pyglet.resource.reindex()

node = pyglet.resource.image("node_red_tiny.png")
