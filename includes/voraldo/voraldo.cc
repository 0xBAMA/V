#include "../voraldo/voraldo.h"


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

Voraldo_object::Voraldo_object(){
	return; //
}

//-------------------------------------------------------------------------------------------------

Voraldo_object::Voraldo_object(int x,int y,int z){

//  ██╗    ███╗   ██╗    ██╗    ████████╗
//  ██║    ████╗  ██║    ██║    ╚══██╔══╝
//  ██║    ██╔██╗ ██║    ██║       ██║   
//  ██║    ██║╚██╗██║    ██║       ██║   
//  ██║    ██║ ╚████║    ██║       ██║   
//  ╚═╝    ╚═╝  ╚═══╝    ╚═╝       ╚═╝

//-------------------------------------------------------------------------------------------------
	
	//I'm really only using this to generate an empty block - sometimes that's neccesary
	//It declares an "empty" block (allocated but all state 0) with user defined x,y,z extents

	data = new Block(x,y,z); 

	current_palette = 0;

	savetype = "bmp";
	filetype = "uncompressed, using integers";
}

//-------------------------------------------------------------------------------------------------

void Voraldo_object::load_block_from_file(std::string filename){

//  ██╗          ██████╗      █████╗     ██████╗ 
//  ██║         ██╔═══██╗    ██╔══██╗    ██╔══██╗
//  ██║         ██║   ██║    ███████║    ██║  ██║
//  ██║         ██║   ██║    ██╔══██║    ██║  ██║
//  ███████╗    ╚██████╔╝    ██║  ██║    ██████╔╝
//  ╚══════╝     ╚═════╝     ╚═╝  ╚═╝    ╚═════╝
//-------------------------------------------------------------------------------------------------


	//code to load file into block
	//check dimensions first and then load it all in
	int tempx = 0;
	int tempy = 0;  //variables for incoming json values
	int tempz = 0;

	nlohmann::json j; //declare json object
	std::fstream f;	  //declare fstream object

	f.open(filename); //open the user's file

	f >> j;			  //read in all the content of that file

	tempx = j.value("xdim",0);
	tempy = j.value("ydim",0); //take some specific values from the json input file
	tempz = j.value("zdim",0);	//the formatting is j.value("key name", "default value if key not found")

	data = new Block(tempx,tempy,tempz); //define the block using those values

	current_palette = j.value("palette",0);	

	savetype = j.value("image_output_type","bmp");

	std::string filetype = j.value("filetype","not found"); 



	if(filetype == "uncompressed, using integers"){

		std::string current_slice;
		std::string current_row;

		unsigned char dot_holder; //strips the std::string of the periods

		int value;

		for(int i = 0; i < tempz; i++){				//slices - i = z


			current_slice = j.value("slice[" + std::to_string(i) + "]","load failed");
			nlohmann::json j2 = nlohmann::json::parse(current_slice);	
			//grab the std::string, and recursively parse it as a JSON object
			//within the JSON object loaded from the input file.

			for(int j = 0; j < tempy; j++){			//rows 	 - j = y

				if(current_slice != "load failed");
				current_row = j2.value("row[" + std::to_string(j) + "]","load failed");
				std::stringstream ss;

				ss << current_row;	//FIFO style

				for(int k = 0; k < tempx; k++){		//cells  - k = x

					ss >> value;	//read out entries

					if(!ss.eof())
						ss >> dot_holder;	//take in the dot 

					data->set_data_by_index(k,j,i,value);
				}
			}
		}
	}else if(filetype == "compressed using unsigned characters instead of ints"){
		//to be implemented

		//but cast int to unsigned char before output - this is fine because we 
		//are only dealing with 0-255 as possible states.

	}else if(filetype == "compressed bitwise using unsigned long long ints"){
		//to be implemented

		//this breaks the data up into two pieces - The first piece is a compressed array that gives me boolean occupancy
		//data, for every cell in the array explicitly. The second piece is a 1 dimensional list of states, which will be
		//interpreted at the same time, we go through a deterministic traversal of the space, so the first cell encountered
		//will always be the first cell encountered, allowing it to be consistently interpreted the same way every time.

		//in the other representations, the state data is explicitly defined, and the occupancy data was defined implicitly
		//by the fact the the values stored are nonzero.

	}else if(filetype == "not found"){
		std::cout << "filetype information not included in save file." << std::endl;
	}
}

//-------------------------------------------------------------------------------------------------

void Voraldo_object::dump_that_shit_in_a_file(std::string filename){

//  ███████╗     █████╗     ██╗   ██╗    ███████╗
//  ██╔════╝    ██╔══██╗    ██║   ██║    ██╔════╝
//  ███████╗    ███████║    ██║   ██║    █████╗  
//  ╚════██║    ██╔══██║    ╚██╗ ██╔╝    ██╔══╝  
//  ███████║    ██║  ██║     ╚████╔╝     ███████╗
//  ╚══════╝    ╚═╝  ╚═╝      ╚═══╝      ╚══════╝
	//Compression - using the bits of an integer? ok here's the idea - keep occupancy information like this,
	//bits indicating whether or not there will be data there. Then in some kind of ordered format, 1 dimensional,
	//keep all the voxels that are indeed occupied, according to the compressed data. This could really, 
	//really reduce filesize.
//-------------------------------------------------------------------------------------------------

	//As is the way, we had some fun naming the save function.

	//The goal here is to use an object of the JSON class to output the data in a structured way.
	//This has a number of benefits, which I feel I can most concisely describe as having 
	//object-oriented input and output files. 

	nlohmann::json j;
	std::string slicestring;

	j["xdim"] = data->get_x_res();
	j["ydim"] = data->get_y_res();
	j["zdim"] = data->get_z_res();

	j["image_output_type"] = savetype;	//the basics are easy key value pairs
	j["palette"] = current_palette;

	for(int i = 0; i < data->get_z_res(); i++){	//the block data has more formatting i = z

		slicestring.clear();		//make sure there's nothing in the std::string for this iteration

		nlohmann::json j2; //this bitch gets declared every time you deal with a new slice

		for(int j = 0; j < data->get_y_res(); j++){ //j = y
			std::string rowstring;
			for(int k = 0; k < data->get_x_res(); k++){ //k = x

				int state_of_current_entry = data->get_data_by_index(k,j,i);

				if(state_of_current_entry > 255){
					std::cout << "clipped on output " << std::endl;
					state_of_current_entry = 255;
				}

				rowstring.append(std::to_string(state_of_current_entry));
				rowstring.append(".");
			}

			j2["row[" + std::to_string(j) + "]"] = rowstring;
		}

		slicestring = j2.dump();

		j["slice[" + std::to_string(i) + "]"] = slicestring;
	}

	std::fstream f;

	f.open(filename);

	f << j.dump(4); //the four is the number of spaces to indent in the output file

	f.close();

}

