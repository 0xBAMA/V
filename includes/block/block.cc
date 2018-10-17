//this file contains the implementation of the block class

#include "block.h"
#include <cstdlib>

Block::Block(int x, int y, int z){
	//this function initializes a block of the specified
	//dimensions, and fills it with zeroes

	//the block will have x * y * z voxels, but the cells
	//will be indexed from 0 to x-1, 0 to y-1, 0 to z-1

	x_res = x;
	y_res = y;
	z_res = z;

	data = new int**[x_res];
	for(int i = 0; i < x_res; i++){
		data[i] = new int*[y_res];
		for(int j = 0; j < y_res; j++){
			data[i][j] = new int[z_res];
			for(int k = 0; k < z_res; k++){
				int randcheck = rand()%696;
				if(randcheck == 69){
					data[i][j][k] = rand()%256;
				}
				//data[i][j][k] = rand()%256;
				//data[i][j][k] = 0;
			}
		}
	}
}