//
//  terrain.cpp
//  HW3
//
//  Created by HyunChoi on 2020/03/09.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#include "terrain.hpp"
#include <stdlib.h>

terrain::terrain(int size) {
    this->width = size;
    this->height = size;
    std::vector<int> pos;
    
    map = new float* [width];
    
    for(int i=0; i < width; i++) {
        map[i] = new float [height];
    }
    


    // set four corners
    int x = 0;
    int y = 0;
    
    map[x][y] = 10.0f;
    map[width-x-1][y] = 10.0f;
    map[x][height-y-1] = 10.0f;
    map[height-x-1][width-y-1] = 10.0f;
    
    
    
}

void terrain::d_step(int x, int y, int cnt, int r) {
    float avg;

    
    avg = map[x][y] + map[width-x-1][y] + map[x][height-y-1] +map[height-x-1][width-y-1];
    avg /= 4;
    
    c_x = (x + width-x-1) / 2;
    c_y = (y + height-y-1) / 2;
    
    map[c_x][c_y] = avg;
    
}


void terrain::s_step(int x, int y, int cnt, int r) {
    
}