//-------------------------------------------------------------------------------------------------
// ▓█████▄  ██▓  ██████  ██▓███   ██▓    ▄▄▄     ▓██   ██▓
// ▒██▀ ██▌▓██▒▒██    ▒ ▓██░  ██▒▓██▒   ▒████▄    ▒██  ██▒
// ░██   █▌▒██▒░ ▓██▄   ▓██░ ██▓▒▒██░   ▒██  ▀█▄   ▒██ ██░
// ░▓█▄   ▌░██░  ▒   ██▒▒██▄█▓▒ ▒▒██░   ░██▄▄▄▄██  ░ ▐██▓░
// ░▒████▓ ░██░▒██████▒▒▒██▒ ░  ░░██████▒▓█   ▓██▒ ░ ██▒▓░
//  ▒▒▓  ▒ ░▓  ▒ ▒▓▒ ▒ ░▒▓▒░ ░  ░░ ▒░▓  ░▒▒   ▓▒█░  ██▒▒▒ 
//  ░ ▒  ▒  ▒ ░░ ░▒  ░ ░░▒ ░     ░ ░ ▒  ░ ▒   ▒▒ ░▓██ ░▒░ 
//  ░ ░  ░  ▒ ░░  ░  ░  ░░         ░ ░    ░   ▒   ▒ ▒ ░░  
//    ░     ░        ░               ░  ░     ░  ░░ ░     
//  ░                                             ░ ░     
//-------------------------------------------------------------------------------------------------
void Voraldo_object::display(std::string display_type){
	using namespace cimg_library;

	/*img(10,10,0) = 128.0;
	img(10,20,1) = 128.0;
	img(10,30,2) = 128.0;

	for(int i = 0; i < 16; i++){
		double temp[] = {4.0*i, 0.0, 4.0*i};
		img.draw_text(100+5*i, 100+5*i, "Hey Dogarino", temp);
	}*/


	//http://cimg.eu/reference/structcimg__library_1_1CImgDisplay.html
	//I want to do menus, within this window dealio. I think it'll be cool and a good way to do interactive modeling

	//that's for tomorrow. do something to implement keyboard commands, yeah, but menus


	if(display_type == "simple"){

//		^ y axis
// 		|
// 		|
// 		|
// 		|__________> x axis
// 		/
// 	   /
// 	  v z axis

	//as we move positively in the block's x axis, we move positively in the x axis in image coordinates.
	//as we move positively in the block's y axis, we move negatively in the y axis in image coordinates.
	//as we move positively in the block's z axis, we move positively in the y axis and also negatively in the x axis.

	//this is a simple implementation of the view, for testing purposes.

//	------------------------->	0 to 512, x coord
//	|
//	|
//	|			x - start point is (x=256,y=256)
//	|
//	|
//	v 	0 to 512, ycoord

		CImg<int> img(800,800,1,3,0);  //declare image with three colors per pixel, one channel. 
		//full of zero values, which are doubles, for no real reason

		int curr_x = 400;
		int curr_y = 400;

		for(int y = 0; y < data->get_y_res(); y++){	
			for(int x = 0; x < data->get_x_res(); x++){
				for(int z = 0; z < data->get_z_res(); z++){

					int state = data->get_data_by_index(x,y,z);

					RGB temp = get_color_for_state(state);

					if((temp.red != 0)||(temp.green != 0)||(temp.blue != 0)){
						img(curr_x,curr_y,0) = temp.red;
						img(curr_x,curr_y,1) = temp.green;
						img(curr_x,curr_y,2) = temp.blue;
					}

					if(state == 118){
						img(curr_x,curr_y,0) = 255-y;
						img(curr_x,curr_y,1) = 255-y;
						img(curr_x,curr_y,2) = 255-y;
					}

					curr_x--;
					curr_y++;
				}
				curr_x += data->get_z_res();
				curr_y -= data->get_z_res();

				curr_x++;
			}
			curr_x -= data->get_x_res();
			curr_y--;
		}

		img.save_png("example.png");
		img.save_bmp("example.bmp");
		img.display("VORALDO");             // Display the image in a display window.
	}else if(display_type == "raycast"){
		//This will require some time.

		//the information about the camera
		//	>position,
	}
}


//-------------------------------------------------------------------------------------------------

