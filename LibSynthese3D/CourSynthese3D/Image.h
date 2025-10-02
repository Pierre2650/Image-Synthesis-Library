#pragma once
#include <iostream>
#include <vector>
#include <fstream>  // for ofstream

class Image {
public:
	std::string format = "";
	std::string comment = "# ";
	
	int width, height;
	
	std::vector<std::vector<int>> Matrix;

	Image( int width, int height);
	Image(std::string format, int width, int height);
	Image(std::string format, std::string comment,int width, int height);

    void WritePBM(std::string path, std::vector<std::vector<int>> Matrix );

};