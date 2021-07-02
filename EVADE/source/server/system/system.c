/**************************************************************
 * 必要な演算を行う
 * 具体的にはキャラの移動や死亡判定、当たり判定など
 * 
***************************************************************/

#include "../../game.h"

//必要な関数
void SystemGame(void);                           //バトル画面に必要な演算をまとめておく
void SystemPick(void);                           //ピック画面に必要な演算
void SystemCount(void);                          //カウントダウンをする
SDL_Rect Rect(FLOATPOINT fpoint, SDL_Rect rect); //fpointの値をSDL_Rect型に変換して返す
CHARA CharaMove(CHARA chara);                    //キャラを動かす
CHARA CharaMoveLimit(CHARA chara);               //キャラの移動制限
double Angle(FLOATPOINT base, FLOATPOINT to);    //キャラの向き合う角度を決める
CHARA MakeBullet(CHARA chara, int number);       //弾幕の生成
CHARA BottanFunction(CHARA chara);               //ボタンによる機能
void BottanSet(int id, BOTTAN bottan);           //クライアントから受け取ったボタンをセットする
void StatusAdapt(void);                          //プレイヤーのHP・SPを変化させる
bool MainPick(int id, int buttun);               //メインピックの処理
bool SupportPick(int id, int buttun);            //サポートピックの処理
void SetSupport(CHARA chara, int number);        //サポートの使用を開始する
bool EndPick(void);                              //ピックが終了したことを検知する
void WordSystem(int num, int count);             //送る文字を決め移動させる
int PickNumberSet(RENDERDATA js, int id);        //ピック画面で触れているアイコンの番号を調べる
void WordMatch(int id);                          //勝敗の送る文字を決める
void ChangeYourTurn(int id);                     //your turnに変える
void ChangeEnemyTurn(int id);                    //enemu turnに変える
//bool EndBattle(void);                                //バトル終了フラグ

//取ってくる関数
extern void HitRectRect(RENDERDATA ch1, RENDERDATA ch2);  //当たり判定の検知
extern int HitPointRect(RENDERDATA dot, RENDERDATA rect); //点と四角形の当たり判定
extern SHOT ManageShot(SHOT shot);                        //弾の処理
extern BULLET ManageBullet(BULLET bullet);                //弾幕の処理
extern void ManageSupport(void);                          //サポートの処理
extern BULLET SetBullet(int type);                        //弾幕の準備

//必要なグローバル変数
extern int PickMain; //メインピックの選択する方
extern int PickSub;  //サポートピックの選択する方
int TouchIcon[4];    //送る文字の種類

//取ってくるグローバル変数
extern GAME game; //ゲームの構造体
//extern CHARA chara[4]; //キャラクタの構造体　4個分
extern BOTTAN bottan[4];  //各プレイヤーのボタン
extern int timer;         //タイマー関数で扱う変数
extern RENDERDATA JS[4];  //カーソル
extern RENDERDATA HP_b1;  //HPbox1
extern RENDERDATA HP_b1s; //HPbox1sub
extern RENDERDATA HP_b2;  //HPbox2
extern RENDERDATA HP_b2s; //HPbox2sub
extern RENDERDATA SP_b1;  //SPbox1
extern RENDERDATA SP_b2;  //SPbox2
extern RENDERDATA FC[12]; //フレーム
extern RENDERDATA BF[8];  //弾幕フレーム
extern int pleyer1;       //メインプレイヤー1
extern int pleyer2;       //メインプレイヤー2
extern RENDERDATA cursor;
extern bool AreaIn;                //エリアに入っているフラグ
extern RENDERDATA MainIcon[10];    //メインが使う弾幕のアイコン
extern RENDERDATA SupportIcon[12]; //サポートが使うアイコン
extern int MainIconSend[10];       //MainIconを送る相手　-1ならどちらも、もしくは送らない
extern int SupportIconSend[12];    //SupportIconを送る相手　-1ならどちらも、もしくは送らない
extern RENDERDATA word;            //文字
extern RENDERDATA wait;            //待ての文字
extern RENDERDATA turn[4];         //ピック画面用
extern RENDERDATA count;           //カウントダウン

