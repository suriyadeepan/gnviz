#!/usr/bin/python

###
# Brace youself... Import statements incoming!
###
import csv

mobility_handle = list(csv.reader(open("Mobility.txt",'rb'),delimiter=' '))

#for row in mobility_handle:
#	coli = 0
#	for col in row:
#		if coli == 2:
#			print col
#			break
#		coli += 1
#	print ""


print "----------------------"
print "length:", len(mobility_handle)
print mobility_handle[400]
