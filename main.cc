//This file contains the main function, which serves as a driver for the Voraldo class
#include <iostream>
#include <string>
#include "includes/voraldo/voraldo.h"

void exit_salutations(Voraldo_object* block);

int main(){

	Voraldo_object main_block(256,256,256);
	//Voraldo_object main_block;
	//main_block.load_block_from_file("example.txt");

	//do a for loop

	// for(int loopint = 0; loopint < 150; loopint++){
	// 	double params[] = {double(rand()%256),double(rand()%256),double(rand()%256),double(rand()%12),118.0};
	// 	main_block.draw("sphere",params);
	// }


	double elipsoid_params[] = {128.0,128.0,128.0,64.0,16.0,16.0,115.0};
	//			[origin,radii,state]
	main_block.draw("elipsoid",elipsoid_params);
	elipsoid_params[3] = 16;
	elipsoid_params[4] = 64;
	elipsoid_params[6] = 116;
	main_block.draw("elipsoid",elipsoid_params);
	elipsoid_params[4] = 16;
	elipsoid_params[5] = 64;
	elipsoid_params[6] = 117;
	main_block.draw("elipsoid",elipsoid_params);


	double sphere_params[] = {128.0,128.0,128.0,22.0,114.0};
	//main_block.draw("sphere", sphere_params);//centerpoint
	sphere_params[0] = 192;
	main_block.draw("sphere", sphere_params);//plusx
	sphere_params[0] = 128;
	sphere_params[1] = 192;
	main_block.draw("sphere", sphere_params);//plusy
	sphere_params[1] = 128;
	sphere_params[2] = 192;
	main_block.draw("sphere", sphere_params);//plusz
	sphere_params[2] = 128;
	sphere_params[0] = 64;
	main_block.draw("sphere", sphere_params);//minusx
	sphere_params[0] = 128;
	sphere_params[1] = 64;
	main_block.draw("sphere", sphere_params);//minusy
	sphere_params[1] = 128;
	sphere_params[2] = 64;
	main_block.draw("sphere", sphere_params);//minusz

	//don't need to maintain params anymore

	main_block.dump_that_shit_in_a_file("example.txt");

	main_block.display("simple");
	
	exit_salutations(&main_block);

	//std::cout << "size of ullong " << sizeof(unsigned long long int) << std::endl;

	//std::string s;
	//std::cout << std::endl << "max number of characters in a string " << s.max_size() << std::endl;

	return 0;
	
}

void exit_salutations(Voraldo_object* block){

	std::cout << std::endl << "Made " << block->get_number_of_cells() << " cells" << std::endl << std::endl;

}