/****************************************************************
 * 関数名      void SystemGame(void)
 * 機能       必要な演算を行う
 * 引数       なし
 * 戻り値      なし
*****************************************************************/
void SystemGame(void)
{
    //各キャラごとの処理/////////////////////////////////////////////////////////////////////////////////////////////
    for (int k = 0; k < 4; k++)
    {
        game.chara[k] = BottanFunction(game.chara[k]);                                               //ボタンの機能
        game.chara[k].RenderData.dst = Rect(game.chara[k].floatpoint, game.chara[k].RenderData.dst); //floatpointの値をdst型に代入する
        //自分がメインキャラ
        if (game.chara[k].main)
        {
            game.chara[k].RenderData.angle = Angle(game.chara[k].floatpoint, game.chara[game.chara[k].id_e].floatpoint); //相手の方向を向く角度を決める
            if (game.chara[k].sp < MAX_SP && !(game.count % 5))
                game.chara[k].sp++;   //スタミナ回復
            StatusAdapt();            //HP・SPバーの変更
            if (game.chara[k].hp < 0) //ゲーム終了判定
            {
                winner = game.chara[k].id_e; //勝者を決める
                game.count = 0;              //カウントリセット
                mode++;                      //ゲームモードの移行
                return;
            }
        }
        //自分がサポートキャラ
        else
        {
        }
        //各弾幕ごとの処理////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int i = 0; i < MAX_BULLET; i++)
        {
            game.chara[k].bullet[i] = ManageBullet(game.chara[k].bullet[i]);
            //各弾ごとの処理////////////////////////////////////////////////////////////////////////////////////////////////
            for (int j = 0; j < MAX_SHOT; j++)
            {
                if (game.chara[k].bullet[i].shot[j].stts == UNUSED)
                {
                    break; //探索終了
                }
                if (game.chara[k].bullet[i].shot[j].stts == USE)
                {
                    game.chara[k].bullet[i].shot[j].RenderData.dst = Rect(game.chara[k].bullet[i].shot[j].floatpoint, game.chara[k].bullet[i].shot[j].RenderData.dst); //floatpointの値をdst型に代入する
                    game.chara[k].bullet[i].shot[j] = ManageShot(game.chara[k].bullet[i].shot[j]);                                                                     //弾の管理
                    //当たり判定
                    if (HitPointRect(game.chara[game.chara[k].id_e].RenderData, game.chara[k].bullet[i].shot[j].RenderData))
                    {
                        if (AreaIn && game.chara[k].id_e == game.chara[support[6].id].id_f)
                            game.chara[k].bullet[i].shot[j].pow *= 2;                             //ダメージ2倍
                        game.chara[game.chara[k].id_e].hp -= game.chara[k].bullet[i].shot[j].pow; //HP減少
                        game.chara[k].bullet[i].shot[j].pow = 0;                                  //弾の無効化
                    }
                }
            }
        }
    }
    //サポート行動実行
    ManageSupport();

    game.count++; //カウントアップ
}

/****************************************************************
 * 関数名   SDL_Rect Rect(FLOATPOINT fpoint)
 * 機能     dstのx、yにfpointの値を代入して返す
 * 引数1    FLOATPOINT fpoint
 * 引数2    SDL_Rect dst
 * 戻り値   SDL_Rect
*****************************************************************/
SDL_Rect Rect(FLOATPOINT fpoint, SDL_Rect rect)
{
    rect = (SDL_Rect){fpoint.x,
                      fpoint.y,
                      rect.w,
                      rect.h};
    return rect;
}

/****************************************************************
 * 関数名   CHARA CharaMove(CHARA chara)
 * 機能     キャラを動かす
 * 引数     CHARA chara
 * 戻り値   CHARA
*****************************************************************/
CHARA CharaMove(CHARA chara)
{
    if (chara.bottan.six)
    {
        if (chara.speed == 5 || chara.speed == -5)
            chara.speed /= 5;
    }
    else if (chara.speed == 1 || chara.speed == -1)
        chara.speed *= 5;
    //正規
    if (chara.bottan.up)
        chara.floatpoint.y -= chara.speed;
    if (chara.bottan.down)
        chara.floatpoint.y += chara.speed;
    if (chara.bottan.right)
        chara.floatpoint.x += chara.speed;
    if (chara.bottan.left)
        chara.floatpoint.x -= chara.speed;
    //移動制限
    chara = CharaMoveLimit(chara);

    return chara;
}

