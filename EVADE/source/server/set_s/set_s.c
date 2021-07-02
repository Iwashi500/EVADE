/***************************************************************
 * ウィンドウに表示させるためのファイル
 * メインからレンダラー、ゲームの構造体を取ってくる
 * 
*****************************************************************/

#include "../../game.h"

//必要な関数
RENDERDATA SetCharaImage(int type);                                    //キャラの画像の準備
BULLET SetBullet(int type);                                            //弾幕の準備
SHOT SetShotImage(int type, FLOATPOINT emit, double angle, int color); //弾の画像の準備
void SetTexture(void);                                                 //テクスチャの準備
void SetBackGround(void);                                              //背景関係の準備
void SetPleyerNumber(void);                                            //プレイヤーのナンバーを決める
void RandomSetPleyer(void);                                            //ランダムにプレイヤー番号を決定

//取ってくる関数

//必要なグローバル変数
int mainOne = 0; //メイン1
int mainTwo = 1; //メイン2
int subOne = 3;  //サポート1
int subTwo = 2;  //サポート2

//取ってくるグローバル変数
extern GAME game;                  //ゲームの変数
extern int pleyer1;                //メインプレイヤー１
extern int pleyer2;                //メインプレイヤー2
extern RENDERDATA BF[8];           //バレットフレーム構造体
extern RENDERDATA BF_b;            //バレットフレームの後ろ
extern RENDERDATA Num_i[8];        //番号
extern RENDERDATA BG_choice;       //背景
extern RENDERDATA FC[12];          //フレーム
extern RENDERDATA JS[4];           //カーソル
extern RENDERDATA HP;              //HPバー構造体
extern RENDERDATA HP_b1;           //HPbox1
extern RENDERDATA HP_b1s;          //HPbox1sub
extern RENDERDATA HP_b2;           //HPbox2
extern RENDERDATA HP_b2s;          //HPbox2sub
extern RENDERDATA SP_b1;           //SPbox1
extern RENDERDATA SP_b2;           //SPbox2
extern RENDERDATA area;            //回復エリアサークル
extern RENDERDATA cursor;          //弾幕位置の魔法陣
extern RENDERDATA blind[4];        //目隠し用の黒
extern RENDERDATA MainIcon[10];    //メインが使う弾幕のアイコン
extern RENDERDATA SupportIcon[12]; //サポートが使うアイコン
extern RENDERDATA trap;            //感知式の地雷
extern RENDERDATA bom;             //時限爆弾
extern int MainIconSend[10];       //MainIconを送る相手　-1ならどちらも、もしくは送らない
extern int SupportIconSend[12];    //SupportIconを送る相手　-1ならどちらも、もしくは送らない
extern RENDERDATA word;            //文字
extern int TouchIcon[4];           //送る文字の種類
extern RENDERDATA wait;            //please waitの表示用
extern RENDERDATA title;           //タイトル
extern RENDERDATA you[4];          //youの文字用
extern RENDERDATA team[2];         //teamの文字
extern RENDERDATA TeamColor[4];    //teamのカラーの文字
extern RENDERDATA VS;              //VSの文字
extern RENDERDATA turn[4];         //ピック画面用
extern RENDERDATA count;           //カウントダウン

/****************************************************************
 * 関数名   RENDERDATA SetCharaImage(int type)
 * 機能     キャラの画像の準備をする
 * 引数     int type
 *          準備する画像の種類を決める
 * 戻り値   RENDERDATA
 *          セットしたものを返す
*****************************************************************/
RENDERDATA SetCharaImage(int type)
{
    RENDERDATA rd;
    rd.type = type; //種類を入れる

    switch (type)
    {
    case 0: //アリス
        rd.src.x = rd.src.y = 0;
        rd.src.w = 500;
        rd.src.h = 500;
        rd.dst.x = WINDOW_WIDE * 1 / 4;
        rd.dst.y = WINDOW_HIGH / 2;
        rd.dst.w = 100;
        rd.dst.h = 100;
        break;
    case 1: //星
        rd.src.x = rd.src.y = 0;
        rd.src.w = 500;
        rd.src.h = 500;
        rd.dst.x = WINDOW_WIDE * 3 / 4;
        rd.dst.y = WINDOW_HIGH / 2;
        rd.dst.w = 100;
        rd.dst.h = 100;
        break;

    case 2: //
        rd.type = 14;
        rd.src.x = rd.src.y = 0;
        rd.src.w = rd.src.h = 400;
        rd.dst.x = 200;
        rd.dst.y = 400;
        rd.dst.w = 200;
        rd.dst.h = 200;
        break;
    case 3:
        rd.type = 107;
        rd.src.x = rd.src.y = 0;
        rd.src.w = rd.src.h = 400;
        rd.dst.x = 1200;
        rd.dst.y = 400;
        rd.dst.w = 200;
        rd.dst.h = 200;
        break;
    }

    return rd;
}

