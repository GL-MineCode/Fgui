#ifndef __INC_FGUI_GRAPHIC_
#define __INC_FGUI_GRAPHIC_

#include "Fgui_includes.hpp"
#include "Fgui_math.hpp"
#include "Fgui_defs.hpp"

void paint3DSurface(SDL_Renderer* r,SDL_Texture* texture,SDL_FPoint& perA,SDL_FPoint& perB,SDL_FPoint& perC,SDL_FPoint& perD,float uv_x = 0,float uv_y = 0,float uv_w = 1,float uv_h = 1){

	SDL_FPoint perE = quadrilateralCentroid(perA,perB,perC,perD);
	SDL_FPoint ETP = {
		uv_x + uv_w / 2,
		uv_y + uv_h / 2
	};
	SDL_Vertex rect[9] = {
		/* 0 */{perA, {255, 255, 255, 255}, {uv_x, uv_y}},
		/* 1 */{perB, {255, 255, 255, 255}, {uv_x + uv_w, uv_y}},
		/* 2 */{perC, {255, 255, 255, 255}, {uv_x + uv_w, uv_y + uv_h}},
		/* 3 */{perD, {255, 255, 255, 255}, {uv_x, uv_y + uv_h}},
		/* 4 */{perE, {255, 255, 255, 255}, ETP},
	};
	const int ind[12] = {
		0,1,4,
		1,2,4,
		2,3,4,
		3,0,4,
	};
	SDL_RenderGeometry(r, texture, rect, 9, ind,12);
}

void paint3DSurfaceQ(SDL_Renderer* r,SDL_Texture* texture,SDL_FPoint& perA,SDL_FPoint& perB,SDL_FPoint& perC,SDL_FPoint& perD,float uv_x = 0,float uv_y = 0,float uv_w = 1,float uv_h = 1){

	SDL_FPoint perE = quadrilateralCentroid(perA,perB,perC,perD);
	SDL_FPoint perAB = lineMidpoint(perA,perB);
	SDL_FPoint perBC = lineMidpoint(perB,perC);
	SDL_FPoint perCD = lineMidpoint(perC,perD);
	SDL_FPoint perDA = lineMidpoint(perD,perA);
	paint3DSurface(r,texture,perA,perAB,perE,perDA,uv_x,uv_y,uv_w / 2,uv_h / 2);
	paint3DSurface(r,texture,perAB,perB,perBC,perE,uv_x + uv_w / 2,uv_y,uv_w / 2,uv_h / 2);
	paint3DSurface(r,texture,perE,perBC,perC,perCD,uv_x + uv_w / 2,uv_y + uv_h / 2,uv_w / 2,uv_h / 2);
	paint3DSurface(r,texture,perDA,perE,perCD,perD,uv_x,uv_y + uv_h / 2,uv_w / 2,uv_h / 2);
}

void paint3DSurfaceDQ(SDL_Renderer* r,SDL_Texture* texture,SDL_FPoint& perA,SDL_FPoint& perB,SDL_FPoint& perC,SDL_FPoint& perD,float uv_x = 0,float uv_y = 0,float uv_w = 1,float uv_h = 1){

	SDL_FPoint perE = quadrilateralCentroid(perA,perB,perC,perD);
	SDL_FPoint perAB = lineMidpoint(perA,perB);
	SDL_FPoint perBC = lineMidpoint(perB,perC);
	SDL_FPoint perCD = lineMidpoint(perC,perD);
	SDL_FPoint perDA = lineMidpoint(perD,perA);
	paint3DSurfaceQ(r,texture,perA,perAB,perE,perDA,uv_x,uv_y,uv_w / 2,uv_h / 2);
	paint3DSurfaceQ(r,texture,perAB,perB,perBC,perE,uv_x + uv_w / 2,uv_y,uv_w / 2,uv_h / 2);
	paint3DSurfaceQ(r,texture,perE,perBC,perC,perCD,uv_x + uv_w / 2,uv_y + uv_h / 2,uv_w / 2,uv_h / 2);
	paint3DSurfaceQ(r,texture,perDA,perE,perCD,perD,uv_x,uv_y + uv_h / 2,uv_w / 2,uv_h / 2);
}

void paint3DSurfaceI(SDL_Renderer* r,SDL_Texture* texture,SDL_FPoint& perA,SDL_FPoint& perB,SDL_FPoint& perC,SDL_FPoint& perD,int gen,float uv_x = 0,float uv_y = 0,float uv_w = 1,float uv_h = 1){
	SDL_FPoint perE = quadrilateralCentroid(perA,perB,perC,perD);
	SDL_FPoint perAB = lineMidpoint(perA,perB);
	SDL_FPoint perBC = lineMidpoint(perB,perC);
	SDL_FPoint perCD = lineMidpoint(perC,perD);
	SDL_FPoint perDA = lineMidpoint(perD,perA);
	if(gen == 0){
		paint3DSurface(r,texture,perA,perAB,perE,perDA,uv_x,uv_y,uv_w / 2,uv_h / 2);
		paint3DSurface(r,texture,perAB,perB,perBC,perE,uv_x + uv_w / 2,uv_y,uv_w / 2,uv_h / 2);
		paint3DSurface(r,texture,perE,perBC,perC,perCD,uv_x + uv_w / 2,uv_y + uv_h / 2,uv_w / 2,uv_h / 2);
		paint3DSurface(r,texture,perDA,perE,perCD,perD,uv_x,uv_y + uv_h / 2,uv_w / 2,uv_h / 2);
		return;
	}
	paint3DSurfaceI(r,texture,perA,perAB,perE,perDA,gen - 1,uv_x,uv_y,uv_w / 2,uv_h / 2);
	paint3DSurfaceI(r,texture,perAB,perB,perBC,perE,gen - 1,uv_x + uv_w / 2,uv_y,uv_w / 2,uv_h / 2);
	paint3DSurfaceI(r,texture,perE,perBC,perC,perCD,gen - 1,uv_x + uv_w / 2,uv_y + uv_h / 2,uv_w / 2,uv_h / 2);
	paint3DSurfaceI(r,texture,perDA,perE,perCD,perD,gen - 1,uv_x,uv_y + uv_h / 2,uv_w / 2,uv_h / 2);
}

SDL_Rect getTextSize(TTF_Font *f, const char *str)
{
	SDL_Rect size = {0, 0};
	TTF_SizeUTF8(f, str, &size.w, &size.h);
	return size;
}

SDL_Rect paintTextWithRect(SDL_Renderer *r, int x, int y, TTF_Font *f, const char *str, SDL_Color color, SDL_Color color_rect)
{
	if (!str[0])
		return {x, y, -1, -1};
	SDL_Surface *sur = TTF_RenderUTF8_Blended(f, str, color);
	SDL_Rect size = {x, y, sur->w, sur->h};
	SDL_Texture *tex = SDL_CreateTextureFromSurface(r, sur);

	SDL_SetTextureAlphaMod(tex, color.a);
	SDL_SetRenderDrawColor(r, color_rect.r, color_rect.g, color_rect.b, color_rect.a);
	SDL_RenderFillRect(r, &size);
	SDL_RenderCopy(r, tex, NULL, &size);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(sur);
	return size;
}

SDL_Rect paintText(SDL_Renderer *r, int x, int y, TTF_Font *f, const char *str, SDL_Color color,const SDL_FPoint& center = {0.0f,0.0f})
{
	if (!str[0])
		return {x, y, -1, -1};
	SDL_Surface *sur = TTF_RenderUTF8_Blended(f, str, color);
	SDL_Rect size = {x - (float)sur->w * center.x, y - (float)sur->h * center.y, sur->w, sur->h};
	SDL_Texture *tex = SDL_CreateTextureFromSurface(r, sur);

	SDL_SetTextureAlphaMod(tex, color.a);

	SDL_RenderCopy(r, tex, NULL, &size);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(sur);
	return size;
}