/****************************************************************
 * 関数名   CharaMoveLimit
 * 機能     キャラの移動に上限をつける
 * 引数     CHARA chara
 * 戻り値   CHARA
*****************************************************************/
CHARA CharaMoveLimit(CHARA chara)
{
    float angle = FREQUENCY_2_RADIAN(chara.RenderData.angle); //角度をラジアン角に直す
    SDL_Point point[4];
    int maxX = -1, maxY = -1, minX = WINDOW_WIDE + 1, minY = WINDOW_HIGH + 1; //各最大値と最小値

    point[0] = (SDL_Point){
        (chara.RenderData.dst.x + (chara.RenderData.dst.w / 2)) - (((chara.RenderData.dst.w / 2) * cos(angle)) - ((chara.RenderData.dst.h / 2) * sin(angle))),
        (chara.RenderData.dst.y + (chara.RenderData.dst.h / 2)) - (((chara.RenderData.dst.w / 2) * sin(angle)) + (chara.RenderData.dst.h / 2) * cos(angle))};
    //回転角と幅・高さから他の３点の座標を求める
    point[1] = (SDL_Point){point[0].x + chara.RenderData.dst.w * cos(angle), point[0].y + chara.RenderData.dst.w * sin(angle)};
    point[2] = (SDL_Point){point[1].x - chara.RenderData.dst.h * sin(angle), point[1].y + chara.RenderData.dst.h * cos(angle)};
    point[3] = (SDL_Point){point[2].x - chara.RenderData.dst.w * cos(angle), point[2].y - chara.RenderData.dst.w * sin(angle)};

    //最大値と最小値を求める
    for (int i = 0; i < 4; i++)
    {
        if (maxX < point[i].x)
            maxX = point[i].x; //最大値更新
        if (maxY < point[i].y)
            maxY = point[i].y; //最大値更新
        if (minX > point[i].x)
            minX = point[i].x; //最小値更新
        if (minY > point[i].y)
            minY = point[i].y; //最小値更新
    }

    //判定
    if (minX <= 0)
        chara.floatpoint.x -= minX;
    if (maxX >= WINDOW_WIDE)
        chara.floatpoint.x -= maxX - WINDOW_WIDE;
    if (minY <= 130)
        chara.floatpoint.y -= minY - 130;
    if (maxY >= WINDOW_HIGH)
        chara.floatpoint.y -= maxY - WINDOW_HIGH;

    return chara;
}

/****************************************************************
 * 関数名   int Angle(void)
 * 機能     星をアリスの向きに回転させるための角度を決める
 * 引数1    FLOATPOINT base
 *         角度を変えるキャラの位置
 * 引数2    FLOATPOINT to
 *          向きたいキャラの位置
 * 戻り値   angleの値をdouble型で返す
*****************************************************************/
double Angle(FLOATPOINT base, FLOATPOINT to)
{
    double angle; //向く角度
    angle = atan2(to.y - base.y, to.x - base.x);
    angle = RADIAN_2_FREQUENCY(angle);
    angle = angle + 90;
    return angle;
}

/****************************************************************
 * 関数名      CHARA MakeBullet(CHARA chara, int number)
 * 機能       弾幕の生成をする
 * 引数1       CHARA chara
 * 引数2      int number
 *              押したボタンの番号
 * 戻り値     CHARA
*****************************************************************/
CHARA MakeBullet(CHARA chara, int number)
{
    if (chara.bullet[number].stts == USE)
        return chara; //弾幕再生中なら終了
    if (chara.sp < chara.bullet[number].cost)
        return chara; //コストが足りなかったら終了

    chara.bullet[number].stts = USE;                                                                  //使用中に変更
    chara.bullet[number].count = 0;                                                                   //カウントリセット
    chara.bullet[number].base.x = chara.floatpoint.x + chara.RenderData.dst.w / 2 - cursor.dst.w / 2; //弾幕の発生場所,emitの始点位置
    chara.bullet[number].base.y = chara.floatpoint.y + chara.RenderData.dst.h / 2 - cursor.dst.h / 2;
    chara.bullet[number].emit.x = chara.bullet[number].base.x;
    chara.bullet[number].emit.y = chara.bullet[number].base.y;
    chara.bullet[number].angle = chara.RenderData.angle; //弾幕の角度
    if (chara.id_m == pleyer1)
        chara.bullet[number].color = 0; //赤色
    else
        chara.bullet[number].color = 1;    //青色
    chara.sp -= chara.bullet[number].cost; //スタミナ消費

    //バレットフレームの色を変える
    int i;
    if (chara.id_m == pleyer1)
        i = 0;
    else
        i = 4;
    BF[i + number].type = 12;

    chara.bullet[number].number = i + number;

    return chara;
}

