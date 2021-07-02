/***************************************************************
 * ウィンドウに表示させるためのファイル
 * メインからレンダラー、ゲームの構造体を取ってくる
 * 
*****************************************************************/

#include "../../game.h"

//必要な関数
void WindowEnd(void);                 //ウィンドウの終了処理
void RenderCopy(RENDERDATA data);     //レンダラーに貼り付ける
SDL_Point CenterPoint(SDL_Rect rect); //中心点を取る
void RenderFillCopy(RENDERDATA data); //四角形を表示する

//取ってくる関数

//必要なグローバル変数
SDL_Texture *texture[TEXTURES];
//取ってくるグローバル変数
extern SDL_Renderer *renderer; //レンダラー
static int a;
/****************************************************************
 * 関数名       BackGround
 * 機能         背景を一面に貼る
 * 引数         なし
 * 戻り値       なし
*****************************************************************/
void BackGround(void)
{
    //アイコン作成用
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    // SDL_RenderClear(renderer);

    //実際
    SDL_RenderCopyEx(renderer, texture[5], &(SDL_Rect){0, 0, 2050, 2050}, &(SDL_Rect){-110, -555, 2110, 2110}, 0, NULL, SDL_FLIP_NONE);
    //SDL_RenderCopyEx(renderer, texture[6], &(SDL_Rect){405, 422, 1222, 1222}, &(SDL_Rect){339, -111, 1222, 1222}, a++, NULL, SDL_FLIP_NONE);
    //SDL_RenderCopyEx(renderer, texture[7], &(SDL_Rect){661, 672, 715, 715}, &(SDL_Rect){588, 142, 716, 716}, (360 - a), NULL, SDL_FLIP_NONE);
}

/****************************************************************
 * 関数名       void WindowEnd(void)
 * 機能         ウィンドウの終了処理
 * 引数         なし
 * 戻り値       なし
*****************************************************************/
void WindowEnd(void)
{

    for (int i = 0; i < TEXTURES; i++)
        SDL_DestroyTexture(texture[i]);

    printf("Finish Window\n");
}

/****************************************************************
 * 関数名       void RenderCopy(RENDERDATA data)
 * 機能         レンダラーに貼り付ける
 * 引数         RENDERDATA data
 * 戻り値       なし
*****************************************************************/
void RenderCopy(RENDERDATA data)
{
    //HP,SPバーのとき
    if (data.type >= 201 && data.type <= 205)
    {
        RenderFillCopy(data);
        return;
    }
    else if (data.type >= TEXTURES || data.type < 0)
    {
        //printf("type error\n");
        //printf("type = %d\n", data.type);
        return;
    }
    //貼り付け
    SDL_RenderCopyEx(renderer, texture[data.type], &data.src, &data.dst, data.angle, NULL, SDL_FLIP_NONE);
}

/****************************************************************
 * 関数名       RenderFillCopy
 * 機能         四角形を表示する
 * 引数         RENDERDATA 
 * 戻り値       なし
*****************************************************************/
void RenderFillCopy(RENDERDATA data)
{
    switch (data.type % 100)
    {
    case 1: //緑
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        break;
    case 2: //黃
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        break;
    case 3: //赤
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        break;
    case 4: //オレンジ　SP用
        SDL_SetRenderDrawColor(renderer, 255, 120, 0, 255);
        break;
    case 5: //黒　目隠し用
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        break;
    }
    SDL_RenderFillRect(renderer, &data.dst);
}