SDL_Rect paintText_wrapped(SDL_Renderer *r, int x, int y,int warpLen, TTF_Font *f, const char *str, SDL_Color color,const SDL_FPoint& center = {0.0f,0.0f})
{
	if (!str[0])
		return {x, y, -1, -1};
	SDL_Surface *sur = TTF_RenderUTF8_Blended_Wrapped(f, str, color, warpLen);
	SDL_Rect size = {x - (float)sur->w * center.x, y - (float)sur->h * center.y, sur->w, sur->h};
	SDL_Texture *tex = SDL_CreateTextureFromSurface(r, sur);

	SDL_SetTextureAlphaMod(tex, color.a);

	SDL_RenderCopy(r, tex, NULL, &size);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(sur);
	return size;
}

SDL_Rect paintText_wm(SDL_Renderer *r, int x, int y,int wmax, TTF_Font *f, const char *str, SDL_Color color,const SDL_FPoint& center = {0.0f,0.0f})
{
	if (!str[0])
		return {x, y, -1, -1};
	SDL_Surface *sur = TTF_RenderUTF8_Blended(f, str, color);
	SDL_Rect size = {x, y, sur->w, sur->h};
	SDL_Rect wmax_rc = size;
	wmax_rc.x = 0;
	wmax_rc.y = 0;
	if(size.w > wmax)
	{
		wmax_rc.w = wmax;
		size.w = wmax;
	}

	size.x -= (float)sur->w * center.x;
	size.y -= (float)sur->h * center.y;

	SDL_Texture *tex = SDL_CreateTextureFromSurface(r, sur);

	SDL_SetTextureAlphaMod(tex, color.a);

	SDL_RenderCopy(r, tex, &wmax_rc, &size);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(sur);
	return size;
}

SDL_Rect paintText_roll(SDL_Renderer *r, int x, int y,int wmax,float speed, TTF_Font *f, const char *str, SDL_Color color,SDL_FPoint center = {0.0f,0.0f})
{
	if (!str[0])
		return {x, y, -1, -1};
	SDL_Surface *sur = TTF_RenderUTF8_Blended(f, str, color);
	SDL_Rect size = {x, y, sur->w, sur->h};
	SDL_Rect wmax_rc = size;
	if(size.w < wmax){
		wmax_rc.x = 0;
	}else{
		int xll = (size.w - wmax);
		float temp = fmod(((SDL_GetTicks64() / 10.0f) * speed),xll * 2 + 200);
		if(temp < xll){
			wmax_rc.x = temp;
		}
		else if(temp < (xll + 100)){
			wmax_rc.x = xll;
		}
		else if(temp < (xll * 2 + 100)){
			wmax_rc.x = (xll * 2 + 100) - temp;
		}
		else{
			wmax_rc.x = 0;
		}
	}
	wmax_rc.y = 0;
	if(size.w > wmax)
	{
		wmax_rc.w = wmax;
		size.w = wmax;
	}

	size.x -= (float)sur->w * center.x;
	size.y -= (float)sur->h * center.y;

	SDL_Texture *tex = SDL_CreateTextureFromSurface(r, sur);

	SDL_SetTextureAlphaMod(tex, color.a);

	SDL_RenderCopy(r, tex, &wmax_rc, &size);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(sur);
	return size;
}

SDL_Rect paintText_compressed(SDL_Renderer *r, int x, int y,int wmax, TTF_Font *f, const char *str, SDL_Color color,SDL_FPoint center = {0.0f,0.0f}){
	if (!str[0])
		return {x, y, -1, -1};
	SDL_Surface *sur = TTF_RenderUTF8_Blended(f, str, color);
	SDL_Rect size = {x, y, sur->w, sur->h};
	if(size.w > wmax)
	{
		size.w = wmax;
	}

	size.x -= (float)sur->w * center.x;
	size.y -= (float)sur->h * center.y;

	SDL_Texture *tex = SDL_CreateTextureFromSurface(r, sur);

	SDL_SetTextureAlphaMod(tex, color.a);

	SDL_RenderCopy(r, tex, NULL, &size);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(sur);
	return size;
};

void paintGradientRectV(SDL_Renderer *ren, SDL_Rect r, SDL_Color in, SDL_Color out)
{
	SDL_Vertex vert[] = {
		{{r.x, r.y}, in},
		{{r.x + r.w, r.y}, in},
		{{r.x + r.w, r.y + r.h}, out},
		{{r.x, r.y + r.h}, out},
	};
	int RectIndece[] = {0, 1, 2, 2, 3, 0};
	SDL_RenderGeometry(ren, 0, vert, 4, RectIndece, sizeof(RectIndece) / sizeof(RectIndece[0]));
}

void paintGradientRectH(SDL_Renderer *ren, SDL_Rect r, SDL_Color in, SDL_Color out)
{
	SDL_Vertex vert[] = {
		{{r.x, r.y}, in},
		{{r.x + r.w, r.y}, out},
		{{r.x + r.w, r.y + r.h}, out},
		{{r.x, r.y + r.h}, in},
	};
	int RectIndece[] = {0, 1, 2, 2, 3, 0};
	SDL_RenderGeometry(ren, 0, vert, 4, RectIndece, sizeof(RectIndece) / sizeof(RectIndece[0]));
}

void paintX(SDL_Renderer* render,SDL_Rect rct,SDL_Color color){
	SDL_SetRenderDrawColor(render,ColorArg(color));
	SDL_RenderDrawLine(render, rct.x, rct.y, rct.x + rct.w, rct.y + rct.h);
	SDL_RenderDrawLine(render, rct.x + rct.w, rct.y, rct.x, rct.y + rct.h);
}

void paintAnnotation(SDL_Renderer *ren, SDL_Rect rect)
{
	SDL_SetRenderDrawColor(ren,255, 255, 238, 123);
	SDL_RenderFillRect(ren,&rect);
	SDL_SetRenderDrawColor(ren,255, 0, 0, 122);
	SDL_RenderDrawRect(ren,&rect);
	int
		p1x = rect.x + rect.w - 7,
		p1y = rect.y + rect.h - 4,
		p2x = rect.x + rect.w - 4,
		p2y = rect.y + rect.h - 7;
	SDL_SetRenderDrawColor(ren, 68, 68, 68, 255);
	SDL_RenderDrawLine(ren, p1x, p1y, p2x, p2y);
	p1x--;
	p2y--;
	SDL_SetRenderDrawColor(ren, 237, 237, 230, 255);
	SDL_RenderDrawLine(ren, p1x, p1y, p2x, p2y);
	p1x--;
	p2y--;
	SDL_SetRenderDrawColor(ren, 68, 68, 68, 255);
	SDL_RenderDrawLine(ren, p1x, p1y, p2x, p2y);
	p1x--;
	p2y--;
	SDL_SetRenderDrawColor(ren, 237, 237, 230, 255);
	SDL_RenderDrawLine(ren, p1x, p1y, p2x, p2y);
	p1x--;
	p2y--;
	SDL_SetRenderDrawColor(ren, 68, 68, 68, 255);
	SDL_RenderDrawLine(ren, p1x, p1y, p2x, p2y);
	p1x--;
	p2y--;
	SDL_SetRenderDrawColor(ren, 237, 237, 230, 255);
	SDL_RenderDrawLine(ren, p1x, p1y, p2x, p2y);
	p1x--;
	p2y--;
	SDL_SetRenderDrawColor(ren, 68, 68, 68, 255);
	SDL_RenderDrawLine(ren, p1x, p1y, p2x, p2y);
	rect.x--;
	rect.y--;
	rect.w += 2;
	rect.h += 2;
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderDrawRect(ren, &rect);
}