/****************************************************************
 * 関数名      CHARA BottanFunction(CHARA chara)
 * 機能       各ボタンによる機能を行う
 * 引数       CHARA chara
 * 戻り値      CHARA
*****************************************************************/
CHARA BottanFunction(CHARA chara)
{
    chara = CharaMove(chara); //キャラの移動
    if (chara.main)           //メイン
    {
        if (chara.bottan.one)
            chara = MakeBullet(chara, 0); //弾幕の生成
        if (chara.bottan.two)
            chara = MakeBullet(chara, 1); //弾幕の生成
        if (chara.bottan.three)
            chara = MakeBullet(chara, 2); //弾幕の生成
        if (chara.bottan.four)
            chara = MakeBullet(chara, 3); //弾幕の生成
    }
    else //サポート
    {
        if (chara.bottan.one)
            SetSupport(chara, 0); //サポート行動の開始
        if (chara.bottan.two)
            SetSupport(chara, 1); //サポート行動の開始
        if (chara.bottan.three)
            SetSupport(chara, 2); //サポート行動の開始
        if (chara.bottan.four)
            SetSupport(chara, 3); //サポート行動の開始
    }
    if (chara.bottan.escape)
    {
        game.count = 0;              //カウントリセット
        mode++;                      //ゲームモードの移行
        chara.bottan.escape = false; //戻す
    }
    return chara;
}

/****************************************************************
 * 関数名      BottanSet
 * 機能       クライアントから受け取ったボタン情報を各キャラの中のBOTTANに格納する
 * 引数1       int id
 *              送ってきたクライアントの番号
 * 引数2      BOTTAN bottan
 *              更新されたボタン情報
 * 戻り値      なし
*****************************************************************/
void BottanSet(int id, BOTTAN bottan)
{
    game.chara[id].bottan = bottan;
}

