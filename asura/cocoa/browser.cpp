namespace browser_impl
{

std::string directory()
{
    std::string result = "";
    @autoreleasepool
    {
        NSOpenPanel* panel = [NSOpenPanel openPanel];
        [panel setCanChooseDirectories:YES];
        [panel setCanChooseFiles:NO];

        if([panel runModal] == NSModalResponseOK)
        {
            NSArray* urls = [panel URLs];
            const char* path = [[[urls objectAtIndex:0] path] UTF8String];
            if(path) result = std::string(path);
        }
    }

    return result;
}

}
