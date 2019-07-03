//
//  HuffmanCoding.h
//  Project 3 Part 2
//
//  Created by Mai Pham on 5/8/18.
//  Copyright © 2018 Mai Pham. All rights reserved.
//

#ifndef HuffmanCoding_h
#define HuffmanCoding_h

#include <iostream>
#include <fstream>
#include <queue>
#include <string>
using namespace std;

struct node {
    int freq = 0;
    char letter = NULL;
    node *leftChild;
    node *rightChild;
    node (int f, char l)    {
        freq = f;
        letter = l;
        leftChild = NULL;
        rightChild = NULL;
    }
    node (node *lc, node *rc)   {
        freq = lc -> freq + rc ->freq;
        letter = NULL;
        leftChild = lc;
        rightChild = rc;
    }
};

struct comp {
    bool operator ()(const node* a, const node* b) {
        return a->freq > b->freq;
    }
};

class HuffmanCoding {
private:
    string text;
    int frequency[128];
    string bits[128];
public:
    HuffmanCoding(string file, string type )    {
        text = "";                                      // initialize private data
        for (int i = 0; i < 128; i++)   {
            frequency[i] = 0;
            bits[i] = '$';
        }
        if (type == "compression")                      // select type of work
            compression(file);
        else
            decompression(file);
    }
    
    void compression(string file)   {
        char t;
        priority_queue<node*, vector<node*>, comp> pq;
        
        // read in characters from text file while construct
        // frequency table
        ifstream textFileIn;
        textFileIn.open(file);
        if(!textFileIn.is_open())
            cout << "No text file found. " << endl;
        while (textFileIn >> noskipws >> t)   {
            text = text + t;
            frequency[t]++;
        }
        frequency[13] = 0;
        //cout << text << endl;
        
        // create individual node for each chars
        for (int i = 0; i < 128; i++)   {
            if (frequency[i] > 0)   {
                //cout << i << " - ";
                pq.push(new node (frequency[i], i));
            }
        }
        
        // combine the two smallest nodes and create a new one for that
        while (pq.size() > 1)   {
            node *leftChild = pq.top();
            pq.pop();
            node *rightChild = pq.top();
            pq.pop();
            pq.push(new node(leftChild, rightChild));
        }
        
        // compression the code and output into file
        // textFileOut << "Char\tFreq\tBits" << endl;
        ofstream textFileOut("moneyCompOut.txt");
        compressionCode(pq.top(), "", textFileOut);
        printData(textFileOut);
    }
    
    void compressionCode(node *root, string code, ofstream &textFileOut)   {
        if (!root)
            return;
        if (!root->leftChild && !root->rightChild)  {
            bits[root->letter] = code;
            // textFileOut << root->letter << "\t" << root->freq << "\t" << code << endl;
        }
        compressionCode(root->leftChild, code + '0', textFileOut);
        compressionCode(root->rightChild, code + '1', textFileOut);
    }
    
    void printData(ofstream &textFileOut)  {
        int currentBits = 0, total = 0;
        
        for (int i = 0; i < 128; i++)   {
            if (frequency[i] > 0)  {
                //cout << "lenght " << code.length() << endl;
                //cout << "frq " << frequency[i] << endl;
                //cout << "total bits " << currentBits << endl;
                //cout << bits[i] << endl;
                //textFileOut << i << "\t";
                textFileOut << static_cast<char>(i) << "\t" << frequency[i] << "\t" << bits[i] << endl;
                currentBits = frequency[i] * bits[i].length();
                total += currentBits;
            }
        }
        textFileOut << "************************" << endl;
        textFileOut << "Number of characters: " << text.length()-1 << endl;
        textFileOut << "Number of bits: " << total << endl;
        for (int i = 1; i < text.length(); i++)
            textFileOut << bits[text[i]];
    }
    
    void decompression(string file) {
        string code;
        string pattern;
        
        ifstream fileDecompIn;
        fileDecompIn.open(file);
        if(!fileDecompIn.is_open())
            cout << "No text file found. " << endl;
        
        getline(fileDecompIn, text);
        while (text[0] != '*')   {
            string b = text.substr(4, text.length());
            char c = text[0];
            int n = text[2] - '0';
            bits[c] = b;
            frequency[c] = n;
            //cout << c << "\t" << n << "\t" << b << endl;
            getline(fileDecompIn, text);
        }
        while(!fileDecompIn.eof())    {
            // getline(fileDecomp, text);
            fileDecompIn >> text;
        }
        
        ofstream fileDecompOut("moneyDecompOut.txt");
        //cout << text << endl;
        int i = 0;
        while (i < text.length())   {
            for (int j = 0; j < 128; j++)   {
                if (bits[j] != "$") {
                    code = bits[j];
                    //cout << "current matching code = " << code << endl;
                    //cout << code.length() << endl;
                    int a = 0;
                    while (a < code.length())   {
                        if (text[i+a] == code[a])
                            a++;
                        else
                            break;
                    }
                    //cout << a << endl;
                    if (a == code.length())  {
                        pattern = pattern + static_cast<char>(j);
                        //cout << "current pattern = " << pattern << endl;
                        i = i + a;
                    }
                }
            }
            // cout << "current i" << i << endl;
        }
        fileDecompOut << pattern << endl;
    }
};

#endif /* HuffmanCoding_h */
