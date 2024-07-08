#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "framebuffer.h"

#include <stdio.h>

const char *HELP = "ImageToAscii [image file]";

float lerp(float x0, float x1, float t) {
	return x0 + t * (x1 - x0);
}

int main(int argCount, char *args[]) {
	if(argCount != 2) {
		puts(HELP);
		return 1;
	}

	FrameBuffer *framebuffer = loadFrameBuffer(args[1]);
	if(framebuffer == NULL) {
		puts("Couldn't open file");
		puts(HELP);
		return 1;
	}

	FrameBuffer *mono = allocateFrameBuffer(framebuffer->width, framebuffer->height);
	if(mono == NULL) {
		puts("Out of memory!");
		return 1;
	}

	for(int y = 0; y < framebuffer->height; ++y) {
		for(int x = 0; x < framebuffer->width; ++x) {
			unsigned int pixel = framebuffer->pixels[y * framebuffer->width + x];
			unsigned int red   = (pixel & 0x00'FF'00'00) >> 16;
			unsigned int green = (pixel & 0x00'00'FF'00) >> 8;
			unsigned int blue  = (pixel & 0x00'00'00'FF) >> 0;
			unsigned int monop = sqrt(0.299f * powf(red, 2) + 0.587f * powf(green, 2) + 0.114f * powf(blue, 2));
			mono->pixels[y * mono->width + x] = monop << 16 | monop << 8 | monop << 0 | 0xFF << 24;
		}
	}

	//stbi_write_png("mono.png", mono->width, mono->height, 4, mono->pixels, mono->width * 4);

	// * 2 since the terminal includes white space between lines
	int ascii_width = mono->width / 6 * 2;
	int ascii_height = mono->height / 6;
	size_t ascii_size = (ascii_width + 1) * ascii_height;
	char *ascii = calloc(ascii_size, 1);
	if(ascii == NULL) {
		puts("Out of memory!");
		return 1;
	}

#if 1
	const char brightnessMap[] = {
		' ', '.', '\'', '`', '^', '"', ',', ':', ';', 'I', 'l', '!', 'i', '>', '<', '~', '+', '_', '-', '?', 
		'[', ']', '}', '{', '1', ')', '(', '|', '\\', '/', 't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 
		'z', 'X', 'Y', 'U', 'J', 'C', 'L', 'Q', '0', 'O', 'Z', 'm', 'w', 'q', 'p', 'b', 'k', 'd', 'h', 
		'a', 'o', '*', '#', 'M', 'W', '&', '8', '%', 'B', '@', '$'
	};
#else
	const char brightnessMap[] = {
		' ', '.', '`', '^', '"', ':', 'u', 'v', 'c',  '@', '$'
	};
#endif

	for(int y = 0; y < ascii_height; ++y) {
		for(int x = 0; x < ascii_width; ++x) {
			int buff_x = lerp(0, mono->width  - 1, x / (float)ascii_width);
			int buff_y = lerp(0, mono->height - 1, y / (float)ascii_height);
			unsigned int pixel = mono->pixels[buff_y * mono->width + buff_x];
			int index = lerp(0, sizeof(brightnessMap) - 1, (pixel & 0xFF) / 255.0f);
			ascii[y * (ascii_width + 1) + x] = brightnessMap[index];
		}
		ascii[y * (ascii_width + 1) + ascii_width] = '\n';
	}

	fwrite(ascii, 1, ascii_size, stdout);

	free(ascii);
	destroyFrameBuffer(mono);
	destroyFrameBuffer(framebuffer);
	return 0;
}

