#include "model/ResistorComponent.h"
#include "model/CapacitorComponent.h"
#include "model/InductorComponent.h"
#include <gui/Properties.h>

IGridComponent::Tool IGridComponent::currentTool = IGridComponent::Tool::Selector;
float IGridComponent::maxLineWidth = 20;
gui::CoordType IGridComponent::selectionDisance2 = 5 * 5;
gui::CoordType IGridComponent::refreshOffset = 2;

gui::Properties IGridComponent::_resistorProperties;
gui::Properties IGridComponent::_capacitorPropertes;
gui::Properties IGridComponent::_inductorPropertes;

static cnt::SafeFullVector<td::String> s_attribStrings;

IGridComponent *IGridComponent::createResistorComponent(const gui::Point &initPoint, td::ColorID fillColor, td::ColorID lineColor)
{
    ResistorComponent *pComp = new ResistorComponent(initPoint, fillColor, lineColor);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createCapacitorComponent(const gui::Point &initPoint, td::ColorID fillColor, td::ColorID lineColor)
{
    CapacitorComponent *pComp = new CapacitorComponent(initPoint, fillColor, lineColor);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createInductorComponent(const gui::Point &initPoint, td::ColorID fillColor, td::ColorID lineColor)
{
    InductorComponent *pComp = new InductorComponent(initPoint, fillColor, lineColor);
    pComp->init();
    return pComp;
}

gui::Properties *IGridComponent::getProperties(IGridComponent::Type shapeType)
{
    initProperties();
    switch (shapeType)
    {
    case Type::Resistor:
        return &_resistorProperties;
    case Type::Capacitor:
        return &_capacitorPropertes;
    case Type::Inductor:
        return &_inductorPropertes;
    default:
        assert(false);
    }
    return nullptr;
}

void IGridComponent::initProperties()
{
    if (_resistorProperties.size() > 0)
        return;

    {
        ResistorComponent sh({0, 0}, td::ColorID::SysText, td::ColorID::SysText);
        _resistorProperties.reserve(11);
        sh.initProperties(&_resistorProperties);
    }

    {
        CapacitorComponent sh({0, 0}, td::ColorID::SysText, td::ColorID::SysText);
        _capacitorPropertes.reserve(12);
        sh.initProperties(&_capacitorPropertes);
    }

    {
        CapacitorComponent sh({0, 0}, td::ColorID::SysText, td::ColorID::SysText);
        _inductorPropertes.reserve(12);
        sh.initProperties(&_inductorPropertes);
    }
}

td::String *IGridComponent::getAttribsDesc()
{
    if (s_attribStrings.size() == 0)
    {
        s_attribStrings.reserve(3);
        s_attribStrings[0] = "Line only";
        s_attribStrings[1] = "Fill only";
        s_attribStrings[2] = "Line & fill";
    }

    return s_attribStrings.begin();
}
