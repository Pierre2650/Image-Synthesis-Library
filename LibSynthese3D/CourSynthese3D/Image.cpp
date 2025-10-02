// Synthese3D.cpp : Définit les fonctions de la bibliothèque statique.
//
#include "pch.h"
#include "framework.h"
#include "Image.h"
#include <vector>
#include <iostream>
#include <fstream>  // for ofstream



/// ------------- Construct -------------

Image::Image( int width, int height) {
    this->width = width;
    this->height = height;

}

Image::Image(std::string format, int width, int height) {
	this->format = format;
	this->width = width;
	this->height = height;
}

Image::Image(std::string format, std::string comment, int width, int height) {
	this->format = format;
	this->comment = comment;
	this->width = width;
	this->height = height;
}

void Image::WritePBM(std::string path , std::vector<std::vector<int>> Matrix ) {
    path += "/PBM_Test.pbm";

    std::ofstream outfile(path);

    // Check if the file opened successfully
    if (!outfile) {
        std::cout << "Error opening file!" << std::endl;
        return;
    }

    outfile << this->format <<"\n";
    outfile << this->comment << "\n";
    outfile << this->width << " "<< this->height << "\n";

    for (int i = 0; i < this->height; i++) {

        for (int j = 0; j < this->width; j++) {
            outfile << Matrix[i][j];
        }

    }

    // Write to the file

    /*outfile << "Hello, this is a test.\n";
    outfile << "\n";
    outfile << "Writing numbers: " << 123 << "\n";
    outfile << "Writing more text into the file.\n";*/

    // Always close the file
    outfile.close();

}
