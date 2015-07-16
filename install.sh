sudo apt-get install python-pip
sudo pip install pyglet

if [ -z "$GNVIZ" ]
then
	echo "export GNVIZ=$PWD" >> ~/.bashrc
	echo 'export PATH=$PATH:$GNVIZ/src' >> ~/.bashrc

	source ~/.bashrc
else
	echo ">> gnViz is already installed"
fi
