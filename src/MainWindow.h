#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include <gui/FileDialog.h>
#include "MainView.h"
#include "DialogSettings.h"
#include "ContextMenus.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    MainView _view;
    ContextMenus _contextMenus;

public:
    MainWindow()
        : gui::Window(gui::Size(1200, 800))
    {
        setTitle(tr("Circuit Solver"));
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        _toolBar.setIconSize(gui::ToolBar::IconSize::SystemDefault);

        EditorView *pEditor = _view.getEditor();
        pEditor->setSymbolsPopover(_toolBar.getSymbolsPopover());

        setToolBar(_toolBar);
        setCentralView(&_view);
        setContextMenus(&_contextMenus);
    }

    ~MainWindow()
    {
    }

protected:
    bool shouldClose() override
    {
        return true;
    }

    void onClose() override
    {
        gui::Window::onClose();
    }

    void onInitialAppearance() override // will be called only once
    {
        EditorView *pEditor = _view.getEditor();
        pEditor->setFocus();
    }

    bool onToolbarsPopoverSelectionChange(gui::PopoverView *pPOView, td::UINT2 ctrlID, td::UINT2 selection) override
    {
        IGridComponent::currentTool = (IGridComponent::Tool)selection;
        EditorView *pEditor = _view.getEditor();
        pEditor->updateCursor();
        return true;
    }

    bool onActionItem(gui::ActionItemDescriptor &aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();

        if (menuID == 10 & actionID == 10)
        {
            td::UINT4 settingsID = 728289; // bilo koji unikatan broj
            auto pDlg = getAttachedWindow(settingsID);
            if (pDlg)
                pDlg->setFocus();
            else
            {
                DialogSettings *pSettingsDlg = new DialogSettings(this, settingsID);
                pSettingsDlg->keepOnTopOfParent();
                pSettingsDlg->setMainTB(&_toolBar);
                pSettingsDlg->open();
            }
            return true;
        }

        if (menuID == 100)
        {
            // context menu;
            return true;
        }
        if (menuID == 20 && firstSubMenuID == 0 && lastSubMenuID == 0)
        {
            switch (actionID)
            {
            case 10:
            {
                td::String dlgTitle(tr("OpenF"));
                gui::OpenFileDialog::show(this, dlgTitle, {{"Circuit Solver file", "*.csol"}}, actionID, [this](gui::FileDialog *pFileDlg)
                                          {
                          auto status = pFileDlg->getStatus();
                          if (status == gui::FileDialog::Status::OK)
                          {
                              EditorView* pEditor = _view.getEditor();
                              td::String fileName = pFileDlg->getFileName();
                              pEditor->load(fileName);
                              mu::dbgLog("User pressed OK! Selected file to open =%s", fileName.c_str());
                          }
                          else
                          {
                              mu::dbgLog("User cancelled opening!");
                          } });
                return true;
            }

            case 20:
            {
                td::String dlgTitle(tr("SaveT"));
                const char *defaultFileName = "MyCircuit";

                gui::SaveFileDialog::show(this, dlgTitle, {{"Circuit Solver file", "*.csol"}}, actionID, [this](gui::FileDialog *pFileDlg)
                                          {
                          auto status = pFileDlg->getStatus();
                          if (status == gui::FileDialog::Status::OK)
                          {
                              gui::SaveFileDialog* saveFD = (gui::SaveFileDialog*) pFileDlg;
                              EditorView* pEditor = _view.getEditor();
                              td::String fileName = pFileDlg->getFileName();
                              pEditor->save(fileName);
                              
                              mu::dbgLog("User pressed OK! Selected file to save =%s", fileName.c_str());
                          }
                          else
                              mu::dbgLog("User cancelled saving!"); }, defaultFileName);
                return true;
            }

            case 30:
            {
                td::String dlgTitle(tr("Export"));
                const char *defaultFileName = "MyCustomModel";

                gui::SaveFileDialog::show(this, dlgTitle, {{tr("dTwin model"), "*.dmodl"}}, actionID, [this](gui::FileDialog *pFileDlg)
                                          {
                          auto status = pFileDlg->getStatus();
                          if (status == gui::FileDialog::Status::OK)
                          {
                              gui::SaveFileDialog* saveFD = (gui::SaveFileDialog*) pFileDlg;
                              
                              EditorView* pEditor = _view.getEditor();
                              td::String fileName = pFileDlg->getFileName();
                              pEditor->exportModel(fileName);
                              
                              mu::dbgLog("User pressed OK! Selected file to save =%s", fileName.c_str());
                          }
                          else
                              mu::dbgLog("User cancelled saving!"); }, defaultFileName);
                return true;
            }
            default:
                return false;
            }
        }
        if (menuID == 20 && firstSubMenuID == 0 && lastSubMenuID == 0)
        {
            if (actionID == 20)
            {
                return true;
            }

            if (actionID == 40)
            {
                return true;
            }

            if (actionID == 50)
            {
                return true;
            }
        }
        if (menuID == 255)
        {
            if (actionID == 10)
            {
                return true;
            }
            else if (actionID == 20)
            {
                return true;
            }
        }

        td::String msgText("Handling onActionItem MainWindow");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);

        showAlert(msgText, informativeText);
        return true;
    }
};
