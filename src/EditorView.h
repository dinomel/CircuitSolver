//
//  EditorView.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 8. 4. 2025..
//

#pragma once
#include <gui/Canvas.h>
#include <gui/Context.h>
#include <gui/Symbol.h>
#include <gui/Image.h>
#include <gui/Shape.h>
#include <gui/DrawableString.h>
#include "property/DefaultSettings.h"
#include "model/GridModel.h"
#include <gui/Cursor.h>
#include <gui/PropertyEditorSwitcher.h>
#include <iostream>
#include "model/GridComponent.h"

// global parameters
extern DefaultSettings g_defaultSettings;

extern const int gridSize;

class EditorView : public gui::Canvas
{
    enum class LastEvent : unsigned char
    {
        None,
        MouseClick,
        Drag
    };

protected:
    GridModel _model;
    GridComponent *_pSelectedShape = nullptr;
    GridComponent *_pCreatingComponent = nullptr;
    gui::PropertyEditorSwitcher *_pPropSwitcher = nullptr;
    gui::Point _lastMouseClickPoint;
    LastEvent _lastEvent = LastEvent::None;
    gui::Alert::CallBack _callBackDeleteSelectedShape;

protected:
    void updatePropertyValues()
    {
        _pPropSwitcher->updateValues();
        setFocus(); // to this
    }

    void deleteSelectedShape()
    {
        cnt::PushBackVector<GridComponent *> selectedComponents = _model.selectedGridComponents;
        if (selectedComponents.isEmpty())
            return;
        for (GridComponent *pC : selectedComponents)
        {
            _model.remove(pC);
        }
        _pPropSwitcher->showView(0);
        setFocus(); // to this
        reDraw();
    }

    void checkDeleteSelectedAnswer(gui::Alert::Answer answer)
    {
        if (answer == gui::Alert::Answer::Yes)
        {
            deleteSelectedShape();
        }
        setFocus(); // to this
    }

    bool getModelSize(gui::Size &modelSize) const override
    {
        modelSize = _model.getModelSize();
        return true;
    }

    void onDraw(const gui::Rect &rDraw) override
    {
        //        mu::dbgLog("x=%.1f y=%.1f w=%.1f h=%.1f", rDraw.left, rDraw.top, rDraw.width(), rDraw.height());
        _model.draw(rDraw);

        cnt::PushBackVector<GridComponent *> selectedComponents = _model.selectedGridComponents;
        if (selectedComponents.isEmpty())
            return;
        for (GridComponent *pC : selectedComponents)
        {
            gui::Rect r;
            pC->getBoundingRect(r);
            gui::Shape::drawSelectionRect(r);
        }
    }

    bool onActionItem(gui::ActionItemDescriptor &aiDesc) override
    {
        //        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        //        auto pAI = aiDesc.getActionItem();
        //
        //        if (menuID == 100)
        //        {
        //            assert(_pSelectedShape);
        //            if (!_pSelectedShape)
        //                return true;
        //            // context menu;
        //            switch (actionID)
        //            {
        //            case 10:
        //                // move front
        //                // TODO: vjezba
        //                if (_model.setBack(_pSelectedShape))
        //                    reDraw();
        //                break;
        //            case 20:
        //                // move back
        //                // TODO: vjezba
        //                if (_model.setFront(_pSelectedShape))
        //                    reDraw();
        //                break;
        //            case 30:
        //            {
        //                // Delete without checking
        //                deleteSelectedShape();
        //                break;
        //            }
        //            }
        //            return true;
        //        }
        return false;
    }

