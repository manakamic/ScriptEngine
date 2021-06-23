//!
//! @file script_engine.cpp
//!
//! @brief スクリプトエンジンの実装
//!
//! @details スクリプトの解説
//!
//! コマンド: @
//! 構文: "@"
//! コマンド単体で使用します。
//! クリック待ち状態になります。
//!
//! コマンド: m [message]
//! 構文: "m, メッセージ"
//! メッセージを 1 行描画します。
//! メッセージは最大 3 行です。
//! (コマンドを続けて指定出来ます)
//! クリックや選択肢の決定で全ての c コマンドと m コマンドがクリアされます。
//! 4 つ目以降の m コマンドは、先頭の m コマンドを上書きします。
//!
//! コマンド: w [wait]
//! 構文: "w, 数値"
//! 数値分のフレーム数を待ちます。
//!
//! コマンド: j [jump]
//! 構文: "j, ラベル"
//! l コマンドが設定された行へスクリプト処理を移動させます。
//! l コマンドで指定されていないラベルはエラーとなります。
//!
//! コマンド: l [label]
//! 構文: "l, ラベル"
//! j コマンドでスクリプトの処理を移動させる為のラベルを指定します。
//! j コマンドで使用されなければ何も行わない行となります。
//!
//! コマンド: c [choise]
//! 構文: "c, ラベル, 選択肢メッセージ"
//! 選択肢メッセージを 1 行描画します。
//! 選択肢メッセージは最大 3 行です。
//! (コマンドを続けて指定出来ます)
//! 選択肢の決定で全ての c コマンドと m コマンドがクリアされます。
//! 4 つ目以降の c コマンドは、先頭の c コマンドを上書きします。
//! 選択肢の決定後は
//!
//! コマンド: i [image]
//! 構文: "i, 画像ラベル, パス付の画像ファイル名"
//! 画像を DX ライブラリのロード関数で処理します。
//! ロード後の画像ハンドル値は画像ラベルで取得します。
//!
//! コマンド: d [draw]
//! 構文: "d, 描画インデックス, 描画 X 座標, 描画 Y 座標, 画像ラベル"
//! i コマンドで処理された画像を描画します。(画像ラベルで指定)
//! 描画インデックスで指定された順番で表示します。
//! d コマンドは重ねて描画を行います。(基本的に表示数の制限はありません)
//! d コマンドの取り消しは、同じ描画インデックスの d コマンドで
//! 処理を上書きをして行います。
//! スクリーンの XY 座標を指定できます。スクリーンの左上が X:0 Y:0 となり
//! X は右方向、Y は下方向に増加します。
//!
#include "dx_wrapper.h"
#include "script_engine.h"
#include "scripts_data.h"
#include "input_manager.h"
#include "command_label.h"
#include "command_image.h"
#include "command_choice.h"
#include "command_message.h"
#include "command_draw.h"
#include "amg_string.h"
#include <algorithm>

namespace {
    // スクリプト コマンド
    constexpr auto COMMAND_A = '@';
    constexpr auto COMMAND_M = 'm';
    constexpr auto COMMAND_W = 'w';
    constexpr auto COMMAND_J = 'j';
    constexpr auto COMMAND_L = 'l';
    constexpr auto COMMAND_C = 'c';
    constexpr auto COMMAND_I = 'i';
    constexpr auto COMMAND_D = 'd';
    constexpr auto COMMAND_E = 'e';

    // マウスカーソル画像とクリック待ち画像を特定するラベル名
    constexpr auto CURSOR_IMAGE_LABEL = "カーソル";
    constexpr auto CLICK_WAIT_IMAGE_LABEL = "クリック待ち";

    constexpr auto FONT_SIZE = 24;

    constexpr auto MSG_WORD_MAX = 42;
    constexpr auto MSG_STRING_MAX = MSG_WORD_MAX * 2; // 2 : MultiByte String

    constexpr auto MSG_LINE_MAX = 3;
    constexpr auto MSG_LINE_WIDTH = MSG_WORD_MAX * FONT_SIZE;
    constexpr auto MSG_LINE_HEIGHT = 24;
    constexpr auto MSG_LINE_GAP_HEIGHT = 16;
    constexpr auto MSG_LINE_GRID_HEIGHT = MSG_LINE_HEIGHT + MSG_LINE_GAP_HEIGHT;

