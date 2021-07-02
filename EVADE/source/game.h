#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/**********************************************************************************************
 * マクロ
************************************************************************************************/
#define MAX_SHOT 2000    //一体が出す弾の最大数
#define MAX_BULLET 4     //弾幕の選ぶ数
#define MAX_SPORT 3      //サポートの選べる数
#define TEXTURES 110     //テクスチャの読み込む
#define WINDOW_WIDE 1850 //画面の最大幅
#define WINDOW_HIGH 1000 //画面の高さ
#define RENDER_NUM 1000  //RENDERDATA配列の大きさ
#define true 1
#define false 0
#define bool _Bool
#define MAX_HP 760 // プレイヤーの最大体力
#define MAX_SP 530 //プレイヤーの最大スタミナ
#define RADIAN_2_FREQUENCY(angle) ((angle)*180 / M_PI)
#define FREQUENCY_2_RADIAN(angle) ((angle)*M_PI / 180)
#define FONT_PATH "images/font/Hannari.otf"
#define FONT_WIDE 25
#define FONT_HIGH 50
#define FONT_PATH2 "images/font/GauFontLoveRocketNeo.TTF"
#define FONT_WIDE2 25
#define FONT_HIGH2 70

/**********************************************************************************************
 * 列挙体
************************************************************************************************/

typedef enum {
    UNUSED = 0, //不使用
    FINISH = 1, //使用済み
    USE = 2     //使用中
} STTS;

/**********************************************************************************************
 * 構造体
************************************************************************************************/

typedef struct
{
    float x;
    float y;
} FLOATPOINT;

typedef struct
{
    bool up;     //上ボタン
    bool down;   //下ボタン
    bool right;  //右ボタン
    bool left;   //左ボタン
    bool escape; //エスケープボタン
    bool one;    //1ボタン
    bool two;    //2ボタン
    bool three;  //3ボタン
    bool four;   //4ボタン
    bool six;    //6ボタン
    // bool lshift; //左シフト
    // bool rshift; //右シフト
    // bool space;  //スペース
} BOTTAN;

typedef struct
{
    SDL_Rect src; //切り取りサイズ
    SDL_Rect dst; //貼り付けサイズ
    double angle; //角度
    int type;     //種類
} RENDERDATA;

typedef struct
{
    RENDERDATA RenderData; //画像必要データ
    FLOATPOINT floatpoint; //細かい位置
    SDL_Rect mask;         //当たり判定
    //SDL_Rect center; //回転の中心位置　画像の真ん中固定にする予定なのでいらない
    float speed;  //速度
    float accel;  //加速度
    float Aspeed; //角速度
    float Aaccel; //角加速度
    int still;    //残る時間
    int count;    //カウント
    STTS stts;    //状態
    int motion;   //動きのタイプ
    float pow;    //弾の威力
} SHOT;

typedef struct
{
    SHOT shot[MAX_SHOT]; //弾の変数
    int type;            //弾幕の種類
    int color;           //弾の色(プレイヤー番号)　0、プレイヤー1、赤　：　1、プレイヤー2、青
    int cost;            //必要なスタミナ量
    int count;           //カウント
    STTS stts;           //状態
    double angle;        //弾幕の角度
    FLOATPOINT base;     //弾幕の再生位置
    FLOATPOINT emit;     //弾の発射位置
    int number;          //弾幕がセットされたボタンの番号
} BULLET;

typedef struct
{
    BULLET bullet[MAX_BULLET]; //弾幕データ
    RENDERDATA RenderData;     //画像の必要データ
    FLOATPOINT floatpoint;     //細かい座標
    float speed;               //速度
    int hp;                    //体力
    int sp;                    //スタミナ
    int id_m;                  //自分のクライアント番号
    int id_f;                  //自分の味方のクライアント番号
    int id_e;                  //敵(メイン)のクライアント番号
    int count;                 //カウント
    //STTS stts; //状態　いらなくね？
    BOTTAN bottan;      //キャラのボタン
    int support_num[4]; //サポートの種類
    bool main;          //メインかサポートかのフラグ　true：メイン 、false：サポート
    //BULLET smoll;       //小弾幕用
} CHARA;

//こっち使う
typedef struct
{
    CHARA chara[4]; //キャラの構造体
    int count;      //カウント
    int scene;      //現在のシーン
} GAME;

//サポート用
typedef struct
{
    int id;     //使用したプレイヤーのid
    int count;  //カウント
    bool flag;  //使用フラグ
    int number; //セットしているボタンの番号
} SUPPORT;

//必要な変数
int mode;            //ゲームのモード
BOTTAN reset;        //ボタンの初期値
SUPPORT support[12]; //サポート用の変数12個分
int winner;          //勝者の番号
int MainPickCount;   //メインがピックした数
int SubPickCount;    //サポートがピックした数
// int mainOne = 0;     //メイン1
// int mainTwo = 1;     //メイン2
// int subOne = 3;      //サポート1
// int subTwo = 2;      //サポート2