SDL_Rect paintOutRelief(SDL_Renderer* ren,SDL_Rect base){
	SDL_SetRenderDrawColor(ren,192,192,192,255);
    SDL_RenderFillRect(ren,&base);
    SDL_SetRenderDrawColor(ren,0,0,0,255);
    SDL_RenderDrawRect(ren,&base);
	base.w -= 3;
	base.h -= 3;
	base.x++;
	base.y++;
	SDL_SetRenderDrawColor(ren,255,255,255,255);
	SDL_RenderDrawLine(ren,base.x,base.y,base.x + base.w,base.y);
	SDL_RenderDrawLine(ren,base.x,base.y,base.x,base.y + base.h);
	SDL_SetRenderDrawColor(ren,128,128,128,255);
	SDL_RenderDrawLine(ren,base.x + base.w,base.y + base.h,base.x + base.w,base.y);
	SDL_RenderDrawLine(ren,base.x + base.w,base.y + base.h,base.x,base.y + base.h);
	SDL_SetRenderDrawColor(ren,192,192,192,255);
	SDL_RenderDrawPoint(ren,base.x + base.w,base.y);
	SDL_RenderDrawPoint(ren,base.x,base.y + base.h);
	return base;
}

SDL_Rect paintInRelief(SDL_Renderer* ren,SDL_Rect base){
	SDL_SetRenderDrawColor(ren,128,128,128,255);
    SDL_RenderFillRect(ren,&base);
    SDL_SetRenderDrawColor(ren,255,255,255,255);
    SDL_RenderDrawRect(ren,&base);
	base.w -= 3;
	base.h -= 3;
	base.x++;
	base.y++;
	SDL_SetRenderDrawColor(ren,0,0,0,255);
	SDL_RenderDrawLine(ren,base.x,base.y,base.x + base.w,base.y);
	SDL_RenderDrawLine(ren,base.x,base.y,base.x,base.y + base.h);
	return base;
}

SDL_Rect paintReliefButton(SDL_Renderer *r,SDL_Rect rect,TTF_Font* font,const char* str,bool state){
	if(state){
		paintInRelief(r,rect);
		paintText(r,rect.x + rect.w / 2,rect.y + rect.h / 2,font,str,{200,200,200,255},{0.5f,0.5f});
	}else{
		paintOutRelief(r,rect);
		paintText(r,rect.x + rect.w / 2,rect.y + rect.h / 2,font,str,{0,0,0,255},{0.5f,0.5f});
	}
	return rect;
}

SDL_Rect paintCard(SDL_Renderer *r,SDL_Texture* obverse,SDL_Texture* reverse,SDL_Rect rect,double tangle_rad,SDL_Color color_edge){
	SDL_Rect after_flip = rect;
	double p = cos(tangle_rad);
	after_flip.w = (double)rect.w * p;
	if(after_flip.w > 0){
		after_flip.x += (rect.w - after_flip.w) / 2;
		SDL_RenderCopy(r,obverse,NULL,&after_flip);
		SDL_SetRenderDrawColor(r,0,0,0,(1.0f - p) * 180.0f);
		SDL_RenderFillRect(r,&after_flip);
	}else{
		after_flip.w = -after_flip.w;
		after_flip.x += (rect.w - after_flip.w) / 2;
		SDL_RenderCopy(r,reverse,NULL,&after_flip);
		SDL_SetRenderDrawColor(r,0,0,0,(1.0f + p) * 180.0f);
		SDL_RenderFillRect(r,&after_flip);
	}
	SDL_SetRenderDrawColor(r,color_edge.r,color_edge.g,color_edge.b,color_edge.a);
	SDL_RenderDrawRect(r,&after_flip);
	return after_flip;
}

SDL_Rect paintStereoCard(SDL_Renderer *r,SDL_Texture* obverse,SDL_Texture* reverse,SDL_Rect rect,float x_rotate,float y_rotate,double focus_distance){
	int z = focus_distance;

	SDL_Point p1 = {-rect.w / 2, -rect.h / 2};
	int z1 = z;
	SDL_Point p2 = {+rect.w / 2, -rect.h / 2};
	int z2 = z;
	SDL_Point p3 = {+rect.w / 2, +rect.h / 2};
	int z3 = z;
	SDL_Point p4 = {-rect.w / 2, +rect.h / 2};
	int z4 = z;

	rotatePointAround(p1.x,z1,0,z,p1.x,z1,x_rotate);
	rotatePointAround(p2.x,z2,0,z,p2.x,z2,x_rotate);
	rotatePointAround(p3.x,z3,0,z,p3.x,z3,x_rotate);
	rotatePointAround(p4.x,z4,0,z,p4.x,z4,x_rotate);

	rotatePointAround(p1.y,z1,0,z,p1.y,z1,y_rotate);
	rotatePointAround(p2.y,z2,0,z,p2.y,z2,y_rotate);
	rotatePointAround(p3.y,z3,0,z,p3.y,z3,y_rotate);
	rotatePointAround(p4.y,z4,0,z,p4.y,z4,y_rotate);

	SDL_FPoint o1,o2,o3,o4;

	projectPoint(p1.x,p1.y,z1,o1.x,o1.y,focus_distance);
	projectPoint(p2.x,p2.y,z2,o2.x,o2.y,focus_distance);
	projectPoint(p3.x,p3.y,z3,o3.x,o3.y,focus_distance);
	projectPoint(p4.x,p4.y,z4,o4.x,o4.y,focus_distance);

	int shift_x = rect.x + rect.w / 2;
	int shift_y = rect.y + rect.h / 2;

	o1.x += shift_x;
	o1.y += shift_y;
	o2.x += shift_x;
	o2.y += shift_y;
	o3.x += shift_x;
	o3.y += shift_y;
	o4.x += shift_x;
	o4.y += shift_y;

	int xmod = (abs(x_rotate) + M_PI / 2) / (M_PI);
	int ymod = (abs(y_rotate) + M_PI / 2) / (M_PI);

	if(((xmod + ymod) % 2) == 0){
		paint3DSurfaceDQ(r,obverse,o1,o2,o3,o4);
	}else{
		paint3DSurfaceDQ(r,reverse,o2,o1,o4,o3);
	}

	return rect;
}

void paintFilledTriangle(SDL_Renderer* ren,const SDL_Point& point1,const SDL_Point& point2,const SDL_Point& point3,SDL_Color color){
	const SDL_Vertex vertexs[3] = {
		{{point1.x,point1.y},color},
		{{point2.x,point2.y},color},
		{{point3.x,point3.y},color},
	};
	SDL_RenderGeometry(ren,NULL,vertexs,3,NULL,0);
}

// ============ 圆形 / 圆角矩形绘制（高效版）============
// 全部基于整数扫描线 + Bresenham 中点画圆：不创建临时纹理、不切换渲染目标、
// 无手动堆内存管理，彻底避免每帧 SDL_CreateTexture / SDL_SetRenderTarget 的高开销。

