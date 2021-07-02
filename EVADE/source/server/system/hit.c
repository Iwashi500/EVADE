/**************************************************************
 * 当たり判定用のファイル
 * ベクトル演算などはここでしか使わないだろうからここに入れている
 * もし弾幕で使うのであればベクトル演算の関数はヘッダファイルにいれるかも
 * もしくは専用のファイルを作る
 * 
***************************************************************/

#include "../../game.h"

//必要な関数
void HitRectRect(RENDERDATA ch1, RENDERDATA ch2);  //四角形同士の当たり判定
int HitPointRect(RENDERDATA dot, RENDERDATA rect); //点と四角形の当たり判定
int InnerVc(SDL_Point one, SDL_Point two);         //ベクトルの内積を計算する
int CrossVc(SDL_Point one, SDL_Point two);         //ベクトルの外積を計算する
int PointHit(int mark[8]);                         //法施ベクトル上に投影された点の重なりを調べる

//取ってくる関数

//必要なグローバル変数
int hit; //当たり判定の検知

//取ってくるグローバル変数
//extern CHARA chara[4]; //キャラクタの構造体　4個分
extern GAME game; //ゲームの構造体

/****************************************************************
 * 関数名 void HitRectRect(CHARA ch1, CHARA ch2)
 * 機能   四角形同士の当たり判定の検知を行う
 *      各辺の法線ベクトルに各頂点を投影し、その投影点を比較し当たり判定を行う
 * 引数1     CHARA ch1
 * 引数2    CHARA ch2
 * 戻り値   なし
*****************************************************************/
void HitRectRect(RENDERDATA ch1, RENDERDATA ch2)
{
    int flag = 1;        //初めは当たり判定がある状態
    SDL_Point point[8];  //各頂点の座標
    SDL_Point vector[4]; //各辺のベクトル  長方形の向かい合う辺は平行なので各2つの辺で良い
    SDL_Point normal;    //法線ベクトル
    int mark[8];         //法線ベクトルに投射したときの各頂点の位置
    //8個の配列は　０~３に一体目、４~８に２体目のデータを入れる
    float angle = ch1.angle / 180 * M_PI;
    //一体目の左上の座標を求める
    //詳しい求めかたは、下のコメントアウトした部分を見る
    point[0] = (SDL_Point){
        (ch1.dst.x + (ch1.dst.w / 2)) - (((ch1.dst.w / 2) * cos(angle)) - ((ch1.dst.h / 2) * sin(angle))),
        (ch1.dst.y + (ch1.dst.h / 2)) - (((ch1.dst.w / 2) * sin(angle)) + (ch1.dst.h / 2) * cos(angle))};
    //回転角と幅・高さから他の３点の座標を求める
    point[1] = (SDL_Point){point[0].x + ch1.dst.w * cos(angle), point[0].y + ch1.dst.w * sin(angle)};
    point[2] = (SDL_Point){point[1].x - ch1.dst.h * sin(angle), point[1].y + ch1.dst.h * cos(angle)};
    point[3] = (SDL_Point){point[2].x - ch1.dst.w * cos(angle), point[2].y - ch1.dst.w * sin(angle)};

    //ここから２体目
    angle = ch2.angle / 180 * M_PI;
    //２体目の左上の座標を求める
    point[4] = (SDL_Point){
        (ch2.dst.x + (ch2.dst.w / 2)) - (((ch2.dst.w / 2) * cos(angle)) - ((ch2.dst.h / 2) * sin(angle))),
        (ch2.dst.y + (ch2.dst.h / 2)) - (((ch2.dst.w / 2) * sin(angle)) + (ch2.dst.h / 2) * cos(angle))};
    //回転角と幅・高さから他の３点の座標を求める
    point[5] = (SDL_Point){point[4].x + ch2.dst.w * cos(angle), point[4].y + ch2.dst.w * sin(angle)};
    point[6] = (SDL_Point){point[5].x - ch2.dst.h * sin(angle), point[5].y + ch2.dst.h * cos(angle)};
    point[7] = (SDL_Point){point[6].x - ch2.dst.w * cos(angle), point[6].y - ch2.dst.w * sin(angle)};

    //ベクトル生成
    vector[0] = (SDL_Point){point[1].x - point[0].x, point[1].y - point[0].y};
    vector[1] = (SDL_Point){point[2].x - point[1].x, point[2].y - point[1].y};
    vector[2] = (SDL_Point){point[5].x - point[4].x, point[5].y - point[4].y};
    vector[3] = (SDL_Point){point[6].x - point[5].x, point[6].y - point[5].y};

    /*長方形なので向かい合う辺は平行
        よって作るベクトルは各2つで良い
        作り直す
    //ベクトルを作る
    for(int i = 0; i < 3; i++){
        vector[i] = (SDL_Point){point[i+1].x - point[i].x, point[i+1].y - point[i].y};
        vector[i+4] = (SDL_Point){point[i+5].x - point[i+4].x, point[i+5].y - point[i+4].y};
    }
    //最後は別で作る
    vector[3] = (SDL_Point){point[0].x - point[3].x, point[0].y - point[3].y};
    vector[7] = (SDL_Point){point[4].x - point[7].x, point[4].y - point[7].y};
    */

    for (int i = 0; i < 4; i++)
    {
        normal = (SDL_Point){vector[i].y, vector[i].x * (-1)};
        for (int j = 0; j < 8; j++)
        {
            mark[j] = InnerVc(normal, point[j]);
        }
        //座標(mark)をチェック
        if (!(PointHit(mark)))
        {
            //接していないなら終了
            flag = 0;
            break;
        }
    }

    //数値表示しておく
    /*
    printf("point[0] = (%3d, %3d)  ", point[0].x, point[0].y);
    printf("point[1] = (%3d, %3d)  ", point[1].x, point[1].y);  printf("\n");
    printf("point[2] = (%3d, %3d)  ", point[2].x, point[2].y);
    printf("point[3] = (%3d, %3d)  ", point[3].x, point[3].y);  printf("\n");
    printf("point[4] = (%3d, %3d)  ", point[4].x, point[4].y);
    printf("point[5] = (%3d, %3d)  ", point[5].x, point[5].y);  printf("\n");
    printf("point[6] = (%3d, %3d)  ", point[6].x, point[6].y);
    printf("point[7] = (%3d, %3d)  ", point[7].x, point[7].y);  printf("\n");
    printf("\n");
    printf("vector[0] = (%3d, %3d)  ", vector[0].x, vector[0].y);
    printf("vector[1] = (%3d, %3d)  ", vector[1].x, vector[1].y); printf("\n");
    printf("vector[2] = (%3d, %3d)  ", vector[2].x, vector[2].y);
    printf("vector[3] = (%3d, %3d)  ", vector[3].x, vector[3].y); printf("\n");
    printf("vector[4] = (%3d, %3d)  ", vector[4].x, vector[4].y);
    printf("vector[5] = (%3d, %3d)  ", vector[5].x, vector[5].y); printf("\n");
    printf("vector[6] = (%3d, %3d)  ", vector[6].x, vector[6].y);
    printf("vector[7] = (%3d, %3d)  ", vector[7].x, vector[7].y); printf("\n");
    printf("\n\n");
    */

    //演算した頂点から周辺を引いておく  仕様変更のため背景に塗りつぶされた
    /*
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0); //色を赤に設定
    SDL_RenderDrawLine(renderer, point[0].x, point[0].y, point[1].x, point[1].y);
    SDL_RenderDrawLine(renderer, point[1].x, point[1].y, point[2].x, point[2].y);
    SDL_RenderDrawLine(renderer, point[2].x, point[2].y, point[3].x, point[3].y);
    SDL_RenderDrawLine(renderer, point[3].x, point[3].y, point[0].x, point[0].y);
    SDL_RenderDrawLine(renderer, point[4].x, point[4].y, point[5].x, point[5].y);
    SDL_RenderDrawLine(renderer, point[5].x, point[5].y, point[6].x, point[6].y);
    SDL_RenderDrawLine(renderer, point[6].x, point[6].y, point[7].x, point[7].y);
    SDL_RenderDrawLine(renderer, point[7].x, point[7].y, point[4].x, point[4].y);
    */

    //当たり判定
    //hit = flag;
    return;

    //やり直す
    /*
    //int flag = 1; //当たり判定
    //まず1個目
    int x1 = ch1.point.x; //キャラの中心点
    int y1 = ch1.point.y; //キャラの中心点
    int w1 = ch1.dst.w; //キャラの幅
    int h1 = ch1.dst.h; //キャラの高さ
    float angle1 = ch1.angle / 180 * M_PI; //キャラの角度(sin関数用にラジアンに変換しておく)
    SDL_Point one[4]; //ラインを引くキャラの４つの頂点
    SDL_Point middle1 = {ch1.dst.x + ch1.point.x,
                        ch1.dst.y + ch1.point.y}; //キャラの中心点を座標表示に直す

    SDL_Point move1 = {(x1 * cos(angle1)) - (y1 * sin(angle1)) ,
                    (x1 * sin(angle1)) + (y1 * cos(angle1))}; //回転によって移動した座標を計算

    one[0] = (SDL_Point){middle1.x - move1.x,
                         middle1.y - move1.y}; //左上の座標を計算
    one[1] = (SDL_Point){one[0].x + w1 * cos(angle1), one[0].y + w1 * sin(angle1)}; //右上の座標を計算
    one[2] = (SDL_Point){one[1].x - h1 * sin(angle1), one[1].y + h1 * cos(angle1)}; //右下の座標を計算
    one[3] = (SDL_Point){one[2].x - w1 * cos(angle1), one[2].y - w1 * sin(angle1)}; //左下の座標を計算

    //ここから2個目
    int x2 = ch2.point.x; //キャラの中心点
    int y2 = ch2.point.y; //キャラの中心点
    int w2 = ch2.dst.w; //キャラの幅
    int h2 = ch2.dst.h; //キャラの高さ
    float angle2 = ch2.angle / 180 * M_PI; //キャラの角度(sin関数用にラジアンに変換しておく)
    SDL_Point two[4]; //ラインを引くキャラの４つの頂点
    SDL_Point middle2 = {ch2.dst.x + ch2.point.x,
                        ch2.dst.y + ch2.point.y}; //キャラの中心点を座標表示に直す

    SDL_Point move2 = {(x2 * cos(angle2)) - (y2 * sin(angle2)) ,
                    (x2 * sin(angle2)) + (y2 * cos(angle2))}; //回転によって移動した座標を計算

    two[0] = (SDL_Point){middle2.x - move2.x,
                         middle2.y - move2.y}; //左上の座標を計算
    two[1] = (SDL_Point){two[0].x + w2 * cos(angle2), two[0].y + w2 * sin(angle2)}; //右上の座標を計算
    two[2] = (SDL_Point){two[1].x - h2 * sin(angle2), two[1].y + h2 * cos(angle2)}; //右下の座標を計算
    two[3] = (SDL_Point){two[2].x - w2 * cos(angle2), two[2].y - w2 * sin(angle2)}; //左下の座標を計算

    //ベクトル生成
    SDL_Point vector1[4];
    vector1[0] = (SDL_Point){one[1].x - one[0].x, one[1].y - one[0].y};
    vector1[1] = (SDL_Point){one[2].x - one[1].x, one[2].y - one[1].y};
    vector1[2] = (SDL_Point){one[3].x - one[2].x, one[3].y - one[2].y};
    vector1[3] = (SDL_Point){one[0].x - one[3].x, one[0].y - one[3].y};
    
    SDL_Point vector2[4];
    vector2[0] = (SDL_Point){two[1].x - two[0].x, two[1].y - two[0].y};
    vector2[1] = (SDL_Point){two[2].x - two[1].x, two[2].y - two[1].y};
    vector2[2] = (SDL_Point){two[3].x - two[2].x, two[3].y - two[2].y};
    vector2[3] = (SDL_Point){two[0].x - two[3].x, two[0].y - two[3].y};

    //法線生成
    SDL_Point NormalVect = {vector1[0].y, vector1[0].x * (-1)};

    */
}