/****************************************************************
 * 関数名      SystemPick
 * 機能       ピック画面の処理をまとめる
 * 引数       なし
 * 戻り値      なし
*****************************************************************/
void SystemPick(void)
{
    for (int i = 0; i < 4; i++)
    {
        //カーソル移動
        if (game.chara[i].bottan.up)
            JS[i].dst.y -= 5;
        if (game.chara[i].bottan.down)
            JS[i].dst.y += 5;
        if (game.chara[i].bottan.right)
            JS[i].dst.x += 5;
        if (game.chara[i].bottan.left)
            JS[i].dst.x -= 5;
        //移動制限
        if (JS[i].dst.x < 0)
            JS[i].dst.x = 0;
        if (JS[i].dst.x > WINDOW_WIDE - JS[i].dst.w)
            JS[i].dst.x = WINDOW_WIDE - JS[i].dst.w;
        if (JS[i].dst.y < 0)
            JS[i].dst.y = 0;
        if (JS[i].dst.y > WINDOW_HIGH / 2)
            JS[i].dst.y = WINDOW_HIGH / 2;

        TouchIcon[i] = PickNumberSet(JS[i], i); //どのアイコンに触れているかを探す

        //押したボタンによってセットする
        int bottan = -1;
        if (game.chara[i].bottan.one)
            bottan = 0;
        else if (game.chara[i].bottan.two)
            bottan = 1;
        else if (game.chara[i].bottan.three)
            bottan = 2;
        else if (game.chara[i].bottan.four)
            bottan = 3;

        //ボタンを押していたなら
        if (bottan != -1)
        {
            if (game.chara[i].main) //メイン
            {
                if (PickMain == i) //自分の番
                {
                    if (MainPick(i, bottan)) //弾幕をセットする
                    {
                        int change = (i == pleyer1) ? bottan : bottan + 4;
                        BF[change].type = 12; //画像変更
                        ChangeEnemyTurn(PickMain);
                        PickMain = game.chara[i].id_e; //次に選ぶプレイヤーの変更
                        ChangeYourTurn(PickMain);
                    }
                }
            }
            else //サポート
            {
                if (PickSub == i) //自分の番
                {
                    if (SupportPick(i, bottan)) //サポートをセットする
                    {
                        int change = (i == game.chara[pleyer1].id_f) ? bottan + 8 : bottan + 12;
                        BF[change].type = 12; //画像の変更
                        ChangeEnemyTurn(PickSub);
                        PickSub = game.chara[game.chara[i].id_e].id_f; //次に選ぶプレイヤーの変更
                        ChangeYourTurn(PickSub);
                    }
                }
            }
        }

        //モード移行　実際はなくす
        if (EndPick() || game.chara[i].bottan.escape)
        {
            //BF位置、色変更
            for (int i = 0; i < 16; i++)
            {
                BF[i].type = 11;
                BF[i].dst.w = BF[i].dst.h = 100;
                BF[i].dst.x = (((i % 4) + 1) / 2) * 100 + ((i / 8)) * 1550;
                BF[i].dst.y = 800 - (((i % 4) + 1) / 2 * 100) + ((i % 4) / 2) * 200;
            }
            //弾幕アイコン位置変更
            for (int i = 0; i < 10; i++)
            {
                if (MainIconSend[i] != -1)
                {
                    MainIcon[i].dst.w = 70;
                    MainIcon[i].dst.h = 70;
                    if (MainIcon[i].dst.x == 50 + 10) //1ボタン
                    {
                        MainIcon[i].dst.x -= 45;
                        MainIcon[i].dst.y += 95;
                    }
                    else if (MainIcon[i].dst.y == 590 + 10) //2ボタン
                    {
                        MainIcon[i].dst.x -= 65;
                        MainIcon[i].dst.y += 115;
                    }
                    else if (MainIcon[i].dst.y == 830 + 10) //3ボタン
                    {
                        MainIcon[i].dst.x -= 65;
                        MainIcon[i].dst.y += 75;
                    }
                    else if (MainIcon[i].dst.x == 290 + 10) //4ボタン
                    {
                        MainIcon[i].dst.x -= 85;
                        MainIcon[i].dst.y += 95;
                    }
                }
            }
            //サポートアイコン位置変更
            for (int i = 0; i < 12; i++)
            {
                if (SupportIconSend[i] != -1)
                {
                    SupportIcon[i].dst.w = 70;
                    SupportIcon[i].dst.h = 70;
                    if (SupportIcon[i].dst.x == 1440 + 10) //1ボタン
                    {
                        SupportIcon[i].dst.x += 115;
                        SupportIcon[i].dst.y += 96;
                    }
                    else if (SupportIcon[i].dst.y == 590 + 10) //2ボタン
                    {
                        SupportIcon[i].dst.x += 96;
                        SupportIcon[i].dst.y += 115;
                    }
                    else if (SupportIcon[i].dst.y == 830 + 10) //3ボタン
                    {
                        SupportIcon[i].dst.x += 96;
                        SupportIcon[i].dst.y += 75;
                    }
                    else if (SupportIcon[i].dst.x == 1680 + 10) //4ボタン
                    {
                        SupportIcon[i].dst.x += 75;
                        SupportIcon[i].dst.y += 96;
                    }
                }
            }
            //ボタン初期化
            for (int i = 0; i < 4; i++)
            {
                game.chara[i].bottan.down = false;
                game.chara[i].bottan.up = false;
                game.chara[i].bottan.left = false;
                game.chara[i].bottan.right = false;
                game.chara[i].bottan.one = false;
                game.chara[i].bottan.two = false;
                game.chara[i].bottan.three = false;
                game.chara[i].bottan.four = false;
                game.chara[i].bottan.six = false;
                game.chara[i].bottan.escape = false;
            }
            //音楽の変更
            // RENDERDATA rd;
            // rd.type = -3;
            // SendRenderData(rd, -1);
            //モード移行
            mode = 3;
        }
    }
}

