#pragma once

#include "command_base.h"
#include "amg_rect.h"
#include <string>

namespace amg
{
    class CommandMessage final : public CommandBase
    {
    public:
        CommandMessage(unsigned int line, const std::vector<std::string>& script);
        CommandMessage(const CommandMessage&) = default;
        CommandMessage(CommandMessage&&) noexcept = default;

        virtual ~CommandMessage() = default;

        CommandMessage& operator=(const CommandMessage& right) = default;
        CommandMessage& operator=(CommandMessage&& right) noexcept = default;

        bool Check() override;

        inline void Initialize(Rect&& area, const int goal) {
            this->area = area; right_goal = goal;
        }

        inline std::string GetMessage() const { return script[1]; }
        inline const Rect& GetArea() const { return area; }
        inline int GetRightGoal() const { return right_goal; }

        inline void UpdateAreaRight(const int right) { area.right = right; }

    private:
        Rect area;
        int right_goal;
    };
}
