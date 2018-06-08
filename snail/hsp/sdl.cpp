#include "../application.hpp"
#include "../input.hpp"
#include "../font.hpp"
#include "../color.hpp"
#include "../detail/sdl.hpp"
#include <unordered_map>
#include <iostream>
#include <cassert>


namespace {

struct font_cache_key
{
    font_cache_key(int size, elona::snail::font_t::style_t style)
        : size(size)
        , style(style)
    {
    }


    int size;
    elona::snail::font_t::style_t style;


    bool operator==(const font_cache_key& other) const
    {
        return size == other.size && style == other.style;
    }
};

} // namespace



namespace std
{

template <>
struct hash<font_cache_key>
{
    size_t operator()(const font_cache_key& key) const
    {
        return hash<int>()(key.size * 100 + int(key.style));
    }
};

} // namespace std

namespace strutil
{

inline size_t byte_count(uint8_t c)
{
    if (c <= 0x7F)
        return 1;
    else if (c >= 0xc2 && c <= 0xdf)
        return 2;
    else if (c >= 0xe0 && c <= 0xef)
        return 3;
    else if (c >= 0xf0 && c <= 0xf7)
        return 4;
    else if (c >= 0xf8 && c <= 0xfb)
        return 5;
    else if (c >= 0xfc && c <= 0xfd)
        return 6;
    else
        return 1;
}

inline std::string
replace(const std::string& str, const std::string& from, const std::string& to)
{
    auto ret{str};
    std::string::size_type pos{};
    while ((pos = ret.find(from, pos)) != std::string::npos)
    {
        ret.replace(pos, from.size(), to);
        pos += to.size();
    }

    return ret;
}

} // namespace strutil

namespace elona
{
namespace snail
{
namespace hsp
{

namespace detail
{
struct TexBuffer
{
    ::SDL_Texture* texture = nullptr;
    int tex_width = 32;
    int tex_height = 32;
    snail::color color{0, 0, 0, 255};
    int x = 0;
    int y = 0;
    int width = 32;
    int height = 32;
    int mode = 2;
};


int current_buffer;
std::vector<TexBuffer> tex_buffers;
::SDL_Texture* tmp_buffer;

TexBuffer& current_tex_buffer()
{
    return tex_buffers[current_buffer];
}

void set_blend_mode()
{
    switch (current_tex_buffer().mode)
    {
    case 0:
    case 1:
        application::instance().get_renderer().set_blend_mode(
            blend_mode_t::none);
        break;
    case 2:
    case 3:
        application::instance().get_renderer().set_blend_mode(
            blend_mode_t::blend);
        break;
    case 4:
        application::instance().get_renderer().set_blend_mode(
            blend_mode_t::blend);
        break;
    case 5:
        application::instance().get_renderer().set_blend_mode(
            blend_mode_t::add);
        break;
    case 6:
        application::instance().get_renderer().set_blend_mode(
            blend_mode_t::blend);
        break;
    default: break;
    }
}


} // namespace detail

namespace mesbox_detail
{


struct MessageBox
{
    MessageBox(std::string& buffer)
        : buffer(buffer)
    {
    }


