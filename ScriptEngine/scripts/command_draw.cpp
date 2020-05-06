//!
//! @file command_draw.cpp
//!
//! @brief 'd' スクリプトを処理するクラス実装
//!
#include "command_draw.h"
#include "amg_string.h"

namespace {
    constexpr size_t SCRIPT_NUM = 5;
}

namespace amg
{
    CommandDraw::CommandDraw(unsigned int line, const std::vector<std::string>& script)
        : CommandBase(line, script)
    {
        index = 0;
        x = 0;
        y = 0;
        handle = 0;
    }

    bool CommandDraw::Check()
    {
        const auto size = script.size();

        if (size != SCRIPT_NUM) {
            return false;
        }

        auto value = 0;

        if (!string::ToInt(script[1], value)) {
            return false;
        }

        index = value;

        if (!string::ToInt(script[2], value)) {
            return false;
        }

        x = value;

        if (!string::ToInt(script[3], value)) {
            return false;
        }

        y = value;

        return true;
    }
}