    void onPrimaryButtonPressed(const gui::InputDevice &inputDevice) override
    {
        const gui::Point &modelPoint = inputDevice.getModelPoint();

        _lastMouseClickPoint = modelPoint;
        _lastEvent = LastEvent::MouseClick;
        //        mu::dbgLog("EditorView onMouseDown!");
        switch (IGridComponent::currentTool)
        {
        case IGridComponent::Tool::Selector:
        {
            GridComponent *pSelected = _model.getSelectedElement(modelPoint);
            if (!pSelected)
            {
                _model.clearSelected();
                _pPropSwitcher->showView(0);
                reDraw();
            }
            else
            {
                bool isSelected = _model.selectedGridComponents.find(pSelected) != -1;

                if (!isSelected)
                {
                    // TODO: DINO: Ako drzi shift ne treba clearati selected
                    _model.clearSelected();
                    _model.selectComponent(pSelected);
                    reDraw();
                }
            }
            setFocus(); // to this
        }
        break;
        case IGridComponent::Tool::AddWire:
        {
            IGridComponent *iGridComp = IGridComponent::createInductor(modelPoint, g_defaultSettings.getFillColor(), g_defaultSettings.getLineColor());

            _pCreatingComponent = dynamic_cast<GridComponent *>(iGridComp);

            _model.appendShape(_pCreatingComponent);
            reDraw();
        }
        break;
        case IGridComponent::Tool::AddResistor:
        {
            IGridComponent *iGridComp = IGridComponent::createResistor(modelPoint, g_defaultSettings.getFillColor(), g_defaultSettings.getLineColor());

            _pCreatingComponent = dynamic_cast<GridComponent *>(iGridComp);

            _model.appendShape(_pCreatingComponent);
            reDraw();
        }
        break;
        case IGridComponent::Tool::AddCapacitor:
        {
            IGridComponent *iGridComp = IGridComponent::createCapacitor(modelPoint, g_defaultSettings.getFillColor(), g_defaultSettings.getLineColor());

            _pCreatingComponent = dynamic_cast<GridComponent *>(iGridComp);

            _model.appendShape(_pCreatingComponent);
            reDraw();
        }
        break;
        case IGridComponent::Tool::AddInductor:
        {
            IGridComponent *iGridComp = IGridComponent::createInductor(modelPoint, g_defaultSettings.getFillColor(), g_defaultSettings.getLineColor());

            _pCreatingComponent = dynamic_cast<GridComponent *>(iGridComp);

            _model.appendShape(_pCreatingComponent);
            reDraw();
        }
        break;
        default:
            assert(false);
        }
    }

    void onPrimaryButtonReleased(const gui::InputDevice &inputDevice) override
    {
        cnt::PushBackVector<GridComponent *> selectedComponents = _model.selectedGridComponents;
        if (_lastEvent == LastEvent::Drag && !selectedComponents.isEmpty())
        {
            for (GridComponent *pC : selectedComponents)
            {
                pC->snapToGrid();
            }
            reDraw();
        }
        if (_lastEvent == LastEvent::Drag && _pCreatingComponent)
        {
            _pCreatingComponent->snapToGrid();
            reDraw();
        }
        _pCreatingComponent = nullptr;
        _lastEvent = LastEvent::None;
    }

    void onSecondaryButtonPressed(const gui::InputDevice &inputDevice) override
    {
        //        if (IGridComponent::currentTool != IGridComponent::Tool::Selector)
        //            return;
        //
        //        if (!_pSelectedShape)
        //            return;
        //        gui::Rect boundRect;
        //        _pSelectedShape->getBoundingRect(boundRect);
        //        boundRect.inflate(5.0);
        //
        //        const gui::Point &modelPoint = inputDevice.getModelPoint();
        //        if (!boundRect.contains(modelPoint))
        //            return;
        //
        //        td::BYTE ctxMenuID = 100;
        //        Frame::openContextMenu(ctxMenuID, inputDevice);
        //        return;
    }

    void onCursorDragged(const gui::InputDevice &inputDevice) override // onMouseDragged(const gui::Point& modelPoint, td::UINT4 keyModifiers)
    {

        if ((_lastEvent != LastEvent::Drag) && (_lastEvent != LastEvent::MouseClick))
        {
            return;
        }

        _lastEvent = LastEvent::Drag;
        const gui::Point &modelPoint = inputDevice.getModelPoint();

        if (_pCreatingComponent != nullptr)
        {
            _pCreatingComponent->updateEndPoint(modelPoint);
            updatePropertyValues();
            _lastMouseClickPoint = modelPoint;
            reDraw();
            return;
        }

        cnt::PushBackVector<GridComponent *> selectedComponents = _model.selectedGridComponents;
        if (selectedComponents.isEmpty())
            return;

        gui::Point delta(modelPoint.x - _lastMouseClickPoint.x, modelPoint.y - _lastMouseClickPoint.y);
        for (GridComponent *pC : selectedComponents)
        {
            pC->translate(delta);
        }

        updatePropertyValues();
        _lastMouseClickPoint = modelPoint;
        reDraw();
    }

