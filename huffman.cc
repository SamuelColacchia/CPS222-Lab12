/* huffman.cc
 *
 * Implementation of the methods defined in huffman.h
 *
 * Copyright (c) 2013 - Russell C. Bjork
 *
 * MODIFIED BY:
 */

#include "huffman.h"
#include <climits>

HuffmanTree::HuffmanTree()
  : _root(NULL)
{
}


void HuffmanTree::read(istream& treefile)
{
  _root = Node::read(treefile);
}


void HuffmanTree::write(ostream& treefile) const
{
  _root->write(treefile);
}


#ifndef PROFESSOR_VERSION

void HuffmanTree::fillIn(istream& document)
{
/* STUDENT CODE GOES HERE.  REMOVE THIS LINE */
}


void HuffmanTree::compress(istream& originalDocument,
                           ostream& compressedDocument) const
{
/* STUDENT CODE GOES HERE.  REMOVE THIS LINE */
}


void HuffmanTree::decompress(istream& compressedDocument,
                             ostream& decompressedDocument) const
{
  Node *currentNode = _root;
  int currentBit;
  bool endOfFile = true;

  while (endOfFile)
  {
    currentNode = _root;
    while (currentNode->isInternal())
    {
      currentBit = extractBit(compressedDocument);
      if (currentBit == 1)
      {
        currentNode = currentNode->getRChild();
        if (!currentNode->isInternal())
        {
          if (currentNode->getCharacter() == EOF_CHAR)
          {
            cout << "end" << endl;
            endOfFile = false;
          }
          else
          {
            decompressedDocument << currentNode->getCharacter();
          }
        }
      }
      else if (currentBit == 0)
      {
        currentNode = currentNode->getLChild();
        if (!currentNode->isInternal())
        {
          if (currentNode->getCharacter() == EOF_CHAR)
          {
            cout << "end" << endl;
            endOfFile = false;
          }
          else
          {
            decompressedDocument << currentNode->getCharacter();
          }
        }
      }
    }
  }
}
#endif

void HuffmanTree::createCodeTable(int *& bits, int *& count) const
{
  bits = new int[CHAR_MAX + 1];
  count = new int[CHAR_MAX + 1];
  // +1 to allow for 0 entry
  _root->fillInCodeTable(bits, count, 0, 0);
}


// Global variables used by bit operations

#define BITS_PER_CHARACTER    8

static int bitsExtracted = BITS_PER_CHARACTER;
static int bitsInserted = 0;
static char currentExtractChar, currentInsertChar;

int HuffmanTree::extractBit(istream& input)
{
  // Read a single character if need be

  if (bitsExtracted >= BITS_PER_CHARACTER)
  {
    input.get(currentExtractChar);
    bitsExtracted = 0;
  }

  // Extract next bit from current character

  int result = ((signed char)currentExtractChar) < 0;    // Means leftmost bit 1
  currentExtractChar <<= 1;
  bitsExtracted++;
  return result;
}


void HuffmanTree::insertBits(ostream& output, int bits, int count)
{
  for (int i = count - 1; i >= 0; i--)
  {
    // Insert bit at position i into current character

    currentInsertChar <<= 1;
    currentInsertChar |= ((bits & (1 << i)) != 0);
    bitsInserted++;

    // Write character if full

    if (bitsInserted >= BITS_PER_CHARACTER)
    {
      output.put(currentInsertChar);
      bitsInserted = 0;
    }
  }
}


void HuffmanTree::flushBits(ostream& output)
{
  if (bitsInserted > 0)
  {
    // This loop effectively inserts 0's until currentInsertChar is full,
    // putting the first "real" bit in the leftmost position

    while (bitsInserted < BITS_PER_CHARACTER)
    {
      currentInsertChar <<= 1;
      bitsInserted++;
    }
    output.put(currentInsertChar);
  }
}


#ifdef PROFESSOR_VERSION

#define QUOTE(Q)           # Q
#define INCLUDE_NAME(X)    QUOTE(X)
#include INCLUDE_NAME(PROFESSOR_VERSION)
#endif