namespace {

// 生成第一象限圆弧点（Bresenham 中点法），同时含 (x,y) 与 (y,x)，覆盖从 (0,r) 到 (r,0)。
static void arcQuadrant1(int radius, std::vector<SDL_Point>& out) {
	out.clear();
	int x = 0, y = radius;
	int d = 1 - radius, dE = 3, dSE = 5 - 2 * radius;
	while (x <= y) {
		out.push_back({x, y});
		if (x != y) out.push_back({y, x});
		if (d < 0) { d += dE; dE += 2; dSE += 2; }
		else { d += dSE; dE += 2; dSE += 4; --y; }
		++x;
	}
}

// 整圆扫描线填充：纯整数、O(r)，每行一条水平线，无浮点、无内存分配。
static void fillCircleScanline(SDL_Renderer* renderer, int cx, int cy, int r) {
	int x = r;
	for (int y = 0; y <= r; ++y) {
		while (x > 0 && x * x + y * y > r * r) --x;
		SDL_RenderDrawLine(renderer, cx - x, cy + y, cx + x, cy + y);
		if (y > 0) SDL_RenderDrawLine(renderer, cx - x, cy - y, cx + x, cy - y);
	}
}

// 填充 1/4 圆（quadrant: 1=左上 2=右上 3=左下 4=右下），圆心 (cx,cy) 为角心。
static void fillQuarterCircle(SDL_Renderer* renderer, int cx, int cy, int r, int quadrant) {
	int x = r;
	for (int y = 0; y <= r; ++y) {
		while (x > 0 && x * x + y * y > r * r) --x;
		switch (quadrant) {
			case 1: SDL_RenderDrawLine(renderer, cx - x, cy - y, cx, cy - y); break;
			case 2: SDL_RenderDrawLine(renderer, cx, cy - y, cx + x, cy - y); break;
			case 3: SDL_RenderDrawLine(renderer, cx - x, cy + y, cx, cy + y); break;
			case 4: SDL_RenderDrawLine(renderer, cx, cy + y, cx + x, cy + y); break;
		}
	}
}

} // namespace

void paintCircle(SDL_Renderer* renderer, SDL_Color color, SDL_Point center, int radius) {
	if (radius <= 0) return;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	std::vector<SDL_Point> q1;
	arcQuadrant1(radius, q1);
	std::vector<SDL_Point> pts;
	pts.reserve(q1.size() * 4);
	for (auto& p : q1) {
		pts.push_back({center.x + p.x, center.y - p.y}); // 右上
		pts.push_back({center.x - p.x, center.y - p.y}); // 左上
		pts.push_back({center.x + p.x, center.y + p.y}); // 右下
		pts.push_back({center.x - p.x, center.y + p.y}); // 左下
	}
	SDL_RenderDrawPoints(renderer, pts.data(), static_cast<int>(pts.size()));
}

#define PCIRCLE_TOPLEFT 1
#define PCIRCLE_TOPRIGHT 2
#define PCIRCLE_BOTTOMLEFT 4
#define PCIRCLE_BOTTOMRIGHT 8
#define PCIRCLE_ALL (PCIRCLE_TOPLEFT | PCIRCLE_TOPRIGHT | PCIRCLE_BOTTOMLEFT | PCIRCLE_BOTTOMRIGHT)

void paintCircleEx(SDL_Renderer* renderer, SDL_Color edge_color, SDL_Color fill_color, SDL_Point center, int radius, uint32_t flags = PCIRCLE_ALL) {
	if (radius <= 0) return;

	// 填充（flags 控制显示哪些象限）
	SDL_SetRenderDrawColor(renderer, fill_color.r, fill_color.g, fill_color.b, fill_color.a);
	if (flags == PCIRCLE_ALL) {
		fillCircleScanline(renderer, center.x, center.y, radius);
	} else {
		if (flags & PCIRCLE_TOPLEFT)     fillQuarterCircle(renderer, center.x, center.y, radius, 1);
		if (flags & PCIRCLE_TOPRIGHT)    fillQuarterCircle(renderer, center.x, center.y, radius, 2);
		if (flags & PCIRCLE_BOTTOMLEFT)  fillQuarterCircle(renderer, center.x, center.y, radius, 3);
		if (flags & PCIRCLE_BOTTOMRIGHT) fillQuarterCircle(renderer, center.x, center.y, radius, 4);
	}

	// 边缘弧（flags 控制显示哪些象限）
	std::vector<SDL_Point> q1;
	arcQuadrant1(radius, q1);
	std::vector<SDL_Point> pts;
	pts.reserve(q1.size() * 4);
	for (auto& p : q1) {
		if (flags & PCIRCLE_TOPLEFT)     pts.push_back({center.x - p.x, center.y - p.y});
		if (flags & PCIRCLE_TOPRIGHT)    pts.push_back({center.x + p.x, center.y - p.y});
		if (flags & PCIRCLE_BOTTOMLEFT)  pts.push_back({center.x - p.x, center.y + p.y});
		if (flags & PCIRCLE_BOTTOMRIGHT) pts.push_back({center.x + p.x, center.y + p.y});
	}
	SDL_SetRenderDrawColor(renderer, edge_color.r, edge_color.g, edge_color.b, edge_color.a);
	SDL_RenderDrawPoints(renderer, pts.data(), static_cast<int>(pts.size()));
}

void paintRoundedRect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color edge_color, SDL_Color fill_color, int radius) {
	if (rect.w == 0 || rect.h == 0) return;
	standardizationRect(rect, rect);
	radius = std::min(std::min(rect.w, rect.h) / 2, radius);
	if (radius <= 0) {
		SDL_SetRenderDrawColor(renderer, ColorArg(fill_color));
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, ColorArg(edge_color));
		SDL_RenderDrawRect(renderer, &rect);
		return;
	}

	const int rx = rect.x, ry = rect.y, rw = rect.w, rh = rect.h;
	const int c_tl_x = rx + radius, c_tl_y = ry + radius;              // 左上角心
	const int c_tr_x = rx + rw - radius, c_tr_y = ry + radius;         // 右上角心
	const int c_bl_x = rx + radius, c_bl_y = ry + rh - radius;         // 左下角心
	const int c_br_x = rx + rw - radius, c_br_y = ry + rh - radius;    // 右下角心

	// 填充：中部十字矩形 + 四角 1/4 圆
	SDL_SetRenderDrawColor(renderer, fill_color.r, fill_color.g, fill_color.b, fill_color.a);
	SDL_Rect mid = {rx + radius, ry, rw - 2 * radius, rh};
	SDL_RenderFillRect(renderer, &mid);
	mid = {rx, ry + radius, rw, rh - 2 * radius};
	SDL_RenderFillRect(renderer, &mid);
	fillQuarterCircle(renderer, c_tl_x, c_tl_y, radius, 1);
	fillQuarterCircle(renderer, c_tr_x, c_tr_y, radius, 2);
	fillQuarterCircle(renderer, c_bl_x, c_bl_y, radius, 3);
	fillQuarterCircle(renderer, c_br_x, c_br_y, radius, 4);

	// 边缘：四条直线 + 四角圆弧
	SDL_SetRenderDrawColor(renderer, edge_color.r, edge_color.g, edge_color.b, edge_color.a);
	SDL_RenderDrawLine(renderer, c_tl_x, ry, c_tr_x, ry);
	SDL_RenderDrawLine(renderer, c_bl_x, ry + rh, c_br_x, ry + rh);
	SDL_RenderDrawLine(renderer, rx, c_tl_y, rx, c_bl_y);
	SDL_RenderDrawLine(renderer, rx + rw, c_tr_y, rx + rw, c_br_y);

	std::vector<SDL_Point> q1;
	arcQuadrant1(radius, q1);
	std::vector<SDL_Point> pts;
	pts.reserve(q1.size() * 4);
	for (auto& p : q1) {
		pts.push_back({c_br_x + p.x, c_br_y + p.y}); // 右下
		pts.push_back({c_tl_x - p.x, c_tl_y - p.y}); // 左上
		pts.push_back({c_tr_x + p.x, c_tr_y - p.y}); // 右上
		pts.push_back({c_bl_x - p.x, c_bl_y + p.y}); // 左下
	}
	SDL_RenderDrawPoints(renderer, pts.data(), static_cast<int>(pts.size()));
}