void Voraldo_object::draw(std::string drawtype, double* parameters){

//  ██████╗     ██████╗      █████╗     ██╗    ██╗
//  ██╔══██╗    ██╔══██╗    ██╔══██╗    ██║    ██║
//  ██║  ██║    ██████╔╝    ███████║    ██║ █╗ ██║
//  ██║  ██║    ██╔══██╗    ██╔══██║    ██║███╗██║
//  ██████╔╝    ██║  ██║    ██║  ██║    ╚███╔███╔╝
//  ╚═════╝     ╚═╝  ╚═╝    ╚═╝  ╚═╝     ╚══╝╚══╝

//-------------------------------------------------------------------------------------------------

		//10/12 - would a 'drawlist' or something be useful? we have JSON to work with

	//this is the 'infrastructure' of the drawing functionality. Instead of calling draw_sphere(),etc,etc, we are going
	//to have this handled by a single function with more flexibility. This way I can make all kinds of primitives
	//and not have to worry about the use of the Voraldo class exploding and becoming intractibly complicated.

	//I see the benefit here as a fusion of geometry and texture. This is somewhat novel, to me at least. Here, draw 
	//has a somewhat different meaning than it may in some other graphical contexts - in OpenGL, a draw call will
	//draw primitives into a framebuffer, which is effectively what we are doing here. The framebuffer being drawn 
	//into is now three dimensional rather than two dimensional. In our context, this is distinct from the display
	//function. I am somewhat fuzzy on how this is handled specifically in OpenGL - I believe there is a function
	//that moves the image data from the framebuffer to the screen. In any event, this is how we're handling it.

	//draw_from_file(std::string filename)? what would this be like - draw calls spelled out sequentially? good idea or no?

	int drawtype_int;

	std::string s[] {"point","line","rectangular prism","sphere","elipsoid","triangle","cylinder","capsule","dotted capsule"};

	int i = 0;	//we dropped the loop control variable in the conversion to a range-based for loop. 
	//Wonder if there's a better solution.

	for(auto ref_string : s){	//A switch statement doesn't support std::strings as control variables. I've only just seen this type of
	 	if (ref_string == drawtype){	// range-based for-loop. I'm not going to remember the number for each drawing function -
	 		drawtype_int = i;			//as a result, this seemed like a concise way to translate.
	 	}
	 	i++;
	 }

	switch(drawtype_int){
//-------------------------------------------------------------------------------------------------
		case 0:	{	
					//  ██████╗  ██████╗ ██╗███╗   ██╗████████╗
					//  ██╔══██╗██╔═══██╗██║████╗  ██║╚══██╔══╝
					//  ██████╔╝██║   ██║██║██╔██╗ ██║   ██║   
					//  ██╔═══╝ ██║   ██║██║██║╚██╗██║   ██║   
					//  ██║     ╚██████╔╝██║██║ ╚████║   ██║   
					//  ╚═╝      ╚═════╝ ╚═╝╚═╝  ╚═══╝   ╚═╝
		//DRAW A POINT [x,y,z,state] - trivial, uses the block set_data_by_index function.
			int intx = int(parameters[0]);
			int inty = int(parameters[1]);
			int intz = int(parameters[2]);

			int state = int(parameters[3]);

			data->set_data_by_index(intx, inty, intz, state);
			break;
}
//-------------------------------------------------------------------------------------------------
		case 1:	{	
					//  ██╗     ██╗███╗   ██╗███████╗
					//  ██║     ██║████╗  ██║██╔════╝
					//  ██║     ██║██╔██╗ ██║█████╗  
					//  ██║     ██║██║╚██╗██║██╔══╝  
					//  ███████╗██║██║ ╚████║███████╗
					//  ╚══════╝╚═╝╚═╝  ╚═══╝╚══════╝
		//DRAW A LINE [x,y,z for origin, x,y,z for direction, state]
			vec origin(parameters[0], parameters[1], parameters[2]); 
			vec dir(parameters[3], parameters[4], parameters[5]);



			double length = parameters[6];
			int state = int(parameters[7]);

			draw_line(origin, dir, length, state);
			break;
		}
//-------------------------------------------------------------------------------------------------
		case 2:{		
					//  ██████╗ ███████╗ ██████╗████████╗ █████╗ ███╗   ██╗ ██████╗ ██╗   ██╗██╗      █████╗ ██████╗ 
					//  ██╔══██╗██╔════╝██╔════╝╚══██╔══╝██╔══██╗████╗  ██║██╔════╝ ██║   ██║██║     ██╔══██╗██╔══██╗
					//  ██████╔╝█████╗  ██║        ██║   ███████║██╔██╗ ██║██║  ███╗██║   ██║██║     ███████║██████╔╝
					//  ██╔══██╗██╔══╝  ██║        ██║   ██╔══██║██║╚██╗██║██║   ██║██║   ██║██║     ██╔══██║██╔══██╗
					//  ██║  ██║███████╗╚██████╗   ██║   ██║  ██║██║ ╚████║╚██████╔╝╚██████╔╝███████╗██║  ██║██║  ██║
					//  ╚═╝  ╚═╝╚══════╝ ╚═════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝
					//                                                                                               
					//  ██████╗ ██████╗ ██╗███████╗███╗   ███╗                                                       
					//  ██╔══██╗██╔══██╗██║██╔════╝████╗ ████║                                                       
					//  ██████╔╝██████╔╝██║███████╗██╔████╔██║                                                       
					//  ██╔═══╝ ██╔══██╗██║╚════██║██║╚██╔╝██║                                                       
					//  ██║     ██║  ██║██║███████║██║ ╚═╝ ██║                                                       
					//  ╚═╝     ╚═╝  ╚═╝╚═╝╚══════╝╚═╝     ╚═╝ 
		//DRAW A RECTANGULAR PRISM [minx,miny,minz,maxx,maxy,maxz,state]

			vec mintuple(parameters[0], parameters[1], parameters[2]);
			vec maxtuple(parameters[3], parameters[4], parameters[5]);

			int state = int(parameters[6]);

			draw_rectangular_prism(mintuple, maxtuple, state);
			break;
		}
//-------------------------------------------------------------------------------------------------
		case 3:	{	
					//  ███████╗██████╗ ██╗  ██╗███████╗██████╗ ███████╗
					//  ██╔════╝██╔══██╗██║  ██║██╔════╝██╔══██╗██╔════╝
					//  ███████╗██████╔╝███████║█████╗  ██████╔╝█████╗  
					//  ╚════██║██╔═══╝ ██╔══██║██╔══╝  ██╔══██╗██╔══╝  
					//  ███████║██║     ██║  ██║███████╗██║  ██║███████╗
					//  ╚══════╝╚═╝     ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝
		//DRAW A SPHERE [x,y,z,radius,state]

			vec origin(parameters[0], parameters[1], parameters[2]);
			double radius = parameters[3];
			int state = int(parameters[4]);

			draw_sphere(origin, radius, state);
			break;
		}
//-------------------------------------------------------------------------------------------------
		case 4:	{	
					//  ███████╗██╗     ██╗     ██╗██████╗ ███████╗ ██████╗ ██╗██████╗ 
					//  ██╔════╝██║     ██║     ██║██╔══██╗██╔════╝██╔═══██╗██║██╔══██╗
					//  █████╗  ██║     ██║     ██║██████╔╝███████╗██║   ██║██║██║  ██║
					//  ██╔══╝  ██║     ██║     ██║██╔═══╝ ╚════██║██║   ██║██║██║  ██║
					//  ███████╗███████╗███████╗██║██║     ███████║╚██████╔╝██║██████╔╝
					//  ╚══════╝╚══════╝╚══════╝╚═╝╚═╝     ╚══════╝ ╚═════╝ ╚═╝╚═════╝
		//DRAW AN ELIPSOID [x0,y0,z0 for the offset for the center, then a,b,c for semi-axis lengths, and state]

						//this computation uses the x,y,z value for each cell tested - x is the x coordinate of the cell, etc
						//the equation for the elipsoid is ((x-x0)^2)/(a^2) + ((y-y0)^2)/(b^2) + ((z-z0)^2)/(c^2) = 1 
							//(or rather, less than or equal to one to be inside, greater than for outside)

							//multiple samples within the volume of the cell? corners? 3d quincunx kind of idea
							//as in, move up 0.5, test, then try 0.5 down, 0.5 right, 0.5 left, etc. This could get
							//some sense of what the surroundings of that point would be like. potentially useful? we just don't know

							//average it and if it's above a threshold, set it, otherwise don't...? 

							//Ref: http://mathworld.wolfram.com/Ellipsoid.html
			vec origin(parameters[0], parameters[1], parameters[2]);
			vec radii(parameters[3], parameters[4], parameters[5]);

			int state = int(parameters[6]);

			draw_ellipsoid(origin, radii, state);
			break;

			//this expression may be similar to the concept of a 'level set', less than 1 inside, greater than outside
			//I have recently seen some interesting material releated to this type of a concept - specifically, using 
			//them to represent surfaces within particle systems. The position of the particle is evaluated through this
			//type of equation, the value tells whether it is inside or outside. The one I saw was modelling erosion via
			//particles, it was in a CE thesis by a guy named Sebastian Weiss. In any event, the normals could be computed
			//from that expression to express how the particles would interact with the surface of that object.

			//I don't even know the definiton of level set. I need to do some research on that one.
}
//-------------------------------------------------------------------------------------------------							
		case 5:	{	
					//  ████████╗██████╗ ██╗ █████╗ ███╗   ██╗ ██████╗ ██╗     ███████╗
					//  ╚══██╔══╝██╔══██╗██║██╔══██╗████╗  ██║██╔════╝ ██║     ██╔════╝
					//     ██║   ██████╔╝██║███████║██╔██╗ ██║██║  ███╗██║     █████╗  
					//     ██║   ██╔══██╗██║██╔══██║██║╚██╗██║██║   ██║██║     ██╔══╝  
					//     ██║   ██║  ██║██║██║  ██║██║ ╚████║╚██████╔╝███████╗███████╗
					//     ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚══════╝
		//DRAW A TRIANGLE [xyz for point 1, xyz for point 2, xyz for point 3, state]
						//brackets to avoid something called crosses initialization 
			//(declaring a variable after a jump? fix with '{}' to limit scope))

			vec point1(parameters[0], parameters[1], parameters[2]);
			vec point2(parameters[3], parameters[4], parameters[5]);
			vec point3(parameters[6], parameters[7], parameters[8]);

			int state = int(parameters[9]);

			draw_triangle(point1, point2, point3, state);

			break;
		}

//-------------------------------------------------------------------------------------------------							
		case 6:	{	
					//   ██████╗██╗   ██╗██╗     ██╗███╗   ██╗██████╗ ███████╗██████╗ 
					//  ██╔════╝╚██╗ ██╔╝██║     ██║████╗  ██║██╔══██╗██╔════╝██╔══██╗
					//  ██║      ╚████╔╝ ██║     ██║██╔██╗ ██║██║  ██║█████╗  ██████╔╝
					//  ██║       ╚██╔╝  ██║     ██║██║╚██╗██║██║  ██║██╔══╝  ██╔══██╗
					//  ╚██████╗   ██║   ███████╗██║██║ ╚████║██████╔╝███████╗██║  ██║
					//   ╚═════╝   ╚═╝   ╚══════╝╚═╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚═╝  ╚═╝
		//DRAW ARBITRARY CYLINDER - I have yet to implement the function but I have
			//				a high level algorithm laid out pretty completely. I want to talk to Aaron Overton about it, 
			//				he had some very similar problems related to coordinate spaces on his homework that I looked at with 
			//				him like two days ago.

			//[vec for origin point, vec for direction, double for 2D distance i.e. radius, double for distance from plane, state]

			vec origin(parameters[0], parameters[1], parameters[2]);	//these define a plane
			vec direction(parameters[3], parameters[4], parameters[5]);		//this is normalized within the function


			double radius = parameters[6];
			double height = parameters[7];

			int state = int(parameters[8]);

			draw_cylinder(origin, direction, radius, height, state);

			break;
		}
//-------------------------------------------------------------------------------------------------							
		case 7:		{	
					//   ██████╗ █████╗ ██████╗ ███████╗██╗   ██╗██╗     ███████╗
					//  ██╔════╝██╔══██╗██╔══██╗██╔════╝██║   ██║██║     ██╔════╝
					//  ██║     ███████║██████╔╝███████╗██║   ██║██║     █████╗  
					//  ██║     ██╔══██║██╔═══╝ ╚════██║██║   ██║██║     ██╔══╝  
					//  ╚██████╗██║  ██║██║     ███████║╚██████╔╝███████╗███████╗
					//   ╚═════╝╚═╝  ╚═╝╚═╝     ╚══════╝ ╚═════╝ ╚══════╝╚══════╝

		//DRAW 'CAPSULE' - BASED UPON DISTANCE TO A LINE SEGMENT, defined analytically rather than as discrete points
							//I just ordered about 3000 pages worth of math books.

			vec origin(parameters[0], parameters[1], parameters[2]);	//these define a plane
			vec direction(parameters[3], parameters[4], parameters[5]);		//this is normalized within the function


			double radius = parameters[6];
			double height = parameters[7];

			int state = int(parameters[8]);

			draw_capsule(origin, direction, radius, height, state);

			break;
		}
//-------------------------------------------------------------------------------------------------							
		case 8:		{	
					//  ██████╗  ██████╗ ████████╗████████╗███████╗██████╗       
					//  ██╔══██╗██╔═══██╗╚══██╔══╝╚══██╔══╝██╔════╝██╔══██╗      
					//  ██║  ██║██║   ██║   ██║      ██║   █████╗  ██║  ██║      
					//  ██║  ██║██║   ██║   ██║      ██║   ██╔══╝  ██║  ██║      
					//  ██████╔╝╚██████╔╝   ██║      ██║   ███████╗██████╔╝      
					//  ╚═════╝  ╚═════╝    ╚═╝      ╚═╝   ╚══════╝╚═════╝       
					//                                                           
					//   ██████╗ █████╗ ██████╗ ███████╗██╗   ██╗██╗     ███████╗
					//  ██╔════╝██╔══██╗██╔══██╗██╔════╝██║   ██║██║     ██╔════╝
					//  ██║     ███████║██████╔╝███████╗██║   ██║██║     █████╗  
					//  ██║     ██╔══██║██╔═══╝ ╚════██║██║   ██║██║     ██╔══╝  
					//  ╚██████╗██║  ██║██║     ███████║╚██████╔╝███████╗███████╗
					//   ╚═════╝╚═╝  ╚═╝╚═╝     ╚══════╝ ╚═════╝ ╚══════╝╚══════╝

		//DRAW 'DOTTED CAPSULE' - BASED UPON DISTANCE TO ANY OF A SERIES OF POINTS ALONG AN ESTABLISHED LINE SEGMENT,
		//		WITH FINE ENOUGH GRADATIONS TO NOT LOOK LUMPY, plus parameters to tell how they will be spaced if you do want it 
		//		lumpy, or indeed, dashed.

			vec point1(parameters[0], parameters[1], parameters[2]);
			vec point2(parameters[3], parameters[4], parameters[5]);

			double length_of_dots = parameters[6]; 		//filled in spaces
			double length_of_dashes = parameters[7];	//negative spaces

			//my thought right now is to iterate along the line in small steps - once you have traveled a distance
			//equal to 'length_of_dots', at each small step adding an entry to the list of spheres, switching then to
			//not adding them each step and traveling a length equal to 'length_of_dashes' before starting to draw dots again

			//repeat till done
			double radius = parameters[8];
			int state = int(parameters[9]);

			draw_dotted_capsule(point1, point2, length_of_dots, length_of_dashes, radius, state);

			break;
		}
//-------------------------------------------------------------------------------------------------							
		case 9:		{	//...etc.
			//draw code here
			break;
		}
//-------------------------------------------------------------------------------------------------							
		case 10:	{	//...etc.
			//draw code here
			break;
		}

//-------------------------------------------------------------------------------------------------							
		case 11:	{	//...etc.
			//draw code here
			break;
		}

//-------------------------------------------------------------------------------------------------							
		case 12:	{	//...etc.
			//draw code here
			break;
		}

//-------------------------------------------------------------------------------------------------							
		case 13:	{	//...etc.
			//draw code here
			break;
		}
//-------------------------------------------------------------------------------------------------
		default:
			std::cout << "Invalid argument passed to draw function" << std::endl;
			break;

	}
}
//    █████▒█    ██  ███▄    █  ▄████▄  ▄▄▄█████▓ ██▓ ▒█████   ███▄    █   ██████ 
//  ▓██   ▒ ██  ▓██▒ ██ ▀█   █ ▒██▀ ▀█  ▓  ██▒ ▓▒▓██▒▒██▒  ██▒ ██ ▀█   █ ▒██    ▒ 
//  ▒████ ░▓██  ▒██░▓██  ▀█ ██▒▒▓█    ▄ ▒ ▓██░ ▒░▒██▒▒██░  ██▒▓██  ▀█ ██▒░ ▓██▄   
//  ░▓█▒  ░▓▓█  ░██░▓██▒  ▐▌██▒▒▓▓▄ ▄██▒░ ▓██▓ ░ ░██░▒██   ██░▓██▒  ▐▌██▒  ▒   ██▒
//  ░▒█░   ▒▒█████▓ ▒██░   ▓██░▒ ▓███▀ ░  ▒██▒ ░ ░██░░ ████▓▒░▒██░   ▓██░▒██████▒▒
//   ▒ ░   ░▒▓▒ ▒ ▒ ░ ▒░   ▒ ▒ ░ ░▒ ▒  ░  ▒ ░░   ░▓  ░ ▒░▒░▒░ ░ ▒░   ▒ ▒ ▒ ▒▓▒ ▒ ░
//   ░     ░░▒░ ░ ░ ░ ░░   ░ ▒░  ░  ▒       ░     ▒ ░  ░ ▒ ▒░ ░ ░░   ░ ▒░░ ░▒  ░ ░
//   ░ ░    ░░░ ░ ░    ░   ░ ░ ░          ░       ▒ ░░ ░ ░ ▒     ░   ░ ░ ░  ░  ░  
//            ░              ░ ░ ░                ░      ░ ░           ░       ░  
//                             ░                                                  

