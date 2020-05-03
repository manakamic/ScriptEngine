#include "command_image.h"
#include "DxLib.h"

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

        handle = LoadGraph(script[2].c_str());

        if (handle == -1) {
            return false;
        }

        return true;
    }
}
