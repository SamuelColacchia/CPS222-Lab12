# Makefile for Huffman Tree Lab

# It would be wise to add a test target to run ./huffman with the right
# command line arguments.  It can also test the result files in some cases,
# by using diff to compare them.

huffman:	huffman.o node.o driver.o
	g++ -std=c++11 -o $@ $^

huffman.o:

%.o:	%.cc
	g++ -std=c++11 -c $<