    void update()
    {
        auto& input = input::instance();
        buffer += input.get_text();
        if (!input.is_ime_active())
        {
            if (input.is_pressed(key::enter) || input.is_pressed(key::keypad_enter))
            {
                // New line.
                buffer += '\n';
            }
            else if (input.is_pressed(key::backspace) && !buffer.empty())
            {
                // Delete the last character.
                size_t last_byte_count{};
                for (size_t i = 0; i < buffer.size();)
                {
                    const auto byte = strutil::byte_count(buffer[i]);
                    last_byte_count = byte;
                    i += byte;
                }
                buffer.erase(buffer.size() - last_byte_count, last_byte_count);
            }
            else if (
                input.is_pressed(key::key_v)
                && input.is_pressed(key::ctrl))
            {
                // Paste.
                std::unique_ptr<char, decltype(&::SDL_free)> text_ptr{
                    ::SDL_GetClipboardText(), ::SDL_free};

                buffer += strutil::replace(text_ptr.get(), u8"\r\n", u8"\n");
            }
        }
    }


private:
    std::string& buffer;
};

std::vector<std::unique_ptr<MessageBox>> message_boxes;

} // namespace mesbox_detail

namespace font_detail
{
std::unordered_map<font_cache_key, font_t> font_cache;
}

void invert_buffer_region(int x, int y, int width, int height)
{
  int sx = x + width;
  int sy = y + height;
  int tw = detail::current_tex_buffer().tex_width;
  int th = detail::current_tex_buffer().tex_height;

  if (x < 0 || y < 0
      || sx > tw
      || sy > th)
  {
    return;
  }

  auto temp_texture = snail::detail::enforce_sdl(::SDL_CreateTexture(
      application::instance().get_renderer().ptr(),
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_TARGET,
      width,
      height));

  const auto save =
      application::instance().get_renderer().render_target();
  application::instance().get_renderer().set_render_target(temp_texture);

  // source is current tex buffer.
  // destination is temporary texture.
  application::instance().get_renderer().render_image(detail::current_tex_buffer().texture,
                                                      x, y, width, height,
                                                      0, 0, width, height);

  Uint32* raw_pixels = new Uint32[width * height];
  int pitch = width * 4;

  ::SDL_Rect dst = {0, 0, width, height};

  elona::snail::detail::enforce_sdl(::SDL_RenderReadPixels(
      application::instance().get_renderer().ptr(),
      &dst, SDL_PIXELFORMAT_ARGB8888, raw_pixels, pitch));

  ::SDL_DestroyTexture(temp_texture);

  auto inv_texture = snail::detail::enforce_sdl(::SDL_CreateTexture(
      application::instance().get_renderer().ptr(),
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      width,
      height));

  void* raw;
  elona::snail::detail::enforce_sdl(::SDL_LockTexture(inv_texture, nullptr, &raw, &pitch));

  Uint32 *pixels = static_cast<Uint32*>(raw);

  std::cout << detail::current_tex_buffer().tex_width << std::endl;
  for (int ix = x; ix < width; ++ix)
  {
    for (int iy = y; iy < height; ++iy)
    {
      size_t idx = ix * height + iy;
      unsigned char a, r, g, b;
      assert(idx < width * height);

      a = pixels[idx] >> 24 & 0xFF;
      r = pixels[idx] >> 16 & 0xFF;
      g = pixels[idx] >>  8 & 0xFF;
      b = pixels[idx]       & 0xFF;

      // r = 255 - r;
      // g = 255 - g;
      // b = 255 - b;

      pixels[idx] = a << 24 | r << 16 | g << 8 | b;
    }
  }

  application::instance().get_renderer().set_render_target(save);
  application::instance().get_renderer().render_image(inv_texture,
                                                      0, 0, width, height,
                                                      x, y, width, height);
  ::SDL_UnlockTexture(inv_texture);
  ::SDL_DestroyTexture(inv_texture);
}

int timeGetTime()
{
    return ::SDL_GetTicks();
}

void mes(const std::string& text)
{
    constexpr size_t tab_width = 4;

    auto copy = text;
    for (auto i = copy.find('\t'); i != std::string::npos; i = copy.find('\t'))
    {
        copy.replace(i, 1, tab_width, ' ');
    }

    if (copy.size() >= 25 /* TODO */)
    {
        application::instance().get_renderer().render_multiline_text(
            copy,
            detail::current_tex_buffer().x,
            detail::current_tex_buffer().y,
            detail::current_tex_buffer().color);
    }
    else
    {
        application::instance().get_renderer().render_text(
            copy,
            detail::current_tex_buffer().x,
            detail::current_tex_buffer().y,
            detail::current_tex_buffer().color);
    }
}

void mesbox(std::string& buffer)
{
    mesbox_detail::message_boxes.emplace_back(
        std::make_unique<mesbox_detail::MessageBox>(buffer));
}

void picload(basic_image& img, int mode)
{
    if (mode == 0)
    {
        buffer(detail::current_buffer, img.width(), img.height());
    }
    const auto save =
        application::instance().get_renderer().blend_mode();
    application::instance().get_renderer().set_blend_mode(
        blend_mode_t::none);
    application::instance().get_renderer().render_image(
        img, detail::current_tex_buffer().x, detail::current_tex_buffer().y);

#if 0 // disable it temporarily
    if (filesystem::to_utf8_path(filepath).find(u8"interface.bmp") != std::string::npos)
    {
        basic_image ex{filepath.parent_path() / u8"interface_ex.png"};
        application::instance().get_renderer().render_image(ex, 0, 656);
        basic_image ex2{filepath.parent_path() / u8"interface_ex2.png"};
        application::instance().get_renderer().render_image(
            ex2, 144, 656);
        application::instance().get_renderer().render_image(
            ex2, 144, 704);
        basic_image ex3{filepath.parent_path() / u8"interface_ex3.png"};
        application::instance().get_renderer().render_image(
            ex3, 144, 752);
    }
#endif

    application::instance().get_renderer().set_blend_mode(save);
}

void pos(int x, int y)
{
    detail::current_tex_buffer().x = x;
    detail::current_tex_buffer().y = y;
}

void redraw()
{
    auto& renderer = application::instance().get_renderer();
    const auto save = renderer.render_target();
    renderer.set_render_target(nullptr);
    renderer.set_draw_color(snail::color{0, 0, 0, 255});
    renderer.clear();
    renderer.render_image(detail::tex_buffers[0].texture, 0, 0);
    renderer.present();
    renderer.set_render_target(save);
}

void set_color_mod(int r, int g, int b, int window_id)
{
    window_id = window_id == -1 ? detail::current_buffer : window_id;

    r = clamp(r, 0, 255);
    g = clamp(g, 0, 255);
    b = clamp(b, 0, 255);
    snail::detail::enforce_sdl(::SDL_SetTextureColorMod(
        detail::tex_buffers[window_id].texture,
        uint8_t(r),
        uint8_t(g),
        uint8_t(b)));
}

void onkey_0()
{
    mesbox_detail::message_boxes.erase(
        std::end(mesbox_detail::message_boxes) - 1);
}

namespace await_detail
{
uint32_t last_await;
}

void await(int msec)
{
    application::instance().proc_event();
    if (mesbox_detail::message_boxes.back())
        mesbox_detail::message_boxes.back()->update();

    const auto now = ::SDL_GetTicks();
    if (await_detail::last_await == 0)
    {
        await_detail::last_await = now;
    }
    const auto delta = now - await_detail::last_await;
    if (size_t(msec) > delta)
    {
        ::SDL_Delay(msec - delta);
    }
    await_detail::last_await = now;
}

void boxf(int x1, int y1, int x2, int y2, const snail::color& color)
{
    const auto save_alpha = detail::current_tex_buffer().color.a;
    detail::current_tex_buffer().color = color;
    application::instance().get_renderer().set_draw_color(color);
    if (color == snail::color{0, 0, 0, 0})
    {
        application::instance().get_renderer().set_blend_mode(
            blend_mode_t::none);
    }
    else
    {
        application::instance().get_renderer().set_blend_mode(
            blend_mode_t::blend);
    }
    application::instance().get_renderer().fill_rect(
        x1, y1, x2 - x1, y2 - y1);
    detail::current_tex_buffer().color.a = save_alpha;
}

void boxf(const snail::color& color)
{
    boxf(
        0,
        0,
        detail::current_tex_buffer().tex_width,
        detail::current_tex_buffer().tex_height,
        color);
}

void buffer(int window_id, int width, int height)
{
    // Cannot create zero-width or zero-height texture.
    if (width == 0)
        width = 1;
    if (height == 0)
        height = 1;
    if (size_t(window_id) >= detail::tex_buffers.size())
    {
        detail::tex_buffers.resize(window_id + 1);
    }
    if (auto texture = detail::tex_buffers[window_id].texture)
    {
        int img_width;
        int img_height;
        snail::detail::enforce_sdl(
            ::SDL_QueryTexture(texture, NULL, NULL, &img_width, &img_height));
        if (width == img_width && height == img_height)
        {
            gsel(window_id);
            application::instance().get_renderer().clear();
            return;
        }
        else
        {
            ::SDL_DestroyTexture(detail::tex_buffers[window_id].texture);
        }
    }
    detail::tex_buffers[window_id] = {
        snail::detail::enforce_sdl(::SDL_CreateTexture(
            application::instance().get_renderer().ptr(),
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET,
            width,
            height)),
        width,
        height,
    };

    application::instance().register_finalizer(
        [ptr = detail::tex_buffers[window_id].texture]() {
            ::SDL_DestroyTexture(ptr);
        });

    const auto save =
        application::instance().get_renderer().render_target();
    application::instance().get_renderer().set_render_target(
        detail::tex_buffers[window_id].texture);
    application::instance().get_renderer().set_draw_color({0, 0, 0, 0});
    application::instance().get_renderer().clear();
    application::instance().get_renderer().set_render_target(save);

    gsel(window_id);
}

void color(int r, int g, int b)
{
    detail::current_tex_buffer().color = {
        static_cast<uint8_t>(clamp(r, 0, 255)),
        static_cast<uint8_t>(clamp(g, 0, 255)),
        static_cast<uint8_t>(clamp(b, 0, 255)),
        detail::current_tex_buffer().color.a,
    };
    application::instance().get_renderer().set_draw_color(
        detail::current_tex_buffer().color);
}

void font(int size, font_t::style_t style, const std::string& filename)
{
    auto& renderer = application::instance().get_renderer();
    if (renderer.font().size() == size && renderer.font().style() == style)
        return;

    const auto itr = font_detail::font_cache.find({size, style});
    if (itr != std::end(font_detail::font_cache))
    {
        renderer.set_font(itr->second);
    }
    else
    {
        const auto inserted = font_detail::font_cache.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(size, style),
            std::forward_as_tuple(
                filename,
                size,
                style));
        renderer.set_font(inserted.first->second);
    }
}

void gcopy(int window_id, int src_x, int src_y, int src_width, int src_height)
{
    detail::set_blend_mode();
    snail::detail::enforce_sdl(::SDL_SetTextureAlphaMod(
        detail::tex_buffers[window_id].texture,
        detail::current_tex_buffer().color.a));

    if (window_id == detail::current_buffer)
    {
        src_width =
            src_width == 0 ? detail::current_tex_buffer().width : src_width,
        src_height =
            src_height == 0 ? detail::current_tex_buffer().height : src_height,
        application::instance().get_renderer().set_render_target(
            detail::tmp_buffer);
        if (window_id >= 10)
        {
            const auto save =
                application::instance().get_renderer().blend_mode();
            application::instance().get_renderer().set_blend_mode(
                blend_mode_t::none);
            application::instance().get_renderer().set_draw_color(
                {0, 0, 0, 0});
            application::instance().get_renderer().set_blend_mode(save);
        }
        application::instance().get_renderer().clear();
        application::instance().get_renderer().render_image(
            detail::tex_buffers[window_id].texture,
            src_x,
            src_y,
            src_width,
            src_height,
            0,
            0);
        gsel(window_id);
        application::instance().get_renderer().render_image(
            detail::tmp_buffer,
            0,
            0,
            src_width,
            src_height,
            detail::current_tex_buffer().x,
            detail::current_tex_buffer().y);
        return;
    }

    int dst_x = detail::current_tex_buffer().x;
    int dst_y = detail::current_tex_buffer().y;
    src_width = src_width == 0 ? detail::current_tex_buffer().width : src_width;
    src_height =
        src_height == 0 ? detail::current_tex_buffer().height : src_height;
    int dst_width = src_width;
    int dst_height = src_height;

    if (src_x < 0)
    {
        int excess = -src_x;
        dst_x += excess;
        dst_width -= excess;
    }
    if (src_y < 0)
    {
        int excess = -src_y;
        dst_y += excess;
        dst_height -= excess;
    }
    if (src_x + src_width >= detail::tex_buffers[window_id].tex_width)
    {
        int excess =
            src_x + src_width - detail::tex_buffers[window_id].tex_width;
        dst_width -= excess;
    }
    if (src_y + src_height >= detail::tex_buffers[window_id].tex_height)
    {
        int excess =
            src_y + src_height - detail::tex_buffers[window_id].tex_height;
        dst_height -= excess;
    }

    application::instance().get_renderer().render_image(
        detail::tex_buffers[window_id].texture,
        src_x,
        src_y,
        src_width,
        src_height,
        dst_x,
        dst_y,
        dst_width,
        dst_height);
}

int ginfo(int type)
{
    switch (type)
    {
    case 0: return 0; // mouse x
    case 1: return 0; // mouse y
    case 2: return 0; // active window id
    case 3: return detail::current_buffer; // target window id
    case 4: return 0; // window x1
    case 5: return 0; // window y1
    case 6: return 800; // window x2
    case 7: return 600; // window y2
    case 8: return 0; // window scroll x
    case 9: return 0; // window scroll y
    case 10: return 800; // window width
    case 11: return 600; // window height
    case 12:
        return detail::current_tex_buffer().tex_width; // window client width
    case 13:
        return detail::current_tex_buffer().tex_height; // window client height
    case 14: return 0; // font width
    case 15: return 0; // font height
    case 16: return detail::current_tex_buffer().color.r; // current color r
    case 17: return detail::current_tex_buffer().color.g; // current color g
    case 18: return detail::current_tex_buffer().color.b; // current color b
    case 19: return 0; // color mode
    case 20: return 0; // resolution x
    case 21: return 0; // resolution y
    case 22: return detail::current_tex_buffer().x; // current position x
    case 23: return detail::current_tex_buffer().y; // current position y
    default: throw new std::logic_error("Bad ginfo type");
    }
}

void gmode(int mode, int width, int height, int alpha)
{
    detail::current_tex_buffer().mode = mode;
    detail::set_blend_mode();

    detail::current_tex_buffer().width = width;
    detail::current_tex_buffer().height = height;
    detail::current_tex_buffer().color.a = clamp(alpha, 0, 255);
}

void grotate2(
    int window_id,
    int src_x,
    int src_y,
    double angle,
    int dst_width,
    int dst_height)
{
    detail::set_blend_mode();
    snail::detail::enforce_sdl(::SDL_SetTextureAlphaMod(
        detail::tex_buffers[window_id].texture,
        detail::current_tex_buffer().color.a));

    if (window_id == detail::current_buffer)
    {
        assert(0);
    }

    ::SDL_Rect src_rect{
        src_x,
        src_y,
        detail::current_tex_buffer().width == -1
            ? dst_width
            : detail::current_tex_buffer().width,
        detail::current_tex_buffer().height == -1
            ? dst_height
            : detail::current_tex_buffer().height,
    };
    ::SDL_Rect dst_rect{
        detail::current_tex_buffer().x - dst_width / 2,
        detail::current_tex_buffer().y - dst_height / 2,
        dst_width,
        dst_height,
    };

    switch (application::instance().get_renderer().blend_mode())
    {
    case blend_mode_t::none:
        snail::detail::enforce_sdl(::SDL_SetTextureBlendMode(
            detail::tex_buffers[window_id].texture, ::SDL_BLENDMODE_NONE));
        break;
    case blend_mode_t::blend:
        snail::detail::enforce_sdl(::SDL_SetTextureBlendMode(
            detail::tex_buffers[window_id].texture, ::SDL_BLENDMODE_BLEND));
        break;
    case blend_mode_t::add:
        snail::detail::enforce_sdl(::SDL_SetTextureBlendMode(
            detail::tex_buffers[window_id].texture, ::SDL_BLENDMODE_ADD));
        break;
    }
    snail::detail::enforce_sdl(::SDL_RenderCopyEx(
        application::instance().get_renderer().ptr(),
        detail::tex_buffers[window_id].texture,
        &src_rect,
        &dst_rect,
        rad2deg(angle),
        nullptr,
        ::SDL_FLIP_NONE));
}

void grotate(
    int window_id,
    int src_x,
    int src_y,
    double angle,
    int dst_width,
    int dst_height)
{
    if (angle != 0)
    {
        grotate2(window_id, src_x, src_y, angle, dst_width, dst_height);
        return;
    }

    detail::set_blend_mode();
    snail::detail::enforce_sdl(::SDL_SetTextureAlphaMod(
        detail::tex_buffers[window_id].texture,
        detail::current_tex_buffer().color.a));

    if (window_id == detail::current_buffer)
    {
        application::instance().get_renderer().set_render_target(
            detail::tmp_buffer);
        if (window_id < 10)
        {
            application::instance().get_renderer().set_blend_mode(
                blend_mode_t::none);
            application::instance().get_renderer().set_draw_color(
                {0, 0, 0, 0});
        }
        else
        {
            const auto save =
                application::instance().get_renderer().blend_mode();
            application::instance().get_renderer().set_blend_mode(
                blend_mode_t::none);
            application::instance().get_renderer().set_draw_color(
                {0, 0, 0, 0});
            application::instance().get_renderer().set_blend_mode(save);
        }
        application::instance().get_renderer().clear();
        application::instance().get_renderer().render_image(
            detail::tex_buffers[window_id].texture,
            src_x,
            src_y,
            detail::current_tex_buffer().width == -1
                ? dst_width
                : detail::current_tex_buffer().width,
            detail::current_tex_buffer().height == -1
                ? dst_height
                : detail::current_tex_buffer().height,
            0,
            0,
            dst_width,
            dst_height);

        gsel(window_id);
        application::instance().get_renderer().render_image(
            detail::tmp_buffer,
            0,
            0,
            dst_width,
            dst_height,
            detail::current_tex_buffer().x - dst_width / 2,
            detail::current_tex_buffer().y - dst_height / 2);
        return;
    }

    application::instance().get_renderer().render_image(
        detail::tex_buffers[window_id].texture,
        src_x,
        src_y,
        detail::current_tex_buffer().width == -1
            ? dst_width
            : detail::current_tex_buffer().width,
        detail::current_tex_buffer().height == -1
            ? dst_height
            : detail::current_tex_buffer().height,
        detail::current_tex_buffer().x - dst_width / 2,
        detail::current_tex_buffer().y - dst_height / 2,
        dst_width,
        dst_height);
}

void gsel(int window_id)
{
    detail::current_buffer = window_id;
    application::instance().get_renderer().set_render_target(
        detail::current_tex_buffer().texture);
}

void gzoom(
    int window_id,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dst_width,
    int dst_height,
    bool blend)
{
    if (blend)
    {
        detail::set_blend_mode();
        snail::detail::enforce_sdl(::SDL_SetTextureAlphaMod(
            detail::tex_buffers[window_id].texture,
            detail::current_tex_buffer().color.a));
    }
    else
    {
        application::instance().get_renderer().set_blend_mode(
            blend_mode_t::none);
        snail::detail::enforce_sdl(::SDL_SetTextureAlphaMod(
            detail::tex_buffers[window_id].texture, 255));
    }

    if (window_id == detail::current_buffer)
    {
        application::instance().get_renderer().set_render_target(
            detail::tmp_buffer);
        if (window_id < 10)
        {
            application::instance().get_renderer().set_blend_mode(
                blend_mode_t::none);
            application::instance().get_renderer().set_draw_color(
                {0, 0, 0, 0});
        }
        else
        {
            const auto save =
                application::instance().get_renderer().blend_mode();
            application::instance().get_renderer().set_blend_mode(
                blend_mode_t::none);
            application::instance().get_renderer().set_draw_color(
                {0, 0, 0, 0});
            application::instance().get_renderer().set_blend_mode(save);
        }
        application::instance().get_renderer().clear();
        application::instance().get_renderer().render_image(
            detail::tex_buffers[window_id].texture,
            src_x,
            src_y,
            src_width,
            src_height,
            0,
            0,
            dst_width,
            dst_height);
        gsel(window_id);
        application::instance().get_renderer().render_image(
            detail::tmp_buffer,
            0,
            0,
            dst_width,
            dst_height,
            detail::current_tex_buffer().x,
            detail::current_tex_buffer().y);
        return;
    }

    application::instance().get_renderer().render_image(
        detail::tex_buffers[window_id].texture,
        src_x,
        src_y,
        src_width,
        src_height,
        detail::current_tex_buffer().x,
        detail::current_tex_buffer().y,
        dst_width,
        dst_height);
}

void line(int x1, int y1, int x2, int y2)
{
    snail::application::instance().get_renderer().render_line(x1, y1, x2, y2);
}

void line(int x, int y)
{
    line(detail::current_tex_buffer().x, detail::current_tex_buffer().y, x, y);
    detail::current_tex_buffer().x = x;
    detail::current_tex_buffer().y = y;
}

void title(const std::string& title_str)
{
    application::instance().initialize(800, 600, title_str);
    detail::tmp_buffer = snail::detail::enforce_sdl(::SDL_CreateTexture(
        application::instance().get_renderer().ptr(),
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET,
        1000,
        1000));
    application::instance().register_finalizer(
        []() { ::SDL_DestroyTexture(detail::tmp_buffer); });
    input::instance().set_key_repeat(10, 3);
    application::instance().register_finalizer(
        [&]() { font_detail::font_cache.clear(); });
    buffer(0, 800, 600);
}

} // namespace hsp
} // namespace snail
} // namespace elona

