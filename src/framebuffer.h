#pragma once


typedef struct {
	unsigned int *pixels;
	int width;
	int height;
} FrameBuffer;

FrameBuffer *allocateFrameBuffer(int width, int height);
FrameBuffer *loadFrameBuffer(const char *fileName);
void destroyFrameBuffer(FrameBuffer *framebuffer);

