#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>


#include "snail/application.hpp"

#include "config.hpp"
#include "defines.hpp"
#include "elona.hpp"
#include "log.hpp"
#include "util.hpp"
#include "variables.hpp"
#if defined(ELONA_OS_WINDOWS)
#include <windows.h> // MessageBoxA
#endif

namespace
{


struct font_cache_key
{
    font_cache_key(int size, snail::font_t::style_t style)
        : size(size)
        , style(style)
    {
    }


    int size;
    snail::font_t::style_t style;


    bool operator==(const font_cache_key& other) const
    {
        return size == other.size && style == other.style;
    }
};


size_t read_binary(std::istream& in, size_t size, char* buffer)
{
    assert(in);

    std::fill_n(buffer, size, 0);
    in.read(buffer, size);
    if (in.eof())
    {
        in.clear();
    }

    assert(in);

    return size;
}



std::pair<std::unique_ptr<char[]>, size_t> read_binary(
    std::istream& in,
    size_t size)
{
    std::unique_ptr<char[]> buf{new char[size]};
    return {std::move(buf), read_binary(in, size, buf.get())};
}



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



namespace elona
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
        snail::application::instance().get_renderer().set_blend_mode(
            snail::blend_mode_t::none);
        break;
    case 2:
    case 3:
        snail::application::instance().get_renderer().set_blend_mode(
            snail::blend_mode_t::blend);
        break;
    case 4:
        snail::application::instance().get_renderer().set_blend_mode(
            snail::blend_mode_t::blend);
        break;
    case 5:
        snail::application::instance().get_renderer().set_blend_mode(
            snail::blend_mode_t::add);
        break;
    case 6:
        snail::application::instance().get_renderer().set_blend_mode(
            snail::blend_mode_t::blend);
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
        auto& input = snail::input::instance();
        buffer += input.get_text();
        if (!input.is_ime_active())
        {
            if (input.is_pressed(snail::key::enter))
            {
                // New line.
                buffer += '\n';
            }
            else if (input.is_pressed(snail::key::backspace) && !buffer.empty())
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
                input.is_pressed(snail::key::key_v)
                && input.is_pressed(snail::key::ctrl))
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



std::string operator+(const std::string& lhs, int rhs)
{
    return lhs + std::to_string(rhs);
}



std::string operator+(
    elona_vector1<std::string>& lhs,
    elona_vector1<std::string>& rhs)
{
    return lhs(0) + rhs(0);
}



std::string operator+(const std::string& lhs, elona_vector1<std::string>& rhs)
{
    return lhs + rhs(0);
}



std::string operator+(elona_vector1<std::string>& lhs, int rhs)
{
    return lhs(0) + std::to_string(rhs);
}



std::string operator+(elona_vector1<std::string>& lhs, const std::string& rhs)
{
    return lhs(0) + rhs;
}



namespace await_detail
{
uint32_t last_await;
}



void await(int msec)
{
    snail::application::instance().proc_event();
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



// CANNOT BE IMPLEMENTED
void axobj(int, const std::string&, int, int)
{
}



void bcopy(const fs::path& from, const fs::path& to)
{
    ELONA_LOG("Copy file: from " << from << " to " << to);
    fs::copy_file(from, to, fs::copy_option::overwrite_if_exists);
}



// fullscreen
void bgscr(int window_id, int width, int height, int, int)
{
    (void)window_id;
    (void)width;
    (void)height;
}



void boxf(int x1, int y1, int x2, int y2, const snail::color& color)
{
    const auto save_alpha = detail::current_tex_buffer().color.a;
    detail::current_tex_buffer().color = color;
    snail::application::instance().get_renderer().set_draw_color(color);
    if (color == snail::color{0, 0, 0, 0})
    {
        snail::application::instance().get_renderer().set_blend_mode(
            snail::blend_mode_t::none);
    }
    else
    {
        snail::application::instance().get_renderer().set_blend_mode(
            snail::blend_mode_t::blend);
    }
    snail::application::instance().get_renderer().fill_rect(
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



void bload(const fs::path& filename, std::string& data, int size, int)
{
    if (size == 0)
    {
        size = data.size();
    }
    std::ifstream in{filename.native(), std::ios::binary};
    if (!in)
    {
        throw std::runtime_error(
            u8"Error: fail to read "
            + filesystem::make_preferred_path_in_utf8(filename));
    }
    auto buf = read_binary(in, size).first;
    data = std::string{buf.get(), static_cast<size_t>(size)};
}



void bload(const fs::path& filename, int& data, int size, int)
{
    std::ifstream in{filename.native(), std::ios::binary};
    if (!in)
    {
        throw std::runtime_error(
            u8"Error: fail to read "
            + filesystem::make_preferred_path_in_utf8(filename));
    }
    auto buf = read_binary(in, size).first;
    data = *reinterpret_cast<int*>(buf.get());
}



void bload(const fs::path& filename, elona_vector1<int>& data, int size, int)
{
    if (size == 0)
    {
        size = data.size() * sizeof(int);
    }
    std::ifstream in{filename.native(), std::ios::binary};
    if (!in)
    {
        throw std::runtime_error(
            u8"Error: fail to read "
            + filesystem::make_preferred_path_in_utf8(filename));
    }
    auto buf = read_binary(in, size).first;
    for (size_t i = 0; i < length(data); ++i)
    {
        data(i) = reinterpret_cast<int*>(buf.get())[i];
        size -= sizeof(int);
        if (size == 0)
            return;
    }
}



void bsave(const fs::path& filename, const std::string& data)
{
    std::ofstream out{filename.native(), std::ios::binary};
    if (!out)
    {
        throw std::runtime_error(
            u8"Error: fail to write "
            + filesystem::make_preferred_path_in_utf8(filename));
    }
    out.write(reinterpret_cast<const char*>(data.c_str()), data.size());
}



void bsave(const fs::path& filename, int data)
{
    std::ofstream out{filename.native(), std::ios::binary};
    out.write(reinterpret_cast<const char*>(&data), sizeof(data));
}



void bsave(const fs::path& filename, elona_vector1<int>& data)
{
    std::ofstream out{filename.native(), std::ios::binary};
    for (size_t i = 0; i < data.size(); ++i)
    {
        out.write(reinterpret_cast<const char*>(&data(i)), sizeof(int));
    }
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
            snail::application::instance().get_renderer().clear();
            return;
        }
        else
        {
            ::SDL_DestroyTexture(detail::tex_buffers[window_id].texture);
        }
    }
    detail::tex_buffers[window_id] = {
        snail::detail::enforce_sdl(::SDL_CreateTexture(
            snail::application::instance().get_renderer().ptr(),
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET,
            width,
            height)),
        width,
        height,
    };

    snail::application::instance().register_finalizer(
        [ptr = detail::tex_buffers[window_id].texture]() {
            ::SDL_DestroyTexture(ptr);
        });

    const auto save =
        snail::application::instance().get_renderer().render_target();
    snail::application::instance().get_renderer().set_render_target(
        detail::tex_buffers[window_id].texture);
    snail::application::instance().get_renderer().set_draw_color({0, 0, 0, 0});
    snail::application::instance().get_renderer().clear();
    snail::application::instance().get_renderer().set_render_target(save);

    gsel(window_id);
}



void chgdisp(int, int width, int height)
{
    (void)width;
    (void)height;
}



void clrobj(int)
{
}



void color(int r, int g, int b)
{
    detail::current_tex_buffer().color = {
        static_cast<uint8_t>(clamp(r, 0, 255)),
        static_cast<uint8_t>(clamp(g, 0, 255)),
        static_cast<uint8_t>(clamp(b, 0, 255)),
        detail::current_tex_buffer().color.a,
    };
    snail::application::instance().get_renderer().set_draw_color(
        detail::current_tex_buffer().color);
}



void delcom(int)
{
}



void elona_delete(const fs::path& filename)
{
    fs::remove_all(filename);
}

#if defined(ELONA_OS_WINDOWS)
std::wstring get_utf16(const std::string& str)
{
    if (str.empty())
        return std::wstring();
    int sz = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), 0, 0);
    std::wstring res(sz, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &res[0], sz);
    return res;
}

