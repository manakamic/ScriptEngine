#pragma once

#include "command_base.h"
#include <string>

namespace amg
{
    class CommandImage final : public CommandBase
    {
    public:
        CommandImage(unsigned int line, const std::vector<std::string>& script);
        CommandImage(const CommandImage&) = default;
        CommandImage(CommandImage&&) noexcept = default;

        virtual ~CommandImage() = default;

        CommandImage& operator=(CommandImage const& right) = default;
        CommandImage& operator=(CommandImage&& right) = default;

        bool Check() override;

        inline std::string GetLabel() const { return script[1]; }
        inline int GetHandle() const { return handle; }

    private:
        int handle;
    };
}
