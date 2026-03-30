#include "MibletsApp.h"
#include <exception>
#include <iostream>

int main()
{
    try
    {
        miblets::MibletsApp app;
        if (!app.Initialize())
        {
            std::cerr << "Failed to initialize Miblets App.\n";
            return 1;
        }

        app.Run();
        app.Shutdown();
        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 1;
    }
}