    constexpr auto MSG_WINDOW_WIDTH = MSG_LINE_WIDTH;
    constexpr auto MSG_WINDOW_HEIGHT = MSG_LINE_GRID_HEIGHT * MSG_LINE_MAX - MSG_LINE_GAP_HEIGHT;
    constexpr auto MSG_WINDOW_CENTER_Y = 600;
    constexpr auto MSG_WINDOW_TOP = MSG_WINDOW_CENTER_Y - MSG_WINDOW_HEIGHT / 2;
    constexpr auto MSG_WINDOW_BOTTOM = MSG_WINDOW_TOP + MSG_WINDOW_HEIGHT;

    constexpr auto CLICK_WAIT_IMAGE_OFFSET_Y = 28;

    constexpr auto CHOICE_WORD_MAX = 24;

    constexpr auto CHOICE_LINE_MAX = 3;
    constexpr auto CHOICE_LINE_WIDTH = CHOICE_WORD_MAX * FONT_SIZE;
    constexpr auto CHOICE_LINE_HEIGHT = 24;
    constexpr auto CHOICE_LINE_GAP_HEIGHT = 16;
    constexpr auto CHOICE_LINE_GRID_HEIGHT = CHOICE_LINE_HEIGHT + CHOICE_LINE_GAP_HEIGHT;

    constexpr auto CHOICE_WINDOW_WIDTH = CHOICE_LINE_WIDTH;
    constexpr auto CHOICE_WINDOW_HEIGHT = CHOICE_LINE_GRID_HEIGHT * CHOICE_LINE_MAX - CHOICE_LINE_GAP_HEIGHT;
    constexpr auto CHOICE_WINDOW_CENTER_Y = 360;
    constexpr auto CHOICE_WINDOW_TOP = CHOICE_WINDOW_CENTER_Y - CHOICE_WINDOW_HEIGHT / 2;

    // 一度計算したら固定値な物
    int screen_width = 0;
    int screen_height = 0;
    int screen_center_x = 0;

    int message_window_left = 0;
    int message_window_right = 0;

    int click_wait_x = 0;
    int click_wait_y = 0;

    int choice_window_left = 0;
    int choice_window_right = 0;

    unsigned int message_window_color = 0;
    unsigned int message_string_color = 0;

    unsigned int choice_normal_color = 0;
    unsigned int choice_select_color = 0;

#ifdef _DEBUG
    unsigned int message_area_color = 0;
#endif
}

namespace amg
{
    ScriptEngine::ScriptEngine()
    {
        input_manager = nullptr;
        scripts_data = nullptr;
        state = ScriptState::PARSING;
        max_line = 0;
        now_line = 0;
        wait_count = 0;
        cursor_x = 0;
        cursor_y = 0;
        cursor_image_handle = -1;
        click_wait_image_handle = -1;
        is_click_wait_visible = false;
        is_message_output = false;
    }

    ScriptEngine::~ScriptEngine()
    {
        Destroy();
    }

    //!
    //! @fn bool ScriptEngine::Initialize(const TCHAR* path)
    //! @brief スクリプトエンジンの初期化
    //! @param[in] path パス付のスクリプト用 Json ファイル名
    //! @return 処理の成否
    //! @details スクリプトの事前の処理と
    //! DX ライブラリの設定などを行い
    //! スクリプトエンジンが動作する様にします。
    //!
    bool ScriptEngine::Initialize(const TCHAR* path)
    {
        if (path == nullptr || input_manager != nullptr || scripts_data != nullptr) {
            return false;
        }

        input_manager.reset(new InputManager());
        scripts_data.reset(new ScriptsData());

        if (!scripts_data->LoadJson(path)) {
            return false;
        }

        max_line = scripts_data->GetScriptNum();

        if (max_line <= 0) {
            return false;
        }

        PreParsing();

        if (!InitializeCursor()) {
            return false;
        }

        if (!InitializeClickWait()) {
            return false;
        }

        if (!InitializeStrings()) {
            return false;
        }

        return true;
    }

    //!
    //! @fn bool ScriptEngine::IsExit() const
    //! @brief DX ライブラリの キーチェックで ESC キーを判定
    //! @return ESC キーが押されたか
    //!
    bool ScriptEngine::IsExit() const
    {
        if (input_manager == nullptr) {
            return false;
        }

        return input_manager->IsExit();
    }