void paintFilledCircle(SDL_Renderer* renderer, SDL_Color color, SDL_Point center, float radius) {
	if (radius <= 0.0f) return;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	fillCircleScanline(renderer, center.x, center.y, static_cast<int>(radius + 0.5f));
}

void paintEllipse(SDL_Renderer* renderer, const SDL_Rect& rect, 
                      const SDL_Color* lineColor, const SDL_Color* fillColor) {
    if (!renderer) return;
    
    int centerX = rect.x + rect.w / 2;
    int centerY = rect.y + rect.h / 2;
    int radiusX = rect.w / 2;
    int radiusY = rect.h / 2;
    
    if (fillColor) {
        SDL_SetRenderDrawColor(renderer, fillColor->r, fillColor->g, fillColor->b, fillColor->a);
        
        for (int y = -radiusY; y <= radiusY; y++) {
            int width = (int)(radiusX * sqrt(1 - (y * y) / (float)(radiusY * radiusY)));
            
            SDL_RenderDrawLine(renderer, 
                              centerX - width, 
                              centerY + y, 
                              centerX + width, 
                              centerY + y);
        }
    }
    
    if (lineColor) {
        SDL_SetRenderDrawColor(renderer, lineColor->r, lineColor->g, lineColor->b, lineColor->a);
        
        int x = 0;
        int y = radiusY;
        int radiusX2 = radiusX * radiusX;
        int radiusY2 = radiusY * radiusY;
        int twoRadiusX2 = 2 * radiusX2;
        int twoRadiusY2 = 2 * radiusY2;
        int p;
        int px = 0;
        int py = twoRadiusX2 * y;
        
        p = (int)(radiusY2 - (radiusX2 * radiusY) + (0.25 * radiusX2));
        while (px < py) {
            x++;
            px += twoRadiusY2;
            if (p < 0) {
                p += radiusY2 + px;
            } else {
                y--;
                py -= twoRadiusX2;
                p += radiusY2 + px - py;
            }
            
            SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
            SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
            SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        }
        
        p = (int)(radiusY2 * (x + 0.5) * (x + 0.5) + radiusX2 * (y - 1) * (y - 1) - radiusX2 * radiusY2);
        while (y > 0) {
            y--;
            py -= twoRadiusX2;
            if (p > 0) {
                p += radiusX2 - py;
            } else {
                x++;
                px += twoRadiusY2;
                p += radiusX2 - py + px;
            }
            
            SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
            SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
            SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        }
    }
}

void paintHatchingLines(SDL_Renderer *renderer, SDL_Color color, SDL_Rect rect, int line_size = 1, int freq = 8)
{
	int top = rect.y;
	int bottom = rect.y + rect.h;
	int left = rect.x;
	int right = rect.x + rect.w;
	int max_range = (bottom - top) + (right - left) - 1;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (size_t j = freq / 2;; j += freq)
	{
		int i = j;
		for (size_t k = 0; k < line_size; k++)
		{
			if(i >= max_range) return;
			int x1 = left + i;
			int y1 = top;
			if (x1 >= right)
			{
				y1 = top + (x1 - right) + 1;
				x1 = right - 1;
			}
			int x2 = left;
			int y2 = top + i;
			if (y2 >= bottom)
			{
				x2 = left + (y2 - bottom) + 1;
				y2 = bottom - 1;
			}
			SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
			i++;
		}
	}
}

void paintColorTable(SDL_Renderer* renderer, SDL_Rect rect) {
    if (rect.w <= 0 || rect.h <= 0) return;

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, rect.w, rect.h, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface) return;

    // 锁定surface
    if (SDL_MUSTLOCK(surface)) {
        SDL_LockSurface(surface);
    }

    for (int y = 0; y < rect.h; ++y) {
        float S;
        if (rect.h == 1) {
            S = 1.0f - y;
        } else {
            S = 1.0f - static_cast<float>(y) / (rect.h - 1);
        }

        for (int x = 0; x < rect.w; ++x) {
            float H;
            if (rect.w == 1) {
                H = 0.0f;
            } else {
                H = (static_cast<float>(x) / (rect.w - 1)) * 360.0f;
            }

            Uint8 r, g, b;
            HSVtoRGB(H, S, 1.0f, r, g, b);

            SDL_Color color = { r, g, b, 255 };
            Uint32 pixel = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);

            Uint8* row = (Uint8*)surface->pixels + y * surface->pitch;
            Uint32* pixelPtr = (Uint32*)row + x;
            *pixelPtr = pixel;
        }
    }

    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) return;

    SDL_Rect oldViewport;
    SDL_RenderGetViewport(renderer, &oldViewport);

    SDL_RenderSetViewport(renderer, &rect);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_RenderSetViewport(renderer, &oldViewport);
    SDL_DestroyTexture(texture);
}

void paintValueTable(SDL_Renderer* renderer,float H,float S, SDL_Rect rect) {
    if (rect.w <= 0 || rect.h <= 0) return;

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, rect.w, rect.h, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface) return;

    // 锁定surface
    if (SDL_MUSTLOCK(surface)) {
        SDL_LockSurface(surface);
    }

    for (int y = 0; y < rect.h; ++y) {
        float V = 1.0f - static_cast<float>(y) / (rect.h - 1);
        Uint8 r, g, b;
        HSVtoRGB(H, S, V, r, g, b);

        SDL_Color color = { r, g, b, 255 };
        Uint32 pixel = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);

        for (int x = 0; x < rect.w; ++x) {
            Uint8* row = (Uint8*)surface->pixels + y * surface->pitch;
            Uint32* pixelPtr = (Uint32*)row + x;
            *pixelPtr = pixel;
        }
    }

    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) return;

    SDL_Rect oldViewport;
    SDL_RenderGetViewport(renderer, &oldViewport);

    SDL_RenderSetViewport(renderer, &rect);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_RenderSetViewport(renderer, &oldViewport);
    SDL_DestroyTexture(texture);
}

void paintTransparency(SDL_Renderer* renderer,int x,int y,int side_length,int cw,int ch){
	SDL_Rect sqare = {x,y,side_length,side_length};
	int right = x + side_length * cw;
	int bottom = y + side_length * ch;
	int temp_x = 0;
	int temp_y = 0;
	for (int _x = x; _x < right; _x+=side_length)
	{
		for (int _y = y; _y < bottom; _y+=side_length)
		{
			sqare.x = _x;
			sqare.y = _y;
			if(((temp_x + temp_y) % 2) == 0){
				SDL_SetRenderDrawColor(renderer,255,255,255,255);
			}else{
				SDL_SetRenderDrawColor(renderer,122,122,122,255);
			}
			SDL_RenderFillRect(renderer,&sqare);
			temp_x++;
		}
		temp_x = 0;
		temp_y++;
	}
}

