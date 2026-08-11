#ifndef __INC_FGUI_MATH_
#define __INC_FGUI_MATH_

#include "Fgui_includes.hpp"

inline SDL_FPoint quadrilateralCentroid(const SDL_FPoint& p1, const SDL_FPoint& p2, const SDL_FPoint& p3, const SDL_FPoint& p4) {
    float sum_area_terms = (p1.x * p2.y - p2.x * p1.y) +
                            (p2.x * p3.y - p3.x * p2.y) +
                            (p3.x * p4.y - p4.x * p3.y) +
                            (p4.x * p1.y - p1.x * p4.y);

    if (std::abs(sum_area_terms) < 1e-8) {
        return SDL_FPoint{
            (p1.x + p2.x + p3.x + p4.x) / 4,
            (p1.y + p2.y + p3.y + p4.y) / 4
        };
    }

    float sum_x = (p1.x + p2.x) * (p1.x * p2.y - p2.x * p1.y) +
                   (p2.x + p3.x) * (p2.x * p3.y - p3.x * p2.y) +
                   (p3.x + p4.x) * (p3.x * p4.y - p4.x * p3.y) +
                   (p4.x + p1.x) * (p4.x * p1.y - p1.x * p4.y);

    float sum_y = (p1.y + p2.y) * (p1.x * p2.y - p2.x * p1.y) +
                   (p2.y + p3.y) * (p2.x * p3.y - p3.x * p2.y) +
                   (p3.y + p4.y) * (p3.x * p4.y - p4.x * p3.y) +
                   (p4.y + p1.y) * (p4.x * p1.y - p1.x * p4.y);

    float centroid_x = sum_x / (3 * sum_area_terms);
    float centroid_y = sum_y / (3 * sum_area_terms);

    return SDL_FPoint{
        centroid_x,
        centroid_y
    };
}

void HSVtoRGB(float H, float S, float V, Uint8 &r, Uint8 &g, Uint8 &b) {
    H = fmodf(H, 360.0f);
    if (H < 0) H += 360.0f;

    if (S == 0) {
        r = g = b = static_cast<Uint8>(V * 255);
        return;
    }

    float C = V * S;
    float H_prime = H / 60.0f;
    float X = C * (1 - fabsf(fmodf(H_prime, 2.0f) - 1.0f));
    float m = V - C;

    float R_temp, G_temp, B_temp;
    if (H_prime >= 0 && H_prime < 1) {
        R_temp = C;
        G_temp = X;
        B_temp = 0;
    } else if (H_prime < 2) {
        R_temp = X;
        G_temp = C;
        B_temp = 0;
    } else if (H_prime < 3) {
        R_temp = 0;
        G_temp = C;
        B_temp = X;
    } else if (H_prime < 4) {
        R_temp = 0;
        G_temp = X;
        B_temp = C;
    } else if (H_prime < 5) {
        R_temp = X;
        G_temp = 0;
        B_temp = C;
    } else {
        R_temp = C;
        G_temp = 0;
        B_temp = X;
    }

    R_temp += m;
    G_temp += m;
    B_temp += m;

    R_temp = (R_temp < 0) ? 0 : (R_temp > 1 ? 1 : R_temp);
    G_temp = (G_temp < 0) ? 0 : (G_temp > 1 ? 1 : G_temp);
    B_temp = (B_temp < 0) ? 0 : (B_temp > 1 ? 1 : B_temp);

    r = static_cast<Uint8>(R_temp * 255);
    g = static_cast<Uint8>(G_temp * 255);
    b = static_cast<Uint8>(B_temp * 255);
}

void RGBtoHSV(Uint8 r_u8, Uint8 g_u8, Uint8 b_u8, float &H, float &S, float &V) {
    // 将RGB分量归一化到[0, 1]范围
    float r = static_cast<float>(r_u8) / 255.0f;
    float g = static_cast<float>(g_u8) / 255.0f;
    float b = static_cast<float>(b_u8) / 255.0f;

    // 计算最大值、最小值和差值
    float max_val = std::max(r, std::max(g, b));
    float min_val = std::min(r, std::min(g, b));
    float delta = max_val - min_val;

    // 初始化V为最大值
    V = max_val;

    if (delta == 0.0f) { // 灰度情况
        H = 0.0f;
        S = 0.0f;
    } else { // 彩色情况
        S = delta / max_val;

        // 计算色相H
        if (max_val == r) {
            H = 60.0f * ((g - b) / delta);
        } else if (max_val == g) {
            H = 60.0f * (2.0f + (b - r) / delta);
        } else { // max_val == b
            H = 60.0f * (4.0f + (r - g) / delta);
        }

        // 确保H在[0, 360)范围内
        H = std::fmod(H, 360.0f);
        if (H < 0.0f) {
            H += 360.0f;
        }
    }
}

