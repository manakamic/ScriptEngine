//!
//! @file command_choice.h
//!
//! @brief 'c' スクリプトを処理するクラス定義
//!
#pragma once

#include "command_base.h"
#include "amg_rect.h"
#include <string>

namespace amg
{
    class CommandChoice final : public CommandBase
    {
    public:
        CommandChoice(unsigned int line, const std::vector<std::string>& script);
        CommandChoice(const CommandChoice&) = default;
        CommandChoice(CommandChoice&&) noexcept = default;

        virtual ~CommandChoice() = default;

        CommandChoice& operator=(const CommandChoice& right) = default;
        CommandChoice& operator=(CommandChoice&& right) noexcept = default;

        bool Check() override;

        inline void Initialize(Rect&& area, const int line) {
            this->area = area; this->line = line;
        }

        inline std::string GetLabel() const { return script[1]; }
        inline std::string GetMessage() const { return script[2]; }
        inline const Rect& GetArea() const { return area; }
        inline unsigned int GetLineNumber() const { return line; }

        inline unsigned int GetColor() const { return color; }
        inline void SetColor(const unsigned int color) { this->color = color; }

        inline bool IsCursorOver() const { return is_cursor_over; }
        inline void SetCursorOver(const bool cursor_over) { is_cursor_over = cursor_over; }

    private:
        Rect area;
        unsigned int line;
        unsigned int color;
        bool is_cursor_over;
    };
}
