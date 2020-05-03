#include "input_manager.h"
#include "DxLib.h"

namespace {
    constexpr unsigned int dx_mouse_config_num = static_cast<unsigned int>(amg::InputManager::KeyConfig::EXIT);
    constexpr int dx_mouse_config[dx_mouse_config_num] = {
        MOUSE_INPUT_LEFT, MOUSE_INPUT_RIGHT
    };

    int GetDxMouseConfig(const amg::InputManager::KeyConfig key_name)
    {
        return dx_mouse_config[static_cast<unsigned int>(key_name)];
    }
}

namespace amg
{
    void InputManager::Update()
    {
        input_key.last = input_key.fresh;
        input_key.fresh = CheckHitKey(KEY_INPUT_ESCAPE);

        input_mouse.last = input_mouse.fresh;
        input_mouse.fresh = GetMouseInput();
    }

    bool InputManager::IsClick() const
    {
        return IsKeyDown(KeyConfig::DECIDE);
    }

    bool InputManager::IsExit() const
    {
        return IsKeyDown(KeyConfig::EXIT);
    }

    bool InputManager::IsKey(const KeyConfig key_name) const
    {
        if (key_name == KeyConfig::EXIT) {
            return (input_key.fresh == 1);
        }
        else {
            const auto config = GetDxMouseConfig(key_name);

            return ((input_mouse.fresh & config) != 0);
        }
    }

    bool InputManager::IsKeyDown(const KeyConfig key_name) const
    {
        if (key_name == KeyConfig::EXIT) {
            return (input_key.last == 0) && (input_key.fresh == 1);
        }
        else {
            const auto config = GetDxMouseConfig(key_name);

            return ((input_mouse.last & config) == 0) && ((input_mouse.fresh & config) != 0);
        }
    }
}
