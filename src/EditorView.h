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
#include "model/GridModel.h"
#include <gui/Cursor.h>
#include <gui/PropertyEditorSwitcher.h>
#include <iostream>
#include "model/GridComponent.h"

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
    GridComponent *_pCreatingComponent = nullptr;
    // 0: not selected, 1: startNode selected, 2: endNode selected
    int _selectedNode = 0;
    gui::PropertyEditorSwitcher *_pPropSwitcher = nullptr;
    gui::Point _lastMouseClickPoint;
    LastEvent _lastEvent = LastEvent::None;
    gui::Alert::CallBack _callBackDeleteSelectedShape;
    SymbolsPopover *_symbolsPopover;

protected:
    void updatePropertyValues()
    {
        _pPropSwitcher->updateValues();
        setFocus(); // to this
    }

    void deleteSelectedComponents()
    {
        cnt::PushBackVector<GridComponent *> selectedComponents = _model.selectedGridComponents;
        if (selectedComponents.isEmpty())
            return;
        for (GridComponent *pC : selectedComponents)
        {
            _model.remove(pC);
        }
        _model.updateFloatingNodes();
        _pPropSwitcher->showView(0);
        setFocus(); // to this
        reDraw();
        _model.solve(false);
        updatePropertyValues();
    }

    void checkDeleteSelectedAnswer(gui::Alert::Answer answer)
    {
        if (answer == gui::Alert::Answer::Yes)
        {
            deleteSelectedComponents();
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
        _model.draw(rDraw);
    }

    bool onActionItem(gui::ActionItemDescriptor &aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();

        if (menuID == 100)
        {
            cnt::PushBackVector<GridComponent *> selectedComponents = _model.selectedGridComponents;
            if (selectedComponents.isEmpty())
                return true;
            // context menu;
            switch (actionID)
            {
            case 10:
                for (GridComponent *pC : selectedComponents)
                {
                    _model.setBack(pC);
                }
                reDraw();
                break;
            case 20:
                for (GridComponent *pC : selectedComponents)
                {
                    _model.setFront(pC);
                }
                reDraw();
                break;
            case 30:
            {
                deleteSelectedComponents();
                break;
            }
            }
            return true;
        }
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
            _selectedNode = _model.getSelectedNode(modelPoint, pSelected);
            if (!pSelected)
            {
                _model.clearSelected();
                _pPropSwitcher->showView(0);
                reDraw();
            }
            else if (_selectedNode != 0)
            {
                _model.clearSelected();
                _model.selectComponent(pSelected);

                int pos = (int)pSelected->getType();
                if (_pPropSwitcher)
                    _pPropSwitcher->setCurrentEditor(pos + 1, pSelected, true);

                reDraw();
            }
            else
            {
                bool isSelected = _model.selectedGridComponents.find(pSelected) != -1;

                if (!isSelected)
                {
                    // nije bilo selectovano

                    if (!inputDevice.getKey().isShiftPressed())
                        _model.clearSelected();
                    _model.selectComponent(pSelected);

                    if (_model.selectedGridComponents.size() == 1)
                    {
                        int pos = (int)pSelected->getType();
                        if (_pPropSwitcher)
                            _pPropSwitcher->setCurrentEditor(pos + 1, pSelected, true);
                    }
                    else if (_pPropSwitcher)
                        _pPropSwitcher->showView(0);

                    reDraw();
                }
                else if (inputDevice.getKey().isShiftPressed())
                {
                    _model.selectedGridComponents.remove(pSelected);

                    if (_model.selectedGridComponents.size() == 1)
                    {
                        pSelected = _model.selectedGridComponents[0];
                        int pos = (int)pSelected->getType();
                        if (_pPropSwitcher)
                            _pPropSwitcher->setCurrentEditor(pos + 1, pSelected, true);
                    }
                    reDraw();
                }
            }

            setFocus(); // to this
        }
        break;
        case IGridComponent::Tool::AddWire:
        case IGridComponent::Tool::AddResistor:
        case IGridComponent::Tool::AddCapacitor:
        case IGridComponent::Tool::AddInductor:
        case IGridComponent::Tool::AddDCVoltageSource:
        case IGridComponent::Tool::AddACVoltageSource:
        case IGridComponent::Tool::AddCurrentSource:
        case IGridComponent::Tool::AddGround:
        {
            IGridComponent *iGridComp = IGridComponent::createGridComponent(modelPoint, IGridComponent::currentTool);

            _pCreatingComponent = dynamic_cast<GridComponent *>(iGridComp);

            _model.appendGridComponent(_pCreatingComponent);
            reDraw();

            _model.solve(false);
            updatePropertyValues();
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
            _model.updateFloatingNodes();
            reDraw();

            _model.solve(false);
            updatePropertyValues();
        }
        if (_pCreatingComponent)
        {
            _pCreatingComponent->snapToGrid();
            if (!_pCreatingComponent->hasLength())
                _model.remove(_pCreatingComponent);
            _model.updateFloatingNodes();
            reDraw();

            _model.solve(false);
            updatePropertyValues();
        }
        if (_selectedNode != 0)
        {
            GridComponent *pC = selectedComponents[0];
            if (!pC->hasLength())
            {
                _model.remove(pC);
                _model.updateFloatingNodes();
                _pPropSwitcher->showView(0);
                reDraw();

                _model.solve(false);
                updatePropertyValues();
            }
        }
        _selectedNode = 0;
        _pCreatingComponent = nullptr;
        _lastEvent = LastEvent::None;
    }

    void onSecondaryButtonPressed(const gui::InputDevice &inputDevice) override
    {
        if (IGridComponent::currentTool != IGridComponent::Tool::Selector)
            return;

        const gui::Point &modelPoint = inputDevice.getModelPoint();

        GridComponent *pSelected = _model.getSelectedElement(modelPoint);

        if (!pSelected)
            return;

        bool isSelected = _model.selectedGridComponents.find(pSelected) != -1;

        if (!isSelected)
        {
            _model.clearSelected();
            _model.selectComponent(pSelected);
            reDraw();
        }

        td::BYTE ctxMenuID = 100;
        Frame::openContextMenu(ctxMenuID, inputDevice);
    }

    void onCursorDragged(const gui::InputDevice &inputDevice) override
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

        if (_selectedNode == 0)
        {
            for (GridComponent *pC : selectedComponents)
            {
                pC->translate(delta);
            }
        }
        else if (_selectedNode == 1)
        {
            GridComponent *pC = selectedComponents[0];
            pC->updateStartPoint(pC->getStartPoint() + delta);
        }
        else
        {
            GridComponent *pC = selectedComponents[0];
            pC->updateEndPoint(pC->getEndPoint() + delta);
        }

        updatePropertyValues();
        _lastMouseClickPoint = modelPoint;
        reDraw();
    }

    bool onZoom(const gui::InputDevice &inputDevice) override
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
        cnt::PushBackVector<GridComponent *> selectedComponents = _model.selectedGridComponents;

        switch (key.getChar())
        {
        case ' ':
        {
            IGridComponent::currentTool = IGridComponent::Tool::Selector;
            _symbolsPopover->setCurrentSelection(0);
            updateCursor();
        }
        break;
        case 'w':
        case 'W':
        {
            IGridComponent::currentTool = IGridComponent::Tool::AddWire;
            _symbolsPopover->setCurrentSelection(1);
            updateCursor();
        }
        break;
        case 'r':
        case 'R':
        {
            IGridComponent::currentTool = IGridComponent::Tool::AddResistor;
            _symbolsPopover->setCurrentSelection(2);
            updateCursor();
        }
        break;
        case 'c':
        case 'C':
        {
            IGridComponent::currentTool = IGridComponent::Tool::AddCapacitor;
            _symbolsPopover->setCurrentSelection(3);
            updateCursor();
        }
        break;
        case 'l':
        case 'L':
        {
            IGridComponent::currentTool = IGridComponent::Tool::AddInductor;
            _symbolsPopover->setCurrentSelection(4);
            updateCursor();
        }
        break;
        case 'v':
        {
            IGridComponent::currentTool = IGridComponent::Tool::AddDCVoltageSource;
            _symbolsPopover->setCurrentSelection(5);
            updateCursor();
        }
        break;
        case 'V':
        {
            IGridComponent::currentTool = IGridComponent::Tool::AddACVoltageSource;
            _symbolsPopover->setCurrentSelection(6);
            updateCursor();
        }
        break;
        case 'i':
        case 'I':
        {
            IGridComponent::currentTool = IGridComponent::Tool::AddCurrentSource;
            _symbolsPopover->setCurrentSelection(7);
            updateCursor();
        }
        break;
        case 'g':
        case 'G':
        {
            IGridComponent::currentTool = IGridComponent::Tool::AddGround;
            _symbolsPopover->setCurrentSelection(8);
            updateCursor();
        }
        break;
        case 's':
        case 'S':
        {
            reDraw();
            _model.solve(true);
            updatePropertyValues();
        }
        break;
        }

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
                _model.updateFloatingNodes();
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
                _model.updateFloatingNodes();
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
                _model.updateFloatingNodes();
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
                _model.updateFloatingNodes();
                updatePropertyValues();
                reDraw();
            }
            break;
            case gui::Key::Virtual::Delete:
            case gui::Key::Virtual::NumDelete:
            case gui::Key::Virtual::Esc:
            {
                showYesNoQuestionAsync(&_callBackDeleteSelectedShape, tr("DelShape"), tr("SureDelShape"), tr("Yes"), tr("No"));
            }
            default:
                break;
            }
        }
        return true;
    }

