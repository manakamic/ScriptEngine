#include "dx_wrapper.h"
#include "script_engine.h"
#include <windows.h>
#ifdef _DEBUG
#include <crtdbg.h>
#endif

namespace {
    constexpr auto SCREEN_WIDTH = 1280;
    constexpr auto SCREEN_HEIGHT = 720;
    constexpr auto SCREEN_DEPTH = 32;
    constexpr auto SCRIPTS_JSON_PATH = "escape_from_amg.json";
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    auto window_mode = FALSE;

#ifdef _DEBUG
    window_mode = TRUE;

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    amg::DxWrapper::ChangeWindowMode(window_mode);

    amg::DxWrapper::SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);

    if (amg::DxWrapper::DxLib_Init() == -1) { // ＤＸライブラリ初期化処理
        return -1; // エラーが起きたら直ちに終了
    }

    amg::ScriptEngine script_engine;

    if (!script_engine.Initialize(SCRIPTS_JSON_PATH)) {
        return -1;
    }

    amg::DxWrapper::SetDrawScreen(amg::DxWrapper::DX_SCREEN_BACK);

    while ((amg::DxWrapper::ProcessMessage() != -1) && !script_engine.IsExit()) {
        script_engine.Update();

        amg::DxWrapper::ClearDrawScreen();
        script_engine.Render();
        amg::DxWrapper::ScreenFlip();
    }

    script_engine.Destroy();

    amg::DxWrapper::DxLib_End();

    return 0;
}
