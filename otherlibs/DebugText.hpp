#ifndef __INC_DEBUGTEXT_
#define __INC_DEBUGTEXT_

/**
 * @file DebugText.hpp
 * @author Gao Li (https://github.com/GL-MineCode)
 * @brief A minimalist and lightweight text (bitmap font) drawing library based on SDL2, supporting the UTF-8 character set
 * @version 1.0
 * @date 2026-06-07
 * 
 * @copyright Copyright (c) 2026
 * 
 * @details Licensed under the MIT License. See LICENSE file for full license.
 */

#include <SDL.h>
#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>
#include <string>
// #include "decode.hpp"

#ifndef __INC_DECODE_

/**
 * @brief Convert a UTF-8 encoded string to a vector of Unicode codepoints
 * 
 * Decodes a UTF-8 string into its individual Unicode codepoints.
 * Performs validation at each step and returns whatever has been
 * successfully decoded so far if an invalid sequence is encountered.
 *
 * @param utf8_str The UTF-8 encoded input string
 * @return std::vector<uint32_t> A vector of Unicode codepoints decoded from the input
 */
std::vector<uint32_t> utf8_to_codepoints_noexcept(const std::string& utf8_str) noexcept {
    std::vector<uint32_t> codepoints;
    
    for (size_t i = 0; i < utf8_str.size(); ) {
        uint8_t first_byte = static_cast<uint8_t>(utf8_str[i]);
        
        size_t char_len = 0;
        uint32_t codepoint = 0;
        
        if (first_byte <= 0x7F) {
            char_len = 1;
            codepoint = first_byte;
        } else if ((first_byte & 0xE0) == 0xC0) {
            char_len = 2;
            codepoint = first_byte & 0x1F;
        } else if ((first_byte & 0xF0) == 0xE0) {
            char_len = 3;
            codepoint = first_byte & 0x0F;
        } else if ((first_byte & 0xF8) == 0xF0) {
            char_len = 4;
            codepoint = first_byte & 0x07;
        } else {
            return codepoints;
        }
        
        if (i + char_len > utf8_str.size()) {
            return codepoints;
        }
        
        for (size_t j = 1; j < char_len; ++j) {
            uint8_t next_byte = static_cast<uint8_t>(utf8_str[i + j]);
            
            if ((next_byte & 0xC0) != 0x80) {
                return codepoints;
            }
            
            codepoint = (codepoint << 6) | (next_byte & 0x3F);
        }
        
        if ((char_len == 1 && codepoint > 0x7F) ||
            (char_len == 2 && (codepoint < 0x80 || codepoint > 0x7FF)) ||
            (char_len == 3 && (codepoint < 0x800 || codepoint > 0xFFFF)) ||
            (char_len == 4 && (codepoint < 0x10000 || codepoint > 0x10FFFF))) {
            return codepoints;
        }
        
        codepoints.push_back(codepoint);
        i += char_len;
    }
    
    return codepoints;
}

#endif

// You can replace this header with your own one.
#include "fusionpixel_12px_mono.hpp"

/**
 * @brief Read a 4-byte integer from a byte array without alignment concerns
 * 
 * Interprets the first 4 bytes at the given address as a little-endian int.
 * Used for reading the glyph count and glyph offset map from the font data.
 *
 * @param data Pointer to the byte array
 * @return int The integer value read from the array
 */
inline int bytearray_read_int(const uint8_t* data) {
    return *(const int*)data;
}

/**
 * @brief Create an SDL surface with rendered debug text using bitmap font
 * 
 * Parses the input text (supports UTF-8), computes the required surface
 * dimensions (handling newlines and tabs), then renders each glyph from
 * the embedded bitmap font data onto the surface with the specified color.
 *
 * @param text The UTF-8 text string to render
 * @param color The foreground color for the text
 * @return SDL_Surface* A newly created SDL surface with the rendered text,
 *         or nullptr if the text is empty or surface creation fails
 */
