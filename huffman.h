/* huffman.h
 *
 * A Huffman tree, with support for compressing and decompressing files
 *
 * Copyright (c) 2013 - Russell C. Bjork
 */

#include <iostream>
#include <array>
#include <vector>
#include <queue>
using namespace std;

class HuffmanTree
{
    public:

        /* Constructor for an empty tree */
        HuffmanTree();
        /* Read contents of tree from a file. */
        void read(istream & treefile);
        /* Write this tree to a file that can be read by read. */
        void write(ostream & treefile) const;
        /* Fill in tree based on the characters occurring in a document. */
        void fillIn(istream & document);
        /* Compress a document using this tree.  */
        void compress(istream & originalDocument,
                      ostream & compressedDocument) const;
        /* Decompress a document that was compressed by the above. */
        void decompress(istream & compressedDocument,
                        ostream & decompressedDocument) const;
    private:

        /* Create a code table to facilitate compressing a file.  Entries for
         * characters not represented by this table will not be filled in and
         * should not be used. */
        void createCodeTable(int * & bits, int * & count) const;
        /* Extract the next bit from a binary input stream */
        static int extractBit(istream & input);
        /* Write count of bits to a binary output stream */
        static void insertBits(ostream & output, int bits, int count);
        /* Flush any bits not yet output to the stream */
        static void flushBits(ostream & output);

        /* A node in a Huffman tree.  The nodes are of two kinds: internal
         * nodes that have two children, and leaves that store a key.  Both
         * derive from the common base class.
         */
        class Node
        {
            public:

                /* Test to see whether this node is an internal node */
                virtual bool isInternal() const = 0;
                /* Get the total frequency of occurrence of the characters
                 * appearing in the subtree rooted at this node - used when
                 * filling in a tree from a document */
                virtual int getFrequency() const = 0;
                /* Accessor for left subtree of this node - should only be
                 * called on internal nodes.  */
                virtual Node * getLChild() const;
                /* Accessor for right subtree of this node - should only be
                 * called on internal nodes. */
                virtual Node * getRChild() const;
                /* Accessor for character stored in this node - should only be
                 * called on leaf nodes. */
                virtual char getCharacter() const;
                /* Fill in a code table with information contained in the
                 * subtree rooted at this node.  bits and count are the
                 * arrays of entries; bitsSoFar and countSoFar are the portion
                 * of the code that has already been determined by ancestor
                 * nodes. */
                virtual void fillInCodeTable(int bits [],
                                             int count [],
                                             int bitsSoFar,
                                             int countSoFar) const = 0;
                /* Write the subtree rooted at this node to a file */
                virtual void write(ostream & treefile) const = 0;
                /* Read a subtree that has been written by write and return
                 * pointer to root node. */
                static Node * read(istream & treefile);
        };

        class InternalNode : public Node
        {
            public:

                /* Constructor */
                InternalNode(Node * lchild, Node * rchild);
                bool isInternal() const;
                int getFrequency() const;
                void write(ostream & treefile) const;
                Node * getLChild() const;
                Node * getRChild() const;
                void fillInCodeTable(int bits [],
                                     int count [],
                                     int bitsSoFar,
                                     int countSoFar) const;
            private:

                Node * _lchild, * _rchild;
        };

        class LeafNode : public Node
        {
            public:

                /* Constructor.  Frequency is only used when constructing
                 * a tree from a document.  It is not stored in a tree file.
                 */
                LeafNode(char character, int frequency = 0);
                bool isInternal() const;
                int getFrequency() const;
                void write(ostream & treefile) const;
                char getCharacter() const;
                void fillInCodeTable(int bits [],
                                     int count [],
                                     int bitsSoFar,
                                     int countSoFar) const;
            private:

                char _character;
                int _frequency;
        };

        /* An object of this class is used to compare pointers to two nodes
         * based on frequency of occurrence of the characters appearing in
         * their subtrees - used for maintaining the order in a priority queue.
         */
        class NodeFrequencyComparator
        {
            public:
                bool operator()(Node * a, Node * b);
        };

        /* The root of this tree */
        Node * _root;
};

/* Character to be compressed and then used to mark end of a compressed file */
#define EOF_CHAR '\004'
