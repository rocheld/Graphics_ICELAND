//
//  terrain.cpp
//  HW3
//
//  Created by HyunChoi on 2020/03/09.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#include "Terrain.hpp"
#include <stdlib.h>

using namespace std;

Terrain::Terrain(int size) {
    this->width = size-1;
    this->height = size-1;
    this->squareLen = size -1;

    // set four corners
    int x = width -1;
    int y = height - 1;
    
    vector<float> temp;
    
    for(int i=0; i < size; i++) {
        h_map.push_back(temp);
        for(int j=0; j < size; j++) {
            h_map[i].push_back(0.0f);
        }
    }
    
    // Initialize
    h_map[0][0] = 10.0f;
    h_map[width][0] = 10.0f;
    h_map[0][height] = 10.0f;
    h_map[height][width] = 10.0f;
    c_x = width;
    c_y = height;
    while(squareLen > 1) {
        
    }
    
}

void Terrain::d_step(int x, int y, int size, int r) {
    
    float avg;

    avg = h_map[x][y] + h_map[width-x-1][y] + h_map[x][height-y-1] +h_map[height-x-1][width-y-1];
    avg /= 4;
    
    c_x = x / 2;
    c_y = c_x;
    size = size / 2;
    
    h_map[c_x][c_y] = avg + r;
    
    s_step(c_x,c_y, size, 1);
}


void Terrain::s_step(int x, int y, int size, int r) {
    
    if(x-size >= 0) {
        
    }
    
    if(x+size <= width) {
        
    }
    
    if(y-size >= 0) {
        
    }
    
    if(y+size <= height) {
        
    }
    
}