void Voraldo_object::draw_line(vec origin, vec dir, double length_scale, int state){
					//  ██╗     ██╗███╗   ██╗███████╗
					//  ██║     ██║████╗  ██║██╔════╝
					//  ██║     ██║██╔██╗ ██║█████╗  
					//  ██║     ██║██║╚██╗██║██╔══╝  
					//  ███████╗██║██║ ╚████║███████╗
					//  ╚══════╝╚═╝╚═╝  ╚═══╝╚══════╝
	//normalizing the direction vector
	double original_length = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
	double xdir_normalized = dir[0]/original_length;
	double ydir_normalized = dir[1]/original_length;
	double zdir_normalized = dir[2]/original_length;


	//establish the origin, not sure if it's really
	double curr_x_double = origin[0];
	double curr_y_double = origin[1];
	double curr_z_double = origin[2];

	int curr_x_int, curr_y_int, curr_z_int;

	//we now have a unit vector - this is a vector of length one
	//based upon this, we can make an arbitrary length line, by multiplying its x, y and z components

	for(double i = 0; i <= length_scale; i+=0.1){
		//this will express that the length_scale*the unit vector (length = 1) equals the total length
		curr_x_double += 0.1*xdir_normalized;
		curr_y_double += 0.1*ydir_normalized;
		curr_z_double += 0.1*zdir_normalized;

		curr_x_int = int(floor(curr_x_double));
		curr_y_int = int(floor(curr_y_double));
		curr_z_int = int(floor(curr_z_double));

		//then write state to cell located by the "curr int" variables 
		data->set_data_by_index(curr_x_int,curr_y_int,curr_z_int,state);

	}

}