SDL_Surface* createDBTextSurface(const char* text, const SDL_Color& color) {
    int sur_width = 0;
    int sur_height = 0;
    
    std::vector<uint32_t> codepoints = utf8_to_codepoints_noexcept(text);

    int num_glyph = bytearray_read_int(font_data);
    const int* glyph_map = reinterpret_cast<const int*>(font_data + sizeof(int));

    //Default line height
    #define DBTEXT_DEFAULT_LINE_HEIGHT 12

    //Default tab width
    #define DBTEXT_DEFAULT_TAB_WIDTH 48

    int line_max_height = DBTEXT_DEFAULT_LINE_HEIGHT;

    int cnt_width = 0;

    for(auto i : codepoints){
        uint32_t r = i;
        if (static_cast<int>(r) >= num_glyph) {
            r = 1;
        }
        if(r == '\n'){
            sur_height += line_max_height;
            sur_width = std::max(cnt_width,sur_width);
            line_max_height = DBTEXT_DEFAULT_LINE_HEIGHT;
            cnt_width = 0;
            continue;
        }
        else if(r == '\t'){
            cnt_width += DBTEXT_DEFAULT_TAB_WIDTH;
            continue;
        }
        int glyph_offset = glyph_map[r];
        const uint8_t* glyph_ptr = font_data + glyph_offset;
        uint8_t width = *glyph_ptr;
        uint8_t height = *(glyph_ptr+1);
        line_max_height = std::max<int>(line_max_height,height);
        cnt_width += width;
    }
    sur_width = std::max(cnt_width,sur_width);
    sur_height += line_max_height;

    if(sur_width == 0 || sur_height == 0) return nullptr;
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0,sur_width, sur_height,32, SDL_PIXELFORMAT_RGBA8888);
    if (!surface) return nullptr;
    SDL_FillRect(surface, NULL, 0x00000000);

    int cx = 0;
    int cy = 0;

    //变量复用
    line_max_height = DBTEXT_DEFAULT_LINE_HEIGHT;

    for(auto i : codepoints){
        uint32_t r = i;
        if (static_cast<int>(r) >= num_glyph) {
            r = 1;
        }
        if(r == '\n'){
            cy += line_max_height;
            line_max_height = DBTEXT_DEFAULT_LINE_HEIGHT;
            cx = 0;
            continue;
        }
        else if(r == '\t'){
            cx += DBTEXT_DEFAULT_TAB_WIDTH;
            continue;
        }
        int glyph_offset = glyph_map[r];
        const uint8_t* glyph_ptr = font_data + glyph_offset;
        uint8_t width = *glyph_ptr;
        glyph_ptr++;
        uint8_t height = *glyph_ptr;
        glyph_ptr++;

        uint32_t color_value = (color.r << 24) | (color.g << 16) | (color.b << 8) | (color.a);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int pos = x + y * width;
                int byte_index = pos / 8;
                int bit_index = 7 - (pos % 8);
                
                uint8_t bit = (glyph_ptr[byte_index] >> bit_index) & 1;
                
                uint32_t* pixel = reinterpret_cast<uint32_t*>(
                    static_cast<uint8_t*>(surface->pixels) + 
                    (y + cy) * surface->pitch + 
                    (x + cx) * 4
                );

                // if(((uint64_t)pixel - (uint64_t)(surface->pixels)) >= surface->pitch * surface->h){
                //     printf("WTF?\n");
                // }

                if (bit == 0) {
                    *pixel = color_value;
                } else {
                    *pixel = 0x00000000;
                }
            }
        }

        line_max_height = std::max<int>(line_max_height,height);
        cx += width;
    }

    return surface;
}

/**
 * @brief Render debug text onto an SDL renderer at the specified position
 * 
 * Creates a text surface via createDBTextSurface(), converts it to a texture,
 * and copies it to the renderer. Supports scaling via ptsize and anchor-point
 * positioning via the center parameter.
 *
 * @param renderer The SDL renderer to draw onto
 * @param ux The x-coordinate of the text position (before center adjustment)
 * @param uy The y-coordinate of the text position (before center adjustment)
 * @param color The text color
 * @param ptsize Scaling factor (1.0f = original font size)
 * @param text The UTF-8 text string to render
 * @param center Anchor point as a fraction of width/height (0,0 = top-left, 0.5,0.5 = center, 1,1 = bottom-right)
 * @return SDL_Rect The actual rectangle of the rendered text on the renderer
 */
SDL_Rect paintDBText(SDL_Renderer* renderer,int ux,int uy,const SDL_Color& color,float ptsize,const char* text,const SDL_FPoint& center = {0.0f,0.0f}){
    SDL_Surface* sur = createDBTextSurface(text,color);
    if(!sur) return {-1,-1,-1,-1};
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer,sur);
    SDL_Rect temp = {
        ux,
        uy,
        round(sur->w * ptsize),
        round(sur->h * ptsize)
    };
    temp.x -= temp.w * center.x;
    temp.y -= temp.h * center.y;
    SDL_RenderCopy(renderer,tex,NULL,&temp);
    SDL_FreeSurface(sur);
    SDL_DestroyTexture(tex);
    return temp;
}

/**
 * @brief Render formatted debug text (printf-style) onto an SDL renderer
 * 
 * Convenience wrapper around paintDBText() that accepts printf-style
 * format strings with variable arguments. The formatted result is
 * limited to 256 characters.
 *
 * @param renderer The SDL renderer to draw onto
 * @param ux The x-coordinate of the text position
 * @param uy The y-coordinate of the text position
 * @param color The text color
 * @param ptsize Scaling factor (1.0f = original font size)
 * @param center Anchor point as a fraction of width/height
 * @param text The printf-style format string
 * @param ... Variable arguments for the format string
 * @return SDL_Rect The actual rectangle of the rendered text on the renderer
 */
SDL_Rect paintDBTextFormat(SDL_Renderer* renderer, int ux, int uy, SDL_Color color, float ptsize,const SDL_FPoint& center, const char* text, ...) {
    char buffer[256];
    va_list args;
    
    va_start(args, text);
    vsnprintf(buffer, sizeof(buffer), text, args);
    va_end(args);

    return paintDBText(renderer,ux,uy,color,ptsize,buffer,center);
}

#endif //__INC_DEBUGTEXT_
