// Synthese3D.cpp : Définit les fonctions de la bibliothèque statique.
//
#include "pch.h"
#include "framework.h"
#include "Image.h"
#include <vector>
#include <iostream>
#include <fstream>  // for ofstream
#include "Synthese3D.h"


/// ------------- Construct -------------

Image::Image( int width, int height, std::vector<std::vector<Color>> Matrix) {

    /*try {

        if (Matrix.size() != height || Matrix[0].size() != width) {
            throw std::runtime_error("Image Matrix doesnt fit Image resolution");
        }
    }
    catch (const std::runtime_error& e) {
        std::cout << "Caught a runtime_error: " << e.what() << std::endl;
        return;
    }*/

	this->width = width;
	this->height = height;
    this->Matrix = Matrix;
}

Image::Image(std::string comment, int width, int height, std::vector<std::vector<Color>> Matrix){

	this->comment = comment;
	this->width = width;
	this->height = height;
    this->Matrix = Matrix;
}

void Image::WriteImage(std::string path) {
    //Check width and height?

    path += "/PBM_Test.ppm";

    std::ofstream outfile(path);

    // Check if the file opened successfully
    if (!outfile) {
        std::cout << "Error opening file!" << std::endl;
        std::cout << "path: " << path << std::endl;
        return;
    }

    outfile << this->format <<"\n";
    outfile << this->comment << "\n";
    outfile << this->width << " "<< this->height << "\n";
    outfile << "255\n";

    for (int i = 0; i < this->height; i++) {

        for (int j = 0; j < this->width; j++) {
            outfile << Matrix[i][j].x << " " << Matrix[i][j].y << " " << Matrix[i][j].z << "  ";
        }
        outfile << "\n";
    }
    outfile.close();

}