/****************************************************************
 * 関数名      StatusAdapt
 * 機能       プレイヤーのHP・SPバーの大きさを数値に合わせる
 * 引数       なし
 * 戻り値      なし
*****************************************************************/
void StatusAdapt(void)
{
    //HPバー
    //プレイヤー１
    HP_b1.dst.w = game.chara[pleyer1].hp;                     //大きさ調整
    HP_b1s.dst.w = game.chara[pleyer1].hp - 540;              //大きさ調整
    HP_b1.dst.x = HP_b1s.dst.x = 40 + (MAX_HP - HP_b1.dst.w); //位置調整
    //色調整
    if (game.chara[pleyer1].hp > MAX_HP - 220)
        HP_b1.type = HP_b1s.type = 201; //緑
    else if (game.chara[pleyer1].hp > MAX_HP - 560)
        HP_b1.type = HP_b1s.type = 202; //黃
    else
        HP_b1.type = HP_b1s.type = 203; //赤
    //プレイヤー２
    HP_b2.dst.w = game.chara[pleyer2].hp;
    HP_b2s.dst.w = game.chara[pleyer2].hp - 540;
    //色調整
    if (game.chara[pleyer2].hp > MAX_HP - 220)
        HP_b2.type = HP_b2s.type = 201; //緑
    else if (game.chara[pleyer2].hp > MAX_HP - 560)
        HP_b2.type = HP_b2s.type = 202; //黃
    else
        HP_b2.type = HP_b2s.type = 203; //赤

    //SPバー
    SP_b1.dst.w = game.chara[pleyer1].sp;       //SPバーの大きさ調整
    SP_b1.dst.x = 270 + (MAX_SP - SP_b1.dst.w); //SPバーの位置調整
    SP_b2.dst.w = game.chara[pleyer2].sp;       //SPバーの大きさ調整
}

/****************************************************************
 * 関数名      MainPick
 * 機能       メインキャラのピック画面での処理
 * 引数1       RENDERDATA
 * 引数2      int
 *              ボタンを押したクライアント番号
 * 引数3      int
 *              押したボタンの番号
 * 戻り値     bool
 *          何か弾幕をセットできたらtrue
*****************************************************************/
bool MainPick(int id, int bottun)
{
    if (TouchIcon[id] == -1) //どのアイコンにも触れていない
        return false;
    if (game.chara[id].bullet[bottun].type != -1) //すでに弾幕がセットされているなら
        return false;

    if (MainIcon[TouchIcon[id]].dst.w == 110) //まだこのアイコンが選ばれていない
    {
        MainIcon[TouchIcon[id]].dst.w = 100;
        MainIcon[TouchIcon[id]].dst.h = 100;
        MainIcon[TouchIcon[id]].dst.x = BF[bottun].dst.x + 10;
        MainIcon[TouchIcon[id]].dst.y = BF[bottun].dst.y + 10;
        MainIconSend[TouchIcon[id]] = id;
        game.chara[id].bullet[bottun] = SetBullet(TouchIcon[id]);
        MainPickCount++; //ピックした数を増やす
        return true;
    }
    else //すでに選ばれている
        return false;
}

/****************************************************************
 * 関数名      SupportPick
 * 機能       サポートキャラのピック画面での処理
* 引数1       RENDERDATA
 * 引数2      int
 *              ボタンを押したクライアント番号
 * 引数3      int 
 *              押したボタンの番号
 * 戻り値      なし
*****************************************************************/
bool SupportPick(int id, int buttun)
{
    if (TouchIcon[id] == -1) //どのアイコンにも触れていない
        return false;
    if (game.chara[id].support_num[buttun] != -1)
        return false;

    if (SupportIcon[TouchIcon[id]].dst.w == 110) //まだこのアイコンが選ばれていなければ
    {
        SupportIcon[TouchIcon[id]].dst.w = 100;
        SupportIcon[TouchIcon[id]].dst.h = 100;
        SupportIcon[TouchIcon[id]].dst.x = BF[buttun + 8].dst.x + 10;
        SupportIcon[TouchIcon[id]].dst.y = BF[buttun + 8].dst.y + 10;
        int set[12] = {0, 1, 4, 5, 8, 9, 2, 3, 6, 7, 10, 11};
        SupportIconSend[TouchIcon[id]] = id;
        game.chara[id].support_num[buttun] = set[TouchIcon[id]];
        support[id].number = buttun; //セットしたボタン
        SubPickCount++;              //ピックした数を増やす
        return true;
    }
    else
        return false;
}

