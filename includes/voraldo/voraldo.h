#include "../block/block.h" //my code to hold a block of voxels
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

#include <iostream>
#include <string>

using vec = linalg::aliases::double3;
using mat = linalg::aliases::double4x4;
using veContainer = std::vector<vec>;

struct RGB{ //since this file is going to be where the image output/display functions are handled,
			//I need a way to hold a tuple for RGB that I can use as a return type from functions
	int red;
	int green;
	int blue;	//yes, it did occur to me that it is somewhat ironic that the float3 alias definition is right above it
					//however, I think it is also somewhat useful to have these as integers right now.
};

class Voraldo_object{
	public:
		Voraldo_object();					//default constructor leaves block data unassigned
		Voraldo_object(int x,int y,int z); 	//create block from x,y,z dimensions
	/*	Voraldo_object(std::string filename);	//create empty block from dimensions kept in file*/

		void dump_that_shit_in_a_file(std::string filename); //output contents of the block to a file
		void load_block_from_file(std::string filename);		//delete old block, read file contents, declare new block and fill

		//display_and_save_image_to_file(std::string filename); 
			//slap extension onto filename and output using CImg functions

		void draw(std::string drawtype, double* parameters);
		void display(std::string display_type);

		int get_number_of_cells(){return data->get_x_res()*data->get_y_res()*data->get_z_res();}

		RGB get_color_for_state(int state); //this function requires no knowledge of what palette is currently set
		void set_palette(int new_palette);	//this function sets the value of current_palette to another value

	private:
		Block* data;
		int current_palette;
		std::string savetype;

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