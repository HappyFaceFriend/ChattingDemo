#include "ImGuiArithmetics.h"

ImVec2 ImGuiArithmetics::operator+(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x + b.x, a.y + b.y);
}

ImVec2 ImGuiArithmetics::operator-(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x - b.x, a.y - b.y);
}

ImVec2 ImGuiArithmetics::operator*(const ImVec2& a, float f)
{
    return ImVec2(a.x * f, a.y * f);
}

ImVec2 ImGuiArithmetics::operator/(const ImVec2& a, float f)
{
    return ImVec2(a.x / f, a.y / f);
}
