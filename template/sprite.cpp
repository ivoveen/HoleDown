// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
//#include<iostream>

using namespace Tmpl8;

// constructor
Sprite::Sprite(Surface* surface, unsigned int frameCount) :
	width(surface->width / frameCount),
	height(surface->height),
	numFrames(frameCount),
	currentFrame(0),
	flags(0),
	start(new unsigned int* [frameCount]),
	surface(surface)
{
	InitializeStartData();
}
Sprite::Sprite() {

}

// destructor
Sprite::~Sprite()
{
	delete surface;
	for (unsigned int i = 0; i < numFrames; i++) delete start[i];
	delete start;
}

// draw sprite to target surface
void Sprite::Draw(Surface* target, int x, int y)
{
	if (x < -width || x >(target->width + width)) return;
	if (y < -height || y >(target->height + height)) return;
	int x1 = x, x2 = x + width;
	int y1 = y, y2 = y + height;
	uint* src = GetBuffer() + currentFrame * width;
	if (x1 < 0) src += -x1, x1 = 0;
	if (x2 > target->width) x2 = target->width;
	if (y1 < 0) src += -y1 * width * numFrames, y1 = 0;
	if (y2 > target->height) y2 = target->height;
	uint* dest = target->pixels;
	int xs;
	if (x2 > x1 && y2 > y1)
	{
		unsigned int addr = y1 * target->width + x1;
		const int w = x2 - x1;
		const int h = y2 - y1;
		for (int j = 0; j < h; j++)
		{
			const int line = j + (y1 - y);
			const int lsx = start[currentFrame][line] + x;
			xs = (lsx > x1) ? lsx - x1 : 0;
			for (int i = xs; i < w; i++)
			{
				const uint c1 = *(src + i);
				if (c1 & 0xffffff) *(dest + addr + i) = c1;
			}
			addr += target->width;
			src += width * numFrames;
		}
	}
}


// draw scaled sprite
void Sprite::DrawScaled(int x1, int y1, int w, int h, bool flipped, Surface* target)
{
	float xFactor = ((float)width / (float)w);
	float yFactor = ((float)height / (float)h);
	int srcwidth = static_cast<int>(w);
	int srcheight = static_cast<int>(h);
	int dstwidth = target->width;
	int dstheight = target->height;
	uint* src = GetBuffer() + currentFrame * width;

	if ((srcwidth + x1) > dstwidth) srcwidth = dstwidth - x1;
	if ((srcheight + y1) > dstheight) srcheight = dstheight - y1;
	if (x1 < 0) src -= static_cast<int>(x1 * xFactor), srcwidth += x1, x1 = 0;
	if (y1 < 0) src += -static_cast<int>(y1 * yFactor) * width * numFrames, srcheight += y1, y1 = 0;
	if (width == 0 || height == 0) return;
	if ((srcwidth < 0) && (srcheight < 0)) return;
	for (int y = 0; y < srcheight; y++) {
		for (int x = 0; x < srcwidth; x++)
		{
			int u = static_cast<int>((float)x * xFactor);
			int v = static_cast<int>((float)y * yFactor);
			uint color = src[u + v * width * numFrames];
			if (color & 0xffffff) {
				
				if (flipped)target->pixels[x1 + srcwidth -x + ((y1 + y) * target->width)] = color;
				else target->pixels[x1 + x + ((y1 + y) * target->width)] = color;;
			}
		}
	}
}

// prepare sprite outline data for faster rendering
void Sprite::InitializeStartData()
{
	for (unsigned int f = 0; f < numFrames; ++f)
	{
		start[f] = new unsigned int[height];
		for (int y = 0; y < height; ++y)
		{
			start[f][y] = width;
			uint* addr = GetBuffer() + f * width + y * width * numFrames;
			for (int x = 0; x < width; ++x) if (addr[x])
			{
				start[f][y] = x;
				break;
			}
		}
	}
}