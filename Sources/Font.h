#pragma once

#include "../Resources/Image.h"
#include "../Math/Vector.h"
#include "../Collections/List.h"
#include "../Collections/TreeMap.h"
#include <Kt/Handle.h>
#include "../Graphics/Graphics.h"
#include "../Graphics/Painter.h"
#include "stb_truetype.h"

namespace Kt {
	template <class T1, class T2> struct pair {
		typedef T1 first_type;
		typedef T2 second_type;

		T1 first;
		T2 second;
		pair() : first(T1()), second(T2()) {}
		pair(const T1& x, const T2& y) : first(x), second(y) {}
		template <class U, class V> pair (const pair<U,V> &p) : first(p.first), second(p.second) { }
		bool operator<(pair<T1, T2> other) {
			if (first == other.first) return second < other.second;
			else return first < other.first;
		}
		bool operator==(pair<T1, T2> other) {
			return first == other.first && second == other.second;
		}
	};

	class Item;

	struct Hyperlink {
		int x, y, width, height;
		Text href;
	};

	class Font {
	public:
		Font(Text name, unsigned size = 23);
		void render(Painter* painter, TextureUnit unit, Text text, real x, real y, Color color = Color::Black, real width = -1);
		//void render(Image& image, Text text, Color color, List<Vector2i>* positions = nullptr);
		//void render(Image& image, Text text, Color color, int width, List<Hyperlink>* links = nullptr, List<Handle<Item> >* items = nullptr, List<Vector2i>* positions = nullptr);
		//Image render(Text text, Color color, List<Vector2i>* positions = nullptr);
		//Image render(Text text, Color color, int width, List<Hyperlink>* links = nullptr, List<Handle<Item> >* items = nullptr, List<Vector2i>* positions = nullptr);
		real width(Text text);
		real height();
		real height(Text text, real width);
		static void setunifont(const char* fontname);
		static void setboldfont(const char* fontname);
		static int getHeight(Text name, unsigned size);
	private:
		int renderChar(Char c, int x, int y, Color color);
		int measureChar(Char c);
		int offsets[256];
		int widths[256];
		int realwidths[256];
		int lefts[256];
		Text name;
		unsigned size;
		Texture* texture;
		void* myface;
		//int height;
		bool bold;
		stbtt_bakedchar baked[256 - 32];
		VertexBuffer* vb;
		IndexBuffer* ib;
		stbtt_fontinfo info;
		real baseline;
	};

	extern TreeMap<pair<Text, int>, Font*> fonts;
}