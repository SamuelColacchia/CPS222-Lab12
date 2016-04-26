/* driver.cc
 *
 * Driver program for lab
 *
 * Copyright (c) 2013 - Russell C. Bjork
 */
 
#include "huffman.h"
#include <fstream>
#include <string.h>

/* Print a usage message */
void usage()
{
    cout << "usage:" << endl;
    cout << "huffman -f treefile originalDocument" << endl;
    cout << "huffman -c treefile originalDocument compressedDocument" << endl;
    cout << "huffman -d treefile compressedDocument decompressedDocument" << endl;
    cout << "-f form creates a tree file based on character frequencies in " <<
                "a document" << endl;
    cout << "-c compresses a document; -d decompresses" << endl;
}

/* Main program */
int main(int argc, char ** argv)
{
    if (argc < 2 || strlen(argv[1]) != 2)
    {
        usage();
        return 1;
    }
    
    char command = argv[1][1];
    HuffmanTree theTree;
    
    switch(command)
    {
        case 'f':
            
            if (argc == 4)
            {
                ifstream document(argv[3]);
                if (document.good())
                {
                    theTree.fillIn(document);
                    if (document.eof())
                        document.close();
                    else
                    {
                        cerr << "Error reading file: " 
                            << argv[3] << endl;
                        return 1;
                    }
                }
                else            
                {
                    cerr << "Error opening file: " << argv[3] << endl;
                    return 1;
                }
                
                ofstream treefile(argv[2], ios::out | ios::binary);
                if (treefile.good())
                {
                    theTree.write(treefile);
                    if (treefile.good())
                    {
                        treefile.close();
                        return 0;
                    }
                    else
                    {
                        cerr << "Error writing file: " << argv[2] << endl;
                        return 1;
                    }
                }
                else
                {
                    cerr << "Error creating file: " << argv[2] << endl;
                    return 1;
                }
            }
            else
            {
                usage();
                return 1;
            }
        
        case 'c':
        
            if (argc == 5)
            {
                ifstream treefile(argv[2], ios::in | ios::binary);
                if (treefile.good())
                {
                    theTree.read(treefile);
                    char expectedEOF;
                    treefile.get(expectedEOF);
                    if (treefile.eof())
                        treefile.close();
                    else
                    {
                        cerr << "Error or wrong format reading file: "
                             << argv[2] << endl;
                        return 1;
                    }
                }
                else
                {
                    cerr << "Error opening file: " << argv[2] << endl;
                    return 1;
                }
                
                ifstream originalDocument(argv[3]);
                ofstream compressedDocument(argv[4], ios::out | ios::binary);
                if (originalDocument.good() && compressedDocument.good())
                {
                    theTree.compress(originalDocument, compressedDocument);
                    if (originalDocument.eof() && compressedDocument.good())
                    {
                        originalDocument.close();
                        compressedDocument.close();
                        return 0;
                    }
                    else if (! originalDocument.eof())
                    {
                        cerr << "Error reading file: " << argv[3] << endl;
                        return 1;
                    }
                    else
                    {
                        cerr << "Error writing file: " << argv[4] << endl;
                        return 1;
                    }
                }
                else if (originalDocument.fail())
                {
                    cerr << "Error opening file: " << argv[3] << endl;
                    return 1;
                }
                else 
                {
                    cerr << "Error creating file: " << argv[4] << endl;
                    return 1;
                }
            }
            else
            {
                usage();
                return 1;
            }
        
        
        case 'd':
        
            if (argc == 5)
            {
                ifstream treefile(argv[2], ios::in | ios::binary);
                if (treefile.good())
                {
                    theTree.read(treefile);
                    char expectedEOF;
                    treefile.get(expectedEOF);
                    if (treefile.eof())
                        treefile.close();
                    else
                    {
                        cerr << "Error or wrong format reading file: "
                             << argv[2] << endl;
                        return 1;
                    }
                }
                else
                {
                    cerr << "Error opening file: " << argv[2] << endl;
                    return 1;
                }
                
                ifstream compressedDocument(argv[3], ios::in | ios::binary);
                ofstream decompressedDocument(argv[4]);
                if (compressedDocument.good() && decompressedDocument.good())
                {
                    theTree.decompress(compressedDocument, decompressedDocument);
                    if (compressedDocument.good() && decompressedDocument.good())
                    {
                        char junk;
                        compressedDocument.get(junk);   // Force eof
                        if (compressedDocument.eof())
                        {
                            compressedDocument.close();
                            decompressedDocument.close();
                            return 0;
                        }
                        else
                        {
                            cerr << "Wrong format reading file: " << argv[3] << endl;
                            return 1;
                        }
                    }
                    else if (! compressedDocument.good())
                    {
                        cerr << "Error reading file: " << argv[3] << endl;
                        return 1;
                    }
                    else
                    {
                        cerr << "Error writing file: " << argv[4] << endl;
                        return 1;
                    }
                }
                else if (compressedDocument.good())
                {
                    cerr << "Error opening file: " << argv[3] << endl;
                    return 1;
                }
                else 
                {
                    cerr << "Error creating file: " << argv[4] << endl;
                    return 1;
                }
            }
            else
            {
                usage();
                return 1;
            }
            
        default:
        
            usage();
            return 1;
    }
}

    

