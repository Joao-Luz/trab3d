#include "Textures.h"

#include <iostream>
#include <cassert>
#include <fstream>
#include <memory>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Converts a four-character array to an integer, using little-endian form
int to_int(const char *bytes) {
	return (int)(((unsigned char)bytes[3] << 24) |
				 ((unsigned char)bytes[2] << 16) |
				 ((unsigned char)bytes[1] << 8) |
				 (unsigned char)bytes[0]);
}

// Converts a two-character array to a short, using little-endian form
short to_short(const char *bytes) {
	return (short)(((unsigned char)bytes[1] << 8) |
				   (unsigned char)bytes[0]);
}

// Reads the next four bytes as an integer, using little-endian form
int read_int(std::ifstream &input) {
	char buffer[4];
	input.read(buffer, 4);
	return to_int(buffer);
}

// Reads the next two bytes as a short, using little-endian form
short read_short(std::ifstream &input) {
	char buffer[2];
	input.read(buffer, 2);
	return to_short(buffer);
}

unsigned char* load_bitmap(std::string path, int* width_addr, int* height_addr) {
	std::ifstream input;
	input.open(path, std::ifstream::binary);
	assert(!input.fail() || !"Could not find file");
	char buffer[2];
	input.read(buffer, 2);
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
	input.ignore(8);
	int data_offset = read_int(input);
	int width, height;

	// Read the header
	int header_size = read_int(input);
	switch (header_size)
	{
	case 40:
		// V3
		width = read_int(input);
		height = read_int(input);
		input.ignore(2);
		assert(read_short(input) == 24 || !"Image is not 24 bits per pixel");
		assert(read_short(input) == 0 || !"Image is compressed");
		break;
	case 12:
		// OS/2 V1
		width = read_short(input);
	height= read_short(input);
		input.ignore(2);
		assert(read_short(input) == 24 || !"Image is not 24 bits per pixel");
		break;
	case 64:
		// OS/2 V2
		assert(!"Can't load OS/2 V2 bitmaps");
		break;
	case 108:
		// Windows V4
		assert(!"Can't load Windows V4 bitmaps");
		break;
	case 124:
		// Windows V5
		assert(!"Can't load Windows V5 bitmaps");
		break;
	default:
		assert(!"Unknown bitmap format");
	}

	// Read the data
	int bytes_per_row = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int size = bytes_per_row * height;

	char* pixels = new char[size];

	input.seekg(data_offset, std::ios_base::beg);
	input.read(pixels, size);

	// Get the data into the right format
	auto pixels_formatted = new unsigned char[width * height * 3];

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++) {
				pixels_formatted[3 * (width * y + x) + c] = pixels[bytes_per_row * y + 3 * x + (2 - c)];
			}
		}
	}

	input.close();
	delete pixels;

	*width_addr = width;
	*height_addr = height;

	return pixels_formatted;
}

int Textures::load_texture(std::string path, std::string name) {
	int width, height;
	unsigned char* bitmap = load_bitmap(path, &width, &height);
	GLuint id;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap);

	delete bitmap;

	return id;
}