public:
    EditorView()
        : gui::Canvas({gui::InputDevice::Event::CursorShape, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks, gui::InputDevice::Event::CursorDrag, gui::InputDevice::Event::Zoom, gui::InputDevice::Event::Keyboard}), _callBackDeleteSelectedShape(std::bind(&EditorView::checkDeleteSelectedAnswer, this, std::placeholders::_1))
    {
    }

    void setSymbolsPopover(SymbolsPopover *popover)
    {
        _symbolsPopover = popover;
    }

    const GridModel *getModel() const
    {
        return &_model;
    }

    void exportModel(const td::String &fileName)
    {
        if (_model.exportModel(fileName))
        {
            mu::dbgLog("Model exported to file = %s", fileName.c_str());
        }
    }

    void save(const td::String &fileName)
    {
        if (_model.save(fileName))
        {
            mu::dbgLog("Circuit saved to file = %s", fileName.c_str());
        }
    }

    
    void load(const td::String& fileName)
    {
        if (_pPropSwitcher)
        {
            _pPropSwitcher->showView(0);
        }
        if (_model.load(fileName))
        {
            mu::dbgLog("Circuit opened from file = %s", fileName.c_str());
        }
        scale(1.0);
        reDraw();
    }

    void setPropSwitcher(gui::PropertyEditorSwitcher *pPropSwitcher)
    {
        _pPropSwitcher = pPropSwitcher;
    }

    void updateCursor() const
    {
        if (IGridComponent::currentTool == IGridComponent::Tool::Selector)
        {
            setCursor(gui::Cursor::Type::Default, true);
        }
        else
        {
            setCursor(gui::Cursor::Type::Adder, true);
        }
    }
};
