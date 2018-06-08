#pragma once

#include "blend_mode.hpp"
#include "color.hpp"
#include "font.hpp"
#include "image.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "window.hpp"
#include <string>

namespace elona
{
namespace snail
{
namespace hsp
{

template <typename T>
constexpr T rad2deg(T rad)
{
    return rad * 180.0 / 3.14159265358979323846264;
}

template <typename T>
inline T clamp(const T& x, const T& min, const T& max)
{
    return std::min(std::max(x, min), max);
}

void invert_buffer_region(int, int, int, int);
int timeGetTime();
void mes(const std::string& text);
void mesbox(std::string& buffer);
void picload(basic_image& img, int mode);
void pos(int x, int y);
void redraw();
void set_color_mod(int r, int g, int b, int window_id);
void onkey_0();
void await(int msec);
void boxf(int x1, int y1, int x2, int y2, const color& color);
void boxf(const color& color);
void buffer(int window_id, int width, int height);
void color(int r, int g, int b);
void font(int size, font_t::style_t style, const std::string& filename);
void gcopy(int window_id, int src_x, int src_y, int src_width, int src_height);
int ginfo(int type);
void gmode(int mode, int width, int height, int alpha);
void grotate2(
    int window_id,
    int src_x,
    int src_y,
    double angle,
    int dst_width,
    int dst_height);
void grotate(
    int window_id,
    int src_x,
    int src_y,
    double angle,
    int dst_width,
    int dst_height);
void gsel(int window_id);
void gzoom(
    int window_id,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dst_width,
    int dst_height,
    bool blend);
void line(int x, int y);
void line(int x1, int y1, int x2, int y2);
void title(const std::string& title_str);

} // namespace hsp
} // namespace snail
} // namespace elona