    //!
    //! @fn bool ScriptEngine::InitializeCursor()
    //! @brief スクリプトエンジン用マウスカーソル画像の初期化
    //! @return 処理の成否
    //!
    bool ScriptEngine::InitializeCursor()
    {
        auto handle = 0;

        if (!GetImageHandle(CURSOR_IMAGE_LABEL, handle)) {
            return false;
        }

        cursor_image_handle = handle;

        DxWrapper::SetMouseDispFlag(DxWrapper::FALSE);

        return true;
    }

    //!
    //! @fn bool ScriptEngine::InitializeClickWait()
    //! @brief スクリプトエンジン用クリック待ち画像の初期化
    //! @return 処理の成否
    //!
    bool ScriptEngine::InitializeClickWait()
    {
        auto handle = 0;

        if (!GetImageHandle(CLICK_WAIT_IMAGE_LABEL, handle)) {
            return false;
        }

        click_wait_image_handle = handle;

        return true;
    }

    //!
    //! @fn bool ScriptEngine::InitializeStrings()
    //! @brief スクリプトエンジン用文字列描画の初期化
    //! @return 処理の成否
    //!
    bool ScriptEngine::InitializeStrings()
    {
        DxWrapper::SetFontSize(FONT_SIZE);

        auto screen_depth = 0;

        if (DxWrapper::GetScreenState(&screen_width, &screen_height, &screen_depth) != 0) {
            return false;
        }

        screen_center_x = screen_width / 2;

        message_window_left = screen_center_x - MSG_WINDOW_WIDTH / 2;
        message_window_right = message_window_left + MSG_WINDOW_WIDTH;

        click_wait_x = message_window_right;
        click_wait_y = MSG_WINDOW_BOTTOM - CLICK_WAIT_IMAGE_OFFSET_Y;

        choice_window_left = screen_center_x - CHOICE_WINDOW_WIDTH / 2;
        choice_window_right = choice_window_left + CHOICE_WINDOW_WIDTH;

        message_window_color = DxWrapper::GetColor(128, 128, 255);
        message_string_color = DxWrapper::GetColor(255, 255, 255);

        choice_normal_color = DxWrapper::GetColor(64, 64, 255);
        choice_select_color = DxWrapper::GetColor(128, 128, 255);

#ifdef _DEBUG
        message_area_color = DxWrapper::GetColor(255, 0, 0);
#endif

        return true;
    }

    //!
    //! @fn void ScriptEngine::Destroy()
    //! @brief 明示的なスクリプトエンジンの終了処理
    //! @details 無理に呼び出す必要はありませんが
    //! インスタンスを再利用したい場合などに呼び出します。
    //!
    void ScriptEngine::Destroy()
    {
        input_manager.reset();
        input_manager = nullptr;

        scripts_data.reset();
        scripts_data = nullptr;

        state = ScriptState::PARSING;
        max_line = 0;
        now_line = 0;
        wait_count = 0;
        cursor_x = 0;
        cursor_y = 0;
        cursor_image_handle = -1;
        click_wait_image_handle = -1;
        is_click_wait_visible = false;
        is_message_output = false;

        image_list.clear();
        label_list.clear();
        choice_list.clear();
        message_list.clear();
        draw_list.clear();
    }

    //!
    //! @fn void ScriptEngine::Update()
    //! @brief スクリプトエンジンの更新処理
    //! @details 毎フレーム呼び出す必要があります。
    //!
    void ScriptEngine::Update()
    {
        input_manager->Update();

        DxWrapper::GetMousePoint(&(cursor_x), &(cursor_y));

        auto is_update_message = false;

        switch (state) {
        case ScriptState::PARSING:
            Parsing();
            break;

        case ScriptState::TIME_WAIT:
            TimeWait();
            is_update_message = true;
            break;

        case ScriptState::CLICK_WAIT:
            ClickWait();
            is_update_message = true;
            break;

        case ScriptState::CHOICE_WAIT:
            ChoiceWait();
            is_update_message = true;
            break;

        case ScriptState::END:
            break;
        }

        if (is_update_message) {
            UpdateMessage();
        }
    }

