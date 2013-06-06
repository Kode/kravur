#include "stdafx.h"
#include "Font.h"
#include "../Resources/Image.h"
#include "../Item.h"
#include "../Exception.h"
#include "../Files/File.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#ifdef SYS_WINDOWS
#include <windows.h>
#endif

using namespace Kt;

TreeMap<pair<Text, int>, Font*> Kt::fonts;

namespace {
	//char* unifont = "arialuni.ttf";
	const char* unifont = "FreeSerif.ttf";
	const char* boldfont = "FreeSerif.ttf";
	const int uniydif = 1;
}

void Font::setunifont(const char* fontname) {
	unifont = fontname;
}

void Font::setboldfont(const char* fontname) {
	boldfont = fontname;
}

int Font::getHeight(Text name, unsigned size) {
	return size;
}

Font::Font(Text name, unsigned size) : name(name), size(size), bold(false) {
	ImageGrey image(2048, 2048);
	{
		Kt::DiskFile file(name, Kt::DiskFile::MODE_READ);
		const unsigned char* data = (const unsigned char*)file.ReadAll();
		unsigned int* pixels = image.Lock(0);
		int bitmapStatus = stbtt_BakeFontBitmap(data, 0, static_cast<float>(size), (unsigned char*)pixels, 2048, 2048, 32, 256 - 32, baked);
		if (bitmapStatus < 0) throw Exception("Text is too big.");
		image.Unlock();
	}

	texture = image.Tex();
	image.grabTexture();
	VertexStructure structure;
	structure.add("position", Float3VertexData, PositionVertexType);
	structure.add("color", ColorVertexData, ColorVertexType);
	structure.add("texcoord", Float2VertexData, TexCoordVertexType);
	vb = Graphics::createVertexBuffer(4 * 1000, structure);
	ib = Graphics::createIndexBufferForQuads(1000);

	{
		Kt::DiskFile file(name, Kt::DiskFile::MODE_READ);
		const unsigned char* data = (const unsigned char*)file.ReadAll();
		stbtt_InitFont(&info, data, 0);
		int ascent;
		real scale = stbtt_ScaleForPixelHeight(&info, static_cast<float>(size));
		stbtt_GetFontVMetrics(&info, &ascent, 0, 0);
		baseline = ascent * scale;
	}
}

void Font::render(Painter* painter, TextureUnit unit, Text text, real xpos, real ypos, Color color, real width) {
	painter->end();
//	Graphics::setTexturing(true);
	real* vertices = vb->lock(0, text.length() * 4);
	real x = painter->getTranslation().x() + xpos;
	real y = painter->getTranslation().y() + ypos;
	y += baseline;
	int lastBreak_i = 0, lastBreak_vi = 0;
	int vi = 0;
	bool foundSpace = false;
	for (int i = 0; i < text.length(); ++i) {
		if (text[i].value > 256) continue; //TODO: add unicode support

		if (text[i] == '\n') {
			y += size;
			x = painter->getTranslation().x();
			continue;
		}
		if (!foundSpace) {
			lastBreak_i = i;
			lastBreak_vi = vi;
		}
		if (text[i] == ' ') {
			foundSpace = true;
			lastBreak_i = i;
			lastBreak_vi = vi;
		}

		stbtt_aligned_quad quad;
		stbtt_GetBakedQuad(baked, 2048, 2048, text[i].value - 32, &x, &y, &quad, 1);

		if (width > 0 && x - painter->getTranslation().x() > width) {
			y += size;
			x = painter->getTranslation().x();

			i = lastBreak_i;
			if (foundSpace) ++i;
			vi = lastBreak_vi;
			stbtt_GetBakedQuad(baked, 2048, 2048, text[i].value - 32, &x, &y, &quad, 1);
			foundSpace = false;
		}

		unsigned vlength = 6;
		int i2 = 0;

		vertices[vi * 4 * vlength + i2++] = quad.x0; vertices[vi * 4 * vlength + i2++] = quad.y0; vertices[vi * 4 * vlength + i2++] = 1;
		((unsigned*)vertices)[vi * 4 * vlength + i2++] = color.value();
		vertices[vi * 4 * vlength + i2++] = quad.s0; vertices[vi * 4 * vlength + i2++] = quad.t0;

		vertices[vi * 4 * vlength + i2++] = quad.x1; vertices[vi * 4 * vlength + i2++] = quad.y0; vertices[vi * 4 * vlength + i2++] = 1;
		((unsigned*)vertices)[vi * 4 * vlength + i2++] = color.value();
		vertices[vi * 4 * vlength + i2++] = quad.s1; vertices[vi * 4 * vlength + i2++] = quad.t0;

		vertices[vi * 4 * vlength + i2++] = quad.x0; vertices[vi * 4 * vlength + i2++] = quad.y1; vertices[vi * 4 * vlength + i2++] = 1;
		((unsigned*)vertices)[vi * 4 * vlength + i2++] = color.value();
		vertices[vi * 4 * vlength + i2++] = quad.s0; vertices[vi * 4 * vlength + i2++] = quad.t1;

		vertices[vi * 4 * vlength + i2++] = quad.x1; vertices[vi * 4 * vlength + i2++] = quad.y1; vertices[vi * 4 * vlength + i2++] = 1;
		((unsigned*)vertices)[vi * 4 * vlength + i2++] = color.value();
		vertices[vi * 4 * vlength + i2++] = quad.s1; vertices[vi * 4 * vlength + i2++] = quad.t1;

		++vi;
	}
	vb->unlock();
	texture->set(unit);
	vb->set();
	ib->set();

	Graphics::setTextureOperation(SelectFirstOperation, CurrentColorArgument, CurrentColorArgument);
	Graphics::drawIndexedVertices(0, vi * 2); //text.length() * 2);
	Graphics::setTextureOperation(ModulateOperation, TextureColorArgument, CurrentColorArgument);
}

int Font::measureChar(Char c) {
	return static_cast<int>(baked[c.value - 32].xadvance);
}

int Font::renderChar(Char c, int x, int y, Color color) {
	return 10;
}

real Font::width(Text text) {
	real x = 0, y = 0;
	for (int i = 0; i < text.length(); ++i) {
		stbtt_aligned_quad quad;
		stbtt_GetBakedQuad(baked, 2048, 2048, text[i].value - 32, &x, &y, &quad, 1);
	}
	return x;
}

real Font::height() {
	return static_cast<real>(size);
}

real Font::height(Text text, real width) {
	real x = 0;
	real y = 0;
	y += baseline;
	int lastBreak_i = 0;
	for (int i = 0; i < text.length(); ++i) {
		if (text[i] == '\n') {
			y += size;
			continue;
		}
		if (text[i] == ' ') {
			lastBreak_i = i;
		}

		stbtt_aligned_quad quad;
		stbtt_GetBakedQuad(baked, 2048, 2048, text[i].value - 32, &x, &y, &quad, 1);

		if (width > 0 && x > width) {
			y += size;
			x = 0;
			i = lastBreak_i + 1;
			stbtt_GetBakedQuad(baked, 2048, 2048, text[i].value - 32, &x, &y, &quad, 1);
		}
	}
	return y;
}