int dialog_windows(const std::string& message, int option)
{
    UINT type = MB_ICONINFORMATION;
    int ret = 0;
    if (option == 1 || option == 3)
    {
        type = MB_ICONWARNING;
    }
    std::wstring message_wstr = get_utf16(message);
    switch (option)
    {
    case 0: // Info, OK
    case 1: // Warning, OK
        MessageBoxW(NULL, message_wstr.c_str(), L"Message", MB_OK | type);
        return DIALOG_OK;
    case 2: // Info, Yes/No
    case 3: // Warning, Yes/No
        ret = MessageBoxW(
            NULL, message_wstr.c_str(), L"Message", MB_YESNO | type);
        if (ret == IDYES)
        {
            return DIALOG_YES;
        }
        else
        {
            return DIALOG_NO;
        }
    case 16: // Open file dialog
    case 17: // Save as dialog
    case 32: // Color selection
    case 33: // Color selection with matrix
    default: return 0;
    }
}
#elif defined(ELONA_OS_MACOS)
int dialog_macos(const std::string& message, int option)
{
    std::cout << message << std::endl;
    return 1;
}
#endif

int dialog(const std::string& message, int option)
{
#if defined(ELONA_OS_WINDOWS)
    return dialog_windows(message, option);
#elif defined(ELONA_OS_MACOS)
    return dialog_macos(message, option);
#else
    return 0;
#endif
}



