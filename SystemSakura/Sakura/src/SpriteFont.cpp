#include "SpriteFont.h"

#include "SpriteBatch.h"
#include "ImageLoader.h"

#include <SDL/SDL.h>

int closestPow2(int i) {
    i--;
    int pi = 1;
    while (i > 0) {
        i >>= 1;
        pi <<= 1;
    }
    return pi;
}

#define MAX_TEXTURE_RES 4096

namespace Sakura {

    SpriteFont::SpriteFont() {
		/* Empty */
    }

	SpriteFont::~SpriteFont(){
		
	}

	void SpriteFont::initTTF(const char* font, int size, char cs, char ce, TexParam param /*= DEFAULT*/){
		// Initialize SDL_ttf
		if (!TTF_WasInit()) {
			TTF_Init();
		}
		TTF_Font* f = TTF_OpenFont(font, size);
		if (f == nullptr) {
			(stderr, "Failed to open TTF font %s\n", font);
			fflush(stderr);
			throw 281;
		}
		m_fontHeight = TTF_FontHeight(f);
		m_regStart = cs;
		m_regLength = ce - cs + 1;
		int padding = size / 8;

		// First measure all the regions
		glm::ivec4* glyphRects = new glm::ivec4[m_regLength];
		int i = 0, advance;
		for (char c = cs; c <= ce; c++) {
			TTF_GlyphMetrics(f, c, &glyphRects[i].x, &glyphRects[i].z, &glyphRects[i].y, &glyphRects[i].w, &advance);
			glyphRects[i].z -= glyphRects[i].x;
			glyphRects[i].x = 0;
			glyphRects[i].w -= glyphRects[i].y;
			glyphRects[i].y = 0;
			i++;
		}

		// Find best partitioning of glyphs
		int rows = 1, w, h, bestWidth = 0, bestHeight = 0, area = MAX_TEXTURE_RES * MAX_TEXTURE_RES, bestRows = 0;
		std::vector<int>* bestPartition = nullptr;
		while (rows <= m_regLength) {
			h = rows * (padding + m_fontHeight) + padding;
			auto gr = createRows(glyphRects, m_regLength, rows, padding, w);

			// Desire a power of 2 texture
			w = closestPow2(w);
			h = closestPow2(h);

			// A texture must be feasible
			if (w > MAX_TEXTURE_RES || h > MAX_TEXTURE_RES) {
				rows++;
				delete[] gr;
				continue;
			}

			// Check for minimal area
			if (area >= w * h) {
				if (bestPartition) delete[] bestPartition;
				bestPartition = gr;
				bestWidth = w;
				bestHeight = h;
				bestRows = rows;
				area = bestWidth * bestHeight;
				rows++;
			}
			else {
				delete[] gr;
				break;
			}
		}

		// Can a bitmap font be made?
		if (!bestPartition) {
			std::fprintf(stderr, "Failed to Map TTF font %s to texture. Try lowering resolution.\n", font);
			fflush(stderr);
			throw 282;
		}
		// Create the texture
		glGenTextures(1, &m_texID);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bestWidth, bestHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		// Now draw all the glyphs
		SDL_Color fg = { 255, 255, 255, 255 };
		int ly = padding;
		for (int ri = 0; ri < bestRows; ri++) {
			int lx = padding;
			for (std::size_t ci = 0; ci < bestPartition[ri].size(); ci++) {
				int gi = bestPartition[ri][ci];

				SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(f, (char)(cs + gi), fg);

				// Pre-multiplication occurs here
				unsigned char* sp = (unsigned char*)glyphSurface->pixels;
				int cp = glyphSurface->w * glyphSurface->h * 4;
				for (int i = 0; i < cp; i += 4) {
					float a = sp[i + 3] / 255.0f;
					sp[i] *= (unsigned char)a;
					sp[i + 1] = sp[i];
					sp[i + 2] = sp[i];
				}

				// Save glyph image and update coordinates
				glTexSubImage2D(GL_TEXTURE_2D, 0, lx, bestHeight - ly - 1 - glyphSurface->h, glyphSurface->w, glyphSurface->h, GL_BGRA, GL_UNSIGNED_BYTE, glyphSurface->pixels);
				glyphRects[gi].x = lx;
				glyphRects[gi].y = ly;
				glyphRects[gi].z = glyphSurface->w;
				glyphRects[gi].w = glyphSurface->h;

				SDL_FreeSurface(glyphSurface);
				glyphSurface = nullptr;

				lx += glyphRects[gi].z + padding;
			}
			ly += m_fontHeight + padding;
		}

		// Draw the unsupported glyph
		int rs = padding - 1;
		int* pureWhiteSquare = new int[rs * rs];
		memset(pureWhiteSquare, 0xffffffff, rs * rs * sizeof(int));
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rs, rs, GL_RGBA, GL_UNSIGNED_BYTE, pureWhiteSquare);
		delete[] pureWhiteSquare;
		pureWhiteSquare = nullptr;

		/* Setup all the texture parameters */

