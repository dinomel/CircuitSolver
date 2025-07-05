//
//  GridModel.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 8. 4. 2025..
//

#pragma once
#include "GridComponent.h"
#include <cnt/PushBackVector.h>
#include <arch/FileSerializer.h>
#include <arch/ArchiveIn.h>
#include <arch/ArchiveOut.h>
#include "../CircuitSolver.h"

// global parameters
extern DefaultSettings g_defaultSettings;

class GridModel
{
    cnt::PushBackVector<GridComponent *, 1024> _gridComponents;
    gui::Size _modelSize;
    std::unique_ptr<CircuitSolver> _circuitSolver;

protected:
    void clean()
    {
        for (auto pGridComponent : _gridComponents)
        {
            pGridComponent->release();
        }
    }

public:
    cnt::PushBackVector<GridComponent *> selectedGridComponents;

public:
    GridModel()
        : _modelSize(10, 10)
    {
    }

    ~GridModel()
    {
        clean();
    }

    void solve(bool forceSolve)
    {
        if (!forceSolve && !g_defaultSettings.getAutoSolve())
            return;

        if (_gridComponents.isEmpty())
            return;

        _circuitSolver = std::make_unique<CircuitSolver>(_gridComponents);
        std::vector<std::pair<std::complex<double>, std::complex<double>>> results = _circuitSolver->solve();

        for (int i = 0; i < _gridComponents.size(); i++)
        {
            _gridComponents[i]->setCurrent(results[i].first);
            _gridComponents[i]->setVoltage(results[i].second);
            _gridComponents[i]->calculatePower();
        }
    }

    const cnt::PushBackVector<GridComponent *, 1024> &getGridComponents() const
    {
        return _gridComponents;
    }

    const gui::Size &getModelSize() const
    {
        return _modelSize;
    }

    void updateFloatingNodes() const
    {
        std::map<CoordinatePoint, int> nodePointsRepetition;
        for (GridComponent *pC : _gridComponents)
        {
            ++nodePointsRepetition[pC->getStartCoordinate()];
            if (pC->getType() != IGridComponent::Type::Ground)
                ++nodePointsRepetition[pC->getEndCoordinate()];
        }
        for (GridComponent *pC : _gridComponents)
        {
            pC->startNode->updateIsFloating(nodePointsRepetition[pC->getStartCoordinate()] < 2);
            if (pC->getType() != IGridComponent::Type::Ground)
                pC->endNode->updateIsFloating(nodePointsRepetition[pC->getEndCoordinate()] < 2);
        }
    }

    void clearSelected()
    {
        selectedGridComponents = {};
    }

    void selectComponent(GridComponent *component)
    {
        selectedGridComponents.push_back(component);
    }

    void draw(const gui::Rect &rDraw) const
    {
        gui::Rect boundingRect;
        for (auto pGridComponent : _gridComponents)
        {
            pGridComponent->getBoundingRect(boundingRect);
            if (boundingRect.intersects(rDraw))
            {
                bool isSelected = selectedGridComponents.find(pGridComponent) != -1;
                pGridComponent->draw(isSelected);
            }
        }
    }

    void appendGridComponent(GridComponent *pGridComponent)
    {
        gui::Rect boundRect;
        pGridComponent->getBoundingRect(boundRect);
        boundRect.inflate(10, 0, 0);
        gui::Rect currentModelRect(gui::Point(0, 0), _modelSize);
        currentModelRect.unija(boundRect);
        _modelSize.width = currentModelRect.width();
        _modelSize.height = currentModelRect.height();
        _gridComponents.push_back(pGridComponent);
    }

    bool save(const td::String &fileName) const
    {
        arch::FileSerializerOut fs;
        if (!fs.open(fileName))
            return false;
        arch::ArchiveOut ar("GETF", fs);
        try
        {
            td::UINT4 nElems = (td::UINT4)_gridComponents.size();
            double modelW = _modelSize.width;
            double modelH = _modelSize.height;
            ar << nElems << modelW << modelH;

            for (auto pGridComponent : _gridComponents)
            {
                pGridComponent->save(ar);
            }
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    bool load(const td::String &fileName)
    {
        arch::FileSerializerIn fs;
        if (!fs.open(fileName))
            return false;
        clean();
        _gridComponents.clean();
        selectedGridComponents.clean();

        arch::ArchiveIn ar(fs);
        ar.setSupportedMajorVersion("GETF");
        try
        {
            td::UINT4 nElems = 0;
            double modelW = 0;
            double modelH = 0;
            ar >> nElems >> modelW >> modelH;
            _modelSize.width = modelW;
            _modelSize.height = modelH;

            if (nElems == 0)
                return false;

            _gridComponents.reserve(nElems);

            for (td::UINT4 iElem = 0; iElem < nElems; ++iElem)
            {
                double startNodeX, startNodeY, endNodeX, endNodeY;

                ar >> startNodeX >> startNodeY >> endNodeX >> endNodeY;
                gui::Point startNode(startNodeX, startNodeY);
                gui::Point endNode(endNodeX, endNodeY);

                td::BYTE gct = 0;
                ar >> gct;
                IGridComponent::Type gridComponentType = (IGridComponent::Type)gct;

                IGridComponent *iGridComp = IGridComponent::createGridComponent(startNode, endNode, gridComponentType);
                GridComponent *pGridComponent = dynamic_cast<GridComponent *>(iGridComp);
                pGridComponent->load(ar);

                appendGridComponent(pGridComponent);
                pGridComponent->updateShape();
            }
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    bool exportModel(const td::String &fileName) const
    {
        arch::FileSerializerOut fs;
        if (!fs.open(fileName))
            return false;
        arch::ArchiveOut ar("GETF", fs);
        try
        {
            std::string s = _circuitSolver->exportModel();
            for (int i = 0; i < s.size(); i++)
            {
                ar << s[i];
            }
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    // 0: not selected, 1: startNode selected, 2: endNode selected
    int getSelectedNode(const gui::Point &pt, GridComponent *parent)
    {
        if (parent == nullptr)
            return 0;
        if (parent->startNode->canBeSelected(pt))
            return 1;
        else if (parent->endNode->canBeSelected(pt))
            return 2;
        return 0;
    }

    GridComponent *getSelectedElement(const gui::Point &pt)
    {
        size_t nElems = _gridComponents.size();
        if (nElems == 0)
            return nullptr;
        for (size_t i = nElems; i > 0; --i)
        {
            GridComponent *pGridComponent = _gridComponents[i - 1];
            if (pGridComponent->canBeSelected(pt))
            {
                return pGridComponent;
            }
        }
        return nullptr;
    }

    void remove(GridComponent *&component)
    {
        if (component)
        {
            _gridComponents.remove(component);
            selectedGridComponents.remove(component);
            component->release();
            component = nullptr;
        }
    }

    bool setFront(GridComponent *pGridComponent)
    {
        auto nComponents = _gridComponents.size();
        if (nComponents <= 1)
            return false;

        if (pGridComponent)
        {
            auto pos = _gridComponents.find(pGridComponent);
            if (pos > 0)
                return _gridComponents.move(pos, 0);
            return false;
        }
        return false;
    }

    bool setBack(GridComponent *pGridComponent)
    {
        auto nComponents = _gridComponents.size();
        if (nComponents <= 1)
            return false;

        if (pGridComponent)
        {
            auto pos = _gridComponents.find(pGridComponent);
            if (pos < nComponents - 1)
                return _gridComponents.move(pos, nComponents - 1);
            return false;
        }
        return false;
    }
};
