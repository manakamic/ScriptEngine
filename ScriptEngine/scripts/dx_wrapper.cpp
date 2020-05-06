//!
//! @file dx_wrapper.cpp
//!
//! @brief Wrapper class for DXLibrary.
//!
#include "dx_wrapper.h"
#include "DxLib.h"

namespace amg
{
    int DxWrapper::SetMainWindowText(const TCHAR* window_text)
    {
        return DxLib::SetMainWindowText(window_text);
    }

    int DxWrapper::ChangeWindowMode(int flag)
    {
        return DxLib::ChangeWindowMode(flag);
    }

    int DxWrapper::SetGraphMode(int screen_size_x, int screen_size_y, int color_bit_depth, int refresh_rate)
    {
        return DxLib::SetGraphMode(screen_size_x, screen_size_y, color_bit_depth, refresh_rate);
    }

    int DxWrapper::DxLib_Init()
    {
        return DxLib::DxLib_Init();
    }

    int DxWrapper::DxLib_End()
    {
        return DxLib::DxLib_End();
    }

    int DxWrapper::ProcessMessage()
    {
        return DxLib::ProcessMessage();
    }

    int DxWrapper::SetDrawScreen(int draw_screen)
    {
        return DxLib::SetDrawScreen(draw_screen);
    }

    int DxWrapper::ClearDrawScreen(const tagRECT* clear_rect)
    {
        return DxLib::ClearDrawScreen(clear_rect);
    }

    int DxWrapper::ScreenFlip()
    {
        return DxLib::ScreenFlip();
    }

    int DxWrapper::GetColor(int red, int green, int blue)
    {
        return DxLib::GetColor(red, green, blue);
    }

    int DxWrapper::SetMouseDispFlag(int disp_flag)
    {
        return DxLib::SetMouseDispFlag(disp_flag);
    }

    int DxWrapper::SetFontSize(int font_size)
    {
        return DxLib::SetFontSize(font_size);
    }

    int DxWrapper::GetScreenState(int* size_x, int* size_y, int* color_bit_depth)
    {
        return DxLib::GetScreenState(size_x, size_y, color_bit_depth);
    }

    int DxWrapper::GetMousePoint(int* x_buf, int* y_buf)
    {
        return DxLib::GetMousePoint(x_buf, y_buf);
    }

    int DxWrapper::CheckHitKey(int key_code)
    {
        return DxLib::CheckHitKey(key_code);
    }

    int DxWrapper::GetMouseInput()
    {
        return DxLib::GetMouseInput();
    }

    int DxWrapper::LoadGraph(const TCHAR* file_name, int not_use_3d_flag)
    {
        return DxLib::LoadGraph(file_name, not_use_3d_flag);
    }

    int DxWrapper::DrawBox(int x1, int y1, int x2, int y2, unsigned int color, int fill_flag)
    {
        return DxLib::DrawBox(x1, y1, x2, y2, color, fill_flag);
    }

    int DxWrapper::DrawString(int x, int y, const TCHAR* string, unsigned int color, unsigned int edge_color)
    {
        return DxLib::DrawString(x, y, string, color, edge_color);
    }

    int DxWrapper::DrawGraph(int x, int y, int gr_handle, int trans_flag)
    {
        return DxLib::DrawGraph(x, y, gr_handle, trans_flag);
    }

    int DxWrapper::SetDrawArea(int x1, int y1, int x2, int y2)
    {
        return DxLib::SetDrawArea(x1, y1, x2, y2);
    }

    int DxWrapper::SetDrawBlendMode(int blend_mode, int blend_param)
    {
        return DxLib::SetDrawBlendMode(blend_mode, blend_param);
    }
}