/****************************************************************
 * 関数名   void HitPointRect(SDL_Rect point, RENDERDATA rect)
 * 機能     点と四角形の当たり判定
 * 引数1      SDL_Rect point 
 *          点の座標
 * 引数2      RENDERDATA rect
 *          四角形の座標
 * 戻り値   なし
*****************************************************************/
int HitPointRect(RENDERDATA dot, RENDERDATA rect)
{
    SDL_Point center = {dot.dst.x + (dot.dst.w / 2), dot.dst.y + (dot.dst.h / 2)}; //点の中心座標
    SDL_Point point[4];                                                            //四角形の各点の座標
    SDL_Point vector[5];                                                           //四角形の各辺のベクトル　5個目は点と各頂点のベクトル用
    long int sign[4];                                                              //ベクトルの外積の符号

    double angle = FREQUENCY_2_RADIAN(rect.angle); //ラジアン角に戻す
    //四角形の左上の座標を取る
    point[0] = (SDL_Point){
        (rect.dst.x + (rect.dst.w / 2)) - (((rect.dst.w / 2) * cos(angle)) - ((rect.dst.h / 2) * sin(angle))),
        (rect.dst.y + (rect.dst.h / 2)) - (((rect.dst.w / 2) * sin(angle)) + (rect.dst.h / 2) * cos(angle))};
    //回転角と幅・高さから他の３点の座標を求める
    point[1] = (SDL_Point){point[0].x + rect.dst.w * cos(angle), point[0].y + rect.dst.w * sin(angle)};
    point[2] = (SDL_Point){point[1].x - rect.dst.h * sin(angle), point[1].y + rect.dst.h * cos(angle)};
    point[3] = (SDL_Point){point[2].x - rect.dst.w * cos(angle), point[2].y - rect.dst.w * sin(angle)};
    //四角形のベクトルを求める
    vector[0] = (SDL_Point){point[1].x - point[0].x, point[1].y - point[0].y};
    vector[1] = (SDL_Point){point[2].x - point[1].x, point[2].y - point[1].y};
    vector[2] = (SDL_Point){point[3].x - point[2].x, point[3].y - point[2].y};
    vector[3] = (SDL_Point){point[0].x - point[3].x, point[0].y - point[3].y};

    //点に向くベクトル
    vector[4] = (SDL_Point){center.x - point[0].x, center.y - point[0].y};
    //線上なら当たり判定なし
    if ((sign[0] = CrossVc(vector[0], vector[4])) == 0)
    {
        hit = 0; //当たり判定なし
        return 0;
    }
    //当たり判定を調べる
    else
    {
        for (int i = 1; i < 4; i++)
        {
            vector[4] = (SDL_Point){center.x - point[i].x, center.y - point[i].y};
            sign[i] = CrossVc(vector[i], vector[4]);
            if (sign[0] * sign[i] <= 0)
            {
                hit = 0; //当たり判定なし
                return 0;
            }
        }
    }

    //当たり判定あり
    //hit = 1;
    //printf("hit\n");
    return 1;
}

