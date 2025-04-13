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
    ResistorGridComponent r1(10, {0, 0});
    ResistorGridComponent r2(10, {0, 0});
    CapacitorGridComponent c(0.01, {0, 0});
    InductorGridComponent l(1, {0, 0});
    NodeGridComponentOld n({0, 0}, 0, true);
    NodeGridComponent nn({0, 0});
    std::vector<GridComponent *> components({
        &r1,
        &r2,
        &c,
        &l,
        &l,
        &n,
    });
    Component *component = components[0]->getComponent();
    Resistor *r = dynamic_cast<Resistor *>(component);
    Capacitor *cap = dynamic_cast<Capacitor *>(components[2]->getComponent());

    //    Resistor r = &(components[0]->getComponent());
    std::cout << cap->capacitance;

    Application app(argc, argv);
    auto appProperties = app.getProperties();
    td::String trLang = appProperties->getValue("translation", "BA");
    app.init(trLang);
    return app.run();
}
