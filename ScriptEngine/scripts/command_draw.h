//!
//! @file command_draw.h
//!
//! @brief 'd' スクリプトを処理するクラス定義
//!
#pragma once

#include "command_base.h"
#include <string>

namespace amg
{
    class CommandDraw final : public CommandBase
    {
    public:
        CommandDraw(unsigned int line, const std::vector<std::string>& script);
        CommandDraw(const CommandDraw&) = default;
        CommandDraw(CommandDraw&&) noexcept = default;

        virtual ~CommandDraw() = default;

        CommandDraw& operator=(const CommandDraw& right) = default;
        CommandDraw& operator=(CommandDraw&& right) noexcept = default;

        bool Check() override;

        inline std::string GetLabel() const { return script[4]; }

        inline int GetIndex() const { return index; }
        inline int GetX() const { return x; }
        inline int GetY() const { return y; }
        inline int GetHandle() const { return handle; }
        inline void SetHandle(const int handle) { this->handle = handle; }

    private:
        int index;
        int x;
        int y;
        int handle;
    };
}