inline SDL_FPoint lineMidpoint(const SDL_FPoint& p1, const SDL_FPoint& p2){
	return {(p1.x + p2.x) / 2.0f,(p1.y + p2.y) / 2.0f};
}

inline void rotatePoint(int x,int y,int& ox,int& oy,float angle){
	float cosA = cos(angle);
	float sinA = sin(angle);
	ox = x*cosA - y*sinA;
	oy = x*sinA + y*cosA;
}

inline void standardizationRect(const SDL_Rect& rect,SDL_Rect& orect){
	if(rect.w < 0){
		orect.x += rect.w;
		orect.w = -rect.w;
	}
	if(rect.h < 0){
		orect.y += rect.h;
		orect.h = -rect.h;
	}
}

inline void rotatePointAround(int x,int y,int ax,int ay,int& ox,int& oy,float angle){
	int sx = x - ax;
	int sy = y - ay;
	float cosA = cos(angle);
	float sinA = sin(angle);
	ox = sx*cosA - sy*sinA + ax;
	oy = sx*sinA + sy*cosA + ay;
}

inline void colorTween(const SDL_Color& source,const SDL_Color& target,SDL_Color& output,float anip){
	output.r = float(target.r - source.r) * anip + source.r;
	output.g = float(target.g - source.g) * anip + source.g;
	output.b = float(target.b - source.b) * anip + source.b;
	output.a = float(target.a - source.a) * anip + source.r;
}

inline void projectPoint(float x, float y, float z, float& ox, float& oy, float focus_distance) {
    ox = ((focus_distance * x) / z);
    oy = ((focus_distance * y) / z);
}

inline bool isPointInRect(const SDL_Point& p,const SDL_Rect& r)
{
	if (r.x <= p.x && r.x + r.w >= p.x && r.y <= p.y && r.y + r.h >= p.y)
	{
		return true;
	}
	return false;
}

inline bool isPointInsideRect(const SDL_Point& p,const SDL_Rect& r)
{
	if (r.x < p.x && r.x + r.w > p.x && r.y < p.y && r.y + r.h > p.y)
	{
		return true;
	}
	return false;
}

inline bool isRectInRect(const SDL_Rect &rect1, const SDL_Rect &rect2)
{
	SDL_Point reco1 = {rect1.x + rect1.w / 2, rect1.y + rect1.h / 2};
	SDL_Point reco2 = {rect2.x + rect2.w / 2, rect2.y + rect2.h / 2};
	if (abs(reco1.x - reco2.x) <= (rect1.w / 2 + rect2.w / 2) && abs(reco1.y - reco2.y) <= (rect1.h / 2 + rect2.h / 2))
	{
		return true;
	}
	return false;
}

SDL_Rect ShrinkRect(const SDL_Rect& rect, double ratio) {
    int newWidth = static_cast<int>(rect.w * ratio);
    int newHeight = static_cast<int>(rect.h * ratio);
    int dx = (rect.w - newWidth) / 2;
    int dy = (rect.h - newHeight) / 2;
    SDL_Rect newRect;
    newRect.x = rect.x + dx;
    newRect.y = rect.y + dy;
    newRect.w = newWidth;
    newRect.h = newHeight;
    return newRect;
}

SDL_Rect ShrinkRect(const SDL_Rect& rect, double x_ratio, double y_ratio) {
    int newWidth = static_cast<int>(rect.w * x_ratio);
    int newHeight = static_cast<int>(rect.h * y_ratio);
    int dx = (rect.w - newWidth) / 2;
    int dy = (rect.h - newHeight) / 2;
    SDL_Rect newRect;
    newRect.x = rect.x + dx;
    newRect.y = rect.y + dy;
    newRect.w = newWidth;
    newRect.h = newHeight;
    return newRect;
}

SDL_Rect ExtendRect(const SDL_Rect& a, const SDL_Rect& b)
{
    bool a_valid = (a.w > 0 && a.h > 0);
    bool b_valid = (b.w > 0 && b.h > 0);

    if (!a_valid && !b_valid) {
        return {0, 0, 0, 0};
    }
    if (!a_valid) {
        return b;
    }
    if (!b_valid) {
        return a;
    }

    int a_x1 = a.x;
    int a_x2 = a.x + a.w;
    int b_x1 = b.x;
    int b_x2 = b.x + b.w;

    int x_min = std::min(a_x1, b_x1);
    int x_max = std::max(a_x2, b_x2);

    int a_y1 = a.y;
    int a_y2 = a.y + a.h;
    int b_y1 = b.y;
    int b_y2 = b.y + b.h;

    int y_min = std::min(a_y1, b_y1);
    int y_max = std::max(a_y2, b_y2);

    SDL_Rect result;
    result.x = x_min;
    result.y = y_min;
    result.w = x_max - x_min;
    result.h = y_max - y_min;
    return result;
}

