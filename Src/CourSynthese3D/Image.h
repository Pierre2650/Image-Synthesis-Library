#pragma once
#include <iostream>
#include <vector>
#include <fstream>  // for ofstream
#include "Synthese3D.h"

class Image {
public:
	std::string format = "P3";
	std::string comment = "# ";
	
	int width, height;
	
	std::vector<std::vector<Color>> Matrix;

	Image( int width, int height, std::vector<std::vector<Color>> Matrix);
	Image( std::string comment, int width, int height, std::vector<std::vector<Color>> Matrix);

    void WriteImage(std::string path);


};