static std::wstring utf8_to_utf_16(const std::string& str)
{
    if (str.empty()) return std::wstring();
    int sz = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), 0, 0);
    std::wstring res(sz, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &res[0], sz);
    return res;
}

static std::string utf16_to_utf_8(const std::wstring& str)
{
    if (str.empty()) return std::string();
    int sz = WideCharToMultiByte(CP_UTF8, 0, &str[0], (int)str.size(), nullptr, 0, nullptr, nullptr);
    std::string res(sz, 0);
    WideCharToMultiByte(CP_UTF8, 0, &str[0], (int)str.size(), &res[0], sz, nullptr, nullptr);
    return res;
}
