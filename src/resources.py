import pyglet

pyglet.resource.path = ["../res"]
pyglet.resource.reindex()

node = pyglet.resource.image("node_red_tiny.png")
