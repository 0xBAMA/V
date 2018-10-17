//this file contains declarations of the block class

#include <iostream>
using namespace std;

class Block{
	public:
		//Block(string filename); //constructor to load from file
		Block(int x,int y,int z);

		int get_x_res(){return x_res;}
		int get_y_res(){return y_res;}
		int get_z_res(){return z_res;}

		void set_data_by_index(int x,int y,int z,int set){

			//validate the input - make sure you are in the block
			bool x_valid = x < x_res && x >= 0;
			bool y_valid = y < y_res && y >= 0;
			bool z_valid = z < z_res && z >= 0;

			//all dimensions valid, do as the user asks
			if(x_valid && y_valid && z_valid){
				if(set <= 256){
					data[x][y][z] = set;
				}else{
					data[x][y][z] = 256;
				}
			}else{
				cout << endl << "Invalid index for set_data_by_index()" << endl;
				cout << "you used " << std::to_string(x) << " which should be between 0 and ";
				cout << std::to_string(x_res) << endl;

				cout << "you used " << std::to_string(y) << " which should be between 0 and ";
				cout << std::to_string(y_res) << endl;

				cout << "you used " << std::to_string(z) << " which should be between 0 and ";
				cout << std::to_string(z_res) << endl;
			}

			return;
		}

		int get_data_by_index(int x,int y,int z){

			//validate the input
			bool x_valid = x < x_res && x >= 0;
			bool y_valid = y < y_res && y >= 0;
			bool z_valid = z < z_res && z >= 0;

			if(x_valid && y_valid && z_valid){
				return(data[x][y][z]);
			}else{
				cout << endl << "Invalid index for get_data_by_index()" << endl;
				return(0); //if it is outside, return an empty state
			}
		}

	private:
		int x_res; //how many voxels along x axis
		int y_res; //how many voxels along y axis
		int z_res; //how many voxels along z axis

		int ***data; //pointer that holds the 3D array data for the block
};