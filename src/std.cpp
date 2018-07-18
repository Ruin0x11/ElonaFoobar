#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>


#include "asura/asura.hpp"
#include "snail/android.hpp"
#include "snail/application.hpp"
#include "snail/hsp.hpp"
#include "snail/window.hpp"

#include "config.hpp"
#include "defines.hpp"
#include "elona.hpp"
#include "i18n.hpp"
#include "log.hpp"
#include "macro.hpp"
#include "util.hpp"
#include "variables.hpp"
#ifdef ELONA_OS_WINDOWS
#include <windows.h> // MessageBoxA
#endif

namespace
{



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


namespace elona
{



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



void await(int msec)
{
    snail::hsp::await(msec);

    // On Android, potentially quicksave if SDL detects that the app's
    // focus was lost and the player is being queried for input in
    // pc_turn().
    if (defines::is_android
        && snail::application::instance().was_focus_lost_just_now())
    {
        if (player_queried_for_input
            && config::instance().get<bool>("core.config.android.quicksave")
            && !std::uncaught_exception())
        {
            ELONA_LOG("Focus lost, quicksaving game.");
            snail::android::toast(i18n::s.get("core.locale.ui.save_on_suspend"),
                                  snail::android::toast_length::long_length);
            save_game();
        }
    }
}



void boxf(int x, int y, int width, int height, const snail::color& color)
{
    snail::hsp::boxf(x, y, width, height, color);
}



void boxf()
{
    snail::hsp::boxf({0, 0, 0, 0});
}



void boxl(int x, int y, int width, int height, const snail::color& color)
{
    const auto x2 = x + width;
    const auto y2 = y + height;
    line(x, y, x2, y, color);
    line(x2, y, x2, y2, color);
    line(x2, y2, x, y2, color);
    line(x, y2, x, y, color);
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

    // Trim trailing NUL bytes from the string that was read.
    data.erase(std::remove(data.begin(), data.end(), '\0'), data.end());
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
    for (size_t i = 0; i < data.size(); ++i)
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
    snail::hsp::buffer(window_id, width, height);
}



void color(int r, int g, int b)
{
    snail::hsp::color(r, g, b);
}



asura::dialog::response dialog(const std::string& message, dialog_type_t option) {
    using namespace asura;

    dialog::state state = {message};
    dialog::message_type type =
        dialog::message_type::information;
    dialog::response res = dialog::response::ok;

    if (option == dialog_type_t::warning_ok
        || option == dialog_type_t::warning_yes_no) {
        type = dialog::message_type::warning;
    }
    switch (option)
    {
    case dialog_type_t::info_ok:
    case dialog_type_t::warning_ok:
        res = dialog::ok(state, type);
        return res;
    case dialog_type_t::info_yes_no:
    case dialog_type_t::warning_yes_no:
        res = dialog::yes_or_no(state, type);
        return res;
    case dialog_type_t::open_file:
    case dialog_type_t::save_as:
    case dialog_type_t::color_selection:
    case dialog_type_t::color_selection_with_matrix:
    default:
        return res;
    }
}



void exec(const std::string&, int)
{
}



void font(int size, snail::font_t::style_t style)
{
    const auto& filepath = filesystem::path(u8"font")
        / filesystem::u8path(lang(
              config::instance().font1, config::instance().font2));
    snail::hsp::font(size, style, filepath);
}



void gcopy(
    int window_id,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dst_width,
    int dst_height)
{
    snail::hsp::gcopy(
        window_id, src_x, src_y, src_width, src_height, dst_width, dst_height);
}



void gcopy_c(int window_id, int src_x, int src_y, int src_width, int src_height)
{
    gcopy_c(
        window_id, src_x, src_y, src_width, src_height, src_width, src_height);
}



void gcopy_c(
    int window_id,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dst_width,
    int dst_height)
{
    pos(ginfo(22) - dst_width / 2, ginfo(23) - dst_height / 2);
    snail::hsp::gcopy(
        window_id, src_x, src_y, src_width, src_height, dst_width, dst_height);
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
    UNUSED(limit);
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
    return snail::hsp::ginfo(type);
}



void gmode(int mode, int alpha)
{
    snail::hsp::gmode(mode, alpha);
}



template <typename T>
constexpr T rad2deg(T rad)
{
    return rad * 180.0 / 3.14159265358979323846264;
}



void grotate(
    int window_id,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dst_width,
    int dst_height,
    double angle)
{
    snail::hsp::grotate(window_id, src_x, src_y, src_width, src_height, dst_width, dst_height, angle);
}



void grotate(
    int window_id,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    double angle)
{
    snail::hsp::grotate(window_id, src_x, src_y, src_width, src_height, angle);
}



void gsel(int window_id)
{
    snail::hsp::gsel(window_id);
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



void line(int x1, int y1, int x2, int y2, const snail::color& color)
{
    snail::hsp::line(x1, y1, x2, y2, color);
}



void mes(const std::string& text)
{
    snail::hsp::mes(text);
}



void mes(int n)
{
    mes(std::to_string(n));
}



void mesbox(std::string& buffer, bool text)
{
    snail::hsp::mesbox(buffer, config::instance().keywait, text);
}



void mmload(const fs::path& filepath, int id, int mode)
{
    UNUSED(filepath);
    UNUSED(id);
    UNUSED(mode);
}



void mmplay(int id)
{
    UNUSED(id);
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



void objprm(int, const std::string&)
{
}



void pget(int x, int y)
{
    UNUSED(x);
    UNUSED(y);
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
    snail::hsp::picload(img, mode);
}



void pos(int x, int y)
{
    snail::hsp::pos(x, y);
}


void redraw()
{
    snail::hsp::redraw();
}



int stick(int allow_repeat_keys)
{
    auto check_key_pressed = [allow_repeat_keys](int n, snail::key key) {
        if ((1 << n) & allow_repeat_keys)
        {
            return (1 << n) * snail::input::instance().is_pressed(key);
        }
        else
        {
            return (1 << n)
                * snail::input::instance().was_pressed_just_now(key);
        }
    };

    int ret{};

    ret += check_key_pressed(0, snail::key::left);
    ret += check_key_pressed(1, snail::key::up);
    ret += check_key_pressed(2, snail::key::right);
    ret += check_key_pressed(3, snail::key::down);
    ret += check_key_pressed(4, snail::key::space);
    ret += check_key_pressed(5, snail::key::enter);
    ret += check_key_pressed(5, snail::key::keypad_enter);
    ret += check_key_pressed(6, snail::key::ctrl);
    ret += check_key_pressed(7, snail::key::escape);
    // ret += check_key_pressed(8,  /* Mouse left */,  false);
    // ret += check_key_pressed(9,  /* Mouse right */);
    ret += check_key_pressed(10, snail::key::tab);

    if (allow_repeat_keys == 15)
    {
        if (ret & 1 || ret & 4)
        {
            ret |= 2 * snail::input::instance().is_pressed(snail::key::up);
            ret |= 8 * snail::input::instance().is_pressed(snail::key::down);
        }
        if (ret & 2 || ret & 8)
        {
            ret |= 1 * snail::input::instance().is_pressed(snail::key::left);
            ret |= 4 * snail::input::instance().is_pressed(snail::key::right);
        }
    }

    return ret;
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



void title(
    const std::string& title_str,
    const std::string& display_mode,
    snail::window::fullscreen_mode_t fullscreen_mode)
{
    snail::hsp::title(title_str, display_mode, fullscreen_mode);
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



void set_color_mod(int r, int g, int b, int window_id)
{
    snail::hsp::set_color_mod(r, g, b, window_id);
}



void ematan(int, int, int)
{
}



void apledit(int& out, int kind, int column_no)
{
    UNUSED(column_no);

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



int HMMBITCHECK(int x, int n)
{
    return x & (1 << n) ? 1 : 0;
}



int timeGetTime()
{
    return snail::hsp::timeGetTime();
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
    snail::hsp::onkey_0();
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



} // namespace elona
