#include "model/ResistorGridComponent.h"
#include "model/CapacitorGridComponent.h"
#include "model/InductorGridComponent.h"
#include "model/NodeGridComponent.h"
#include "model/WireGridComponent.h"
#include "model/DCVoltageSourceGridComponent.h"
#include "model/ACVoltageSourceGridComponent.h"
#include "model/CurrentSourceGridComponent.h"
#include <gui/Properties.h>

int Component::nextID = 0;

IGridComponent::Tool IGridComponent::currentTool = IGridComponent::Tool::Selector;
float IGridComponent::maxLineWidth = 20;
gui::CoordType IGridComponent::selectionDisance2 = 5 * 5;
gui::CoordType IGridComponent::refreshOffset = 2;

gui::Properties IGridComponent::_resistorProperties;
gui::Properties IGridComponent::_capacitorProperties;
gui::Properties IGridComponent::_inductorProperties;
gui::Properties IGridComponent::_wireProperties;
gui::Properties IGridComponent::_dcVoltageProperties;
gui::Properties IGridComponent::_acVoltageProperties;
gui::Properties IGridComponent::_currentProperties;

static cnt::SafeFullVector<td::String> s_attribStrings;

IGridComponent *IGridComponent::createResistor(const gui::Point &initPoint)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    ResistorGridComponent *pComp = new ResistorGridComponent(10, startNode, endNode);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createCapacitor(const gui::Point &initPoint)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    CapacitorGridComponent *pComp = new CapacitorGridComponent(0.1, startNode, endNode);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createInductor(const gui::Point &initPoint)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    InductorGridComponent *pComp = new InductorGridComponent(1, startNode, endNode);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createWire(const gui::Point &initPoint)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    WireGridComponent *pComp = new WireGridComponent(startNode, endNode);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createDCVoltage(const gui::Point &initPoint)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    DCVoltageSourceGridComponent *pComp = new DCVoltageSourceGridComponent(5, startNode, endNode);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createACVoltage(const gui::Point &initPoint)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    ACVoltageSourceGridComponent *pComp = new ACVoltageSourceGridComponent(5, startNode, endNode);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createCurrent(const gui::Point &initPoint)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    CurrentSourceGridComponent *pComp = new CurrentSourceGridComponent(5, startNode, endNode);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createNode(const gui::Point &initPoint)
{
    NodeGridComponent *pComp = new NodeGridComponent(IGridComponent::getClosestGridPoint(initPoint));
    pComp->init();
    return pComp;
}

gui::Properties *IGridComponent::getProperties(IGridComponent::Type gridComponentType)
{
    initProperties();
    switch (gridComponentType)
    {
    case Type::Resistor:
        return &_resistorProperties;
    case Type::Capacitor:
        return &_capacitorProperties;
    case Type::Inductor:
        return &_inductorProperties;
    case Type::Wire:
        return &_wireProperties;
    case Type::DCVoltageSource:
        return &_dcVoltageProperties;
    case Type::ACVoltageSource:
        return &_acVoltageProperties;
    case Type::CurrentSource:
        return &_currentProperties;
    default:
        assert(false);
    }
    return nullptr;
}

void IGridComponent::initProperties()
{
    if (_resistorProperties.size() > 0)
        return;

    NodeGridComponent nC({0, 0});
    NodeGridComponent *pnC = &nC;

    {
        ResistorGridComponent sh(0, pnC, pnC);
        _resistorProperties.reserve(13);
        sh.initProperties(&_resistorProperties);
    }

    {
        CapacitorGridComponent sh(0, pnC, pnC);
        _capacitorProperties.reserve(12);
        sh.initProperties(&_capacitorProperties);
    }

    {
        InductorGridComponent sh(0, pnC, pnC);
        _inductorProperties.reserve(12);
        sh.initProperties(&_inductorProperties);
    }

    {
        WireGridComponent sh(pnC, pnC);
        _wireProperties.reserve(12);
        sh.initProperties(&_wireProperties);
    }

    {
        DCVoltageSourceGridComponent sh(0, pnC, pnC);
        _dcVoltageProperties.reserve(12);
        sh.initProperties(&_dcVoltageProperties);
    }

    {
        ACVoltageSourceGridComponent sh(0, pnC, pnC);
        _acVoltageProperties.reserve(12);
        sh.initProperties(&_acVoltageProperties);
    }

    {
        CurrentSourceGridComponent sh(0, pnC, pnC);
        _currentProperties.reserve(12);
        sh.initProperties(&_currentProperties);
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
