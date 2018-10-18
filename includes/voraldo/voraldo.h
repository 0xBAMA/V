#include "../CImg.h"		//single-header library for image display and saving								License below
#include "../json.hpp"  	//single-header library for highly flexible structured output file formatting		MIT License
#include "../linalg.h"      //single-header library for linear algebra (vectors)								Public Domain License

//  [..         [..    [....     [.......          [.       [..      [.....        [....     
//   [..       [..   [..    [..  [..    [..       [. ..     [..      [..   [..   [..    [..  
//    [..     [..  [..        [..[..    [..      [.  [..    [..      [..    [..[..        [..
//     [..   [..   [..        [..[. [..         [..   [..   [..      [..    [..[..        [..
//      [.. [..    [..        [..[..  [..      [...... [..  [..      [..    [..[..        [..
//       [....       [..     [.. [..    [..   [..       [.. [..      [..   [..   [..     [.. 
//        [..          [....     [..      [..[..         [..[........[.....        [....     
//                                                                                          

//Cimg license - from Cimg.eu - CImg is a free, open-source library distributed under the CeCILL-C (close to the GNU LGPL) or CeCILL (compatible with the GNU GPL) licenses. 
//It can be used in commercial applications. 

#include <cstdlib>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>

using vec = linalg::aliases::double3;
using mat = linalg::aliases::double4x4;
using intvec = linalg::aliases::int3;
using veContainer = std::vector<vec>;



//   ▄▄▄▄    ██▓     ▒█████   ▄████▄   ██ ▄█▀
//  ▓█████▄ ▓██▒    ▒██▒  ██▒▒██▀ ▀█   ██▄█▒ 
//  ▒██▒ ▄██▒██░    ▒██░  ██▒▒▓█    ▄ ▓███▄░ 
//  ▒██░█▀  ▒██░    ▒██   ██░▒▓▓▄ ▄██▒▓██ █▄ 
//  ░▓█  ▀█▓░██████▒░ ████▓▒░▒ ▓███▀ ░▒██▒ █▄
//  ░▒▓███▀▒░ ▒░▓  ░░ ▒░▒░▒░ ░ ░▒ ▒  ░▒ ▒▒ ▓▒
//  ▒░▒   ░ ░ ░ ▒  ░  ░ ▒ ▒░   ░  ▒   ░ ░▒ ▒░
//   ░    ░   ░ ░   ░ ░ ░ ▒  ░        ░ ░░ ░ 
//   ░          ░  ░    ░ ░  ░ ░      ░  ░   
//        ░                  ░               
class Block{
	public:
		//Block(std::string filename); //constructor to load from file
		Block(int x,int y,int z);

		int get_x_res(){return x_res;}
		int get_y_res(){return y_res;}
		int get_z_res(){return z_res;}


		//This kind of access is still valuable - I want to keep this function intact, for simple output.
		//I will be doing some kind of mapping here so that I can have the block centered at the origin,
		//rather than having its corner butted up against the origin.

		//There will be some work to do to convert this, but I want to make the array one-dimensional to
		//decrease the overhead there is with the memory usage. In essense, I want to maintain the x,y,z
		//indexing interface, but within those functions get/set, decode that into a location in a long
		//1-dimensional array. Not sure exactly what this will look like
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
				std::cout << std::endl << "Invalid index for set_data_by_index()" << std::endl;
				std::cout << "you used " << std::to_string(x) << " which should be between 0 and ";
				std::cout << std::to_string(x_res) << std::endl;

				std::cout << "you used " << std::to_string(y) << " which should be between 0 and ";
				std::cout << std::to_string(y_res) << std::endl;

				std::cout << "you used " << std::to_string(z) << " which should be between 0 and ";
				std::cout << std::to_string(z_res) << std::endl;
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
				std::cout << std::endl << "Invalid index for get_data_by_index()" << std::endl;
				return(0); //if it is outside, return an empty state
			}
		}

		int get_data_at_point(vec test_point);

	private:
		int x_res; //how many voxels along x axis
		int y_res; //how many voxels along y axis
		int z_res; //how many voxels along z axis

		int ***data; //pointer that holds the 3D array data for the block
};

