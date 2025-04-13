#include "Application.h"
#include <vector>
#include "model/GridComponent.h"
#include "model/ResistorGridComponent.h"
#include "model/CapacitorGridComponent.h"
#include "model/InductorGridComponent.h"
#include "model/NodeGridComponentOld.h"
#include "model/NodeGridComponent.h"
#include <iostream>

// global parameters
DefaultSettings g_defaultSettings;

const int gridSize = 16;

int main(int argc, const char *argv[])
{
    //    ResistorGridComponent r1(10, nullptr, nullptr);
    //    ResistorGridComponent r2(10, nullptr, nullptr);
    //    CapacitorGridComponent c(0.01, nullptr, nullptr);
    //    InductorGridComponent l(1, nullptr, nullptr);
    //    NodeGridComponentOld n(nullptr, nullptr, 0, true);
    //    NodeGridComponent nn({0, 0});
    //    std::vector<GridComponent *> components({
    //        &r1,
    //        &r2,
    //        &c,
    //        &l,
    //        &l,
    //        &n,
    //    });
    //    Component *component = components[0]->getComponent();
    //    Resistor *r = dynamic_cast<Resistor *>(component);
    //    Capacitor *cap = dynamic_cast<Capacitor *>(components[2]->getComponent());
    //
    //    //    Resistor r = &(components[0]->getComponent());
    //    std::cout << cap->capacitance;

    Application app(argc, argv);
    auto appProperties = app.getProperties();
    td::String trLang = appProperties->getValue("translation", "BA");
    app.init(trLang);
    return app.run();
}