void paintSpeechBubble(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color fill_color, SDL_Color edge_color, int direction/* 1up 2right 3down 4left */) {
    
    SDL_SetRenderDrawColor(renderer, fill_color.r, fill_color.g, fill_color.b, fill_color.a);
    SDL_RenderFillRect(renderer, &rect);
    
    SDL_Vertex vertices[3];
    int triangle_size = 10;
    
    switch (direction) {
        case 1: // 向上
            vertices[0] = { {float(rect.x + rect.w / 2) + 1, float(rect.y - triangle_size)}, fill_color, {1,1} };
            vertices[1] = { {float(rect.x + rect.w / 2 - triangle_size) + 1, float(rect.y)}, fill_color, {1,1} };
            vertices[2] = { {float(rect.x + rect.w / 2 + triangle_size) + 1, float(rect.y)}, fill_color, {1,1} };
            break;
        case 2: // 向右
            vertices[0] = { {float(rect.x + rect.w + triangle_size), float(rect.y + rect.h / 2)}, fill_color, {1,1} };
            vertices[1] = { {float(rect.x + rect.w), float(rect.y + rect.h / 2 - triangle_size)}, fill_color, {1,1} };
            vertices[2] = { {float(rect.x + rect.w), float(rect.y + rect.h / 2 + triangle_size)}, fill_color, {1,1} };
            break;
        case 3: // 向下
            vertices[0] = { {float(rect.x + rect.w / 2) + 1, float(rect.y + rect.h + triangle_size)}, fill_color, {1,1} };
            vertices[1] = { {float(rect.x + rect.w / 2 - triangle_size) + 1, float(rect.y + rect.h)}, fill_color, {1,1} };
            vertices[2] = { {float(rect.x + rect.w / 2 + triangle_size) + 1, float(rect.y + rect.h)}, fill_color, {1,1} };
            break;
        case 4: // 向左
            vertices[0] = { {float(rect.x - triangle_size), float(rect.y + rect.h / 2)}, fill_color, {1,1} };
            vertices[1] = { {float(rect.x), float(rect.y + rect.h / 2 - triangle_size)}, fill_color, {1,1} };
            vertices[2] = { {float(rect.x), float(rect.y + rect.h / 2 + triangle_size)}, fill_color, {1,1} };
            break;
    }
    
    SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
    
    SDL_SetRenderDrawColor(renderer, edge_color.r, edge_color.g, edge_color.b, edge_color.a);
    
    switch (direction) {
        case 1:
            // 下边
            SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
            // 左边（上半部分）
            SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);
            // 右边（上半部分）
            SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
            // 上边分开两段
            SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w / 2 - triangle_size, rect.y);
            SDL_RenderDrawLine(renderer, rect.x + rect.w / 2 + triangle_size, rect.y, rect.x + rect.w, rect.y);
            break;
        case 2:
            // 上边
            SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);
            // 下边
            SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
            // 左边
            SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);
            // 右边分开两段
            SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h / 2 - triangle_size);
            SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y + rect.h / 2 + triangle_size, rect.x + rect.w, rect.y + rect.h);
            break;
        case 3:
            // 上边
            SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);
            // 左边（下半部分）
            SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);
            // 右边（下半部分）
            SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
            // 下边分开两段
            SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w / 2 - triangle_size, rect.y + rect.h);
            SDL_RenderDrawLine(renderer, rect.x + rect.w / 2 + triangle_size, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
            break;
        case 4:
            // 上边
            SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);
            // 下边
            SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
            // 右边
            SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
            // 左边分开两段
            SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h / 2 - triangle_size);
            SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h / 2 + triangle_size, rect.x, rect.y + rect.h);
            break;
    }
    
    switch (direction) {
        case 1: // 向上
            SDL_RenderDrawLine(renderer, 
                rect.x + rect.w / 2 - triangle_size, rect.y, 
                rect.x + rect.w / 2, rect.y - triangle_size);
            SDL_RenderDrawLine(renderer, 
                rect.x + rect.w / 2, rect.y - triangle_size, 
                rect.x + rect.w / 2 + triangle_size, rect.y);
            break;
        case 2: // 向右
            SDL_RenderDrawLine(renderer, 
                rect.x + rect.w, rect.y + rect.h / 2 - triangle_size - 1, 
                rect.x + rect.w + triangle_size, rect.y + rect.h / 2 - 1);
            SDL_RenderDrawLine(renderer, 
                rect.x + rect.w + triangle_size, rect.y + rect.h / 2 - 1, 
                rect.x + rect.w, rect.y + rect.h / 2 + triangle_size - 1);
            break;
        case 3: // 向下
            SDL_RenderDrawLine(renderer, 
                rect.x + rect.w / 2 - triangle_size, rect.y + rect.h, 
                rect.x + rect.w / 2, rect.y + rect.h + triangle_size);
            SDL_RenderDrawLine(renderer, 
                rect.x + rect.w / 2, rect.y + rect.h + triangle_size, 
                rect.x + rect.w / 2 + triangle_size, rect.y + rect.h);
            break;
        case 4: // 向左
            SDL_RenderDrawLine(renderer, 
                rect.x, rect.y + rect.h / 2 - triangle_size - 1, 
                rect.x - triangle_size, rect.y + rect.h / 2 - 1);
            SDL_RenderDrawLine(renderer, 
                rect.x - triangle_size, rect.y + rect.h / 2 + 1, 
                rect.x, rect.y + rect.h / 2 + triangle_size + 1);
            break;
    }
}

void paintWritingLines(SDL_Renderer *renderer, const SDL_Point *points, int count, double process) {
    
    if (!renderer || !points || count < 2 || process <= 0.0) {
        return;
    }
    
    if (process >= 1.0) {
        SDL_RenderDrawLines(renderer, points, count);
        return;
    }

    double totalLength = 0.0;
    for (int i = 0; i < count - 1; ++i) {
        int dx = points[i + 1].x - points[i].x;
        int dy = points[i + 1].y - points[i].y;
        totalLength += std::sqrt(static_cast<double>(dx * dx + dy * dy));
    }
    if (totalLength <= 0.0) {
        return;
    }

    double target = totalLength * process;
    double cumLength = 0.0;
    std::vector<SDL_Point> drawPoints;
    drawPoints.reserve(count);
    drawPoints.push_back(points[0]);

    for (int i = 0; i < count - 1; ++i) {
        int dx = points[i + 1].x - points[i].x;
        int dy = points[i + 1].y - points[i].y;
        double segLength = std::sqrt(static_cast<double>(dx * dx + dy * dy));

        if (cumLength + segLength < target) {
            drawPoints.push_back(points[i + 1]);
            cumLength += segLength;
        } else {
            double remaining = target - cumLength;
            if (remaining <= 0.0) {
                break;
            }
            
            double t = remaining / segLength;
            SDL_Point end;
            end.x = static_cast<int>(points[i].x + dx * t);
            end.y = static_cast<int>(points[i].y + dy * t);
            drawPoints.push_back(end);
            
            SDL_RenderDrawLines(renderer, drawPoints.data(), static_cast<int>(drawPoints.size()));
            return;
        }
    }

    if (drawPoints.size() > 1) {
        SDL_RenderDrawLines(renderer, drawPoints.data(), static_cast<int>(drawPoints.size()));
    }
}

class TooltipWindow{
	public:
	struct TWMsg{
		int msg;
		void* data1;
		void* data2;
	};
    protected:
	struct SharedData{
		SDL_Window* window;
    	SDL_Renderer* renderer;
		bool running_state;
		std::mutex mutex_msg_queue;
		
		std::queue<TWMsg> msg_queue;

		//状态机初始化指示，此状态只会在窗口和渲染器创建完成后发生一次，此后不再以该事件调用回调
		#define TOOLTIP_CALLBACK_INIT 1
		//状态机正常循环，状态机可以使用SDL_PollEvent获取窗口事件，也可以做渲染。此时调用短暂的延迟(<100ms)是被允许的
		#define TOOLTIP_CALLBACK_LOOP 2
		//状态机退出指示，此状态只会在窗口即将销毁时调用一次，状态机应该在接受到该事件后释放已分配的资源（如果有）
		#define TOOLTIP_CALLBACK_QUIT 3
		//状态机接收类持有者发送的事件，此时不能渲染，不能调用延迟，SDL_Renderer*为指向TooltipWindow::TWMsg结构体的指针(原始指针应该被转换为TooltipWindow::TWMsg*)
		#define TOOLTIP_CALLBACK_MSG 4