    //!
    //! @fn void ScriptEngine::PreParsing()
    //! @brief スクリプトの事前解析
    //! @details 'l' コマンド(ラベル)と 'i' コマンド(イメージ)を
    //! 予め全て処理してリスト化します。
    //!
    void ScriptEngine::PreParsing()
    {
        while (now_line >= 0 && now_line < max_line) {
            const auto script = scripts_data->GetScript(now_line);
            const auto command = (script[0])[0];

            switch (command) {
            case COMMAND_L:
                OnCommandLabel(now_line, script);
                break;

            case COMMAND_I:
                OnCommandImage(now_line, script);
                break;

            default:
                break;
            }

            ++now_line;
        }

        now_line = 0;
    }

    //!
    //! @fn void ScriptEngine::Parsing()
    //! @brief スクリプトの解析
    //! @details スクリプトを 1 行単位で処理します。
    //! (インタープリタ方式)
    //!
    void ScriptEngine::Parsing()
    {
        auto stop_parsing = false;

        while (!stop_parsing && (now_line >= 0) && (now_line < max_line)) {
            const auto script = scripts_data->GetScript(now_line);
            const auto command = (script[0])[0];

            switch (command) {
            case COMMAND_A:
                OnCommandClick();
                stop_parsing = true;
                break;

            case COMMAND_M:
                OnCommandMessage(now_line, script);
                break;

            case COMMAND_W:
                stop_parsing = OnCommandWait(script);
                break;

            case COMMAND_J:
                if (OnCommandJump(script)) {
                    continue;
                }
                break;

            case COMMAND_C:
                OnCommandChoice(now_line, script);
                break;

            case COMMAND_D:
                OnCommandDraw(now_line, script);
                break;

            case COMMAND_E:
                state = ScriptState::END;
                stop_parsing = true;
                break;

            default:
                break;
            }

            ++now_line;
        }
    }

    //!
    //! @fn void ScriptEngine::UpdateMessage()
    //! @brief 文字列を 1 文字づつ表示させる処理
    //!
    void ScriptEngine::UpdateMessage()
    {
        is_click_wait_visible = false;

        for (auto&& message : message_list) {
            const auto area = message->GetArea();
            const auto right_goal = message->GetRightGoal();

            // クリックされたら全メッセージを表示
            if (input_manager->IsClick()) {
                message->UpdateAreaRight(right_goal);
                continue;
            }

            // 右終端(全文字列)になるまで 1 文字サイズ分づつ足して行く
            if (area.right < right_goal) {
                message->UpdateAreaRight(area.right + FONT_SIZE);
                return; // 1 文字分処理したらメソッド終了
            }
        }

        // return せずに for 文が終わったなら全文字列を表示している
        is_message_output = false;

        if (state == ScriptState::CLICK_WAIT) {
            is_click_wait_visible = (click_wait_image_handle != -1);
        }
    }

    //!
    //! @fn void ScriptEngine::OnCommandClick()
    //! @brief スクリプトの '@' コマンドを処理
    //!
    void ScriptEngine::OnCommandClick()
    {
        if (choice_list.size() > 0) {
            state = ScriptState::CHOICE_WAIT;
        }
        else {
            state = ScriptState::CLICK_WAIT;
        }
    }

    //!
    //! @fn bool ScriptEngine::OnCommandWait(const std::vector<std::string>& scripts)
    //! @brief スクリプトの 'w' コマンドを処理
    //! @param[in] scripts スクリプトの内容
    //! @return 処理の成否
    //!
    bool ScriptEngine::OnCommandWait(const std::vector<std::string>& scripts)
    {
        auto wait = 0;
        auto result = false;

        if (string::ToInt(scripts[1], wait)) {
            wait_count = static_cast<unsigned int>(wait);
            state = ScriptState::TIME_WAIT;
            result = true;
        }

        return result;
    }

    //!
    //! @fn bool ScriptEngine::OnCommandJump(const std::vector<std::string>& scripts)
    //! @brief スクリプトの 'j' コマンドを処理
    //! @param[in] scripts スクリプトの内容
    //! @return 処理の成否
    //!
    bool ScriptEngine::OnCommandJump(const std::vector<std::string>& scripts)
    {
        auto line = 0U;
        const auto result = GetLineNumber(scripts[1], line);

        if (result) {
            now_line = line;
        }

        return result;
    }

