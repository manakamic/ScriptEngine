//!
//! @file command_choice.cpp
//!
//! @brief 'c' スクリプトを処理するクラス実装
//!
#include "command_choice.h"

namespace {
    constexpr size_t SCRIPT_NUM = 3;
}

namespace amg
{
    CommandChoice::CommandChoice(unsigned int line, const std::vector<std::string>& script)
        : CommandBase(line, script) {
        this->line = 0; color = 0; is_cursor_over = false;
    }

    bool CommandChoice::Check()
    {
        const auto size = script.size();

        if (size != SCRIPT_NUM) {
            return false;
        }

        return true;
    }
}
