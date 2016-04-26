/* node.cc
 *
 * Implementation of the nested class Node and its subclasses, used in
 * implementing a Huffman tree.
 *
 * Copyright (c) 2013 - Russell C. Bjork
 */

#include "huffman.h"

#ifndef INTERNAL_NODE_MARKER
#define INTERNAL_NODE_MARKER '\377'
#endif

HuffmanTree::Node * HuffmanTree::Node::getLChild() const
{ throw "getLChild() called on an improper node type."; }

HuffmanTree::Node * HuffmanTree::Node::getRChild() const
{ throw "getRChild() called on an improper node type."; }

char HuffmanTree::Node::getCharacter() const
{ throw "getCharacter() called on an improper node type."; }

HuffmanTree::Node * HuffmanTree::Node::read(istream & treefile)
{
    char character;
    treefile.get(character);
    if (character == INTERNAL_NODE_MARKER)
    {
        Node * lchild = read(treefile);
        Node * rchild = read(treefile);
        return new InternalNode(lchild, rchild);
    }
    else
        return new LeafNode(character);
}

HuffmanTree::InternalNode::InternalNode(HuffmanTree::Node * lchild,
                                        HuffmanTree::Node * rchild)
: _lchild(lchild), _rchild(rchild)
{ }

bool HuffmanTree::InternalNode::isInternal() const
{ return true; }

int HuffmanTree::InternalNode::getFrequency() const
{ return _lchild -> getFrequency() + _rchild -> getFrequency(); }

void HuffmanTree::InternalNode::write(ostream & treefile) const
{
    treefile.put(INTERNAL_NODE_MARKER);
    /*DBG* cerr << "+" << endl; */
    _lchild -> write(treefile);
    _rchild -> write(treefile);
}

HuffmanTree::Node * HuffmanTree::InternalNode::getLChild() const
{ return _lchild; }

HuffmanTree::Node * HuffmanTree::InternalNode::getRChild() const
{ return _rchild; }

void HuffmanTree::InternalNode::fillInCodeTable(
                     int bits [],
                     int count [],
                     int bitsSoFar,
                     int countSoFar) const
{
    _lchild -> fillInCodeTable(bits,
                               count,
                               bitsSoFar << 1,
                               countSoFar + 1);
    _rchild -> fillInCodeTable(bits,
                               count,
                               (bitsSoFar << 1) | 1,
                               countSoFar + 1);
}

HuffmanTree::LeafNode::LeafNode(char character, int frequency)
: _character(character), _frequency(frequency)
{ }

bool HuffmanTree::LeafNode::isInternal() const
{ return false; }

int HuffmanTree::LeafNode::getFrequency() const
{ return _frequency; }

void HuffmanTree::LeafNode::write(ostream & treefile) const
{
    treefile.put(_character);
    /*DBG* cerr << (int)_character << endl;*/
}

char HuffmanTree::LeafNode::getCharacter() const
{ return _character; }

void HuffmanTree::LeafNode::fillInCodeTable(
                     int bits [],
                     int count [],
                     int bitsSoFar,
                     int countSoFar) const
{
    bits[_character] = bitsSoFar;
    count[_character] = countSoFar;
}

bool HuffmanTree::NodeFrequencyComparator::operator() (HuffmanTree::Node * a,
                                                       HuffmanTree::Node * b)
{ return a -> getFrequency() > b -> getFrequency(); }
