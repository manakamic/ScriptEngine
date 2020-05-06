//!
//! @file input_manager.h
//!
//! @brief DX ライブラリの入力処理をスクリプトエンジン用に処理する定義
//!
#pragma once

namespace amg
{
    class InputManager
    {
    public:
        enum class KeyConfig
        {
            DECIDE,
            CANCEL,
            EXIT
        };

        InputManager() = default;
        InputManager(const InputManager&) = default;
        InputManager(InputManager&&) noexcept = default;

        virtual ~InputManager() = default;

        InputManager& operator=(const InputManager& right) = default;
        InputManager& operator=(InputManager&& right) noexcept = default;

        void Update();
        bool IsClick() const;
        bool IsExit() const;

    private:
        struct InputState
        {
            int fresh;
            int last;

            InputState()
            {
                fresh = 0;
                last = 0;
            }
        };

        bool IsKey(const KeyConfig key_name) const;
        bool IsKeyDown(const KeyConfig key_name) const;

        InputState input_key;
        InputState input_mouse;
    };
}
