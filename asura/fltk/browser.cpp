namespace browser_impl
{

std::string directory()
{
    Fl_Native_File_Chooser fnfc;
    fnfc.title("Open directory");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);
    fnfc.directory(".");

    if(fnfc.show() == 0)
    {
        return std::string(fnfc.filename());
    }
    else
    {
        return "";
    }
}

}