    //!
    //! @fn bool ScriptEngine::OnCommandLabel(unsigned int line, const std::vector<std::string>& scripts)
    //! @brief スクリプトの 'l' コマンドを処理
    //! @param[in] line スクリプトの行数
    //! @param[in] scripts スクリプトの内容
    //! @return 処理の成否
    //!
    bool ScriptEngine::OnCommandLabel(unsigned int line, const std::vector<std::string>& scripts)
    {
        std::unique_ptr<CommandLabel> label(new CommandLabel(line, scripts));

        if (!label->Check()) {
            return false;
        }

        label_list.emplace_back(std::move(label));

        return true;
    }

    //!
    //! @fn bool ScriptEngine::OnCommandImage(unsigned int line, const std::vector<std::string>& scripts)
    //! @brief スクリプトの 'i' コマンドを処理
    //! @param[in] line スクリプトの行数
    //! @param[in] scripts スクリプトの内容
    //! @return 処理の成否
    //!
    bool ScriptEngine::OnCommandImage(unsigned int line, const std::vector<std::string>& scripts)
    {
        std::unique_ptr<CommandImage> image(new CommandImage(line, scripts));

        if (!image->Check()) {
            return false;
        }

        image_list.emplace_back(std::move(image));

        return true;
    }

    //!
    //! @fn bool ScriptEngine::OnCommandChoice(unsigned int line, const std::vector<std::string>& scripts)
    //! @brief スクリプトの 'c' コマンドを処理
    //! @param[in] line スクリプトの行数
    //! @param[in] scripts スクリプトの内容
    //! @return 処理の成否
    //!
    bool ScriptEngine::OnCommandChoice(unsigned int line, const std::vector<std::string>& scripts)
    {
        std::unique_ptr<CommandChoice> choice(new CommandChoice(line, scripts));

        if (!choice->Check()) {
            return false;
        }

        auto line_number = 0U;

        if (!GetLineNumber(choice->GetLabel(), line_number)) {
            return false;
        }

        const auto line_index = static_cast<int>(choice_list.size());
        const auto choice_top = CHOICE_WINDOW_TOP + CHOICE_LINE_GRID_HEIGHT * line_index;
        const auto choice_bottom = choice_top + CHOICE_LINE_HEIGHT;
        Rect rect(choice_window_left, choice_top, choice_window_right, choice_bottom);

        choice->Initialize(std::move(rect), line_number);

        const auto size = static_cast<int>(choice_list.size());

        // 最大チョイスライン数を超えたら先頭(インデックス 0 )を削除(上書き仕様)
        if (size > CHOICE_LINE_MAX) {
            choice_list.erase(choice_list.begin() + 0);
        }

        choice_list.emplace_back(std::move(choice));

        return true;
    }

    //!
    //! @fn bool ScriptEngine::OnCommandMessage(unsigned int line, const std::vector<std::string>& scripts)
    //! @brief スクリプトの 'm' コマンドを処理
    //! @param[in] line スクリプトの行数
    //! @param[in] scripts スクリプトの内容
    //! @return 処理の成否
    //!
    bool ScriptEngine::OnCommandMessage(unsigned int line, const std::vector<std::string>& scripts)
    {
        std::unique_ptr<CommandMessage> message(new CommandMessage(line, scripts));

        if (!message->Check()) {
            return false;
        }

        Rect rect;
        int right_goal = 0;

        if (!CalculateMessageArea(message->GetMessage(), rect, right_goal)) {
            return false;
        }

        message->Initialize(std::move(rect), right_goal);

        const auto size = static_cast<int>(message_list.size());

        // 最大メッセージライン数を超えたら先頭(インデックス 0 )を削除
        if (size > MSG_LINE_MAX) {
            message_list.erase(message_list.begin() + 0);
        }

        message_list.emplace_back(std::move(message));

        // メッセージコマンドを処理したらメッセージ表示を有効にする
        is_message_output = true;

        return true;
    }