		if (param & DEFAULT){
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			// Generate the mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			// Wrap Formatting //
			if (param & REPEAT){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			else if (param & EDGE_CLAMP){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			else if (param & TRANS_BORDER){
				float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
			}
			else{
				printf("WARNING : NO TEXTURE WRAP SPECEIFIED, REVERTING TO DEFAULT...\n");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			// Filter //
			if (param & PIXELATED && param & MIPMAP){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			}
			else if (param & PIXELATED){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
			else if (param & LINEAR && param & MIPMAP){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			else if (param & LINEAR){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			else{
				printf("WARNING : NO TEXTURE FILTER SPECEIFIED, REVERTING TO DEFAULT...\n");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			// Mipmap //
			if (param & MIPMAP){
				//Generate the mipmaps
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else{
				printf("WARNING : NO MIPMAP GENERATED FOR TEXTURE ID TTF %s\n", font);
			}
		}

		// Create spriteBatch glyphs
		m_glyphs = new CharGlyph[m_regLength + 1];
		for (i = 0; i < m_regLength; i++) {
			m_glyphs[i].character = (char)(cs + i);
			m_glyphs[i].size = glm::vec2(glyphRects[i].z, glyphRects[i].w);
			m_glyphs[i].uvRect = glm::vec4(
				(float)glyphRects[i].x / (float)bestWidth,
				(float)glyphRects[i].y / (float)bestHeight,
				(float)glyphRects[i].z / (float)bestWidth,
				(float)glyphRects[i].w / (float)bestHeight
				);
		}
		m_glyphs[m_regLength].character = ' ';
		m_glyphs[m_regLength].size = m_glyphs[0].size;
		m_glyphs[m_regLength].uvRect = glm::vec4(0, 0, (float)rs / (float)bestWidth, (float)rs / (float)bestHeight);

		glBindTexture(GL_TEXTURE_2D, 0);
		delete[] glyphRects;
		glyphRects = nullptr;
		delete[] bestPartition;
		bestPartition = nullptr;
		TTF_CloseFont(f);
		font = nullptr;
	}

	void SpriteFont::initPNG(const char* font, const glm::ivec2& size, char cs, char ce, TexParam param /*= DEFAULT*/){
		m_fontHeight = (int)size.y;
		m_regStart = cs;
		m_regLength = ce - cs + 1;
		int padding = (int)size.x / 8;

		GLTexture tempTex;
		tempTex = ImageLoader::loadPNG(std::string(font), param);
		m_texID = tempTex.id;

		const int fileGlyphWidth = tempTex.width / size.x;
		const int fileGlyphHeight = tempTex.height / size.y;

		// Create spriteBatch glyphs
		m_glyphs = new CharGlyph[m_regLength + 1];
		for (int i = 0; i < m_regLength + 1; ++i) {
			m_glyphs[i].character = (char)(cs + i);
			m_glyphs[i].size = size;
			/* Compute the uvRect in file */
			int xTile = i % fileGlyphWidth;
			int yTile = i / fileGlyphWidth;

			glm::vec4 uv;
			uv.x = (xTile / (float)fileGlyphWidth);
			uv.y = (yTile / (float)fileGlyphHeight);
			uv.z = (1.0f / fileGlyphWidth);
			uv.w = (1.0f / fileGlyphHeight);
			m_glyphs[i].uvRect = uv;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void SpriteFont::dispose() {
		if (m_texID != 0) {
			glDeleteTextures(1, &m_texID);
			m_texID = 0;
		}
		if (m_glyphs) {
			delete[] m_glyphs;
			m_glyphs = nullptr;
		}
    }

    std::vector<int>* SpriteFont::createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w) {
        // Blank initialize
        std::vector<int>* l = new std::vector<int>[r]();
        int* cw = new int[r]();
        for (int i = 0; i < r; i++) {
            cw[i] = padding;
        }

        // Loop through all glyphs
        for (int i = 0; i < rectsLength; i++) {
            // Find row for placement
            int ri = 0;
            for (int rii = 1; rii < r; rii++)
            if (cw[rii] < cw[ri]) ri = rii;

            // Add width to that row
            cw[ri] += rects[i].z + padding;

            // Add glyph to the row list
            l[ri].push_back(i);
        }

        // Find the max width
        w = 0;
        for (int i = 0; i < r; i++) {
            if (cw[i] > w) w = cw[i];
        }

		if (cw){
			delete[] cw;
			cw = nullptr;
		}

        return l;
    }

    glm::vec2 SpriteFont::measure(const char* s) {
        glm::vec2 size(0, m_fontHeight);
        float cw = 0;
        for (int si = 0; s[si] != 0; si++) {
            char c = s[si];
            if (s[si] == '\n') {
                size.y += m_fontHeight;
                if (size.x < cw)
                    size.x = cw;
                cw = 0;
            } else {
                // Check for correct glyph
                int gi = c - m_regStart;
                if (gi < 0 || gi >= m_regLength)
                    gi = m_regLength;
                cw += m_glyphs[gi].size.x;
            }
        }
        if (size.x < cw)
            size.x = cw;
        return size;
    }

    void SpriteFont::draw(SpriteBatch& batch, const char* s, glm::vec2 position, glm::vec2 scaling, 
                          float depth, ColorRGBA8 tint, Justification just /* = Justification::LEFT */) {
		glm::vec2 tp = position;
		// Apply justification
		if (just == Justification::MIDDLE) {
			tp.x -= measure(s).x * scaling.x / 2;
		}
		else if (just == Justification::RIGHT) {
			tp.x -= measure(s).x * scaling.x;
		}
		for (int si = 0; s[si] != 0; si++) {
			char c = s[si];
			if (s[si] == '\n') {
				tp.y -= m_fontHeight * scaling.y;
				tp.x = position.x;
				if (just == Justification::MIDDLE) {
					tp.x -= measure(s).x * scaling.x / 2;
				}
				else if (just == Justification::RIGHT) {
					tp.x -= measure(s).x * scaling.x;
				}
			}
			else {
				// Check for correct glyph
				int gi = c - m_regStart;
				if (gi < 0 || gi >= m_regLength)
					gi = m_regLength;
				glm::vec4 destRect(tp, m_glyphs[gi].size * scaling);
				batch.draw(destRect, m_glyphs[gi].uvRect, m_texID, depth, tint);
				tp.x += m_glyphs[gi].size.x * scaling.x;
			}
		}
    }

}