//   ██▀███    ▄████  ▄▄▄▄   
//  ▓██ ▒ ██▒ ██▒ ▀█▒▓█████▄ 
//  ▓██ ░▄█ ▒▒██░▄▄▄░▒██▒ ▄██
//  ▒██▀▀█▄  ░▓█  ██▓▒██░█▀  
//  ░██▓ ▒██▒░▒▓███▀▒░▓█  ▀█▓
//  ░ ▒▓ ░▒▓░ ░▒   ▒ ░▒▓███▀▒
//    ░▒ ░ ▒░  ░   ░ ▒░▒   ░ 
//    ░░   ░ ░ ░   ░  ░    ░ 
//     ░           ░  ░      
//                         ░
struct RGB{ //since this file is going to be where the image output/display functions are handled,
			//I need a way to hold a tuple for RGB that I can use as a return type from functions
	int red;
	int green;
	int blue;	//yes, it did occur to me that it is somewhat ironic that the float3 alias definition is right above it
					//however, I think it is also somewhat useful to have these as integers right now.
};


//   ██▒   █▓ ▒█████   ██▀███   ▄▄▄       ██▓    ▓█████▄  ▒█████  
//  ▓██░   █▒▒██▒  ██▒▓██ ▒ ██▒▒████▄    ▓██▒    ▒██▀ ██▌▒██▒  ██▒
//   ▓██  █▒░▒██░  ██▒▓██ ░▄█ ▒▒██  ▀█▄  ▒██░    ░██   █▌▒██░  ██▒
//    ▒██ █░░▒██   ██░▒██▀▀█▄  ░██▄▄▄▄██ ▒██░    ░▓█▄   ▌▒██   ██░
//     ▒▀█░  ░ ████▓▒░░██▓ ▒██▒ ▓█   ▓██▒░██████▒░▒████▓ ░ ████▓▒░
//     ░ ▐░  ░ ▒░▒░▒░ ░ ▒▓ ░▒▓░ ▒▒   ▓▒█░░ ▒░▓  ░ ▒▒▓  ▒ ░ ▒░▒░▒░ 
//     ░ ░░    ░ ▒ ▒░   ░▒ ░ ▒░  ▒   ▒▒ ░░ ░ ▒  ░ ░ ▒  ▒   ░ ▒ ▒░ 
//       ░░  ░ ░ ░ ▒    ░░   ░   ░   ▒     ░ ░    ░ ░  ░ ░ ░ ░ ▒  
//        ░      ░ ░     ░           ░  ░    ░  ░   ░        ░ ░  
//       ░                                        ░           
class Voraldo_object{
	public:
		Voraldo_object();					//default constructor leaves block data unassigned
		Voraldo_object(int x,int y,int z); 	//create block from x,y,z dimensions
	/*	Voraldo_object(std::std::string filename);	//create empty block from dimensions kept in file*/

		void dump_that_shit_in_a_file(std::string filename); //output contents of the block to a file
		void load_block_from_file(std::string filename);		//delete old block, read file contents, declare new block and fill

		//display_and_save_image_to_file(std::std::string filename); 
			//slap extension onto filename and output using CImg functions

		void draw(std::string drawtype, double* parameters);
		void display(std::string display_type);

		int get_number_of_cells(){return data->get_x_res()*data->get_y_res()*data->get_z_res();}

		RGB get_color_for_state(int state); //this function requires no knowledge of what palette is currently set
		void set_palette(int new_palette);	//this function sets the value of current_palette to another value

	private:
		Block* data;
		int current_palette;
		std::string savetype; //bmp
		std::string filetype; //load/save block shit yes I KNOW IT'S AMBIGUOUS

		//Private draw functions -
		//The "draw" function will call these.

		void draw_line(vec origin, vec dir, double length, int state);

		void draw_rectangular_prism(vec mintuple, vec maxtuple, int state);

		void draw_sphere(vec origin, double radius, int state);

		void draw_ellipsoid(vec origin, vec radii, int state);

		void draw_triangle(vec point1, vec point2, vec point3, int state);

		void draw_cylinder(vec origin, vec direction, double radius, double length, int state);

		void draw_capsule(vec origin, vec direction, double radius, double length, int state);

		void draw_dotted_capsule(vec point1, vec point2, double length_of_dots, double length_of_dashes, double radius, int state);
};