    bool onZoom(const gui::InputDevice &inputDevice) override // onScrollWheel(const gui::Point& modelPoint, const gui::Point& dXdY, td::UINT4 keyModifiers)
    {
        auto &modelPoint = inputDevice.getModelPoint();
        double scale = inputDevice.getScale();
        double oldScale = getScale();
        double newScale = oldScale * scale; // scale * (1+ dXdY.y/100);
        scaleToPoint(newScale, modelPoint);
        return true;
    }

    bool onKeyPressed(const gui::Key &key) override
    {
        // call base class
        if (gui::Canvas::onKeyPressed(key))
            return true;

        cnt::PushBackVector<GridComponent *> selectedComponents = _model.selectedGridComponents;

        if (!selectedComponents.isEmpty())
        {
            double mult = 1;
            if (key.isCtrlPressed() || key.isCmdPressed())
                mult = 3;
            double dMove = gridSize * mult;
            gui::Key::Virtual vk = key.getVirtual();
            switch (vk)
            {
            case gui::Key::Virtual::Left:
            case gui::Key::Virtual::NumLeft:
            {
                gui::Point delta(-dMove, 0);
                for (GridComponent *pC : selectedComponents)
                {
                    pC->translate(delta);
                }
                updatePropertyValues();
                reDraw();
            }
            break;
            case gui::Key::Virtual::Right:
            case gui::Key::Virtual::NumRight:
            {
                gui::Point delta(dMove, 0);
                for (GridComponent *pC : selectedComponents)
                {
                    pC->translate(delta);
                }
                updatePropertyValues();
                reDraw();
            }
            break;
            case gui::Key::Virtual::Up:
            case gui::Key::Virtual::NumUp:
            {
                gui::Point delta(0, -dMove);
                for (GridComponent *pC : selectedComponents)
                {
                    pC->translate(delta);
                }
                updatePropertyValues();
                reDraw();
            }
            break;
            case gui::Key::Virtual::Down:
            case gui::Key::Virtual::NumDown:
            {
                gui::Point delta(0, dMove);
                for (GridComponent *pC : selectedComponents)
                {
                    pC->translate(delta);
                }
                updatePropertyValues();
                reDraw();
            }
            break;
            case gui::Key::Virtual::Delete:
            case gui::Key::Virtual::NumDelete:
                showYesNoQuestionAsync(&_callBackDeleteSelectedShape, tr("DelShape"), tr("SureDelShape"), tr("Yes"), tr("No"));
                break;
            default:
                break;
            }
        }
        return true;
    }

public:
    EditorView()
        : gui::Canvas({gui::InputDevice::Event::CursorShape, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks, gui::InputDevice::Event::CursorMove, gui::InputDevice::Event::CursorDrag, gui::InputDevice::Event::Zoom, gui::InputDevice::Event::Keyboard}), _callBackDeleteSelectedShape(std::bind(&EditorView::checkDeleteSelectedAnswer, this, std::placeholders::_1))
    {
        //        setCursor(gui::Cursor::Type::Default);
    }

    const GridModel *getModel() const
    {
        return &_model;
    }

    void save(const td::String &fileName)
    {
        if (_model.save(fileName))
        {
            mu::dbgLog("Shapes saved in file = %s", fileName.c_str());
        }
    }

    void load(const td::String &fileName)
    {
        //        _pSelectedShape = nullptr;
        //        if (_pPropSwitcher)
        //        {
        //            _pPropSwitcher->showView(0);
        //        }
        //        if (_model.load(fileName))
        //        {
        //            mu::dbgLog("Shapes opened from file = %s", fileName.c_str());
        //        }
        //        scale(1.0);
        //        reDraw();
    }

    void setPropSwitcher(gui::PropertyEditorSwitcher *pPropSwitcher)
    {
        _pPropSwitcher = pPropSwitcher;
    }

    void updateCursor() const
    {
        if (IGridComponent::currentTool == IGridComponent::Tool::Selector)
        {
            setCursor(gui::Cursor::Type::Default);
            //            gui::Cursor cursor = gui::Cursor(gui::Cursor::Type::Default);
            //            cursor.push();
        }
        else
        {
            setCursor(gui::Cursor::Type::Adder);
            //            gui::Cursor cursor = gui::Cursor(gui::Cursor::Type::Adder);
            //            cursor.push();
        }
    }
};