/****************************************************************
 * 関数名   RENDERDATA SetShotImage(int type, RENDERDATA chara)
 * 機能     弾の準備をする
 * 引数1     int type
 *          準備する弾の種類を決める
 * 引数2      FLOATPOINT
 *              弾の発生場所
 * 引数3      double
 *              弾幕の角度
 * 引数4      int
 *              作る弾の色
 *              0:赤
 *              1:青
 * 戻り値   SHOT
 *          セットしたものを返す
*****************************************************************/
SHOT SetShotImage(int type, FLOATPOINT emit, double angle, int color)
{
    SHOT shot;
    shot.count = 0;
    bool color_chang = false; //色変えフラグ、大玉のときはtrueにする
    int big = 10;
    int mid = 5;
    int smoll = 2;
    //int i = 0;

    switch (type)
    {
    case 0: //試作用　大玉をまっすぐ飛ばすだけ
        shot.RenderData.type = 17;
        shot.RenderData.src = (SDL_Rect){0, 0, 100, 100};
        color_chang = true;
        shot.RenderData.dst.w = 100;
        shot.RenderData.dst.h = 100;

        // shot.RenderData.type = 4;
        // shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        // shot.RenderData.dst.w = 16;
        // shot.RenderData.dst.h = 16;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x;
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y;
        shot.RenderData.angle = angle;

        shot.speed = 2;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 0;
        shot.pow = 10;
        break;

    case 1: //スピードがランダム
        shot.RenderData.type = 4;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 17;
        shot.RenderData.dst.h = 17;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 0.8 + rand() % 100 / 20;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 0;
        shot.pow = 1;
        break;

    case 2: //サイン波弾
        shot.RenderData.type = 17;
        shot.RenderData.src = (SDL_Rect){0, 0, 100, 100};
        color_chang = true;
        shot.RenderData.dst.w = 100;
        shot.RenderData.dst.h = 100;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x;
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y;
        shot.RenderData.angle = angle - 57;

        shot.speed = 4;
        shot.accel = 0.00;
        shot.Aspeed = 0.0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 1;
        shot.pow = 1;
        break;

    case 3: //パーフェクトフリーズ用
        shot.RenderData.type = 17;
        shot.RenderData.src = (SDL_Rect){0, 0, 100, 100};
        color_chang = true;
        shot.RenderData.dst.w = 30;
        shot.RenderData.dst.h = 30;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 1.5 + (float)(rand() % 100) / 70;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 2;
        shot.pow = mid;
        break;

    case 4: //停止　試験用
        shot.RenderData.type = 4;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 22;
        shot.RenderData.dst.h = 22;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 0;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 3;
        shot.pow = 1;
        break;

    case 5: //停止　弾幕側で止める
        shot.RenderData.type = 4;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 20;
        shot.RenderData.dst.h = 20;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 0;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = -1;
        shot.still = 750;
        shot.pow = big;
        break;

    case 6: //ゆっくり加速　上限付き
        shot.RenderData.type = 4;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 15;
        shot.RenderData.dst.h = 15;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 1.2;
        shot.accel = -0.0135;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 4;
        shot.pow = 1;
        break;

    case 7: //弾から弾をだす練習
        shot.RenderData.type = 17;
        shot.RenderData.src = (SDL_Rect){0, 0, 100, 100};
        color_chang = true;
        shot.RenderData.dst.w = 100;
        shot.RenderData.dst.h = 100;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 2.35;
        shot.accel = -0.0105;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = -1;
        shot.pow = big;
        break;

    case 8: //試作用　座布団をまっすぐ加速して飛ばす(上限付き)
        shot.RenderData.type = 4;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 20;
        shot.RenderData.dst.h = 20;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 0;
        shot.accel = 0.005;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 5;
        shot.pow = big;
        break;

    case 9: //レーザー試験用 へにょり化
        shot.RenderData.type = 17;
        shot.RenderData.src = (SDL_Rect){0, 49, 100, 1};
        color_chang = true;
        shot.RenderData.dst.w = 25;
        shot.RenderData.dst.h = 13;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle - 57;

        shot.speed = 5;
        shot.accel = 0.0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 6;
        shot.pow = 1;
        break;

    case 10: //レーザー試験用　先頭
        shot.RenderData.type = 17;
        shot.RenderData.src = (SDL_Rect){0, 0, 100, 45};
        color_chang = true;
        shot.RenderData.dst.w = 25;
        shot.RenderData.dst.h = 13;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle - 57;

        shot.speed = 5;
        shot.accel = 0.0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 6;
        shot.pow = 1;
        break;

    case 11: //レーザー試験用 最後尾
        shot.RenderData.type = 17;
        shot.RenderData.src = (SDL_Rect){0, 55, 100, 45};
        color_chang = true;
        shot.RenderData.dst.w = 25;
        shot.RenderData.dst.h = 13;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle - 57;

        shot.speed = 5;
        shot.accel = 0.0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 6;
        shot.pow = 1;
        break;

    case 12: //羽レーザー用
        shot.RenderData.type = 17;
        shot.RenderData.src = (SDL_Rect){0, 49, 100, 1};
        color_chang = true;
        shot.RenderData.dst.w = 25;
        shot.RenderData.dst.h = 30;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 0;
        shot.accel = 0.0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 7;
        shot.still = 200;
        shot.pow = smoll;
        break;

    case 13: //羽レーザー用　先頭用
        shot.RenderData.type = 17;
        shot.RenderData.src = (SDL_Rect){0, 0, 100, 45};
        color_chang = true;
        shot.RenderData.dst.w = 25;
        shot.RenderData.dst.h = 30;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 0;
        shot.accel = 0.0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 7;
        shot.still = 200;
        shot.pow = smoll;
        break;

    case 14: //羽レーザー用　最後尾用
        shot.RenderData.type = 17;
        shot.RenderData.src = (SDL_Rect){0, 55, 100, 45};
        color_chang = true;
        shot.RenderData.dst.w = 25;
        shot.RenderData.dst.h = 30;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 0;
        shot.accel = 0.0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 7;
        shot.still = 200;
        shot.pow = smoll;
        break;

    case 15: //マーキュリーポイズン
        shot.RenderData.type = 4;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 30;
        shot.RenderData.dst.h = 30;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 1;
        shot.accel = 0.005;
        shot.Aspeed = 0.5;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 9;
        shot.still = 400;
        shot.pow = 1;
        break;

    case 16: //破裂弾
        shot.RenderData.type = 19;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 25;
        shot.RenderData.dst.h = 25;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 0;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 11;
        shot.still = 1000;
        shot.pow = big;
        break;

    case 17: //今更ノーマル座布団弾
        shot.RenderData.type = 4;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 25;
        shot.RenderData.dst.h = 25;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 3;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 0;
        shot.still = 0;
        shot.pow = mid;
        break;

    case 18: //扇用かな？　氷弾まっすぐ
        shot.RenderData.type = 18;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 20;
        shot.RenderData.dst.h = 20;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 2;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 0;
        shot.still = 0;
        shot.pow = mid;
        break;

    case 19: //花火用　小粒
        shot.RenderData.type = 2;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 18;
        shot.RenderData.dst.h = 18;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 2;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 0;
        shot.still = 0;
        shot.pow = big;
        break;

    case 20: //氷弾渦巻き すわこ弾
        shot.RenderData.type = 18;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 23;
        shot.RenderData.dst.h = 23;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 2; //1.52;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 19;
        shot.still = 1000;
        shot.pow = mid;
        break;

    case 21:
        shot.RenderData.type = 2;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 40;
        shot.RenderData.dst.h = 40;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 3;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 147;
        shot.still = 0;
        shot.pow = big;
        break;

    case 22: //氷弾渦巻き
        shot.RenderData.type = 18;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 20;
        shot.RenderData.dst.h = 25;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        float speed = rand() % 10000;
        shot.speed = (speed / 1000) + 5.0;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 0;
        shot.still = 0;
        shot.pow = smoll;
        break;

    case 23: //砲台
        shot.RenderData.type = 2;
        shot.RenderData.src = (SDL_Rect){16, 0, 16, 16};
        shot.RenderData.dst.w = 20;
        shot.RenderData.dst.h = 20;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 3;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 0;
        shot.still = 0;
        shot.pow = smoll;
        break;

    case 24: //小弾幕
        shot.RenderData.type = 17;
        shot.RenderData.src = (SDL_Rect){0, 0, 100, 100};
        shot.RenderData.dst.w = 20;
        shot.RenderData.dst.h = 20;
        shot.RenderData.dst.x = shot.floatpoint.x = emit.x + cursor.dst.w / 2 - shot.RenderData.dst.w / 2; //カーソルの真ん中から出るようにする
        shot.RenderData.dst.y = shot.floatpoint.y = emit.y + cursor.dst.h / 2 - shot.RenderData.dst.h / 2; //
        shot.RenderData.angle = angle;

        shot.speed = 3;
        shot.accel = 0;
        shot.Aspeed = 0;
        shot.Aaccel = 0;
        shot.stts = USE;
        shot.motion = 0;
        shot.still = 0;
        shot.pow = smoll;
        color_chang = true;
        break;
    }
    if (color_chang)
        shot.RenderData.type -= 14 * color; //青玉から赤玉にする
    else
        shot.RenderData.src.x += shot.RenderData.src.w * 5 * color; //赤色から青色にする

    //サポートの攻撃力上昇
    if (support[5].flag && support[5].count <= 300) //フラグがONなら
    {
        int sup_id = game.chara[support[5].id].id_f; //サポート対象のID
        int shot_id = color ? pleyer2 : pleyer1;
        if (sup_id == shot_id || support[5].id == shot_id)
            shot.pow += 5; //攻撃力上昇
    }

    return shot;
}

