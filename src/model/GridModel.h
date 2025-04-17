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

class GridModel
{
    cnt::PushBackVector<GridComponent *, 1024> _gridComponents;
    gui::Size _modelSize;

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
                pGridComponent->draw();
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
        //        mu::dbgLog("Model w=%.1f, h=%.1f", _modelSize.width, _modelSize.height);

        _gridComponents.push_back(pGridComponent);
    }

    bool load(const td::String &fileName)
    {
        //        arch::FileSerializerIn fs;
        //        if (!fs.open(fileName))
        //            return false;
        //        clean();
        //        _gridComponents.clean();
        //
        //        arch::ArchiveIn ar(fs);
        //        ar.setSupportedMajorVersion("GETF");
        //        try
        //        {
        //            td::UINT4 nElems = 0;
        //            double modelW = 0;
        //            double modelH = 0;
        //            ar >> nElems >> modelW >> modelH;
        //            _modelSize.width = modelW;
        //            _modelSize.height = modelH;
        //
        //            if (nElems == 0)
        //                return false;
        //
        //            _gridComponents.reserve(nElems);
        //
        //            for (td::UINT4 iElem = 0; iElem < nElems; ++iElem)
        //            {
        //                td::BYTE sht = 0;
        //                ar >> sht;
        //                IGridComponent::Type shType = (IGridComponent::Type)sht;
        //                switch (shType)
        //                {
        //                case IGridComponent::Type::Rect:
        //                {
        //                    gui::Rect r(0, 0, 0, 0);
        //                    IGridComponent *pShape = IGridComponent::createRect(gui::Shape::Attribs::LineAndFill, r, td::ColorID::SysText, td::ColorID::SysText, 1.0f, td::LinePattern::Solid);
        //                    pShape->load(ar);
        //                    pShape->init();
        //                    appendShape(pShape);
        //                }
        //                break;
        //                case IGridComponent::Type::RoundRect:
        //                {
        //                    gui::Rect r(0, 0, 0, 0);
        //                    float radius = 0;
        //                    IGridComponent *pShape = IGridComponent::createRoundedRect(gui::Shape::Attribs::LineAndFill, r, radius, td::ColorID::SysText, td::ColorID::SysText, 1.0f, td::LinePattern::Solid);
        //                    pShape->load(ar);
        //                    pShape->init();
        //                    appendShape(pShape);
        //                }
        //                break;
        //                case IGridComponent::Type::Circle:
        //                {
        //                    gui::Point pt(0, 0);
        //                    float radius = 0;
        //                    IGridComponent *pShape = IGridComponent::createCircle(gui::Shape::Attribs::LineAndFill, pt, radius, td::ColorID::SysText, td::ColorID::SysText, 1.0f, td::LinePattern::Solid);
        //                    pShape->load(ar);
        //                    pShape->init();
        //                    appendShape(pShape);
        //                }
        //                break;
        //                default:
        //                {
        //                    assert(false);
        //                    return false;
        //                }
        //                }
        //            }
        //        }
        //        catch (...)
        //        {
        //            return false;
        //        }
        return true;
    }

    bool save(const td::String &fileName) const
    {
        //        arch::FileSerializerOut fs;
        //        if (!fs.open(fileName))
        //            return false;
        //        arch::ArchiveOut ar("GETF", fs);
        //        try
        //        {
        //            td::UINT4 nElems = (td::UINT4)_gridComponents.size();
        //            double modelW = _modelSize.width;
        //            double modelH = _modelSize.height;
        //            ar << nElems << modelW << modelH;
        //            // td::UINT4 iElem = 0;
        //            for (auto pShape : _gridComponents)
        //            {
        //                td::BYTE shType = (td::BYTE)pShape->getType();
        //                ar << shType;
        //                pShape->save(ar);
        //                //++iElem;
        //            }
        //        }
        //        catch (...)
        //        {
        //            return false;
        //        }
        return true;
    }

    const gui::Size &getModelSize() const
    {
        return _modelSize;
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
            // TODO: Za vjezbu
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
            // TODO: Za vjezbu

            auto pos = _gridComponents.find(pGridComponent);
            if (pos < nComponents - 1)
                return _gridComponents.move(pos, nComponents - 1);
            return false;
        }
        return false;
    }
};
