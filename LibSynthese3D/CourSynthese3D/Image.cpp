// Synthese3D.cpp : Définit les fonctions de la bibliothèque statique.
//
#include "pch.h"
#include "framework.h"
#include "Image.h"
#include <iostream>
#include <fstream>  // for ofstream



/// ------------- Construct -------------
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

void Image::CreatePBM(std::vector<std::vector<int>> Matrix) {
	return;
}

bool Image::WritePBM(std::string path) {

    ofstream outfile("example.txt");

    // Check if the file opened successfully
    if (!outfile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    // Write to the file
    outfile << "Hello, this is a test.\n";
    outfile << "Writing numbers: " << 123 << "\n";
    outfile << "Writing more text into the file.\n";

    // Always close the file
    outfile.close();

}