/****************************************************************
 * 関数名      SetSupport
 * 機能       サポート行動を開始する
 * 引数1      chara
 * 引数2      int
 *            押したボタンの番号
 * 戻り値      なし
*****************************************************************/
void SetSupport(CHARA chara, int number)
{
    int id = chara.id_m;                 //ID
    int num = chara.support_num[number]; //ONにするサポート番号

    if (!support[num].flag) //使用中でないなら
    {
        support[num].id = id;     //使用者のID
        support[num].count = 0;   //カウントリセット
        support[num].flag = true; //フラグON
        support[num].number = number;

        //BFの色を変える
        int i;
        if (chara.id_f == pleyer1)
            i = 8;
        else
            i = 12;
        BF[i + number].type = 12;
    }
}

/****************************************************************
 * 関数名      EndPick
 * 機能       ピックが終了したか判定する
 * 引数      なし
 * 戻り値      終了したならtrueを返す
*****************************************************************/
bool EndPick(void)
{
    if (MainPickCount < 8)
        return false;
    if (SubPickCount < 8)
        return false;

    return true;
}

/****************************************************************
 * 関数名      WordSystem
 * 機能       送る文字の種類、位置を決める
 * 引数1      int
 *          送るクライアント番号
 * 引数2      int 
 *          何回目の文字か
 * 戻り値      なし
*****************************************************************/
void WordSystem(int num, int count)
{
    word.type = (game.chara[num].main) ? TouchIcon[num] * 3 + 21 + count : TouchIcon[num] * 3 + 51 + count; //文字のタイプを決める
    switch (word.type % 3)
    {
    case 0: //一段目
        word.src = (SDL_Rect){0, 0, FONT_WIDE * 7, FONT_HIGH};
        word.dst = (SDL_Rect){550, 600, FONT_WIDE * 7, 90};
        break;
    case 1: //二段目
        word.src = (SDL_Rect){0, 0, FONT_WIDE * 7, FONT_HIGH};
        word.dst = (SDL_Rect){550, 700, FONT_WIDE * 7, 90};
        break;
    case 2: //三段目
        word.src = (SDL_Rect){0, 0, 3000, FONT_HIGH};
        if (game.chara[num].main)
            word.dst = (SDL_Rect){550, 800, FONT_WIDE * 40, 90};
        else
            word.dst = (SDL_Rect){550, 800, FONT_WIDE * 35, 90};
        break;
    }
}