/****************************************************************
 * 関数名      BULLET SetBullet(void)
 * 機能       弾幕の準備を行う
 * 引数       なし
 * 戻り値      BULLET
*****************************************************************/
BULLET SetBullet(int type)
{
    BULLET bullet;
    int dai = MAX_SP * 0.4;  //40%
    int tyu = MAX_SP * 0.3;  //30%
    int syou = MAX_SP * 0.2; //20%
    //実験用弾幕のタイプ/中：12,19,20,22///大：6,10,15,18,//必殺：16///////////////////////////////////////////////////////////////////////////////////////////////////
    switch (type)
    {
    case 0: //羽レーザー
        bullet.cost = syou;
        bullet.type = 12;
        break;
    case 1: //扇
        bullet.cost = tyu;
        bullet.type = 19;
        break;
    case 2: //花火
        bullet.cost = tyu;
        bullet.type = 20;
        break;
    case 3: //反射弾
        bullet.cost = tyu;
        bullet.type = 22;
        break;
    case 4: //ばらまき？
        bullet.cost = syou;
        bullet.type = 23;
        break;
    case 5: //PF
        bullet.cost = tyu;
        bullet.type = 6;
        break;
    case 6: //五芒星
        bullet.cost = dai;
        bullet.type = 10;
        break;
    case 7: //破裂弾
        bullet.cost = tyu;
        bullet.type = 15;
        break;
    case 8: //軌道弾
        bullet.cost = dai;
        bullet.type = 18;
        break;
    case 9: //すわこ弾
        bullet.cost = dai;
        bullet.type = 21;
        break;
    case 10: //必殺技　フィットフルナイトメア
        bullet.cost = 100;
        bullet.type = 16;
        break;
    }
    return bullet;
}

