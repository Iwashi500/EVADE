/**********************************************************************************************
 * イベント処理用の関数
 * 今はまだキーボード入力
 * 
************************************************************************************************/

#include "../../game.h"

//必要な関数
bool Event(void);      //イベント操作
bool ShiftSpeed(void); //低速移動中かどうかを決める

//取ってくる関数
//extern void SendData(void); //データを送信する
extern void SendEndCommand(void);               //ネットの終了コマンド送信
extern void SendCommand(BOTTAN bottan, int id); //ボタンのデータを送る

//必要なグローバル変数
SDL_Event event;  //イベントデータの構造体
const Uint8 *key; //なにこれ
BOTTAN bottan;    //このクライアントのボタンの状態
SDL_Joystick *joy;
bool speed; //低速移動用
//BOTTAN reset;     //すべてfalseのもの

//取ってくるグローバル変数
extern GAME game;    //ゲームの情報
extern int endflag;  //エンドフラグ
extern int clientID; //自分のクライアントID

/****************************************************************
 * 関数名   bool Event(void)
 * 機能     イベント関数でボタンが押されたかを確かめる
 * 引数     なし
 * 戻り値   escキーが押されたらSDL_FALSE、それ以外はSDL_TRUE
*****************************************************************/
bool Event(void)
{
    // if (SDL_PollEvent(&event))
    // {
    //     key = SDL_GetKeyboardState(NULL); //更新
    //     //変更の反映
    //     if (key[SDL_SCANCODE_UP])
    //         bottan.up = true;
    //     if (key[SDL_SCANCODE_DOWN])
    //         bottan.down = true;
    //     if (key[SDL_SCANCODE_LEFT])
    //         bottan.left = true;
    //     if (key[SDL_SCANCODE_RIGHT])
    //         bottan.right = true;
    //     if (key[SDL_SCANCODE_SPACE])
    //         bottan.space = true;
    //     if (key[SDL_SCANCODE_ESCAPE])
    //         bottan.escape = true;
    //     SendCommand(bottan, clientID); //送信
    //     bottan = reset;                //初期値に戻す
    // }

    SDL_JoystickUpdate(); //ボタン情報の更新
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_JOYBUTTONDOWN:
            // if (event.jbutton.button == 10)
            //     bottan.escape = true;
            if (event.jbutton.button == 5)
                speed = true;
            if (event.jbutton.button == 0)
                bottan.one = true;
            if (event.jbutton.button == 1)
                bottan.two = true;
            if (event.jbutton.button == 2)
                bottan.three = true;
            if (event.jbutton.button == 3)
                bottan.four = true;
            break;
        case SDL_JOYBUTTONUP:
            if (event.jbutton.button == 5)
                speed = false;
        }
        //低速移動判定
        bottan.six = ShiftSpeed();
        //移動
        if (SDL_JoystickGetAxis(joy, 1) > 30000)
            bottan.down = true;
        if (SDL_JoystickGetAxis(joy, 1) < -30000)
            bottan.up = true;
        if (SDL_JoystickGetAxis(joy, 0) > 30000)
            bottan.right = true;
        if (SDL_JoystickGetAxis(joy, 0) < -30000)
            bottan.left = true;
        SendCommand(bottan, clientID); //送信
        bottan = reset;                //初期値に戻す
    }
    return false;
}

/****************************************************************
 * 関数名   ShiftSpeed
 * 機能     低速移動中かどうかを判断する
 * 引数     なし
 * 戻り値   bool
 *          低速移動中ならtrue
*****************************************************************/
bool ShiftSpeed(void)
{
    if (speed)
        return true;
    else
        return false;
}