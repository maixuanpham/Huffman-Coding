//
//  main.cpp
//  Project 3 Part 2
//
//  Created by Mai Pham on 5/7/18.
//  Copyright © 2018 Mai Pham. All rights reserved.
//

#include "HuffmanCoding.h"
#include <iostream>
using namespace std;

int main() {
    HuffmanCoding moreMoney("moneyIn.txt", "compression");
    HuffmanCoding moreMoneyNow("moneyOut.txt", "decompression");
    return 0;
}