		//状态机回调
		std::function<void(SDL_Window*,SDL_Renderer*,int)> callback_func;

		SharedData() : window(NULL),renderer(NULL),running_state(true){

		}
	};
	std::shared_ptr<SharedData> th_shared_data;
    public:
    TooltipWindow(std::function<void(SDL_Window*,SDL_Renderer*,int)> _callback_func){
		th_shared_data = std::make_shared<SharedData>();
		th_shared_data->callback_func = _callback_func;
        std::thread _th([shared_data = th_shared_data](){
			shared_data->window = SDL_CreateWindow("Tooltip",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,10,10,SDL_WINDOW_HIDDEN | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_TOOLTIP | SDL_WINDOW_SKIP_TASKBAR | SDL_WINDOW_BORDERLESS);
			shared_data->renderer = SDL_CreateRenderer(shared_data->window,-1,SDL_RENDERER_TARGETTEXTURE);

			shared_data->callback_func(shared_data->window,shared_data->renderer,TOOLTIP_CALLBACK_INIT);
			while(shared_data->running_state){
				{
					std::lock_guard<std::mutex> lg(shared_data->mutex_msg_queue);
					while(!shared_data->msg_queue.empty()){
						auto& i = shared_data->msg_queue.front();
						shared_data->callback_func(shared_data->window,(SDL_Renderer*)&i,TOOLTIP_CALLBACK_MSG);
						shared_data->msg_queue.pop();
					}
				}
				shared_data->callback_func(shared_data->window,shared_data->renderer,TOOLTIP_CALLBACK_LOOP);
			}
			shared_data->callback_func(shared_data->window,shared_data->renderer,TOOLTIP_CALLBACK_QUIT);
		});
		_th.detach();
    }

	void PushMsg(int msg,void* data1,void* data2){
		std::lock_guard<std::mutex> lg(th_shared_data->mutex_msg_queue);
		th_shared_data->msg_queue.push({msg,data1,data2});
	}

	~TooltipWindow(){
		th_shared_data->running_state = false;
	}
};

#include <SDL_syswm.h>
HWND SDL_GetWindowHWND(SDL_Window* win){
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    SDL_GetWindowWMInfo(win,&info);
    return info.info.win.window;
}

/**
 * @brief 弹出窗口（工具窗口），用于下拉框等需要独立显示的控件
 * 
 * 该窗口与 SDL_RenderGetWindow(renderer) 所返回的主窗口关联，是**顶层浮动
 * 工具窗口**（非 WS_CHILD 子窗口），可以超出父窗口边界并浮在其它窗口之上。
 * 拥有独立的 SDL 渲染器，可以自行绘制内容。鼠标输入通过 Win32 窗口过程
 * 子类化在消息层面捕获，不占用主线程的 SDL 事件队列，因此可以安全地在
 * Fgui 的主渲染循环中每帧调用 Render 来驱动。
 * 
 * 使用方式：
 * @code
 * PopupWindow tooltip;
 * tooltip.SetRenderCallback([](SDL_Renderer* r,const SDL_Rect& size,const SDL_Point& mouse){
 *     // 绘制弹出内容
 * });
 * tooltip.Open(renderer, screen_x, screen_y, w, h);  // 打开（screen_x/y 为屏幕物理像素）
 * tooltip.Render();                                  // 每帧调用
 * tooltip.Close();                                   // 关闭
 * @endcode
 */
class PopupWindow
{
public:
    // 渲染回调：renderer 为子窗口渲染器；size 为子窗口客户区尺寸；mouse 为鼠标在客户区内的位置
    using RenderCallback = std::function<void(SDL_Renderer*, const SDL_Rect&, const SDL_Point&)>;

private:
    SDL_Window* sdl_window = nullptr;
    SDL_Renderer* renderer = nullptr;
    HWND hwnd = nullptr;
    WNDPROC original_wndproc = nullptr;

    // 是否处于打开状态
    bool opened = false;

    // DPI 缩放：SDL 逻辑坐标 -> 物理像素（子窗口定位/尺寸需转换）
    float dpi_scale = 1.0f;
    // 鼠标坐标 -> 逻辑坐标的换算比例（物理客户区 -> 逻辑渲染空间）
    float render_scale = 1.0f;
    // 弹窗逻辑尺寸（渲染/命中测试坐标空间）
    int logical_w = 0;
    int logical_h = 0;

    // ---- 交互状态（由 Win32 窗口过程写入，主线程每帧读取）----
    SDL_Point mouse_pos = {-1, -1};
    bool mouse_inside = false;
    bool click_pending = false;
    SDL_Point click_pos = {0, 0};
    SDL_Point down_pos = {0, 0};
    bool down_seen = false;
    bool was_down = false;
    bool saw_press = false;
    bool outside_click_pending = false;
    int wheel_accum = 0;

    RenderCallback render_callback;

    // 用于在窗口过程中根据 HWND 找到所属实例
    static std::map<HWND, PopupWindow*>& WindowMap(){
        static std::map<HWND, PopupWindow*> map;
        return map;
    }

    static LRESULT CALLBACK PopupWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        auto it = WindowMap().find(hWnd);
        PopupWindow* self = (it != WindowMap().end()) ? it->second : nullptr;