/****************************************************************
 * 関数名      SetBackGround
 * 機能       背景関係の準備をする
 * 引数       なし
 * 戻り値      なし
*****************************************************************/
void SetBackGround(void)
{
    //画像貼り付け準備(バレットフレームの後ろ)
    BF_b.type = 9;
    BF_b.angle = 0;
    BF_b.src.x = BF_b.src.y = 0;
    BF_b.src.w = BF_b.src.h = 200;

    //画像貼り付け準備(バレットフレーム)
    for (int i = 0; i < 16; i++)
    {
        BF[i].type = 11;
        BF[i].angle = 0;
        BF[i].src.x = BF[i].src.y = 0;
        BF[i].src.w = BF[i].src.h = 100;

        BF[i].dst.w = 120;
        BF[i].dst.h = 120;
        if (i < 8)
        {
            BF[i].dst.x = 50 + 120 * (((i % 4) + 1) / 2);
            BF[i].dst.y = 710 - 120 * (((i % 4) + 1) / 2) + 240 * ((i % 4) / 2);
        }
        else
        {
            BF[i].dst.x = 1440 + ((((i % 4)) + 1) / 2) * 120;
            BF[i].dst.y = 710 - ((((i % 4)) + 1) / 2) * 120 + (((i % 4)) / 2) * 240;
        }
    }

    //弾幕フレーム大
    BF_b.type = 13;
    BF_b.dst.x = 110;
    BF_b.dst.y = 650;
    BF_b.dst.w = 240;
    BF_b.dst.h = 240;

    //チョイス背景
    BG_choice.type = 8;
    BG_choice.src.x = 40;
    BG_choice.src.y = 200;
    BG_choice.dst.x = BG_choice.dst.y = 0;
    BG_choice.dst.w = BG_choice.src.w = 1850;
    BG_choice.dst.h = BG_choice.src.h = 1000;

    //フレーム
    for (int i = 0; i < 12; i++)
    {
        FC[i].type = 10;
        FC[i].src.x = FC[i].src.y = 0;
        FC[i].dst.w = FC[i].src.w = 170;
        FC[i].dst.h = FC[i].src.h = 170;
    }

    //画像貼り付け準備(カーソル)
    for (int i = 0; i < 4; i++)
    {
        JS[i].type = 14;
        JS[i].src.x = JS[i].src.y = 0;
        JS[i].src.w = JS[i].src.h = 400;
        JS[i].dst.w = JS[i].dst.h = 50;
    }

    //画像貼り付け準備(HPバー)
    HP.type = 15;
    HP.src.x = 4;
    HP.src.y = HP.dst.x = HP.dst.y = 0;
    HP.src.w = HP.dst.w = 1850;
    HP.src.h = 186;
    HP.dst.h = 140;

    //図形描画準備(HP初期設定)
    //Player1
    HP_b1.type = 201; //緑
    HP_b1.dst.x = 40;
    HP_b1.dst.y = 25;
    HP_b1.dst.w = 760;
    HP_b1.dst.h = 50;

    HP_b1s.type = 201; //緑
    HP_b1s.dst.x = 40;
    HP_b1s.dst.y = 75;
    HP_b1s.dst.w = 220;
    HP_b1s.dst.h = 50;

    //Player2
    HP_b2.type = 201; //緑
    HP_b2.dst.x = 1050;
    HP_b2.dst.y = 25;
    HP_b2.dst.w = 760;
    HP_b2.dst.h = 50;

    HP_b2s.type = 201; //緑
    HP_b2s.dst.x = 1590;
    HP_b2s.dst.y = 75;
    HP_b2s.dst.w = 220;
    HP_b2s.dst.h = 50;

    //図形描画準備(SP初期設定)
    //Player1
    SP_b1.type = 204; //オレンジ
    SP_b1.dst.x = 270;
    SP_b1.dst.y = 75;
    SP_b1.dst.w = 530;
    SP_b1.dst.h = 50;

    //Player2
    SP_b2.type = 204; //オレンジ
    SP_b2.dst.x = 1050;
    SP_b2.dst.y = 75;
    SP_b2.dst.w = 530;
    SP_b2.dst.h = 50;

    //画像貼り付け準備(番号)
    for (int i = 0; i < 8; i++)
    {
        Num_i[i].type = 16;
        Num_i[i].src.w = 109;
        Num_i[i].src.h = 109;
        Num_i[i].dst.w = 40;
        Num_i[i].dst.h = 40;
        if (i < 4)
        {
            Num_i[i].dst.x = BF[i].dst.x;
            Num_i[i].dst.y = BF[i].dst.y;
        }
        else
        {
            Num_i[i].dst.x = BF[i + 4].dst.x;
            Num_i[i].dst.y = BF[i + 4].dst.y;
        }
        if (i < 4)
            Num_i[i].src.x = 176 * (i % 3);
        else
            Num_i[i].src.x = 176 * ((i - 4) % 3);
        if (i == 3 || i == 7)
            Num_i[i].src.y = 139;
        else
            Num_i[i].src.y = 0;
        // Num_i[i].dst.w = 40;
        // Num_i[i].dst.h = 40;
    }

    //魔法陣？準備
    cursor.type = 108;
    cursor.src = JS[0].src;
    cursor.angle = 0;
    cursor.dst.w = 100;
    cursor.dst.h = 100;

    //回復エリアサークルの準備
    area.type = 20;
    area.src = (SDL_Rect){0, 0, 256, 256};
    area.dst = (SDL_Rect){0, 0, 300, 300};
    area.angle = 0;

    //目隠し用の黒色四角
    for (int i = 0; i < 4; i++)
    {
        blind[i].type = 205; //黒色
    }

    //メインが使う弾幕のアイコンの準備
    for (int i = 0; i < 10; i++)
    {
        MainIcon[i].type = 92 + (i / 5);
        MainIcon[i].src = (SDL_Rect){(i % 5) * FONT_WIDE * 2, 0, FONT_WIDE * 2, FONT_HIGH};
        MainIcon[i].dst = (SDL_Rect){435 + 220 * (i % 5),
                                     75 + 270 * (i / 5),
                                     110,
                                     110};
        //送る相手を決める
        MainIconSend[i] = -1;
    }
    //サポートが使うアイコンの準備
    for (int i = 0; i < 12; i++)
    {
        int number[12] = {0, 1, 4, 5, 8, 9, 2, 3, 6, 7, 10, 11};
        SupportIcon[i].type = 97 + (number[i] / 4);
        SupportIcon[i].src = (SDL_Rect){(number[i] % 4) * FONT_WIDE * 1.95 - 3, 0, FONT_WIDE * 2, FONT_HIGH};
        SupportIcon[i].dst = (SDL_Rect){185 + 220 * (((i % 6) + 1) / 2) + 360 * ((i % 6) / 2),
                                        75 + 270 * (i / 6),
                                        110,
                                        110};
        // if (number[i] < 4)
        //     SupportIcon[i].src.x = 0;
        //送る相手を決める
        SupportIconSend[i] = -1;
    }

    //文字の準備
    word.type = 21;
    word.angle = 0;
    //1文字　30*40, 30*50？
    word.src = (SDL_Rect){0, 0, FONT_WIDE * 11, FONT_HIGH};
    word.dst = word.src;

    //送る文字の種類
    for (int i = 0; i < 4; i++)
        TouchIcon[i] = -1; //未設定

    //please waitなどに使う
    wait.type = 87;
    wait.src = (SDL_Rect){FONT_WIDE2 * 31, 0, FONT_WIDE2 * 33, FONT_HIGH2};
    wait.dst = (SDL_Rect){400, 700, 30 * 19 * 2, 50 * 2};
    wait.angle = 0;

    //感知式の地雷
    trap.type = 96;
    trap.src = (SDL_Rect){0, 0, 1251, 1197};
    trap.angle = 0;

    //時限式の地雷
    bom.type = 96;
    bom.src = (SDL_Rect){0, 0, 1251, 1197};
    bom.angle = 0;

    //タイトルの文字
    title.type = 101;
    title.src = (SDL_Rect){0, 0, 850, 300};
    title.dst = (SDL_Rect){500, 300, 850, 300};
    title.angle = 0;

    //youの文字
    for (int i = 0; i < 4; i++)
    {
        you[i].type = 102;
        you[i].src = (SDL_Rect){0, 0, FONT_WIDE * 4, FONT_HIGH};
        you[i].angle = 0;
        you[i].dst = game.chara[i].RenderData.dst; //キャラの座標
        you[i].dst.y -= 50;
        you[i].dst.w = FONT_WIDE * 4;
        you[i].dst.h = FONT_HIGH * 1;
    }

    //Main Supportの文字
    for (int i = 0; i < 2; i++)
    {
        team[i].type = 103;
        team[i].angle = 0;
        team[i].src = (SDL_Rect){FONT_WIDE2 * i * 9, 0, FONT_WIDE2 * (9 + i * 7), FONT_HIGH2};
        team[i].dst = (SDL_Rect){1530 - i * 1500, 800, FONT_WIDE2 * (13 + i * 5), FONT_HIGH2 * 1.5};
    }
    //チームのカラーの文字
    //赤チーム
    TeamColor[pleyer1].type = 104;
    TeamColor[pleyer1].src = (SDL_Rect){0, 0, FONT_WIDE2 * 7, FONT_HIGH2};
    TeamColor[pleyer1].dst = (SDL_Rect){1530, 700, FONT_WIDE2 * 8.5 * 1.5, FONT_HIGH2 * 1.5};
    TeamColor[game.chara[pleyer1].id_f].type = 104;
    TeamColor[game.chara[pleyer1].id_f].src = (SDL_Rect){0, 0, FONT_WIDE2 * 7, FONT_HIGH2};
    TeamColor[game.chara[pleyer1].id_f].dst = (SDL_Rect){50, 700, FONT_WIDE2 * 8.5 * 1.5, FONT_HIGH2 * 1.5};
    //青チーム
    TeamColor[pleyer2].type = 105;
    TeamColor[pleyer2].src = (SDL_Rect){0, 0, FONT_WIDE2 * 11, FONT_HIGH2};
    TeamColor[pleyer2].dst = (SDL_Rect){1530, 700, FONT_WIDE2 * 8.5 * 1.5, FONT_HIGH2 * 1.5};
    TeamColor[game.chara[pleyer2].id_f].type = 105;
    TeamColor[game.chara[pleyer2].id_f].src = (SDL_Rect){0, 0, FONT_WIDE2 * 11, FONT_HIGH2};
    TeamColor[game.chara[pleyer2].id_f].dst = (SDL_Rect){50, 700, FONT_WIDE2 * 8.5 * 1.5, FONT_HIGH2 * 1.5};

    //VSの文字
    VS.type = 106;
    VS.src = (SDL_Rect){0, 0, FONT_WIDE * 2, FONT_HIGH};
    VS.dst = (SDL_Rect){WINDOW_WIDE / 2 - FONT_WIDE * 2, 20, FONT_WIDE * 4, FONT_HIGH * 2};
    VS.angle = 0;

    //ピック画面のターン表示 enemy turn
    for (int i = 0; i < 4; i++)
    {
        turn[i].type = 7;
        turn[i].angle = 0;
        turn[i].src = (SDL_Rect){FONT_WIDE2 * 19, 0, FONT_WIDE2 * 21, FONT_HIGH2};
        turn[i].dst = (SDL_Rect){600, 500, 30 * 11 * 2, 40 * 2};
    }
}

