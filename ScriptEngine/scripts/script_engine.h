#pragma once

#include "amg_rect.h"
#include <vector>
#include <string>
#include <memory>

namespace amg
{
    class InputManager;
    class ScriptsData;
    class CommandLabel;
    class CommandImage;
    class CommandChoice;
    class CommandMessage;
    class CommandDraw;

    class ScriptEngine {
    public:
        ScriptEngine();
        ScriptEngine(const ScriptEngine&) = default;
        ScriptEngine(ScriptEngine&&) noexcept = default;

        virtual ~ScriptEngine();

        ScriptEngine& operator=(const ScriptEngine& right) = default;
        ScriptEngine& operator=(ScriptEngine&& right) noexcept = default;

        bool Initialize(const char* path);
        void Destroy();

        void Update();
        void Render() const;

        bool IsExit() const;

    private:
        enum class ScriptState {
            PARSING,
            TIME_WAIT,
            CLICK_WAIT,
            CHOICE_WAIT,
            END
        };

        bool InitializeCursor();
        bool InitializeClickWait();
        bool InitializeStrings();

        void PreParsing();
        void Parsing();

        void UpdateMessage();
        bool CalculateMessageArea(const std::string& message, Rect& area, int& right_goal);

        void TimeWait();
        void ClickWait();
        void ChoiceWait();

        bool GetLineNumber(const std::string& str, unsigned int& line) const;
        bool GetImageHandle(const std::string& str, int& handle) const;

        void OnCommandClick();
        bool OnCommandWait(const std::vector<std::string>& script);
        bool OnCommandJump(const std::vector<std::string>& script);
        bool OnCommandLabel(unsigned int line, const std::vector<std::string>& scripts);
        bool OnCommandImage(unsigned int line, const std::vector<std::string>& scripts);
        bool OnCommandChoice(unsigned int line, const std::vector<std::string>& scripts);
        bool OnCommandMessage(unsigned int line, const std::vector<std::string>& scripts);
        bool OnCommandDraw(unsigned int line, const std::vector<std::string>& scripts);

        void RenderCursor() const;
        void RenderImage() const;
        void RenderMessageWindow() const;
        void RenderMessage() const;
        void RenderChoice() const;

        std::unique_ptr<InputManager> input_manager;
        std::unique_ptr<ScriptsData> scripts_data;

        std::vector<std::unique_ptr<CommandImage>> image_list;
        std::vector<std::unique_ptr<CommandLabel>> label_list;
        std::vector<std::unique_ptr<CommandChoice>> choice_list;
        std::vector<std::unique_ptr<CommandMessage>> message_list;
        std::vector<std::unique_ptr<CommandDraw>> draw_list;

        ScriptState state;

        unsigned int max_line;
        unsigned int now_line;
        unsigned int wait_count;

        int cursor_x;
        int cursor_y;

        int cursor_image_handle;
        int click_wait_image_handle;

        bool is_click_wait_visible;
        bool is_message_output;
    };
}