        switch(msg){
            case WM_NCHITTEST:
                // 非穿透：下拉框自身接收鼠标输入（点击/悬停/滚轮），避免事件透传到下方窗口
                return HTCLIENT;
            case WM_MOUSEWHEEL:
                if(self) self->wheel_accum += (int)(short)HIWORD(wParam);
                break;
        }
        if(self && self->original_wndproc){
            return CallWindowProcW(self->original_wndproc, hWnd, msg, wParam, lParam);
        }
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    void ReleaseResources(){
        if(renderer){
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if(sdl_window){
            SDL_DestroyWindow(sdl_window);
            sdl_window = nullptr;
        }
        if(hwnd){
            WindowMap().erase(hwnd);
            hwnd = nullptr;
        }
        original_wndproc = nullptr;
    }

    // 刷新"物理客户区 -> 逻辑渲染空间"的换算比例。
    // 不用 SDL_GetWindowSize/SDL_GetRendererOutputSize（对非高DPI窗口可能都返回逻辑尺寸，
    // 导致比例恒为1、物理坐标未换算，命中测试整体偏移），直接取真实客户区物理尺寸与逻辑尺寸相除。
    void UpdateRenderScale(){
        render_scale = 1.0f;
        if(hwnd){
            RECT cr;
            if(GetClientRect(hwnd, &cr) && cr.right > 0 && cr.bottom > 0){
                float sx = (float)logical_w / (float)cr.right;
                float sy = (float)logical_h / (float)cr.bottom;
                render_scale = std::max(sx, sy);
            }
        }
    }

public:
    PopupWindow() = default;
    PopupWindow(const PopupWindow&) = delete;
    PopupWindow& operator=(const PopupWindow&) = delete;

    ~PopupWindow(){
        // 若 SDL 视频子系统已被 SDL_Quit 关闭，窗口资源已由其释放，避免二次释放
        if(!SDL_WasInit(SDL_INIT_VIDEO)) return;
        Close();
        ReleaseResources();
    }

    void SetRenderCallback(const RenderCallback& cb){
        render_callback = cb;
    }

    /**
     * @brief 计算渲染器对应的 DPI 缩放（SDL 逻辑坐标 -> 物理像素）
     */
    static float GetDpiScaleOf(SDL_Renderer* renderer){
        if(!renderer) return 1.0f;
        SDL_Window* w = SDL_RenderGetWindow(renderer);
        if(!w) return 1.0f;
        int wl = 0, hl = 0, wp = 0, hp = 0;
        SDL_GetWindowSize(w, &wl, &hl);
        SDL_GetRendererOutputSize(renderer, &wp, &hp);
        float s = 1.0f;
        if(wl > 0 && wp > 0) s = (float)wp / (float)wl;
        if(hl > 0 && hp > 0) s = std::max(s, (float)hp / (float)hl);
        return s;
    }

    /**
     * @brief 创建（首次调用）并显示浮动弹出窗口（顶层工具窗口，可超出父窗口）
     * 
     * @param parent_renderer 父渲染器，通过 SDL_RenderGetWindow 获得父窗口
     * @param screen_x 弹出窗口左上角的屏幕物理像素 X 坐标
     * @param screen_y 弹出窗口左上角的屏幕物理像素 Y 坐标
     * @param w 窗口逻辑宽度
     * @param h 窗口逻辑高度
     * @return 是否成功
     */
    bool Open(SDL_Renderer* parent_renderer, int screen_x, int screen_y, int w, int h){
        if(!parent_renderer) return false;
        SDL_Window* parent = SDL_RenderGetWindow(parent_renderer);
        if(!parent) return false;

        // 计算 DPI 缩放：SDL 逻辑坐标 -> 物理像素
        dpi_scale = GetDpiScaleOf(parent_renderer);

        if(!sdl_window){
            // 注意：不用 SDL_WINDOW_TOOLTIP（在部分环境会造成点击穿透/收不到鼠标），用 WS_EX_TOOLWINDOW 样式代替
            sdl_window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
                SDL_WINDOW_BORDERLESS | SDL_WINDOW_SKIP_TASKBAR | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_HIDDEN);
            if(!sdl_window) return false;

            hwnd = SDL_GetWindowHWND(sdl_window);
            if(!hwnd){
                SDL_DestroyWindow(sdl_window);
                sdl_window = nullptr;
                return false;
            }

            // 软件渲染器对弹出窗口的兼容性最好
            renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_SOFTWARE);
            if(!renderer){
                SDL_DestroyWindow(sdl_window);
                sdl_window = nullptr;
                hwnd = nullptr;
                return false;
            }
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

            // 顶层工具窗口：不激活、不占任务栏
            SetWindowLongPtr(hwnd, GWL_EXSTYLE,
                GetWindowLongPtr(hwnd, GWL_EXSTYLE) | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW);

            // 子类化：在消息层面捕获鼠标输入
            WindowMap()[hwnd] = this;
            original_wndproc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)PopupWndProc);
        }

        // 逻辑尺寸（渲染坐标空间为 0..w）
        SDL_SetWindowSize(sdl_window, w, h);
        logical_w = w;
        logical_h = h;
        // 屏幕物理像素定位与定尺寸（顶层浮动弹窗）
        if(hwnd){
            SetWindowPos(hwnd, HWND_TOP,
                screen_x, screen_y,
                (int)std::lround(w * dpi_scale), (int)std::lround(h * dpi_scale),
                SWP_NOACTIVATE);
        }
        // 渲染逻辑坐标空间与父窗口保持一致（物理输出自动按 DPI 缩放）
        if(renderer) SDL_RenderSetLogicalSize(renderer, w, h);

        // 鼠标坐标 -> 逻辑坐标换算比例（物理客户区 -> 逻辑渲染空间）
        UpdateRenderScale();

        SDL_ShowWindow(sdl_window);

        opened = true;
        mouse_inside = false;
        click_pending = false;
        outside_click_pending = false;
        // 鼠标轮询状态机初始化：以当前按键状态为基准，避免把打开时残留的松开误判为点击外部
        was_down = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        saw_press = false;
        down_seen = false;
        wheel_accum = 0;
        return true;
    }

    void Close(){
        if(sdl_window) SDL_HideWindow(sdl_window);
        opened = false;
        click_pending = false;
        outside_click_pending = false;
    }

    bool IsOpen() const{
        return opened;
    }

    // 弹窗窗口句柄（供外部判断焦点是否转移到了本弹窗）
    HWND GetHwnd() const{
        return hwnd;
    }

    // 当前鼠标左键是否处于按下状态（由 PollInput 维护，用于滚动条拖拽等按下沿检测）
    bool IsButtonDown() const{
        return was_down;
    }

    void SetSize(int w, int h){
        if(sdl_window) SDL_SetWindowSize(sdl_window, w, h);
    }

    void SetPosition(int screen_x, int screen_y){
        if(hwnd){
            SetWindowPos(hwnd, NULL, screen_x, screen_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }

    bool IsMouseInside() const{
        return mouse_inside;
    }

    SDL_Point GetMousePos() const{
        if(render_scale > 0.0f && render_scale != 1.0f){
            return {(int)(mouse_pos.x * render_scale), (int)(mouse_pos.y * render_scale)};
        }
        return mouse_pos;
    }

    /**
     * @brief 消费一次点击事件（一次点击只会被消费一次）
     * 
     * @param out_pos 输出点击位置（子窗口客户区坐标）
     * @return 是否有点击发生
     */
    bool ConsumeClick(SDL_Point& out_pos){
        if(click_pending){
            click_pending = false;
            if(render_scale > 0.0f && render_scale != 1.0f){
                out_pos = {(int)(click_pos.x * render_scale), (int)(click_pos.y * render_scale)};
            }
            else{
                out_pos = click_pos;
            }
            return true;
        }
        return false;
    }

    /**
     * @brief 消费一次"点击窗口外部"事件（捕获鼠标期间在窗口外松开时产生）
     */
    bool ConsumeOutsideClick(){
        if(outside_click_pending){
            outside_click_pending = false;
            return true;
        }
        return false;
    }

    /**
     * @brief 消费累计的滚轮增量并清零
     */
    int ConsumeWheel(){
        int v = wheel_accum;
        wheel_accum = 0;
        return v;
    }

    /**
     * @brief 每帧轮询鼠标状态（GetCursorPos + GetAsyncKeyState，不依赖窗口消息路由）
     * 
     * 维护鼠标位置/窗口内标志，并通过按下/松开边沿检测产生"点击"与"点击外部"事件。
     * 打开下拉框时以当前按键状态初始化，避免把打开时残留的松开误判为点击外部。
     */
    void PollInput(){
        if(!hwnd) return;

        POINT p;
        GetCursorPos(&p);
        ScreenToClient(hwnd, &p);
        mouse_pos = {p.x, p.y};
        RECT cr;
        GetClientRect(hwnd, &cr);
        mouse_inside = (p.x >= 0 && p.x < cr.right && p.y >= 0 && p.y < cr.bottom);

        bool down = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

        if(down && !was_down){
            // 检测到按下
            was_down = true;
            saw_press = true;
            down_seen = mouse_inside;
            down_pos = mouse_pos;
            return;
        }

        if(!down && was_down){
            // 检测到松开
            was_down = false;
            if(!saw_press){
                // 打开下拉框那次点击残留的松开 -> 忽略
                return;
            }
            if(mouse_inside){
                click_pos = mouse_pos;
                click_pending = true;
            }
            else{
                outside_click_pending = true;
            }
            return;
        }
    }

    /**
     * @brief 每帧调用：重绘弹出窗口内容
     */
    void Render(){
        if(!opened || !renderer) return;

        // 每帧刷新换算比例（物理客户区 -> 逻辑渲染空间）
        UpdateRenderScale();

        SDL_SetRenderDrawColor(renderer, 46, 46, 46, 255);
        SDL_RenderClear(renderer);
        if(render_callback){
            render_callback(renderer, SDL_Rect{0, 0, logical_w, logical_h}, GetMousePos());
        }
        SDL_RenderPresent(renderer);
    }
};

#endif