/****************************************************************
 * 関数名      PickNumber
 * 機能       ピック画面で触れているアイコンの番号を調べる
 * 引数1      RENDERDATA
 *              カーソルの位置
 * 引数2      int
 *          クライアント番号
 * 戻り値      int
 *              触れているアイコンの番号を調べる
 *              なにも触れていないなら-1を返す
*****************************************************************/
int PickNumberSet(RENDERDATA js, int id)
{
    int num = -1; //選んでいるアイコンの番号

    if (game.chara[id].main) //メインなら
    {
        //カーソルが上段にある
        if (js.dst.y >= 50 - 25 && js.dst.y <= 220 - 25)
        {
            //カーソルがフレームの中にあるなら
            if ((js.dst.x - (400 - 25)) % 220 <= 170 && js.dst.x >= 375 && js.dst.x <= 1425)
            {
                num = (js.dst.x - (400 - 25)) / 220; //選んだアイコンの番号
            }
            else //選択できるものはない
                return num;
        }
        //カーソルが下段にある
        else if (js.dst.y >= 320 - 25 && js.dst.y <= 490 - 25)
        {
            //カーソルがフレームの中にあるなら
            if ((js.dst.x - (400 - 25)) % 220 <= 170 && js.dst.x >= 375 && js.dst.x <= 1425)
            {
                num = (js.dst.x - (400 - 25)) / 220 + 5; //選んだアイコンの番号
            }
            else //選択できるものはない
                return num;
        }
        return num;
    }

    else //サポート
    {
        //カーソルが上段にある
        if (js.dst.y >= 50 - 25 && js.dst.y <= 220 - 25)
        {
            //カーソルがフレームの中にあるのなら
            if ((js.dst.x % 580 >= 150 - 25 && js.dst.x % 580 <= 320 - 25) ||
                (js.dst.x % 580 >= 370 - 25 && js.dst.x % 580 <= 540 - 25))
            {
                num = (js.dst.x % 580 < 330) ? 0 : 1; //選んだフレーム番号
                num += (js.dst.x / 580) * 2;
                //                num += (js.dst.x / 580) * 2;
            }
            else //選択できるものは無い
                return num;
        }
        //カーソルが下段にある
        else if (js.dst.y >= 320 - 25 && js.dst.y <= 490 - 25)
        {
            //カーソルがフレームの中にあるのなら
            if ((js.dst.x % 580 >= 150 - 25 && js.dst.x % 580 <= 320 - 25) ||
                (js.dst.x % 580 >= 370 - 25 && js.dst.x % 580 <= 540 - 25))
            {
                num = (js.dst.x % 580 < 330) ? 6 : 7; //選んだフレーム番号
                num += (js.dst.x / 580) * 2;
                //num += -4 + (js.dst.x / 580) * 2;
            }
            else //選択できるものは無い
                return num;
        }
    }

    return num;
}

/****************************************************************
 * 関数名      SystemCount
 * 機能       バトル前のカウントダウンをする
 * 引数       なし
 * 戻り値      なし
*****************************************************************/
void SystemCount(void)
{
    if (game.count == 0)
    {
        SystemGame(); //キャラの角度決めのために初めだけ通す
        count.dst = (SDL_Rect){850, 400, 150, 150};
    }
    // if (game.count == 600)
    if (game.count == 600)
        mode++;
    game.count++; //カウントアップ

    int countable = game.count / 200;
    count.src = (SDL_Rect){FONT_WIDE * countable, 0, FONT_WIDE, FONT_HIGH};
}

/****************************************************************
 * 関数名      WordMatch
 * 機能       勝敗の結果の送る文字を決める
 * 引数       int
 *              送るクライアントのID
 * 戻り値      なし
*****************************************************************/
void WordMatch(int id)
{
    //勝者側
    if (id == winner || game.chara[id].id_f == winner)
    {
        word.type = 88;
        word.src = (SDL_Rect){0, 0, FONT_WIDE2 * 15, FONT_HIGH2};
        word.dst = (SDL_Rect){530, 420, 30 * 7 * 4, 50 * 4};
        word.angle = 0;
    }
    //敗者1側
    else
    {
        word.type = 88;
        word.src = (SDL_Rect){FONT_WIDE2 * 15, 0, FONT_WIDE2 * 17, FONT_HIGH2};
        word.dst = (SDL_Rect){480, 420, 30 * 8 * 4, 50 * 4};
        word.angle = 0;
    }
}

/****************************************************************
 * 関数名     ChangeYourTurn
 * 機能       自分のturnの中をyour turnに変える
 * 引数       int
 *              自分のID
 * 戻り値      なし
*****************************************************************/
void ChangeYourTurn(int id)
{
    turn[id].src = (SDL_Rect){0, 0, FONT_WIDE2 * 19, FONT_HIGH2};
}

/****************************************************************
 * 関数名     ChangeEnemyTurn
 * 機能       自分のturnの中をenemy turnに変える
 * 引数       int
 *              自分のID
 * 戻り値      なし
*****************************************************************/
void ChangeEnemyTurn(int id)
{
    turn[id].src = (SDL_Rect){FONT_WIDE2 * 19, 0, FONT_WIDE2 * 21, FONT_HIGH2};
}