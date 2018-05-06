#include "util.hpp"

namespace browser_impl
{

std::string directory()
{
    wchar_t wname[PATH_MAX + 1] = L"";

    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(bi));
    bi.hwndOwner = 0;
    bi.pidlRoot = NULL;
    bi.lpszTitle = L"\nOpen Folder:";
    bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;

    LPITEMIDLIST piidl = SHBrowseForFolder(&bi);
    if(!piidl)
    {
        return "";
    }

    bool succeeded = false;
    if(SHGetPathFromIDList(piidl, wname)) { 
        succeeded = true;
        LPMALLOC pMalloc = nullptr;
        if(SUCCEEDED(SHGetMalloc(&pMalloc))) {
            pMalloc->Free(piidl);
            pMalloc->Release();
        }
    }

    if(succeeded == false) {
        return "";
    }

    std::wstring ws(wname);
    std::string name = utf16_to_utf_8(ws);

    if(name.empty())
    {
        return "";
    }

    boost::replace_all(name, "\\", "/");

    if(!boost::algorithm::ends_with(name, std::string("/")))
    {
        name.append("/");
    }

    return name;
}

}
