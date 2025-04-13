#include "model/ResistorGridComponent.h"
#include "model/CapacitorGridComponent.h"
#include "model/InductorGridComponent.h"
#include "model/NodeGridComponentOld.h"
#include "model/NodeGridComponent.h"
#include <gui/Properties.h>

int Component::nextID = 0;

IGridComponent::Tool IGridComponent::currentTool = IGridComponent::Tool::Selector;
float IGridComponent::maxLineWidth = 20;
gui::CoordType IGridComponent::selectionDisance2 = 5 * 5;
gui::CoordType IGridComponent::refreshOffset = 2;

gui::Properties IGridComponent::_resistorProperties;
gui::Properties IGridComponent::_capacitorPropertes;
gui::Properties IGridComponent::_inductorPropertes;

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

IGridComponent *IGridComponent::createNodeOld(const gui::Point &initPoint, int parentComponentID, bool isStartNode)
{
    NodeGridComponent *startNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));
    NodeGridComponent *endNode = dynamic_cast<NodeGridComponent *>(IGridComponent::createNode(initPoint));

    NodeGridComponentOld *pComp = new NodeGridComponentOld(startNode, endNode, parentComponentID, isStartNode);
    pComp->init();
    return pComp;
}

IGridComponent *IGridComponent::createNode(const gui::Point &initPoint)
{
    NodeGridComponent *pComp = new NodeGridComponent(IGridComponent::getClosestGridPoint(initPoint));
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

    NodeGridComponent nC({0, 0});
    NodeGridComponent *pnC = &nC;

    {
        ResistorGridComponent sh(0, pnC, pnC);
        _resistorProperties.reserve(11);
        sh.initProperties(&_resistorProperties);
    }

    {
        CapacitorGridComponent sh(0, pnC, pnC);
        _capacitorPropertes.reserve(12);
        sh.initProperties(&_capacitorPropertes);
    }

    {
        CapacitorGridComponent sh(0, pnC, pnC);
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
