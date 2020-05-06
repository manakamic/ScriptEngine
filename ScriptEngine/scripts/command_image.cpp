//!
//! @file command_image.cpp
//!
//! @brief 'i' スクリプトを処理するクラス実装
//!
#include "dx_wrapper.h"
#include "command_image.h"

namespace {
    constexpr size_t SCRIPT_NUM = 3;
}

namespace amg
{
    CommandImage::CommandImage(unsigned int line, const std::vector<std::string>& script)
        : CommandBase(line, script)
    {
        handle = -1;
    }

    bool CommandImage::Check()
    {
        const auto size = script.size();

        if (size != SCRIPT_NUM) {
            return false;
        }

        handle = DxWrapper::LoadGraph(script[2].c_str());

        if (handle == -1) {
            return false;
        }

        return true;
    }
}
