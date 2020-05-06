//!
//! @file command_label.cpp
//!
//! @brief 'l' スクリプトを処理するクラス実装
//!
#include "command_label.h"

namespace {
    constexpr size_t SCRIPT_NUM = 2;
}

namespace amg
{
    CommandLabel::CommandLabel(unsigned int line, const std::vector<std::string>& script)
        : CommandBase(line, script)
    {
    }

    bool CommandLabel::Check()
    {
        const auto size = script.size();

        if (size != SCRIPT_NUM) {
            return false;
        }

        return true;
    }
}