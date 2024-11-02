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

bool ImGuiArithmetics::operator==(const ImVec2& a, const ImVec2& b)
{
    return a.x == b.x && a.y == b.y;
}

bool ImGuiArithmetics::operator!=(const ImVec2& a, const ImVec2& b)
{
    return !(a == b);
}

ImVec2 ImGuiArithmetics::operator/(const ImVec2& a, float f)
{
    return ImVec2(a.x / f, a.y / f);
}