/****************************************************************
 * 関数名   int InnerVc(SDL_point one, SDL_Point two)
 * 機能     ベクトルの内積を計算する
 * 引数1    SDL_Point one
 *          ベクトルの1個目
 * 引数2    SDL_Point two
 *          ベクトルの2個目
 * 戻り値   int
 *          内積の値
*****************************************************************/
int InnerVc(SDL_Point one, SDL_Point two)
{
    return (one.x * two.x) + (one.y * two.y);
}

/****************************************************************
 * 関数名      SDL_Point CrossVc(SDL_Point one, SDL_Point two)
 * 機能       ベクトルの外積を計算する
 * 引数1      SDL_Point one
 * 引数2      SDL_Point two
 * 戻り値      int
*****************************************************************/
int CrossVc(SDL_Point one, SDL_Point two)
{
    return (int)((one.x * two.y) - (one.y * two.x));
}

/****************************************************************
 * 関数名   int PointHit(int mark[8])
 * 機能     法線ベクトル上に投影した点の重なりを調べる
 * 引数     int mark[8]
 *          投影した点の座標
 * 戻り値   int
 *          重なりがあるなら 1
 *          重なりがないなら 0
*****************************************************************/
int PointHit(int mark[8])
{
    int max_one = mark[0]; //一つ目の最大値
    int min_one = mark[0]; //一つ目の最小値
    int max_two = mark[4]; //二つ目の最大値
    int min_two = mark[4]; //二つ目の最小値

    //最大値・最小値を調べる
    for (int i = 0; i < 4; i++)
    {
        if (max_one < mark[i])
            max_one = mark[i]; //最大値更新
        if (min_one > mark[i])
            min_one = mark[i]; //最小値更新
        if (max_two < mark[i + 4])
            max_two = mark[i + 4]; //最大値更新
        if (min_two > mark[i + 4])
            min_two = mark[i + 4]; //最小値更新
    }

    //重なりの判定
    if (min_one <= min_two && min_two <= max_one)
        return 1;
    else if (min_one <= max_two && max_two <= max_one)
        return 1;
    else if (min_one <= min_two && max_two <= max_one)
        return 1;
    else if (min_two <= min_one && max_one <= max_two)
        return 1;
    else
        return 0;
}