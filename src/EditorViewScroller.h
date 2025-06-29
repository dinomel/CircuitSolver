#pragma once
#include <gui/ViewScroller.h>
#include "EditorView.h"

class EditorViewScroller : public gui::ViewScroller
{
private:
protected:
    EditorView _contentView;

public:
    EditorViewScroller()
        : gui::ViewScroller(ViewScroller::Type::ScrollAndAutoHide, ViewScroller::Type::ScrollAndAutoHide)
    {
        setContentView(&_contentView);
    }

    EditorView *getEditor()
    {
        return &_contentView;
    }
};
