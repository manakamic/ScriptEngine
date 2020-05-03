#pragma once

namespace amg
{
    class Rect
    {
    public:
        Rect() {
            left = 0; top = 0; right = 0; bottom = 0;
        }
        Rect(const int left, const int top, const int right, const int bottom) {
            this->left = left; this->top = top; this->right = right; this->bottom = bottom;
        }
        Rect(const Rect&) = default;
        Rect(Rect&&) noexcept = default;

        virtual ~Rect() = default;

        Rect& operator=(const Rect& right) = default;
        Rect& operator=(Rect&& right) noexcept = default;

        inline void Set(const int left, const int top, const int right, const int bottom) {
            this->left = left; this->top = top; this->right = right; this->bottom = bottom;
        }

        inline bool IsCollision(const int x, const int y) const {
            return (left <= x) && (x <= right) && (top <= y) && (y <= bottom);
        }

        int left;
        int top;
        int right;
        int bottom;
    };
}
