#pragma once

#include "command_base.h"
#include <string>

namespace amg
{
    class CommandLabel final : public CommandBase
    {
    public:
        CommandLabel(unsigned int line, const std::vector<std::string>& script);
        CommandLabel(const CommandLabel&) = default;
        CommandLabel(CommandLabel&&) noexcept = default;

        virtual ~CommandLabel() = default;

        CommandLabel& operator=(const CommandLabel& right) = default;
        CommandLabel& operator=(CommandLabel&& right) noexcept = default;

        bool Check() override;

        inline std::string GetLabel() const { return script[1]; }
    };
}
