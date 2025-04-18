#include "Application.h"
#include <vector>

// global parameters
DefaultSettings g_defaultSettings;

const int gridSize = 16;

int main(int argc, const char *argv[])
{
    Application app(argc, argv);
    auto appProperties = app.getProperties();
    td::String trLang = appProperties->getValue("translation", "BA");
    app.init(trLang);
    return app.run();
}
