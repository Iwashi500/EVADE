/**********************************************************************************************
 * サポートの行動を書く
 * 
************************************************************************************************/
#include "../../game.h"

//必要な関数
void ManageSupport(void);

//取ってくる関数
extern int HitPointRect(RENDERDATA dot, RENDERDATA rect); //点と四角形の当たり判定
extern double Angle(FLOATPOINT base, FLOATPOINT to);      //向きを変える

//必要なグローバル変数
bool AreaIn; //エリアに入っているフラグ

//取ってくるグローバル変数
extern GAME game;           //ゲームの構造体
extern RENDERDATA area;     //回復エリアサークル
extern RENDERDATA blind[4]; //目隠し用の黒
extern RENDERDATA trap;     //感知式の地雷
extern RENDERDATA bom;      //時限爆弾
extern RENDERDATA cursor;   //弾幕位置の魔法陣？
extern RENDERDATA BF[16];   //バレットフレーム
extern int pleyer1;         //メインプレイヤー１
extern int pleyer2;         //メインプレイヤー2

void ManageSupport(void)
{
    for (int i = 0; i < 12; i++)
    {
        if (support[i].flag) //サポートのフラグがONなら
        {
            int id_f = game.chara[support[i].id].id_f; //味方のID
            int id_e = game.chara[support[i].id].id_e; //敵のID
            int bfNum = (id_f == pleyer1) ? support[i].number + 8 : support[i].number + 12;
            switch (i) //毎回の処理
            {
            case 0: //地雷　感知式 CT500
                if (support[i].count == 10500)
                {
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                else if (support[i].count % 10000 >= 500)
                    support[i].count--;
                if (!support[i].count) //場所決め　色決め
                {
                    trap.dst = game.chara[support[i].id].RenderData.dst;
                    if (id_f == pleyer1)
                        trap.type = 96; //赤
                    else
                        trap.type = 100; //青
                }
                if (HitPointRect(game.chara[id_e].RenderData, trap) && support[i].count <= 10000 && support[i].count >= 100)
                {
                    game.chara[support[i].id].bullet[support[i].number].stts = USE;
                    game.chara[support[i].id].bullet[support[i].number].type = 24;
                    game.chara[support[i].id].bullet[support[i].number].count = 0;
                    game.chara[support[i].id].bullet[support[i].number].base.x = trap.dst.x + trap.dst.w / 2 - cursor.dst.w / 2;
                    game.chara[support[i].id].bullet[support[i].number].base.y = trap.dst.y + trap.dst.h / 2 - cursor.dst.h / 2;
                    game.chara[support[i].id].bullet[support[i].number].emit.x = game.chara[support[i].id].bullet[support[i].number].base.x;
                    game.chara[support[i].id].bullet[support[i].number].emit.y = game.chara[support[i].id].bullet[support[i].number].base.y;
                    if (id_f == pleyer1)
                        game.chara[support[i].id].bullet[support[i].number].color = 0;
                    else
                        game.chara[support[i].id].bullet[support[i].number].color = 1;

                    support[i].count += 10000; //地雷の終了
                    break;
                }
                break;

            case 1: //地雷　時限爆弾式 CT1000
                if (support[i].count == 1000)
                {
                    //地雷の終了
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                if (!support[i].count) //初期位置
                {
                    bom.dst = game.chara[support[i].id].RenderData.dst;
                    if (id_f == pleyer1)
                        bom.type = 96; //赤
                    else
                        bom.type = 100; //青
                }
                if (support[i].count == 500) //爆破
                {
                    game.chara[support[i].id].bullet[support[i].number].stts = USE;
                    game.chara[support[i].id].bullet[support[i].number].type = 25;
                    game.chara[support[i].id].bullet[support[i].number].count = 0;
                    game.chara[support[i].id].bullet[support[i].number].base.x = bom.dst.x + bom.dst.w / 2 - cursor.dst.w / 2;
                    game.chara[support[i].id].bullet[support[i].number].base.y = bom.dst.y + bom.dst.h / 2 - cursor.dst.h / 2;
                    game.chara[support[i].id].bullet[support[i].number].emit.x = game.chara[support[i].id].bullet[support[i].number].base.x;
                    game.chara[support[i].id].bullet[support[i].number].emit.y = game.chara[support[i].id].bullet[support[i].number].base.y;
                    if (id_f == pleyer1)
                        game.chara[support[i].id].bullet[support[i].number].color = 0;
                    else
                        game.chara[support[i].id].bullet[support[i].number].color = 1;
                }
                break;

            case 2: //固定砲台　敵機狙い CT1100 弾幕は1000で終了
                if (!support[i].count)
                {
                    game.chara[support[i].id].bullet[support[i].number].stts = USE;
                    game.chara[support[i].id].bullet[support[i].number].type = 26;
                    game.chara[support[i].id].bullet[support[i].number].count = 0;
                    game.chara[support[i].id].bullet[support[i].number].base.x = game.chara[support[i].id].RenderData.dst.x + game.chara[support[i].id].RenderData.dst.w / 2 - cursor.dst.w / 2;
                    game.chara[support[i].id].bullet[support[i].number].base.y = game.chara[support[i].id].RenderData.dst.y + game.chara[support[i].id].RenderData.dst.h / 2 - cursor.dst.h / 2;
                    game.chara[support[i].id].bullet[support[i].number].emit.x = game.chara[support[i].id].bullet[support[i].number].base.x;
                    game.chara[support[i].id].bullet[support[i].number].emit.y = game.chara[support[i].id].bullet[support[i].number].base.y;
                    if (id_f == pleyer1)
                        game.chara[support[i].id].bullet[support[i].number].color = 0;
                    else
                        game.chara[support[i].id].bullet[support[i].number].color = 1;
                }
                game.chara[support[i].id].bullet[support[i].number].angle = Angle(game.chara[support[i].id].bullet[support[i].number].base, game.chara[id_e].floatpoint); //ホーミング
                if (support[i].count == 1100)
                {
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                break;

            case 3: //固定砲台　花火　CT1300　弾幕は1000で終了
                if (!support[i].count)
                {
                    game.chara[support[i].id].bullet[support[i].number].stts = USE;
                    game.chara[support[i].id].bullet[support[i].number].type = 27;
                    game.chara[support[i].id].bullet[support[i].number].count = 0;
                    game.chara[support[i].id].bullet[support[i].number].base.x = game.chara[support[i].id].RenderData.dst.x + game.chara[support[i].id].RenderData.dst.w / 2 - cursor.dst.w / 2;
                    game.chara[support[i].id].bullet[support[i].number].base.y = game.chara[support[i].id].RenderData.dst.y + game.chara[support[i].id].RenderData.dst.h / 2 - cursor.dst.h / 2;
                    game.chara[support[i].id].bullet[support[i].number].emit.x = game.chara[support[i].id].bullet[support[i].number].base.x;
                    game.chara[support[i].id].bullet[support[i].number].emit.y = game.chara[support[i].id].bullet[support[i].number].base.y;
                    if (id_f == pleyer1)
                        game.chara[support[i].id].bullet[support[i].number].color = 0;
                    else
                        game.chara[support[i].id].bullet[support[i].number].color = 1;
                }
                if (support[i].count == 1300)
                {
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                break;

            case 4: //完　スタミナ急速回復 CT500　効果300
                if (support[i].count <= 300)
                {
                    if (game.chara[id_f].sp < MAX_SP && !(support[i].count % 2))
                        game.chara[id_f].sp++; //SP回復
                }
                if (support[i].count == 600)
                {
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                break;

            case 5: //完　味方の弾強化　CT500　効果300
                if (support[i].count == 500)
                {
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                break;

            case 6:                    //完　サポートエリア出現　CT1000　効果600
                if (!support[i].count) //位置決め
                {
                    area.dst.x = game.chara[support[i].id].RenderData.dst.x - 50;
                    area.dst.y = game.chara[support[i].id].RenderData.dst.y - 50;
                    AreaIn = false;
                }
                else if (support[i].count == 1000) //終了
                {
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                else if (support[i].count == 600)
                    AreaIn = false;
                //エリアに入っている判定
                if (support[i].count < 600)
                {
                    if (HitPointRect(game.chara[id_f].RenderData, area))
                    {
                        AreaIn = true;
                        printf("area in\n");
                    }
                    else
                        AreaIn = false;
                }
                //HP回復
                if (AreaIn)
                {
                    if (!(game.count % 5))
                    {
                        game.chara[id_f].hp++; //体力回復
                        if (game.chara[id_f].hp > MAX_HP)
                            game.chara[id_f].hp = MAX_HP;
                    }
                }
                break;

            case 7:                                     //必殺技付与
                if (game.chara[id_f].hp > MAX_HP * 0.5) //20%以上残っている
                {
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                if (!support[i].count)
                {
                    game.chara[support[i].id].bullet[support[i].number].stts = USE;
                    game.chara[support[i].id].bullet[support[i].number].type = 16;
                    game.chara[support[i].id].bullet[support[i].number].count = 0;
                    if (id_f == pleyer1)
                        game.chara[support[i].id].bullet[support[i].number].color = 0;
                    else
                        game.chara[support[i].id].bullet[support[i].number].color = 1;
                }
                break;

            case 8: //完　スタミナ回復鈍化 CT600 効果300
                if (support[i].count == 600)
                {
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                if (support[i].count <= 300)
                {
                    if (game.chara[id_e].sp > 0 && !(support[i].count % 15))
                        game.chara[id_e].sp--; //SP回復
                }
                break;

            case 9: //完　相手の周囲以外暗く CT1200　効果500
                if (support[i].count == 1200)
                {
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                if (support[i].count <= 500)
                {
                    //穴開きの4点を取る
                    SDL_Point point[4];
                    point[0].x = game.chara[id_e].RenderData.dst.x - 100;
                    point[0].y = game.chara[id_e].RenderData.dst.y - 100;
                    point[1].x = point[0].x + 200 + game.chara[id_e].RenderData.dst.w;
                    point[2].y = point[0].y + 200 + game.chara[id_e].RenderData.dst.h;
                    //4つの四角を決める
                    blind[0].dst.x = blind[0].dst.y = blind[1].dst.y = 0;
                    blind[0].dst.w = blind[1].dst.x = point[0].x;
                    blind[0].dst.h = WINDOW_HIGH;
                    blind[1].dst.w = WINDOW_WIDE - blind[0].dst.w;
                    blind[1].dst.h = point[0].y;
                    blind[2].dst.x = point[1].x;
                    blind[2].dst.y = point[0].y;
                    blind[2].dst.w = WINDOW_WIDE - blind[2].dst.x;
                    blind[2].dst.h = point[2].y - point[0].y;
                    blind[3].dst.x = point[0].x;
                    blind[3].dst.y = point[2].y;
                    blind[3].dst.w = WINDOW_WIDE - blind[3].dst.x;
                    blind[3].dst.h = WINDOW_HIGH - blind[3].dst.y;
                }
                break;

            case 10: //完　相手の移動速度ランダム化　CT1200 効果600
                if (support[i].count == 1200)
                {
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                if (support[i].count == 600) //もとに戻す
                {
                    int sign = (game.chara[id_e].speed >= 0) ? 1 : -1;
                    game.chara[id_e].speed = 5 * sign; //もとに戻す
                    break;
                }
                if (!(support[i].count % 50) && support[i].count < 600)
                {
                    float sign = (game.chara[id_e].speed > 0) ? 1 : -1;
                    float randam = rand() % 10000;
                    game.chara[id_e].speed = (10 - (randam / 1000)) * sign;
                    printf("speed = %f\n", game.chara[id_e].speed);
                }
                break;

            case 11: //完　相手上下移動反転　CT1800　効果600
                if (!support[i].count)
                {
                    game.chara[id_e].speed *= -1;
                }
                if (support[i].count == 600) //もとに戻す
                {
                    game.chara[id_e].speed *= -1;
                    break;
                }
                if (support[i].count == 1800)
                {
                    support[i].flag = false;
                    BF[bfNum].type = 11;
                    break;
                }
                break;
            }
        }
        support[i].count++; //カウントアップ
    }
}