void exec(const std::string&, int)
{
}



namespace font_detail
{
std::unordered_map<font_cache_key, snail::font_t> font_cache;
}



void font(int size, snail::font_t::style_t style)
{
    auto& renderer = snail::application::instance().get_renderer();
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
                filesystem::path(u8"font")
                    / lang(config::instance().font1, config::instance().font2),
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
        snail::application::instance().get_renderer().set_render_target(
            detail::tmp_buffer);
        if (window_id >= 10)
        {
            const auto save =
                snail::application::instance().get_renderer().blend_mode();
            snail::application::instance().get_renderer().set_blend_mode(
                snail::blend_mode_t::none);
            snail::application::instance().get_renderer().set_draw_color(
                {0, 0, 0, 0});
            snail::application::instance().get_renderer().set_blend_mode(save);
        }
        snail::application::instance().get_renderer().clear();
        snail::application::instance().get_renderer().render_image(
            detail::tex_buffers[window_id].texture,
            src_x,
            src_y,
            src_width,
            src_height,
            0,
            0);
        gsel(window_id);
        snail::application::instance().get_renderer().render_image(
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

    snail::application::instance().get_renderer().render_image(
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



bool getkey(snail::key key)
{
    return snail::input::instance().is_pressed(key);
}



void getstr(
    std::string& out,
    const std::string& source,
    int offset,
    char delimiter,
    int limit)
{
    (void)limit;
    auto pos = source.find(delimiter, offset);
    if (pos == std::string::npos)
    {
        pos = source.find('\n', offset);
    }
    if (pos == std::string::npos)
    {
        pos = source.size();
    }
    if (pos >= size_t(offset))
    {
        const auto length = pos - offset;
        strsize = length + 1; // Includes the delimiter.
        out = source.substr(offset, length);
    }
    else
    {
        strsize = 0;
        out = "";
    }
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



template <typename T>
constexpr T rad2deg(T rad)
{
    return rad * 180.0 / 3.14159265358979323846264;
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

    switch (snail::application::instance().get_renderer().blend_mode())
    {
    case snail::blend_mode_t::none:
        snail::detail::enforce_sdl(::SDL_SetTextureBlendMode(
            detail::tex_buffers[window_id].texture, ::SDL_BLENDMODE_NONE));
        break;
    case snail::blend_mode_t::blend:
        snail::detail::enforce_sdl(::SDL_SetTextureBlendMode(
            detail::tex_buffers[window_id].texture, ::SDL_BLENDMODE_BLEND));
        break;
    case snail::blend_mode_t::add:
        snail::detail::enforce_sdl(::SDL_SetTextureBlendMode(
            detail::tex_buffers[window_id].texture, ::SDL_BLENDMODE_ADD));
        break;
    }
    snail::detail::enforce_sdl(::SDL_RenderCopyEx(
        snail::application::instance().get_renderer().ptr(),
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
        snail::application::instance().get_renderer().set_render_target(
            detail::tmp_buffer);
        if (window_id < 10)
        {
            snail::application::instance().get_renderer().set_blend_mode(
                snail::blend_mode_t::none);
            snail::application::instance().get_renderer().set_draw_color(
                {0, 0, 0, 0});
        }
        else
        {
            const auto save =
                snail::application::instance().get_renderer().blend_mode();
            snail::application::instance().get_renderer().set_blend_mode(
                snail::blend_mode_t::none);
            snail::application::instance().get_renderer().set_draw_color(
                {0, 0, 0, 0});
            snail::application::instance().get_renderer().set_blend_mode(save);
        }
        snail::application::instance().get_renderer().clear();
        snail::application::instance().get_renderer().render_image(
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
        snail::application::instance().get_renderer().render_image(
            detail::tmp_buffer,
            0,
            0,
            dst_width,
            dst_height,
            detail::current_tex_buffer().x - dst_width / 2,
            detail::current_tex_buffer().y - dst_height / 2);
        return;
    }

    snail::application::instance().get_renderer().render_image(
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
    snail::application::instance().get_renderer().set_render_target(
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
        snail::application::instance().get_renderer().set_blend_mode(
            snail::blend_mode_t::none);
        snail::detail::enforce_sdl(::SDL_SetTextureAlphaMod(
            detail::tex_buffers[window_id].texture, 255));
    }

    if (window_id == detail::current_buffer)
    {
        snail::application::instance().get_renderer().set_render_target(
            detail::tmp_buffer);
        if (window_id < 10)
        {
            snail::application::instance().get_renderer().set_blend_mode(
                snail::blend_mode_t::none);
            snail::application::instance().get_renderer().set_draw_color(
                {0, 0, 0, 0});
        }
        else
        {
            const auto save =
                snail::application::instance().get_renderer().blend_mode();
            snail::application::instance().get_renderer().set_blend_mode(
                snail::blend_mode_t::none);
            snail::application::instance().get_renderer().set_draw_color(
                {0, 0, 0, 0});
            snail::application::instance().get_renderer().set_blend_mode(save);
        }
        snail::application::instance().get_renderer().clear();
        snail::application::instance().get_renderer().render_image(
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
        snail::application::instance().get_renderer().render_image(
            detail::tmp_buffer,
            0,
            0,
            dst_width,
            dst_height,
            detail::current_tex_buffer().x,
            detail::current_tex_buffer().y);
        return;
    }

    snail::application::instance().get_renderer().render_image(
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



int instr(const std::string& str, size_t pos, const std::string pattern)
{
    if (pattern == "\n")
    {
        return instr(str, pos, "\r");
    }
    const auto ret = str.find(pattern, pos);
    return ret == std::string::npos ? -1 : static_cast<int>(ret - pos);
}



int stoi(const std::string& s)
{
    try
    {
        return std::stoi(s);
    }
    catch (std::invalid_argument&)
    {
        return 0;
    }
    catch (std::out_of_range&)
    {
        return 0;
    }
}



size_t length(const std::string& str)
{
    return str.size();
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



void memcpy(
    elona_vector2<int>& src,
    int src_i,
    int src_j,
    elona_vector2<int>& dst,
    int dst_i,
    int dst_j,
    size_t size)
{
    const auto len = length(src);
    const auto len2 = length2(src);
    auto count = size;
    for (size_t i = 0; i < len2; ++i)
    {
        for (size_t j = 0; j < len; ++j)
        {
            src(src_j + j, src_i + i) = dst(dst_j + j, dst_i + i);
            count -= sizeof(int);
            if (count == 0)
                return;
        }
    }
}



// void memexpand(void* memory, size_t size)
// {
// }

// void memfile(void* buf)
// {
// }


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
        snail::application::instance().get_renderer().render_multiline_text(
            copy,
            detail::current_tex_buffer().x,
            detail::current_tex_buffer().y,
            detail::current_tex_buffer().color);
    }
    else
    {
        snail::application::instance().get_renderer().render_text(
            copy,
            detail::current_tex_buffer().x,
            detail::current_tex_buffer().y,
            detail::current_tex_buffer().color);
    }
}



void mes(int n)
{
    mes(std::to_string(n));
}



void mesbox(std::string& buffer)
{
    mesbox_detail::message_boxes.emplace_back(
        std::make_unique<mesbox_detail::MessageBox>(buffer));
}



void mkdir(const fs::path& path)
{
    fs::create_directory(path);
}



void mmload(const fs::path& filepath, int id, int mode)
{
    (void)filepath;
    (void)id;
    (void)mode;
}



void mmplay(int id)
{
    (void)id;
}



void mmstop()
{
}



namespace notemanip
{
std::string* buffer = nullptr;


std::vector<std::string> split_lines(const std::string& str)
{
    std::vector<std::string> lines;
    std::istringstream ss{str};
    std::string buf;
    while (std::getline(ss, buf))
    {
        lines.push_back(buf);
    }
    return lines;
}



size_t count(const std::string& str)
{
    return split_lines(str).size();
}



struct io_error : public std::runtime_error
{
    io_error(const std::string& message)
        : std::runtime_error(message)
    {
    }
};


} // namespace notemanip


void noteadd(const std::string& text, int index, int overwrite)
{
    assert(overwrite == 0 || overwrite == 1);

    if (!notemanip::buffer)
        return;

    auto lines = notemanip::split_lines(*notemanip::buffer);

    if (index == -1)
    {
        index = lines.size();
    }
    else if (size_t(index) >= lines.size())
    {
        lines.resize(index + 1);
    }

    if (overwrite)
    {
        lines[index] = text;
    }
    else
    {
        auto itr = std::begin(lines);
        std::advance(itr, index);
        lines.insert(itr, text);
    }

    notemanip::buffer->clear();
    std::for_each(
        std::begin(lines), std::end(lines), [](const std::string& line) {
            notemanip::buffer->append(line + '\n');
        });
}



void notedel(size_t index)
{
    if (!notemanip::buffer)
        return;

    const auto lines = notemanip::split_lines(*notemanip::buffer);
    notemanip::buffer->clear();
    for (size_t i = 0; i < lines.size(); ++i)
    {
        if (i != index)
        {
            *notemanip::buffer += lines[i] + '\n';
        }
    }
}



void noteget(std::string& out, size_t index)
{
    if (!notemanip::buffer)
    {
        out = "";
        return;
    }

    const auto lines = notemanip::split_lines(*notemanip::buffer);
    if (index >= lines.size())
    {
        out = "";
    }
    else
    {
        out = lines[index];
    }
}



int noteinfo()
{
    return notemanip::buffer ? notemanip::count(*notemanip::buffer) : 0;
}



int notesel(std::string& buf)
{
    notemanip::buffer = &buf;
    return noteinfo();
}



void noteunsel()
{
    notemanip::buffer = nullptr;
}



void objmode(int, int)
{
}



void objprm(int, const std::string&)
{
}



void objsel(int)
{
}



void pget(int x, int y)
{
    (void)x;
    (void)y;
}



void picload(const fs::path& filepath, int mode)
{
    optional<snail::color> keycolor = snail::color{0, 0, 0};
    if (filesystem::to_utf8_path(filepath).find(u8"pcc") != std::string::npos)
    {
        keycolor = snail::color(43, 133, 133);
    }
    if (filesystem::to_utf8_path(filepath).find(u8"bg") != std::string::npos)
    {
        keycolor = none;
    }
    snail::basic_image img{filepath, keycolor};
    if (mode == 0)
    {
        buffer(detail::current_buffer, img.width(), img.height());
    }
    const auto save =
        snail::application::instance().get_renderer().blend_mode();
    snail::application::instance().get_renderer().set_blend_mode(
        snail::blend_mode_t::none);
    snail::application::instance().get_renderer().render_image(
        img, detail::current_tex_buffer().x, detail::current_tex_buffer().y);

#if 0 // disable it temporarily
    if (filesystem::to_utf8_path(filepath).find(u8"interface.bmp") != std::string::npos)
    {
        snail::basic_image ex{filepath.parent_path() / u8"interface_ex.png"};
        snail::application::instance().get_renderer().render_image(ex, 0, 656);
        snail::basic_image ex2{filepath.parent_path() / u8"interface_ex2.png"};
        snail::application::instance().get_renderer().render_image(
            ex2, 144, 656);
        snail::application::instance().get_renderer().render_image(
            ex2, 144, 704);
        snail::basic_image ex3{filepath.parent_path() / u8"interface_ex3.png"};
        snail::application::instance().get_renderer().render_image(
            ex3, 144, 752);
    }
#endif

    snail::application::instance().get_renderer().set_blend_mode(save);
}



void pos(int x, int y)
{
    detail::current_tex_buffer().x = x;
    detail::current_tex_buffer().y = y;
}


void redraw()
{
    auto& renderer = snail::application::instance().get_renderer();
    const auto save = renderer.render_target();
    renderer.set_render_target(nullptr);
    renderer.set_draw_color(snail::color{0, 0, 0, 255});
    renderer.clear();
    renderer.render_image(detail::tex_buffers[0].texture, 0, 0);
    renderer.present();
    renderer.set_render_target(save);
}



void screen(int window_id, int width, int height, int mode, int x, int y)
{
    (void)window_id;
    (void)width;
    (void)height;
    (void)mode;
    (void)x;
    (void)y;
}



void stick(int& out, int allow_repeat_keys)
{
    auto check_key_pressed = [allow_repeat_keys](
                                 int n, snail::key key, bool is_modifier) {
        if ((1 << n) & allow_repeat_keys)
        {
            if (is_modifier)
                return (1 << n)
                    * snail::input::instance().is_pressed_exactly(key);
            else
                return (1 << n) * snail::input::instance().is_pressed(key);
        }
        else
        {
            return (1 << n)
                * snail::input::instance().was_pressed_just_now(key);
        }
    };

    out = 0;

    out += check_key_pressed(0, snail::key::left, false);
    out += check_key_pressed(1, snail::key::up, false);
    out += check_key_pressed(2, snail::key::right, false);
    out += check_key_pressed(3, snail::key::down, false);
    out += check_key_pressed(4, snail::key::space, false);
    out += check_key_pressed(5, snail::key::enter, false);
    out += check_key_pressed(6, snail::key::ctrl, true);
    out += check_key_pressed(7, snail::key::escape, false);
    // out += check_key_pressed(8,  /* Mouse left */,  false);
    // out += check_key_pressed(9,  /* Mouse right */, false);
    out += check_key_pressed(10, snail::key::tab, false);

    if (allow_repeat_keys == 15)
    {
        if (out & 1 || out & 4)
        {
            out |=
                2 * snail::input::instance().is_pressed_exactly(snail::key::up);
            out |= 8
                * snail::input::instance().is_pressed_exactly(snail::key::down);
        }
        if (out & 2 || out & 8)
        {
            out |= 1
                * snail::input::instance().is_pressed_exactly(snail::key::left);
            out |= 4
                * snail::input::instance().is_pressed_exactly(
                      snail::key::right);
        }
    }
}



size_t strlen_u(const std::string& str)
{
    int ret = 0;
    for (size_t i = 0; i < str.size();)
    {
        const auto byte = strutil::byte_count(str[i]);
        ret += byte == 1 ? 1 : 2;
        i += byte;
    }
    return ret;
}



std::string strmid(const std::string& source, int pos, int length)
{
    const auto src_len = source.size();
    if (pos == -1)
    {
        // n characters from right to left.
        length = std::min(size_t(length), src_len);
        return source.substr(src_len - length, length);
    }
    else if (pos >= int(src_len))
    {
        return "";
    }
    else if (pos + length >= int(src_len))
    {
        return source.substr(pos, std::string::npos);
    }
    else
    {
        return source.substr(pos, length);
    }
}



void title(const std::string& title_str)
{
    snail::application::instance().initialize(800, 600, title_str);
    detail::tmp_buffer = snail::detail::enforce_sdl(::SDL_CreateTexture(
        snail::application::instance().get_renderer().ptr(),
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET,
        1000,
        1000));
    snail::application::instance().register_finalizer(
        []() { ::SDL_DestroyTexture(detail::tmp_buffer); });
    snail::input::instance().set_key_repeat(10, 3);
    snail::application::instance().register_finalizer(
        [&]() { font_detail::font_cache.clear(); });
    buffer(0, 800, 600);
}



void width(int width, int height, int, int)
{
    (void)width;
    (void)height;
}



int wpeek(int x, size_t index)
{
    if (index == 0)
    {
        return x >> 16;
    }
    else
    {
        assert(index == 2);
        return x & 0xffff;
    }
}



void wpoke(int& x, size_t index, int y)
{
    if (index == 0)
    {
        x = wpeek(x, 2) + (y << 16);
    }
    else
    {
        assert(index == 2);
        x = (wpeek(x, 0) << 16) + y;
    }
}



// imported functions



void func_1(const std::string&, int)
{
}



void xnotesel(std::string&)
{
}



void xnoteadd(const std::string&)
{
}



// FIXME:
// The texture which created with SDL_TEXTUREACCESS_TARGET cannot be
// locked/unlocked.
namespace gf_detail
{
::SDL_Rect rect;
void* pixels;
int pitch;


template <typename F>
void map(F f)
{
    // const auto texture =
    // snail::application::instance().get_renderer().render_target(); const auto
    // format = snail::detail::enforce_sdl(
    //         ::SDL_AllocFormat(::SDL_MasksToPixelFormatEnum(32, 0, 0, 0, 0)));
    //
    // auto pixels = reinterpret_cast<uint32_t*>(gf_detail::pixels);
    // int pixel_count = gf_detail::pitch / 4 * gf_detail::rect.h;
    //
    // for (int i = 0; i < pixel_count; ++i)
    // {
    //     uint8_t r_;
    //     uint8_t g_;
    //     uint8_t b_;
    //     ::SDL_GetRGB(pixels[i], format, &r_, &g_, &b_);
    //     pixels[i] = ::SDL_MapRGB(format, f(r_, r), f(g_, r), f(b_, r));
    // }
    //
    // ::SDL_UnlockTexture(texture);
}



} // namespace gf_detail



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



void gfini(int width, int height)
{
    (void)width;
    (void)height;
    // gf_detail::rect = {detail::current_tex_buffer().x,
    // detail::current_tex_buffer().y, width, height}; const auto texture =
    // snail::application::instance().get_renderer().render_target();
    // ::SDL_LockTexture(texture, &gf_detail::rect, &gf_detail::pixels,
    // &gf_detail::pitch); LOG("gfini", gf_detail::pitch, gf_detail::rect.h);
}



void gfdec(int r, int g, int b)
{
    (void)r;
    (void)g;
    (void)b;
    // gf_detail::map([](uint8_t v_, int v)
    //     {
    //         v = std::min(v, static_cast<int>(v_));
    //         v_ -= v;
    //         return v_;
    //     });
}



void gfdec2(int r, int g, int b)
{
    (void)r;
    (void)g;
    (void)b;
    // gf_detail::map([](uint8_t v_, int v)
    //     {
    //         v = std::min(v, static_cast<int>(v_) - 1);
    //         v_ -= v;
    //         return v_;
    //     });
}



void gfinc(int r, int g, int b)
{
    (void)r;
    (void)g;
    (void)b;
    // gf_detail::map([](uint8_t v_, int v)
    //     {
    //         v = std::min(v, 255 - static_cast<int>(v_));
    //         v_ -= v;
    //         return v_;
    //     });
}



void ematan(int, int, int)
{
}



int aplsel(const std::string&)
{
    return 0;
}



int aplobj(const std::string&, int)
{
    return 0;
}



void apledit(int& out, int kind, int column_no)
{
    if (kind == 0)
    {
        // Current position of cursor
        out = static_cast<int>(inputlog(0).length());
    }
    else if (kind == 1)
    {
        // Total number of columns
    }
    else if (kind == 2)
    {
        // Number of characters at column "column_no"
    }
}



void func_2(int, int, int, int, int, int)
{
}



void memcpy_(
    std::string& dst,
    std::string& src,
    int size,
    int dst_offset,
    int src_offset)
{
    for (int i = 0; i < size; ++i)
    {
        dst[i + dst_offset] = src[i + src_offset];
    }
}



void DIINIT()
{
}



int DIGETJOYNUM()
{
    return 0;
}



void DIGETJOYSTATE(int, int)
{
}



void HMMBITON(int& x, int n)
{
    x |= 1 << n;
}



void HMMBITOFF(int& x, int n)
{
    x &= ~(1 << n);
}



int HMMBITCHECK(int x, int n)
{
    return x & (1 << n) ? 1 : 0;
}



int sockopen(int, const std::string&, int)
{
    return 0;
}



void sockclose()
{
}



int sockget(const std::string&, int)
{
    return 0;
}



int sockput(const std::string&)
{
    return 0;
}



void netinit()
{
}



void netexec(int&)
{
}



void neterror(const std::string&)
{
}



void neturl(const std::string&)
{
}



void netdlname(const std::string&)
{
}



void netrequest(const std::string&)
{
}



void GetLastError()
{
}



int CreateMutexA(int, int, const std::string&)
{
    return 42; // Any positive number.
}



void CloseHandle(int)
{
}



int func_3()
{
    return 0;
}



int LCMapStringA(int, int, const std::string&, int, const std::string&, int)
{
    return 0;
}



int GetUserDefaultLCID()
{
    return 0;
}



void AppendMenuA()
{
}



void CheckMenuRadioItem()
{
}



void CreateMenu()
{
}



void CreatePopupMenu()
{
}



void DrawMenuBar()
{
}



void SetMenu()
{
}



void keybd_event(int, int, int)
{
}



void GetKeyboardState(elona_vector1<int>&)
{
}



int timeGetTime()
{
    return ::SDL_GetTicks();
}



int ImmGetContext(int)
{
    return 0;
}



void ImmReleaseContext(int, int)
{
}



void ImmSetOpenStatus(int, int)
{
}



int ImmGetOpenStatus(int)
{
    return 0;
}



void onkey_0()
{
    mesbox_detail::message_boxes.erase(
        std::end(mesbox_detail::message_boxes) - 1);
}



void onkey_1()
{
}



void end()
{
}



int talk_conv_jp(std::string& text, int max_line_length)
{
    max_line_length *= 1.5;

    std::string rest{text};
    text.clear();
    int n{};

    while (1)
    {
        const auto len = rest.size();
        if (int(len) < max_line_length)
        {
            text += rest;
            return n;
        }
        size_t line_length = 0;
        while (line_length <= len)
        {
            line_length += strutil::byte_count(rest[line_length]);
            if (int(line_length) > max_line_length)
            {
                // m = strmid(rest, line_length, 2);
                // if (false
                //         || m == u8"。"s
                //         || m == u8"、"s
                //         || m == u8"」"s
                //         || m == u8"』"s
                //         || m == u8"！"s
                //         || m == u8"？"s
                //         || m == u8"…"s)
                // {
                //     line_length += 2;
                // }
                text += rest.substr(0, line_length) + '\n';
                ++n;
                if (rest.size() > line_length)
                {
                    rest = rest.substr(line_length);
                }
                else
                {
                    rest = "";
                }
                break;
            }
        }
    }
}



// Assumes that `text` insists of only ASCII codes.
int talk_conv_en(std::string& text, int max_line_length)
{
    int n = 0;

    std::string rest{text};
    text.clear();
    while (1)
    {
        int line_length = 0;
        while (1)
        {
            const auto p = rest.find(' ');
            if (p == std::string::npos)
            {
                text += rest;
                return n;
            }
            if (int(line_length + p + 1) > max_line_length)
            {
                text.back() = '\n';
                ++n;
                break;
            }
            text += rest.substr(0, p + 1);
            line_length += p + 1;
            rest = rest.substr(p + 1);
        }
    }
}



int talk_conv(std::string& text, int max_line_length)
{
    if (jp)
    {
        return talk_conv_jp(text, max_line_length);
    }
    else
    {
        return talk_conv_en(text, max_line_length);
    }
}



void rm_crlf(std::string& str)
{
    std::string ret;
    for (const auto& c : str)
    {
        if (c != '\n' && c != '\r')
        {
            ret += c;
        }
    }
    str = ret;
}



} // namespace elona
