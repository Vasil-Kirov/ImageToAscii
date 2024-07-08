#include "framebuffer.h"
#include <stb_image.h>

FrameBuffer *allocateFrameBuffer(int width, int height) {
	FrameBuffer *result = malloc(sizeof(FrameBuffer));
	if(!result)
		return NULL;

	result->pixels = calloc(width * height, 4);
	if(!result->pixels) {
		free(result);
		return NULL;
	}
	result->width = width;
	result->height = height;
	return result;
}

FrameBuffer *loadFrameBuffer(const char *fileName) {
	FrameBuffer *result = malloc(sizeof(FrameBuffer));
	if(!result)
		return NULL;
	int channels;
	result->pixels = (unsigned int *)stbi_load(fileName, &result->width, &result->height, &channels, 4);
	if(result->pixels == NULL)  {
		free(result);
		return NULL;
	}
	return result;
}

void destroyFrameBuffer(FrameBuffer *framebuffer) {
	if(framebuffer->pixels)
		free(framebuffer->pixels);
	free(framebuffer);
}