void Voraldo_object::draw_rectangular_prism(vec mintuple, vec maxtuple, int state){
					//  ██████╗ ███████╗ ██████╗████████╗ █████╗ ███╗   ██╗ ██████╗ ██╗   ██╗██╗      █████╗ ██████╗ 
					//  ██╔══██╗██╔════╝██╔════╝╚══██╔══╝██╔══██╗████╗  ██║██╔════╝ ██║   ██║██║     ██╔══██╗██╔══██╗
					//  ██████╔╝█████╗  ██║        ██║   ███████║██╔██╗ ██║██║  ███╗██║   ██║██║     ███████║██████╔╝
					//  ██╔══██╗██╔══╝  ██║        ██║   ██╔══██║██║╚██╗██║██║   ██║██║   ██║██║     ██╔══██║██╔══██╗
					//  ██║  ██║███████╗╚██████╗   ██║   ██║  ██║██║ ╚████║╚██████╔╝╚██████╔╝███████╗██║  ██║██║  ██║
					//  ╚═╝  ╚═╝╚══════╝ ╚═════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝
					//                                                                                               
					//  ██████╗ ██████╗ ██╗███████╗███╗   ███╗                                                       
					//  ██╔══██╗██╔══██╗██║██╔════╝████╗ ████║                                                       
					//  ██████╔╝██████╔╝██║███████╗██╔████╔██║                                                       
					//  ██╔═══╝ ██╔══██╗██║╚════██║██║╚██╔╝██║                                                       
					//  ██║     ██║  ██║██║███████║██║ ╚═╝ ██║                                                       
					//  ╚═╝     ╚═╝  ╚═╝╚═╝╚══════╝╚═╝     ╚═╝ 
	//assumes you know what you're doing, calling the function

	if(mintuple[0] > maxtuple[0] || mintuple[1] > maxtuple[1] || mintuple[2] > maxtuple[2]){
		std::cout << "Trouble is present in the rectangular prism function -" << std::endl;
		std::cout << "What folly is this before me? You're an asshole." << std::endl;
		//The day I'll be happy is the day that computers just give error messages like this
		return;
	}

	for(int i = 0; i < data->get_x_res(); i++){	//this might be able to be optimized to only range within the prism itself
		for(int j = 0; j < data->get_y_res(); j++){	//rather than all cells in the block
			for(int k = 0; k < data->get_z_res(); k++){
				bool inx = (i < maxtuple[0] && i > mintuple[0]);	//check extents
				bool iny = (j < maxtuple[1] && j > mintuple[1]);
				bool inz = (k < maxtuple[2] && k > mintuple[2]);
				//AND them together, and presto
				if(inx && iny && inz){
					data->set_data_by_index(i,j,k,state);
				}
			}
		}
	}

}

void Voraldo_object::draw_sphere(vec origin, double radius, int state){
					//  ███████╗██████╗ ██╗  ██╗███████╗██████╗ ███████╗
					//  ██╔════╝██╔══██╗██║  ██║██╔════╝██╔══██╗██╔════╝
					//  ███████╗██████╔╝███████║█████╗  ██████╔╝█████╗  
					//  ╚════██║██╔═══╝ ██╔══██║██╔══╝  ██╔══██╗██╔══╝  
					//  ███████║██║     ██║  ██║███████╗██║  ██║███████╗
					//  ╚══════╝╚═╝     ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝
	for(int i = 0; i < data->get_x_res(); i++){	
		for(int j = 0; j < data->get_y_res(); j++){
			for(int k = 0; k < data->get_z_res(); k++){
				double testx = (i-origin[0])*(i-origin[0]);	//apply offsets and square
				double testy = (j-origin[1])*(j-origin[1]);
				double testz = (k-origin[2])*(k-origin[2]);

				if((testx + testy + testz) < radius*radius){	//pretty self explainatory, equation of sphere
					data->set_data_by_index(i,j,k,state);
				}
			}
		}
	}

}