/****************************************************************
 * 関数名      SetPleyerNumber
 * 機能       プレイヤーのクライアント番号を決定する　余裕があれば乱数でバラけさせる
 * 引数       なし
 * 戻り値      なし
*****************************************************************/
void SetPleyerNumber(void)
{
    //ランダム試験
    RandomSetPleyer();
    //ここを乱数で代入すればできる
    // mainOne = 0; //メイン1
    // mainTwo = 2; //メイン2
    // subOne = 3;  //サポート1
    // subTwo = 1;  //サポート2

    //プレイヤー決定
    pleyer1 = mainOne;
    pleyer2 = mainTwo;

    //赤チーム
    game.chara[mainOne].id_m = mainOne;
    game.chara[mainOne].id_f = subOne;
    game.chara[mainOne].id_e = mainTwo;
    game.chara[mainOne].main = true;
    game.chara[subOne].id_m = subOne;
    game.chara[subOne].id_f = mainOne;
    game.chara[subOne].id_e = mainTwo;
    game.chara[subOne].main = false;
    //青チーム
    game.chara[mainTwo].id_m = mainTwo;
    game.chara[mainTwo].id_f = subTwo;
    game.chara[mainTwo].id_e = mainOne;
    game.chara[mainTwo].main = true;
    game.chara[subTwo].id_m = subTwo;
    game.chara[subTwo].id_f = mainTwo;
    game.chara[subTwo].id_e = mainOne;
    game.chara[subTwo].main = false;

    //カウントダウン用
    count.type = 6;
    count.angle = 0;
    count.src = (SDL_Rect){0, 0, FONT_WIDE, FONT_HIGH};
    count.dst = (SDL_Rect){850, 400, 150, 150};
}

/****************************************************************
 * 関数名      RandomSetPleyer
 * 機能       プレイヤーのクライアント番号をランダムに決定する
 * 引数       なし
 * 戻り値      なし
*****************************************************************/
void RandomSetPleyer(void)
{
    int set[4] = {0, 1, 2, 3};
    int random = rand() % 4;
    int taiki;
    for (int i = 3; i > 0; i--)
    {
        taiki = set[random];
        set[random] = set[i];
        set[i] = taiki;
        random = rand() % i;
    }
    mainOne = set[0];
    mainTwo = set[1];
    subOne = set[2];
    subTwo = set[3];

    printf("mainOne = %d\n", mainOne);
    printf("mainTwo = %d\n", mainTwo);
    printf("subOne = %d\n", subOne);
    printf("subTwo = %d\n", subTwo);
}