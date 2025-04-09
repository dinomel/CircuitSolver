#include "Application.h"

//global parameters
DefaultSettings g_defaultSettings;

int main(int argc, const char *argv[])
{
    Application app(argc, argv);
    auto appProperties = app.getProperties();
    td::String trLang = appProperties->getValue("translation", "BA");
    app.init(trLang);
    return app.run();
}