SDL_Rect LimitRectInsideRect(const SDL_Rect& src, const SDL_Rect& bounds)
{
    SDL_Rect temp = src;
    int boundLeft = bounds.x;
    int boundRight = bounds.x + bounds.w;
    int tempRight = temp.x + temp.w;
    if (temp.x < boundLeft)
    {
        temp.x = boundLeft;
    }
    else if (tempRight > boundRight)
    {
        temp.x = boundRight - temp.w;
    }
    int boundTop = bounds.y;
    int boundBottom = bounds.y + bounds.h;
    int tempBottom = temp.y + temp.h;
    if (temp.y < boundTop)
    {
        temp.y = boundTop;
    }
    else if (tempBottom > boundBottom)
    {
        temp.y = boundBottom - temp.h;
    }
    return temp;
}

SDL_Rect ClipRect(const SDL_Rect& target, const SDL_Rect& clip) {
    int clip_x = target.x + clip.x;
    int clip_y = target.y + clip.y;
    int clip_w = clip.w;
    int clip_h = clip.h;

    int left   = std::max(target.x, clip_x);
    int right  = std::min(target.x + target.w, clip_x + clip_w);
    int top    = std::max(target.y, clip_y);
    int bottom = std::min(target.y + target.h, clip_y + clip_h);

    if (left < right && top < bottom) {
        return { left, top, right - left, bottom - top };
    } else {
        return { 0, 0, 0, 0 };
    }
}

SDL_Rect RectIntersection(const SDL_Rect& a, const SDL_Rect& b) {
    SDL_Rect intersection = {0, 0, 0, 0};

    int inter_x = std::max(a.x, b.x);
    int inter_y = std::max(a.y, b.y);

    int inter_right = std::min(a.x + a.w, b.x + b.w);
    int inter_bottom = std::min(a.y + a.h, b.y + b.h);

    int inter_w = inter_right - inter_x;
    int inter_h = inter_bottom - inter_y;

    if (inter_w > 0 && inter_h > 0) {
        intersection.x = inter_x;
        intersection.y = inter_y;
        intersection.w = inter_w;
        intersection.h = inter_h;
    }
    return intersection;
}

SDL_Point RelativizePoint(const SDL_Point& point, const SDL_Rect& relative_rect) {
    return {point.x - relative_rect.x,point.y - relative_rect.y};
}

class EMAColor{
    EMASmoother ema_r;
    EMASmoother ema_g;
    EMASmoother ema_b;
    EMASmoother ema_a;
    SDL_Color last_color;
    public:

    EMAColor(){
        this->setAlpha(0.5);
    }

    void resetLastValue(){
		ema_r.resetLastValue();
        ema_g.resetLastValue();
        ema_b.resetLastValue();
        ema_a.resetLastValue();
	}

	void setLastValue(SDL_Color new_color){
		ema_r.setLastValue(new_color.r);
        ema_g.setLastValue(new_color.g);
        ema_b.setLastValue(new_color.b);
        ema_a.setLastValue(new_color.a);
	}

	SDL_Color getLastValue(){
		//return {(uint8_t)ema_r.getLastValue(),(uint8_t)ema_g.getLastValue(),(uint8_t)ema_b.getLastValue(),(uint8_t)ema_a.getLastValue()};
        return last_color;
	}

	void setAlpha(float _alpha = 0.2){
		ema_r.setAlpha(_alpha);
        ema_g.setAlpha(_alpha);
        ema_b.setAlpha(_alpha);
        ema_a.setAlpha(_alpha);
	}

    SDL_Color process(const SDL_Color& input,bool& value_changed)
    {
        SDL_Color new_color = {(uint8_t)ema_r.process(input.r),(uint8_t)ema_g.process(input.g),(uint8_t)ema_b.process(input.b),(uint8_t)ema_a.process(input.a)};
        if(*(uint32_t*)&new_color != *(uint32_t*)&last_color){
            value_changed = true;
        }
        else{
            value_changed = false;
        }
        last_color = new_color;
        return new_color;
    }
};

SDL_Color ColorInterval(const SDL_Color& a,const SDL_Color& b,float pc){
    SDL_Color ret;
    ret.r = (b.r - a.r) * pc + a.r;
    ret.g = (b.g - a.g) * pc + a.g;
    ret.b = (b.b - a.b) * pc + a.b;
    ret.a = (b.a - a.a) * pc + a.a;
    return ret;
}

#endif