    //!
    //! @fn bool ScriptEngine::OnCommandDraw(unsigned int line, const std::vector<std::string>& scripts)
    //! @brief スクリプトの 'd' コマンドを処理
    //! @param[in] line スクリプトの行数
    //! @param[in] scripts スクリプトの内容
    //! @return 処理の成否
    //!
    bool ScriptEngine::OnCommandDraw(unsigned int line, const std::vector<std::string>& scripts)
    {
        std::unique_ptr<CommandDraw> draw(new CommandDraw(line, scripts));

        if (!draw->Check()) {
            return false;
        }

        auto handle = 0;

        if (!GetImageHandle(draw->GetLabel(), handle)) {
            return false;
        }

        draw->SetHandle(handle);

        // 同じ Index の Draw コマンドを消す(上書き仕様)
        const auto index = draw->GetIndex();
        const auto check = [index](const auto& element) -> bool {
            return element->GetIndex() == index;
        };
        const auto remove = std::remove_if(draw_list.begin(), draw_list.end(), check);

        draw_list.erase(remove, draw_list.end());
        draw_list.emplace_back(std::move(draw));

        // 描画リストが複数あるなら Index でソートする
        if (draw_list.size() >= 2) {
            const auto sort = [](const auto& lh, const auto& rh) -> bool {
                return lh->GetIndex() < rh->GetIndex();
            };

            std::sort(draw_list.begin(), draw_list.end(), sort);
        }

        return true;
    }

    //!
    //! @fn void ScriptEngine::ClickWait()
    //! @brief クリック待ち処理
    //!
    void ScriptEngine::ClickWait()
    {
        if (is_message_output) {
            return;
        }

        if (input_manager->IsClick()) {
            state = ScriptState::PARSING;
            message_list.clear();
        }
    }

    //!
    //! @fn void ScriptEngine::ChoiceWait()
    //! @brief 選択待ち処理
    //!
    void ScriptEngine::ChoiceWait()
    {
        const auto is_click = input_manager->IsClick();

        for (auto&& choice : choice_list) {
            const auto area = choice->GetArea();
            auto cursor_over = false;
            auto color = choice_normal_color;

            // 選択エリア内にマウス座標がある
            if (area.IsCollision(cursor_x, cursor_y)) {
                // 選択エリアがクリックされていたら処理は終了
                if (is_click) {
                    state = ScriptState::PARSING;
                    // 指定の行番号にする
                    now_line = choice->GetLineNumber();
                    // 全ての文字列表示をなくす
                    message_list.clear();
                    choice_list.clear();
                    return;
                }

                cursor_over = true;
                color = choice_select_color;
            }

            choice->SetCursorOver(cursor_over);
            choice->SetColor(color);
        }
    }

    //!
    //! @fn void ScriptEngine::TimeWait()
    //! @brief 時間待ち処理
    //!
    void ScriptEngine::TimeWait()
    {
        if (is_message_output) {
            return;
        }

        if (wait_count > 0) {
            --wait_count;
        }
        else {
            state = ScriptState::PARSING;
        }
    }

    //!
    //! @fn bool ScriptEngine::CalculateMessageArea(const std::string& message, Rect& area, int& right_goal)
    //! @brief メッセージ文字列より表示エリアや右終端を計算する
    //! @param[in] message メッセージ文字列
    //! @param[out] area メッセージ表示エリア
    //! @param[out] right_goal メッセージ右終端
    //! @return 処理の成否
    //! @details メッセージの順番や文字数より表示エリアを計算します。
    //! 表示エリアの右側は、初期値は左側と同値とします。
    //! (数学的にはエリアは面積を持たない)
    //! これは左側から 1 文字づつ表示していく仕様の為です。
    //! 実際の右側の値は right_goal に格納します。
    //!
    bool ScriptEngine::CalculateMessageArea(const std::string& message, Rect& area, int& right_goal)
    {
        if (message.empty()) {
            return false;
        }

        const auto line_index = static_cast<int>(message_list.size());
        const auto message_top = MSG_WINDOW_TOP + MSG_LINE_GRID_HEIGHT * line_index;
        const auto message_bottom = message_top + MSG_LINE_HEIGHT;

        area.Set(message_window_left, message_top, message_window_left, message_bottom);

        const auto string_lenght = static_cast<int>(std::strlen(message.c_str()));

        right_goal = message_window_left + ((string_lenght + 1) * (FONT_SIZE / 2));

        return true;
    }

    //!
    //! @fn bool ScriptEngine::GetLineNumber(const std::string& str, unsigned int& line) const
    //! @brief ラベル文字列より行番号を取得
    //! @param[in] str ラベル文字列
    //! @param[out] line ラベルが設定されている行番号
    //! @return 処理の成否
    //!
    bool ScriptEngine::GetLineNumber(const std::string& str, unsigned int& line) const
    {
        for (auto&& label : label_list) {
            if (label->GetLabel() == str) {
                line = label->GetLineNumber();

                return true;
            }
        }

        return false;
    }