void Voraldo_object::draw_ellipsoid(vec origin, vec radii, int state){ 
					//  ███████╗██╗     ██╗     ██╗██████╗ ███████╗ ██████╗ ██╗██████╗ 
					//  ██╔════╝██║     ██║     ██║██╔══██╗██╔════╝██╔═══██╗██║██╔══██╗
					//  █████╗  ██║     ██║     ██║██████╔╝███████╗██║   ██║██║██║  ██║
					//  ██╔══╝  ██║     ██║     ██║██╔═══╝ ╚════██║██║   ██║██║██║  ██║
					//  ███████╗███████╗███████╗██║██║     ███████║╚██████╔╝██║██████╔╝
					//  ╚══════╝╚══════╝╚══════╝╚═╝╚═╝     ╚══════╝ ╚═════╝ ╚═╝╚═════╝

	//bool shell/hull ? switch between <=1 or =1 (this will neccesitate multiple samples per cell, for sure)

	//the equation for the elipsoid is ((x-x0)^2)/(a^2) + ((y-y0)^2)/(b^2) + ((z-z0)^2)/(c^2) = 1 
	//where x,y, and z are the coordinates of any given cell or location under test.

	for(int i = 0; i < data->get_x_res(); i++){	
		for(int j = 0; j < data->get_y_res(); j++){
			for(int k = 0; k < data->get_z_res(); k++){
				double testx = (i-origin[0])*(i-origin[0]);	//apply offsets and square
				double testy = (j-origin[1])*(j-origin[1]);
				double testz = (k-origin[2])*(k-origin[2]);

				double radx = radii[0]*radii[0];
				double rady = radii[1]*radii[1];
				double radz = radii[2]*radii[2];

				double result = testx/radx + testy/rady + testz/radz;

				if(result <= 1){	//point inside ellipsoid - do we want to be able to invert this?
					data->set_data_by_index(i,j,k,state); //(outside, or on the surface, with >= and ==, respectively)
				}
			}
		}
	}
}

void Voraldo_object::draw_triangle(vec point1, vec point2, vec point3, int state){
					//  ████████╗██████╗ ██╗ █████╗ ███╗   ██╗ ██████╗ ██╗     ███████╗
					//  ╚══██╔══╝██╔══██╗██║██╔══██╗████╗  ██║██╔════╝ ██║     ██╔════╝
					//     ██║   ██████╔╝██║███████║██╔██╗ ██║██║  ███╗██║     █████╗  
					//     ██║   ██╔══██╗██║██╔══██║██║╚██╗██║██║   ██║██║     ██╔══╝  
					//     ██║   ██║  ██║██║██║  ██║██║ ╚████║╚██████╔╝███████╗███████╗
					//     ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚══════╝
					//this remains unfinished

	//find the max element, and prepare for the later calculations
	vec line1_2 = point2-point1;		vec line2_1 = point1-point2;
	vec line3_2 = point2-point3;		vec line2_3 = point3-point2;
	vec line1_3 = point3-point1;		vec line3_1 = point1-point3;

	//using one of the functions that the linear algebra library implements in order to find the largest side length
	int divisions = int(floor(linalg::maxelem(vec(linalg::length(line1_2),linalg::length(line1_3),linalg::length(line3_2))))); 
	//this is how many subdivisions each side will have

	if (divisions>2){	//proceed to subdivide and conquer
		//maybe another version could use two vectors and linearly interpolate?

		//Pair 1 is the pair of sides "1_2" and "3_2"
	veContainer pair1a;	vec pair1a_n = linalg::normalize(line1_2); double pair1a_len = linalg::length(line1_2); vec pair1a_origin = point1;
	veContainer	pair1b; vec pair1b_n = linalg::normalize(line3_2); double pair1b_len = linalg::length(line3_2); vec pair1b_origin = point3;


		//Pair 2 is the pair of sides "1_3" and "2_3"
	veContainer pair2a;	vec pair2a_n = linalg::normalize(line1_3); double pair2a_len = linalg::length(line1_3); vec pair2a_origin = point1;
	veContainer pair2b;	vec pair2b_n = linalg::normalize(line2_3); double pair2b_len = linalg::length(line2_3); vec pair2b_origin = point2;
		

		//Pair 3 is the pair of sides "2_1" and "3_1"
	veContainer pair3a;	vec pair3a_n = linalg::normalize(line2_1); double pair3a_len = linalg::length(line2_1); vec pair3a_origin = point2;
	veContainer pair3b; vec pair3b_n = linalg::normalize(line3_1); double pair3b_len = linalg::length(line3_1); vec pair3b_origin = point3;

		
		for(int i = 0; i < divisions; i++){

			pair1a.push_back(vec(pair1a_origin[0]+(i*pair1a_n[0]/pair1a_len),
								 pair1a_origin[1]+(i*pair1a_n[1]/pair1a_len),
								 pair1a_origin[2]+(i*pair1a_n[2]/pair1a_len)));

			pair1b.push_back(vec(pair1b_origin[0]+(i*pair1b_n[0]/pair1b_len),
								 pair1b_origin[1]+(i*pair1b_n[1]/pair1b_len),
								 pair1b_origin[2]+(i*pair1b_n[2]/pair1b_len)));


			pair2a.push_back(vec(pair2a_origin[0]+(i*pair2a_n[0]/pair2a_len),
								 pair2a_origin[1]+(i*pair2a_n[1]/pair2a_len),
								 pair2a_origin[2]+(i*pair2a_n[2]/pair2a_len)));

			pair2b.push_back(vec(pair2b_origin[0]+(i*pair2b_n[0]/pair2b_len),
								 pair2b_origin[1]+(i*pair2b_n[1]/pair2b_len),
								 pair2b_origin[2]+(i*pair2b_n[2]/pair2b_len)));


			pair3a.push_back(vec(pair3a_origin[0]+(i*pair3a_n[0]/pair3a_len),
								 pair3a_origin[1]+(i*pair3a_n[1]/pair3a_len),
								 pair3a_origin[2]+(i*pair3a_n[2]/pair3a_len)));

			pair3b.push_back(vec(pair3b_origin[0]+(i*pair3b_n[0]/pair3b_len),
								 pair3b_origin[1]+(i*pair3b_n[1]/pair3b_len),
								 pair3b_origin[2]+(i*pair3b_n[2]/pair3b_len)));

		}

		//render as lines. 		[origin,direction,length,state]

		for(int k = 0; k < divisions; k++){

			vec pair1a_curr_elem = pair1a[k];
			vec pair1b_curr_elem = pair1b[k];

			vec pair2a_curr_elem = pair2a[k];
			vec pair2b_curr_elem = pair2b[k];

			vec pair3a_curr_elem = pair3a[k];
			vec pair3b_curr_elem = pair3b[k];

			//this isn't done. I'm going to need to focus on it at some point.

			//draw_line(pair1a[x],pair1a[y],pair1a[z],pair)
		}


	}else if(divisions>=0){
		//just go ahead and draw points because they are less than two or three units apart.
		data->set_data_by_index(int(floor(point1[0])),int(floor(point1[1])),int(floor(point1[2])),state);
		data->set_data_by_index(int(floor(point2[0])),int(floor(point2[1])),int(floor(point2[2])),state);
		data->set_data_by_index(int(floor(point3[0])),int(floor(point3[1])),int(floor(point3[2])),state);
	}
}

