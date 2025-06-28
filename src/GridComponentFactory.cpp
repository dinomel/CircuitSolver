#include "model/ResistorGridComponent.h"
#include "model/CapacitorGridComponent.h"
#include "model/InductorGridComponent.h"
#include "model/NodeGridComponent.h"
#include "model/WireGridComponent.h"
#include "model/DCVoltageSourceGridComponent.h"
#include "model/ACVoltageSourceGridComponent.h"
#include "model/CurrentSourceGridComponent.h"
#include "model/GroundGridComponent.h"
#include <gui/Properties.h>

int Component::nextID = 0;

IGridComponent::Tool IGridComponent::currentTool = IGridComponent::Tool::Selector;
float IGridComponent::maxFrequency = 1000;

gui::Properties IGridComponent::_resistorProperties;
gui::Properties IGridComponent::_capacitorProperties;
gui::Properties IGridComponent::_inductorProperties;
gui::Properties IGridComponent::_wireProperties;
gui::Properties IGridComponent::_dcVoltageProperties;
gui::Properties IGridComponent::_acVoltageProperties;
gui::Properties IGridComponent::_currentProperties;
gui::Properties IGridComponent::_groundProperties;

static cnt::SafeFullVector<td::String> s_attribStrings;

IGridComponent *IGridComponent::createResistor(const gui::Point &initPoint)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    ResistorGridComponent *pComp = new ResistorGridComponent(1, startNode, endNode);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createCapacitor(const gui::Point &initPoint)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    CapacitorGridComponent *pComp = new CapacitorGridComponent(10, startNode, endNode);
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

    ACVoltageSourceGridComponent *pComp = new ACVoltageSourceGridComponent(220, startNode, endNode);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createCurrent(const gui::Point &initPoint)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    CurrentSourceGridComponent *pComp = new CurrentSourceGridComponent(10, startNode, endNode);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createGround(const gui::Point &initPoint)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    GroundGridComponent *pComp = new GroundGridComponent(startNode, endNode);
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
    case Type::Ground:
        return &_groundProperties;
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
        _resistorProperties.reserve(18);
        sh.initProperties(&_resistorProperties);
    }

    {
        CapacitorGridComponent sh(0, pnC, pnC);
        _capacitorProperties.reserve(18);
        sh.initProperties(&_capacitorProperties);
    }

    {
        InductorGridComponent sh(0, pnC, pnC);
        _inductorProperties.reserve(18);
        sh.initProperties(&_inductorProperties);
    }

    {
        WireGridComponent sh(pnC, pnC);
        _wireProperties.reserve(18);
        sh.initProperties(&_wireProperties);
    }

    {
        DCVoltageSourceGridComponent sh(0, pnC, pnC);
        _dcVoltageProperties.reserve(18);
        sh.initProperties(&_dcVoltageProperties);
    }

    {
        ACVoltageSourceGridComponent sh(0, pnC, pnC);
        _acVoltageProperties.reserve(18);
        sh.initProperties(&_acVoltageProperties);
    }

    {
        CurrentSourceGridComponent sh(0, pnC, pnC);
        _currentProperties.reserve(18);
        sh.initProperties(&_currentProperties);
    }

    {
        GroundGridComponent sh(pnC, pnC);
        _groundProperties.reserve(18);
        sh.initProperties(&_groundProperties);
    }
}