    //!
    //! @fn bool ScriptEngine::GetImageHandle(const std::string& str, int& handle) const
    //! @brief 画像ラベル文字列より画像ハンドルを取得
    //! @param[in] str 画像ラベル文字列
    //! @param[out] handle 画像ハンドル
    //! @return 処理の成否
    //! @details 画像ハンドルは、DX ライブラリの
    //! 画像ロード関数で得られる描画用の値です。
    //!
    bool ScriptEngine::GetImageHandle(const std::string& str, int& handle) const
    {
        for (auto&& image : image_list) {
            if (image->GetLabel() == str) {
                handle = image->GetHandle();

                return true;
            }
        }

        return false;
    }

    //!
    //! @fn void ScriptEngine::Render() const
    //! @brief スクリプトの全ての描画処理
    //! @details 毎フレーム呼び出す必要があります。
    //!
    void ScriptEngine::Render() const
    {
        RenderImage();
        RenderMessageWindow();
        RenderMessage();
        RenderChoice();
        RenderCursor();
    }

    //!
    //! @fn void ScriptEngine::RenderCursor() const
    //! @brief マウスカーソル画像の描画
    //!
    void ScriptEngine::RenderCursor() const
    {
        if (-1 == cursor_image_handle) {
            return;
        }

        DxWrapper::DrawGraph(cursor_x, cursor_y, cursor_image_handle, DxWrapper::TRUE);
    }

    //!
    //! @fn void ScriptEngine::RenderImage() const
    //! @brief 'd' コマンドによる画像描画
    //!
    void ScriptEngine::RenderImage() const
    {
        for (auto&& draw : draw_list) {
            DxWrapper::DrawGraph(draw->GetX(), draw->GetY(), draw->GetHandle(), DxWrapper::TRUE);
        }
    }

    //!
    //! @fn void ScriptEngine::RenderMessageWindow() const
    //! @brief 'm' コマンドによる文字列用のウィンドウ描画
    //!
    void ScriptEngine::RenderMessageWindow() const
    {
        DxWrapper::SetDrawBlendMode(DxWrapper::DX_BLENDMODE_ALPHA, 64);

        DxWrapper::DrawBox(message_window_left, MSG_WINDOW_TOP,
            message_window_right, MSG_WINDOW_BOTTOM,
            message_window_color, DxWrapper::TRUE);

#ifdef _DEBUG
        // デバッグ中はメッセージエリアに色を付けて確認する
        for (auto&& message : message_list) {
            const auto area = message->GetArea();

            DxWrapper::DrawBox(area.left, area.top, area.right, area.bottom, message_area_color, DxWrapper::TRUE);
        }
#endif

        DxWrapper::SetDrawBlendMode(DxWrapper::DX_BLENDMODE_NOBLEND, 0);
    }

    //!
    //! @fn void ScriptEngine::RenderMessage() const
    //! @brief 'm' コマンドによる文字列描画
    //!
    void ScriptEngine::RenderMessage() const
    {
        for (auto&& message : message_list) {
            const auto area = message->GetArea();

            // 表示エリアを制御して 1文字づつ描画する
            DxWrapper::SetDrawArea(area.left, area.top, area.right, area.bottom);
            DxWrapper::DrawString(area.left, area.top,
                message->GetMessage().c_str(), message_string_color);
        }

        // 表示エリアを全画面に戻す
        DxWrapper::SetDrawArea(0, 0, screen_width, screen_height);

        if (is_click_wait_visible) {
            DxWrapper::DrawGraph(click_wait_x, click_wait_y, click_wait_image_handle, DxWrapper::TRUE);
        }
    }

    //!
    //! @fn void ScriptEngine::RenderChoice() const
    //! @brief 'c' コマンドによる選択エリア付き文字列描画
    //!
    void ScriptEngine::RenderChoice() const
    {
        // 先に選択エリアを描画してしまう
        for (auto&& choice : choice_list) {
            const auto area = choice->GetArea();

            DxWrapper::DrawBox(area.left, area.top, area.right, area.bottom, choice->GetColor(), DxWrapper::TRUE);
        }

        // 次に選択文字列を描画する
        for (auto&& choice : choice_list) {
            const auto area = choice->GetArea();

            DxWrapper::DrawString(area.left, area.top,
                choice->GetMessage().c_str(), message_string_color);
        }
    }
}