void Voraldo_object::draw_cylinder(vec origin, vec direction, double radius, double input_length, int state){
					//   ██████╗██╗   ██╗██╗     ██╗███╗   ██╗██████╗ ███████╗██████╗ 
					//  ██╔════╝╚██╗ ██╔╝██║     ██║████╗  ██║██╔══██╗██╔════╝██╔══██╗
					//  ██║      ╚████╔╝ ██║     ██║██╔██╗ ██║██║  ██║█████╗  ██████╔╝
					//  ██║       ╚██╔╝  ██║     ██║██║╚██╗██║██║  ██║██╔══╝  ██╔══██╗
					//  ╚██████╗   ██║   ███████╗██║██║ ╚████║██████╔╝███████╗██║  ██║
					//   ╚═════╝   ╚═╝   ╚══════╝╚═╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚═╝  ╚═╝
	double len =  linalg::length(direction);

	vec ndirection;

	ndirection = direction / linalg::length(direction); //normalized direction

	auto bvec = origin;
	vec tvec(origin[0] + input_length*ndirection[0],
				origin[1] + input_length*ndirection[1],
				origin[2] + input_length*ndirection[2]); //hacky(due to type incompatabilities)

	auto bx0 = bvec[0]; auto ba = ndirection[0];	auto tx0 = tvec[0]; auto ta = ndirection[0];
	auto by0 = bvec[1]; auto bb = ndirection[1];	auto ty0 = tvec[1]; auto tb = ndirection[1];
	auto bz0 = bvec[2]; auto bc = ndirection[2];	auto tz0 = tvec[2]; auto tc = ndirection[2];

	//I did this on a whiteboard

	double bplanetest = 0.0;
	double tplanetest = 0.0;

	double point_to_line_distance = 0.0;

	for(int i = 0; i < data->get_x_res(); i++){	
		for(int j = 0; j < data->get_y_res(); j++){
			for(int k = 0; k < data->get_z_res(); k++){
				//planetests
				bplanetest = ba * (i - bx0) + bb * (j - by0) + bc * (k - bz0);
				tplanetest = ta * (i - tx0) + tb * (j - ty0) + tc * (k - tz0);

				//using the basic equation for a plane, we can do an interesting test

				//These variables will be greater than zero if the test point is on the side of the plane
				//that the normal vector is pointing towards, and less than zero if the test point is on 
				//the side of the plane that the normal vector is not pointing towards. That is to say, in
				//my case - bplanetest tells me whether the point is above the bottom plane, and tplanetest
				//tells me whether the point is above the top plane. If it is above the bottom, and below 
				//the top - we are within the ends of the cylinder, and can proceed. Thus, the condition
				//for the following if statement:

				if(bplanetest >= 0 && tplanetest <= 0){
					//do the point to line distance thing
					//algorithm from http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html

					//This test takes as a precondition that the point is determined to be within the slice
					//of space established by two parallel planes. Now that this is known, we will see how
					//far they are from the line, which runs perpendicular to both planes. If both of these
					//sequential tests evaluate positively, we know we are within the extents of the cylinder.

					point_to_line_distance = linalg::length(cross(tvec-bvec,bvec-vec(i,j,k)))/linalg::length(tvec-bvec);
					if(point_to_line_distance <= radius){
						data->set_data_by_index(i,j,k,state);
					}
				}
			}
		}
	}
}

void Voraldo_object::draw_capsule(vec origin, vec direction, double radius, double input_length, int state){
					//   ██████╗ █████╗ ██████╗ ███████╗██╗   ██╗██╗     ███████╗
					//  ██╔════╝██╔══██╗██╔══██╗██╔════╝██║   ██║██║     ██╔════╝
					//  ██║     ███████║██████╔╝███████╗██║   ██║██║     █████╗  
					//  ██║     ██╔══██║██╔═══╝ ╚════██║██║   ██║██║     ██╔══╝  
					//  ╚██████╗██║  ██║██║     ███████║╚██████╔╝███████╗███████╗
					//   ╚═════╝╚═╝  ╚═╝╚═╝     ╚══════╝ ╚═════╝ ╚══════╝╚══════╝
					//this coopts almost all of the cylinder function, plus spheres for the endpoints
					//just easier than fully implementing 'distance to a line segment' is right now.
					//I'm finding I really enjoy math in this context.
	double len =  linalg::length(direction);

	vec ndirection;

	ndirection = direction / linalg::length(direction); //normalized direction

	vec bvec = origin;
	vec tvec(origin[0] + input_length*ndirection[0],
			 origin[1] + input_length*ndirection[1],
			 origin[2] + input_length*ndirection[2]); //hacky(due to type incompatabilities)

	auto bx0 = bvec[0]; auto ba = ndirection[0];	auto tx0 = tvec[0]; auto ta = ndirection[0];
	auto by0 = bvec[1]; auto bb = ndirection[1];	auto ty0 = tvec[1]; auto tb = ndirection[1];
	auto bz0 = bvec[2]; auto bc = ndirection[2];	auto tz0 = tvec[2]; auto tc = ndirection[2];

	//I did this on a whiteboard

	double bplanetest = 0.0;
	double tplanetest = 0.0;

	double point_to_line_distance = 0.0;

	for(int x = 0; x < data->get_x_res(); x++){	
		for(int y = 0; y < data->get_y_res(); y++){
			for(int z = 0; z < data->get_z_res(); z++){

				//more details on the algorithm in the cylinder version of this function
				//planetests
				bplanetest = ba * (x - bx0) + bb * (y - by0) + bc * (z - bz0);
				tplanetest = ta * (x - tx0) + tb * (y - ty0) + tc * (z - tz0);

					//do the point to line distance thing
					//algorithm from http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
				if(bplanetest >= 0 && tplanetest <= 0){

					point_to_line_distance = linalg::length(cross(tvec-bvec,bvec-vec(x,y,z)))/linalg::length(tvec-bvec);
					if(point_to_line_distance <= radius){
						data->set_data_by_index(x,y,z,state);
					}
				}else if(linalg::length(vec(x,y,z)-bvec) < radius && linalg::length(vec(x,y,z)-tvec) < radius){
					//check spheres at the endpoints - bvec, tvec
					data->set_data_by_index(x,y,z,state);
				}
			}
		}
	}
}

