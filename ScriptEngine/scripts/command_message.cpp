#include "command_message.h"

namespace {
    constexpr size_t SCRIPT_NUM = 2;
}

namespace amg
{
    CommandMessage::CommandMessage(unsigned int line, const std::vector<std::string>& script)
        : CommandBase(line, script) {
        right_goal = 0;
    }

    bool CommandMessage::Check()
    {
        const auto size = script.size();

        if (size != SCRIPT_NUM) {
            return false;
        }

        return true;
    }
}
