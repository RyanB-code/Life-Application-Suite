#ifndef IMGUI_MODS_H
#define IMGUI_MODS_H

#include "imgui.h"

namespace ImGuiMods{
    void HelpMarker(const char* desc);
    void Header(const char* title, const char* text=nullptr);


    // Resizes child to parent window
    bool BeginResizeableChild(const char* title, const float widthMin, const float widthMax, const float height=200);

    void CenterText(const char* text);

    // Keeps the child window in view if the parent window collapses further than the child window's width
    void CenterChildWindow(const float parentWindowWidth, const float childWindowWidth);
}

#endif