void Voraldo_object::draw_dotted_capsule(vec point1, vec point2, double length_of_dots, double length_of_dashes, double radius, int state){
					//  ██████╗  ██████╗ ████████╗████████╗███████╗██████╗       
					//  ██╔══██╗██╔═══██╗╚══██╔══╝╚══██╔══╝██╔════╝██╔══██╗      
					//  ██║  ██║██║   ██║   ██║      ██║   █████╗  ██║  ██║      
					//  ██║  ██║██║   ██║   ██║      ██║   ██╔══╝  ██║  ██║      
					//  ██████╔╝╚██████╔╝   ██║      ██║   ███████╗██████╔╝      
					//  ╚═════╝  ╚═════╝    ╚═╝      ╚═╝   ╚══════╝╚═════╝       
					//                                                           
					//   ██████╗ █████╗ ██████╗ ███████╗██╗   ██╗██╗     ███████╗
					//  ██╔════╝██╔══██╗██╔══██╗██╔════╝██║   ██║██║     ██╔════╝
					//  ██║     ███████║██████╔╝███████╗██║   ██║██║     █████╗  
					//  ██║     ██╔══██║██╔═══╝ ╚════██║██║   ██║██║     ██╔══╝  
					//  ╚██████╗██║  ██║██║     ███████║╚██████╔╝███████╗███████╗
					//   ╚═════╝╚═╝  ╚═╝╚═╝     ╚══════╝ ╚═════╝ ╚══════╝╚══════╝
					//this uses the imagining of the capsule which consists of a subdivided line, rendered
					//as series of spheres, close enough to blend together. Dashed lines sound like an interesting idea.
					//I am implementing this by generating a vector of points , removing the ones I do not want, then doing distance 
					//checks on all of these points, in essence, treating them as spheres.
	double nx = (point1-point2)[0]/linalg::length(point1-point2);
	double ny = (point1-point2)[1]/linalg::length(point1-point2);
	double nz = (point1-point2)[2]/linalg::length(point1-point2);

	double user_input_length = linalg::length(point1-point2);
	double dot_dash_cycle = length_of_dots + length_of_dashes;

	vec norm = vec(nx,ny,nz);	
	veContainer points;
	double current_iteration = 0.0;

	if(radius > 3.0){
		std::cout << "shes gonna be a thick one" << std::endl;
	}

	//everybody say 'hi, norm' - he represents a vector that starts at point 1 and goes to point 2
	//notably, the length of this vector is unit, i.e. equal to 1. 
	//(for the unfamiliar, this is achieved by scaling x, y and z by the length of the vector in units. 
	//If you scale its components by corresponding amounts, you have the net result of scaling the entire vector.)

	for(double traversed_length = 0; traversed_length < user_input_length; traversed_length+=0.1){	//go along the line	
		if(current_iteration < length_of_dots){
			points.push_back(vec(point1[0]+traversed_length*norm[0],point1[1]+traversed_length*norm[1],point1[2]+traversed_length*norm[2]));
		}else if(current_iteration < dot_dash_cycle){
			//do nothing.
		}else{
			current_iteration = 0.0;
		}
	}

	//the list of points we need to test against is assembled

	for(int x = 0; x < data->get_x_res(); x++){	
		for(int y = 0; y < data->get_y_res(); y++){
			for(int z = 0; z < data->get_z_res(); z++){
				for(vec test_vec : points){
					double testx = (x-test_vec[0])*(x-test_vec[0]);	//apply offsets and square
					double testy = (y-test_vec[1])*(y-test_vec[1]);
					double testz = (z-test_vec[2])*(z-test_vec[2]);

					if((testx + testy + testz) > radius){	//pretty self explainatory, equation of sphere
						data->set_data_by_index(x,y,z,state);
					}
				}
			}
		}
	}
}


//-------------------------------------------------------------------------------------------------

RGB Voraldo_object::get_color_for_state(int state){
	
//  ██████╗  █████╗ ██╗     ███████╗████████╗████████╗███████╗
//  ██╔══██╗██╔══██╗██║     ██╔════╝╚══██╔══╝╚══██╔══╝██╔════╝
//  ██████╔╝███████║██║     █████╗     ██║      ██║   █████╗  
//  ██╔═══╝ ██╔══██║██║     ██╔══╝     ██║      ██║   ██╔══╝  
//  ██║     ██║  ██║███████╗███████╗   ██║      ██║   ███████╗
//  ╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝   ╚═╝      ╚═╝   ╚══════╝

//-------------------------------------------------------------------------------------------------

	RGB temp;

	switch(current_palette){
		case 0:	//Palette 0 is currently set by default
			//within this individual palette entry, final colors are determined directly from the integer state
			//which will give me a result displayed as shades of grey. Useful for visualizing the ambient occlusion function.
			if(state == 114){
				temp.red = 128;
				temp.green = 0;
				temp.blue = 0;
			}else if (state == 115){
				temp.red = 255;
				temp.green = 0;
				temp.blue = 0;
			}else if (state == 116){
				temp.red = 0;
				temp.green = 255;
				temp.blue = 0;
			}else if (state == 117){
				temp.red = 0;
				temp.green = 0;
				temp.blue = 255;
			}else{
				temp.red = state;
				temp.green = state;
				temp.blue = state;
			}
			return temp;
			break;

		case 1:	
				switch(state){ //this kind of structure will be useful in defining specific colors that I want to use.
					case 0: 
						temp.red = 0;
						temp.blue = 0;
						temp.green = 0;
						return temp;	//0 = black
						break;
					case 1:
						temp.red = 64;
						temp.blue = 64;
						temp.green = 64;
						return temp;	
						break;
					case 2:
						temp.red = 128;
						temp.blue = 128;
						temp.green = 128;
						return temp;	//shades of grey
						break;
					case 3:
						temp.red = 192;
						temp.blue = 192;
						temp.green = 192;
						return temp;	
						break;
					case 4:
						temp.red = 256;
						temp.blue = 256;
						temp.green = 256;
						return temp;	//white
						break;
				}
				break;
		case 2: //This entry maps to Palette 2, an alternative color scheme to Palette 1...
				break;
		case 3:
				break;

				//...


	}
}


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

int Block::get_data_at_point(vec test_point){
	intvec index;
	intvec extent;

	extent[0] = x_res;
	extent[1] = y_res;
	extent[2] = z_res;

	bool fuckit = false;

	for(int dim = 0; dim <=2; dim++){ //run once per dimension
		if(std::abs(test_point[dim] <= 1.0)){
			double curr_dim_extent = test_point[dim] + 1.0; //going from the range -1,1 to the range 0,2
			index[dim] = int(curr_dim_extent * floor(extent[dim]/2)); //map the range 0,2 to 0,max_index
		}else{
			//at least one dimension of this point is outside the range. forget about it
			fuckit = true;
		}
	}

	if(!fuckit){
		return get_data_by_index(index[0],index[1],index[2]);
	}else{
		return 0;
	}
}