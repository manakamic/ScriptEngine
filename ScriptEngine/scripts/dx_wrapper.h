//!
//! @file dx_wrapper.h
//!
//! @brief DX ライブラリをラップするクラス
//!
//! @details
//! DX ライブラリはヘッダーファイル内の文字コードは Shitf_JIS です。
//! しかし本プロジェクトでは UTF-8 の文字コードなので
//! DxLib.h を include したファイルで日本語のコメントを書き込むと
//! VisualStudio が誤動作を起こします。
//! よって DxWrapper に使用する DX ライブラリ関数を集約して
//! dx_wrapper.cpp 以外には DxLib.h を include しない様にします。
//! (及び dx_wrapper.cpp では日本語コメントを使用しない)
//!
#pragma once

#include <tchar.h>

struct tagRECT;

namespace amg
{
    class DxWrapper
    {
    private:
        DxWrapper() = default;
        DxWrapper(const DxWrapper&) = default;
        DxWrapper(DxWrapper&&) noexcept = default;

        virtual ~DxWrapper() = default;

        DxWrapper& operator=(const DxWrapper& right) = default;
        DxWrapper& operator=(DxWrapper&& right) noexcept = default;

    public:
        static constexpr int TRUE = 1;
        static constexpr int FALSE = 0;

        static constexpr int KEY_INPUT_ESCAPE = 0x01;

        static constexpr int MOUSE_INPUT_LEFT = 0x0001;
        static constexpr int MOUSE_INPUT_RIGHT = 0x0002;

        static constexpr int DX_SCREEN_BACK = 0xfffffffe;

        static constexpr int DX_BLENDMODE_NOBLEND = 0;
        static constexpr int DX_BLENDMODE_ALPHA = 1;

        static int SetMainWindowText(const TCHAR* window_text);
        static int ChangeWindowMode(int flag);
        static int SetGraphMode(int screen_size_x, int screen_size_y, int color_bit_depth, int refresh_rate = 60);

        static int DxLib_Init();
        static int DxLib_End();

        static int ProcessMessage();

        static int SetDrawScreen(int draw_screen);
        static int ClearDrawScreen(const tagRECT* clear_rect = nullptr);
        static int ScreenFlip();

        static int GetColor(int red, int green, int blue);

        static int SetFontSize(int font_size);

        static int GetScreenState(int* size_x, int* size_y, int* color_bit_depth);

        static int SetMouseDispFlag(int disp_flag);
        static int GetMousePoint(int* x_buf, int* y_buf);

        static int CheckHitKey(int key_code);
        static int GetMouseInput();

        static int LoadGraph(const TCHAR* file_name, int not_use_3d_flag = FALSE);

        static int SetDrawArea(int x1, int y1, int x2, int y2);
        static int SetDrawBlendMode(int blend_mode, int blend_param);

        static int DrawBox(int x1, int y1, int x2, int y2, unsigned int color, int fill_flag);
        static int DrawString(int x, int y, const TCHAR* string, unsigned int color, unsigned int edge_color = 0U);
        static int DrawGraph(int x, int y, int gr_handle, int trans_flag);
    };
}
