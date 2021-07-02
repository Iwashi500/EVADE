/**********************************************************************************************
 * 弾を動かすための関数ファイル
 * 
************************************************************************************************/

#include "../../game.h"

//必要な関数
SHOT ManageShot(SHOT shot);         //弾の処理をする
SHOT MoveShot(SHOT shot);           //弾を動かす
bool OutShot(SHOT shot);            //弾の画面外判定
SHOT AccelShot(SHOT shot);          //弾を加速させる
SHOT AngleShot(SHOT shot);          //弾を曲げる
BULLET ManageBullet(BULLET bullet); //弾幕の処理をする

//取ってくる関数
extern SHOT SetShotImage(int type, FLOATPOINT emit, double angle, int color); //弾の画像の準備

//必要なグローバル変数

//取ってくるグローバル変数
extern RENDERDATA BF[16]; //バレットフレーム

/****************************************************************
 * 関数名      SHOT ManageShot(SHOT shot)
 * 機能       弾を動かす
 * 引数       SHOT shot
 * 戻り値      SHOT
 *          動かしたあとのデータを返す
*****************************************************************/
SHOT ManageShot(SHOT shot)
{
    switch (shot.motion)
    {
    case 0: //何もなし
        break;
    case 1: //サイン軌道
        shot.Aspeed = sinf(FREQUENCY_2_RADIAN(shot.count));
        break;
    case 2: //PF
        if (shot.count == 450)
            shot.speed = 0;
        if (shot.count == 650)
        {
            shot.speed = 1.5;
            shot.RenderData.angle = rand() % 3600 / 10;
            shot.motion = 0;
        }
        break;
    case 3:
        if (shot.count == 600)
        {
            shot.speed = 5;
            shot.motion = 0;
        }
        break;
    case 4:                  //加速度変化
        if (shot.speed >= 5) //スピード上限
        {
            shot.speed = 5;
            shot.accel = 0;
            shot.motion = 0;
            break;
        }
        shot.accel += 0.000095;
        break;
    case 5:
        if (shot.speed >= 2)
        {
            shot.speed = 2;
            shot.accel = 0;
            shot.motion = 0;
        }
        break;
    case 6: //へにょりレーザー
        shot.Aspeed = sinf(FREQUENCY_2_RADIAN(shot.count * 4)) * 4;
        break;
    case 7: //羽レーザー
        if (shot.count == 80)
        {
            shot.RenderData.angle -= 15 * shot.speed + 100; //10*speed+100 10*speed+120も候補
            shot.speed = 4;
            shot.motion = 0;
        }
        break;
    case 8: //羽レーザー
        if (shot.count == 80)
        {
            shot.RenderData.angle += 15 * shot.speed + 100;
            shot.speed = 4;
            shot.motion = 0;
        }
        break;
    case 9: //マーキュリーポイズン
        if (shot.speed >= 2)
        {
            shot.speed = 2;
            shot.Aaccel = shot.Aspeed = 0;
            shot.motion = 0;
        }
        break;
    case 10:
        shot.accel /= 1.0013;
        shot.Aspeed /= 1.0013;
        break;
    case 11: //ここで移動制御
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:;
        //float angle = FREQUENCY_2_RADIAN(shot.RenderData.angle);
        if (shot.count <= 400)
        {
            shot.floatpoint.x -= cosf(FREQUENCY_2_RADIAN(shot.RenderData.angle) - M_PI / 2) * shot.speed; //ここで移動を戻す
            shot.floatpoint.y -= sinf(FREQUENCY_2_RADIAN(shot.RenderData.angle) - M_PI / 2) * shot.speed; //ここで移動を戻す
        }
        if (shot.count <= 100)
        {
            float angle = FREQUENCY_2_RADIAN(shot.RenderData.angle - 60 * (shot.motion - 11));
            shot.floatpoint.x += cosf(angle - M_PI / 2) * shot.speed;
            shot.floatpoint.y += sinf(angle - M_PI / 2) * shot.speed;
        }
        else if (shot.count == 400)
        {
            shot.Aspeed = 0;
            shot.motion = 0;
            shot.speed = 5;
        }
        //  else if (shot.count == 472) {
        //     shot.Aspeed = 0;
        //     shot.motion = 0;
        //     shot.speed  = 5;
        // }
        break;

    case 147: //反射弾　円形なので前期と同じでいいかな
    case 148:
        if (shot.floatpoint.x < 0 || shot.floatpoint.x + shot.RenderData.dst.w > WINDOW_WIDE)
        {
            shot.RenderData.angle *= -1;
            shot.motion++;
            break;
        }
        if (shot.floatpoint.y < 0)
        {
            shot.RenderData.angle -= 90;
            shot.RenderData.angle *= -1;
            shot.RenderData.angle += 90;
            shot.motion++;
            break;
        }
        if (shot.floatpoint.y + shot.RenderData.dst.h > WINDOW_HIGH)
        {
            shot.RenderData.angle += 90;
            shot.RenderData.angle *= -1;
            shot.RenderData.angle -= 90;
            shot.motion++;
            break;
        }
        break;
    case 149:
        shot.motion = 0;
        break;
    default:
        break;
    }
    //すわこ弾
    if (shot.motion >= 19 && shot.motion <= 146)
    {
        shot.RenderData.angle = sqrtf(shot.count * shot.speed * 150) * 0.3 + (5.625 * (shot.motion - 19)); //75
        if (shot.motion >= 83)
            shot.RenderData.angle *= -1;
    }

    shot = AccelShot(shot); //加速度計算
    shot = MoveShot(shot);  //弾を動かす
    shot = AngleShot(shot); //弾を曲げる
    shot.count++;           //カウントアップ
    return shot;
}

/****************************************************************
 * 関数名      SHOT MoveShot(SHOT shot)
 * 機能       弾を動かす
 * 引数       SHOT　shot
 * 戻り値      SHOT
*****************************************************************/
SHOT MoveShot(SHOT shot)
{
    float angle = FREQUENCY_2_RADIAN(shot.RenderData.angle);
    shot.floatpoint.x += cosf(angle - M_PI / 2) * shot.speed;
    shot.floatpoint.y += sinf(angle - M_PI / 2) * shot.speed;
    if (OutShot(shot) && shot.still < shot.count)
        shot.stts = FINISH; //終わり
    return shot;
}

/****************************************************************
 * 関数名      SHOT AccelShot(SHOT shot)
 * 機能       弾を加速させる
 * 引数       SHOT shot
 * 戻り値      SHOT
*****************************************************************/
SHOT AccelShot(SHOT shot)
{
    shot.speed += shot.accel; //加速計算
    return shot;
}

/****************************************************************
 * 関数名      bool OutShot(SHOT shot)
 * 機能       弾が画面外にあるかを調べる
 * 引数       SHOT shot
 * 戻り値      bool
 *          画面外に弾があるならTRUE
 *          画面内にあるならFALSE
*****************************************************************/
bool OutShot(SHOT shot)
{
    float angle = FREQUENCY_2_RADIAN(shot.RenderData.angle); //角度をラジアン角に直す
    SDL_Point point[4];
    long int maxX = -1, maxY = -1, minX = WINDOW_WIDE + 1, minY = WINDOW_HIGH + 1; //各最大値と最小値

    point[0] = (SDL_Point){
        (shot.RenderData.dst.x + (shot.RenderData.dst.w / 2)) - (((shot.RenderData.dst.w / 2) * cos(angle)) - ((shot.RenderData.dst.h / 2) * sin(angle))),
        (shot.RenderData.dst.y + (shot.RenderData.dst.h / 2)) - (((shot.RenderData.dst.w / 2) * sin(angle)) + (shot.RenderData.dst.h / 2) * cos(angle))};
    //回転角と幅・高さから他の３点の座標を求める
    point[1] = (SDL_Point){point[0].x + shot.RenderData.dst.w * cos(angle), point[0].y + shot.RenderData.dst.w * sin(angle)};
    point[2] = (SDL_Point){point[1].x - shot.RenderData.dst.h * sin(angle), point[1].y + shot.RenderData.dst.h * cos(angle)};
    point[3] = (SDL_Point){point[2].x - shot.RenderData.dst.w * cos(angle), point[2].y - shot.RenderData.dst.w * sin(angle)};

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

    //printf("max = (%d, %d)\n", maxX, maxY);
    //printf("min = (%d, %d)\n", minX, minY);

    //判定
    if (maxX < 0)
        return true; //画面外
    if (maxY < 0)
        return true; //画面外
    if (minX > WINDOW_WIDE)
        return true; //画面外
    if (minY > WINDOW_HIGH)
        return true; //画面外

    return false; //画面内
}

/****************************************************************
 * 関数名      SHOT AngleShot(SHOT shot)
 * 機能       弾を曲げる
 * 引数       SHOT shot
 * 戻り値      SHOT
*****************************************************************/
SHOT AngleShot(SHOT shot)
{
    shot.Aspeed += shot.Aaccel;
    shot.RenderData.angle += shot.Aspeed;
    return shot;
}

/****************************************************************
 * 関数名      ManageBullet
 * 機能       弾幕の処理を行う
 * 引数       BULLET
 * 戻り値      BULLET
*****************************************************************/
BULLET ManageBullet(BULLET bullet)
{
    if (bullet.stts != USE)
        return bullet; //再生中でなければ終了

    switch (bullet.type)
    {
    case 0: //弾をまっすぐ飛ばすだけ
        if (bullet.count == 101)
            bullet.stts = FINISH; //弾幕終了
        if (!bullet.count)
            bullet.emit = bullet.base; //弾の生成場所セット
        if (!(bullet.count % 20))
        {
            for (int i = 0; i < MAX_SHOT; i++)
            {
                if (bullet.shot[i].stts < USE)
                {
                    bullet.shot[i] = SetShotImage(0, bullet.emit, bullet.angle, bullet.color); //弾の生成
                    break;
                }
            }
        }
        break;

    case 1: //3wayまっすぐ飛ぶ弾
        if (bullet.count == 151)
        {
            bullet.stts = FINISH;
            break;
        }
        if (!bullet.count)
            bullet.emit = bullet.base; //弾の生成場所設定
        if (!(bullet.count % 25))
        {
            for (int i = 0; i < 3; i++) //3発撃つ
            {
                int k = 0;
                for (int j = k; j < MAX_SHOT; j++)
                {
                    if (bullet.shot[j].stts < USE)
                    {
                        bullet.shot[j] = SetShotImage(0, bullet.emit, bullet.angle + (30 * (i - 1)), bullet.color);
                        k = i; //ここまで調べた
                        break;
                    }
                }
            }
        }
        break;

    case 2: //円状まっすぐ飛ぶ弾
        if (bullet.count == 151)
        {
            bullet.stts = FINISH;
            break;
        }
        if (!(bullet.count % 25))
        {
            for (int i = 0; i < 36; i++) //3発撃つ
            {
                int k = 0;
                for (int j = k; j < MAX_SHOT; j++)
                {
                    if (bullet.shot[j].stts < USE)
                    {
                        bullet.shot[j] = SetShotImage(1, bullet.emit, bullet.angle + (10 * i), bullet.color);
                        k = i; //ここまで調べた
                        break;
                    }
                }
            }
        }
        break;

    case 3: //扇状まっすぐ飛ぶ弾
        if (bullet.count == 151)
        {
            bullet.stts = FINISH;
            break;
        }
        if (!bullet.count)
            bullet.emit = bullet.base; //弾の生成場所設定
        if (!(bullet.count % 25))
        {
            for (int i = 0; i < 7; i++) //3発撃つ
            {
                int k = 0;
                for (int j = k; j < MAX_SHOT; j++)
                {
                    if (bullet.shot[j].stts < USE)
                    {
                        bullet.shot[j] = SetShotImage(0, bullet.emit, bullet.angle + ((i - 3) * 20), bullet.color);
                        k = i; //ここまで調べた
                        break;
                    }
                }
            }
        }
        break;

    case 4: //角度、スピードがバラマキ弾
        if (!bullet.count)
        {
            for (int i = 0; i < 80; i++)
            {
                int k = 0;
                for (int j = k; j < MAX_SHOT; j++)
                {
                    if (bullet.shot[j].stts < USE)
                    {
                        bullet.shot[j] = SetShotImage(1, bullet.base, bullet.angle + rand(), bullet.color);
                        k = i;
                        break;
                    }
                }
            }
        }
        if (bullet.count == 20)
            bullet.stts = FINISH;
        break;

    case 5: //サイン波弾
        if (bullet.count == 200)
        {
            bullet.stts = FINISH;
            break;
        }
        if (!bullet.count)
            bullet.emit = bullet.base; //弾の生成場所設定
        if (!(bullet.count % 25))
        {
            for (int i = 0; i < 2; i++) //2発撃つ
            {
                int k = 0;
                for (int j = k; j < MAX_SHOT; j++)
                {
                    if (bullet.shot[j].stts < USE)
                    {
                        bullet.shot[j] = SetShotImage(2, bullet.emit, bullet.angle, bullet.color);
                        bullet.shot[j].count += 180 * i;            //二個目は逆位相
                        bullet.shot[j].RenderData.angle += 114 * i; //二個目は下から始まる
                        k = i;                                      //ここまで調べた
                        break;
                    }
                }
            }
        }
        break;

    case 6: //パーフェクトフリーズ
        if (bullet.count == 800)
        {
            bullet.stts = FINISH;
            BF[bullet.number].type = 11;
            break;
        }
        if (bullet.count < 300)
        {
            for (int i = 0; i < MAX_SHOT; i++)
            {
                if (bullet.shot[i].stts < USE)
                {
                    bullet.shot[i] = SetShotImage(3, bullet.base, bullet.angle + rand(), bullet.color);
                    bullet.shot[i].count = bullet.count;
                    break;
                }
            }
        }
        else if (bullet.count == 320)
        {
            bullet.emit.x = -500;
        }
        break;

    case 7: //エミッタ実験
        if (bullet.count == 151)
        {
            bullet.stts = FINISH; //弾幕終了
            break;
        }
        if (!bullet.count)
        {

            bullet.emit.x += cosf(FREQUENCY_2_RADIAN(bullet.angle - 135)) * 100;
            bullet.emit.y += sinf(FREQUENCY_2_RADIAN(bullet.angle - 135)) * 100;
        }
        bullet.emit.x += cosf(FREQUENCY_2_RADIAN(bullet.angle)) * 1;
        bullet.emit.y += sinf(FREQUENCY_2_RADIAN(bullet.angle)) * 1;
        if (!(bullet.count % 15))
        {
            for (int i = 0; i < MAX_SHOT; i++)
            {
                if (bullet.shot[i].stts < USE)
                {
                    bullet.shot[i] = SetShotImage(1, bullet.emit, bullet.angle, bullet.color);
                    break;
                }
            }
        }
        break;

    case 8:; //ハート試作
        if (bullet.count == 540)
        {
            bullet.stts = FINISH;
            break;
        }
        float tee = (float)(FREQUENCY_2_RADIAN(bullet.count) / 1.5);
        bullet.emit.x = bullet.base.x - (16 * sinf(tee) * sinf(tee) * sinf(tee)) * 7;
        bullet.emit.y = bullet.base.y - (13 * cos(tee) - 5 * cosf(2 * tee) - 2 * cosf(3 * tee) - cosf(4 * tee)) * 7;
        if (!(bullet.count % 10))
        {
            for (int i = 0; i < MAX_SHOT; i++)
            {
                if (bullet.shot[i].stts < USE)
                {
                    bullet.shot[i] = SetShotImage(4, bullet.emit, bullet.angle, bullet.color);
                    bullet.shot[i].count = bullet.count; //カウント調整
                    break;
                }
            }
        }
        break;

    case 9: //ゾンビクロー
        if (bullet.count == 240)
        {
            bullet.stts = FINISH;
            break;
        }
        if ((bullet.count % 120) < 60)
        {
            bullet.emit.x = bullet.base.x - 200 * cosf(FREQUENCY_2_RADIAN(bullet.angle + bullet.count * 3));
            bullet.emit.y = bullet.base.y - 200 * sinf(FREQUENCY_2_RADIAN(bullet.angle + bullet.count * 3));
        }
        if ((bullet.count % 120) >= 60 && (bullet.count % 120) < 120)
        {
            bullet.emit.x = bullet.base.x - 200 * cosf(FREQUENCY_2_RADIAN(bullet.angle - bullet.count * 3));
            bullet.emit.y = bullet.base.y - 200 * sinf(FREQUENCY_2_RADIAN(bullet.angle - bullet.count * 3));
        }
        if (!(bullet.count % 2))
        {
            for (int i = 0; i < MAX_SHOT; i++)
            {
                if (bullet.shot[i].stts < USE)
                {
                    if (bullet.count < 60)
                    {
                        bullet.shot[i] = SetShotImage(6, bullet.emit, bullet.angle + (40 - bullet.count) * 1.1, bullet.color);
                        break;
                    }
                    else
                    {
                        bullet.shot[i] = SetShotImage(6, bullet.emit, bullet.angle - (40 - (bullet.count % 60)) * 1.1, bullet.color);
                        break;
                    }
                }
            }
        }
        break;

    case 10: //五芒星 + 藍様
        if (bullet.count == 1000)
        {
            bullet.stts = FINISH; //終了処理
            BF[bullet.number].type = 11;
            break;
        }
        if (!bullet.count) //初期位置の設定
        {
            bullet.emit.x = bullet.base.x - 100 * cosf(FREQUENCY_2_RADIAN(bullet.angle + 90));
            bullet.emit.y = bullet.base.y - 100 * sinf(FREQUENCY_2_RADIAN(bullet.angle + 90));
        }
        if (bullet.count <= 250) //毎回の移動
        {
            bullet.emit.x += cosf(FREQUENCY_2_RADIAN(bullet.angle + 104)) * 4;
            bullet.emit.y += sinf(FREQUENCY_2_RADIAN(bullet.angle + 104)) * 4;
        }
        if (!(bullet.count % 50) && bullet.count != 0 && bullet.count < 250) //角度の変更
        {
            for (int j = 0; j < 5; j++)
            {
                for (int i = 0; i < MAX_SHOT; i++) //先にこの向きの弾を作成
                {
                    if (bullet.shot[i].stts < USE)
                    {
                        bullet.shot[i] = SetShotImage(5, bullet.emit, bullet.angle + 104, bullet.color);
                        bullet.shot[i].motion -= (j + 1); //motionを-1〜-5にする
                        break;
                    }
                }
            }
            bullet.angle -= 144; //方向転換
        }

        if (!(bullet.count % 5) && bullet.count <= 250) //弾の生成
        {
            for (int j = 0; j < 5; j++) //五重に出す
            {
                for (int i = 0; i < MAX_SHOT; i++)
                {
                    if (bullet.shot[i].stts < USE)
                    {
                        bullet.shot[i] = SetShotImage(5, bullet.emit, bullet.angle + 104, bullet.color);
                        bullet.shot[i].motion -= (j + 1); //motionを-1〜-5にする
                        break;
                    }
                }
            }
        }
        if (bullet.count >= 250 && bullet.count <= 300) //魔法陣を大玉の位置に戻す
        {
            bullet.emit.x += cosf(FREQUENCY_2_RADIAN(bullet.angle - 56)) * 2;
            bullet.emit.y += sinf(FREQUENCY_2_RADIAN(bullet.angle - 56)) * 2;
        }
        if (bullet.count == 450) //魔法陣の退場
        {
            bullet.emit.x = -300;
        }
        if (bullet.count >= 350 && bullet.count < 470) //星が分裂する
        {
            for (int i = 0; i < MAX_SHOT; i++)
            {
                if (bullet.shot[i].stts == UNUSED)
                {
                    break;
                }
                if (bullet.shot[i].stts == USE && bullet.shot[i].motion < -1)
                {
                    bullet.shot[i].floatpoint.x += cosf(FREQUENCY_2_RADIAN(bullet.angle + 122 + 72 * (bullet.shot[i].motion * -1))) * 1.5;
                    bullet.shot[i].floatpoint.y += sinf(FREQUENCY_2_RADIAN(bullet.angle + 122 + 72 * (bullet.shot[i].motion * -1))) * 1.5;
                }
            }
        }
        else if (bullet.count == 600) //弾の開放 650
        {
            for (int i = 0; i < MAX_SHOT; i++)
            {
                if (bullet.shot[i].stts == UNUSED)
                {
                    break;
                }
                if (bullet.shot[i].stts == USE && bullet.shot[i].motion < -1)
                {
                    bullet.shot[i].speed = 0.25 + (50 - fabsf(50 - (bullet.shot[i].count % 50) * 2)) / 150;
                    bullet.shot[i].speed *= 3; //スピード調整用
                    bullet.shot[i].motion = 0;
                }
            }
        }
        //break;

        //case 11:;                     //弾から弾の練習
        static FLOATPOINT emiter; //1つめの弾が消えた場所
        static double angle;
        if (!bullet.count)
        {
            emiter = bullet.emit;
            angle = bullet.angle;
        }
        if (bullet.count == 370) //初めの大玉
        {
            for (int i = 0; i < MAX_SHOT; i++)
            {
                if (bullet.shot[i].stts < USE)
                {
                    bullet.shot[i] = SetShotImage(7, emiter, angle, bullet.color);
                    break;
                }
            }
        }
        if (bullet.count == (224 + 370)) //要調整　変更時はスピードが変わるカウントを見る
        {
            for (int i = 0; bullet.shot[i].stts != UNUSED; i++) //弾を消す
            {
                if (bullet.shot[i].stts == USE && bullet.shot[i].motion == -1)
                {
                    emiter = bullet.shot[i].floatpoint; //弾の発生場所を記録
                    bullet.shot[i].stts = FINISH;
                    break;
                }
            }
            for (int r = 0; r < 4; r++) //発生場所ずらし
            {
                FLOATPOINT spot;
                spot.x = emiter.x + 50 * cosf(FREQUENCY_2_RADIAN(angle + 90 * r));
                spot.y = emiter.y + 50 * sinf(FREQUENCY_2_RADIAN(angle + 90 * r));
                for (int j = 0; j < 8; j++) //8方向に撃つ
                {
                    int k = 0;
                    for (int i = k; i < MAX_SHOT; i++) //弾の生成
                    {
                        if (bullet.shot[i].stts < USE)
                        {

                            bullet.shot[i] = SetShotImage(8, spot, angle + 45 * j, bullet.color);
                            k = i; //ここまで探した
                            break;
                        }
                    }
                }
            }
        }
        if (bullet.count == 274 + 370) //二列目生成
        {
            for (int r = 0; r < 4; r++) //発生場所ずらし
            {
                FLOATPOINT spot;
                spot.x = emiter.x + 100 * cosf(FREQUENCY_2_RADIAN(angle + 90 * r));
                spot.y = emiter.y + 100 * sinf(FREQUENCY_2_RADIAN(angle + 90 * r));
                for (int j = 0; j < 8; j++) //8方向に撃つ
                {
                    int k = 0;
                    for (int i = k; i < MAX_SHOT; i++) //弾の生成
                    {
                        if (bullet.shot[i].stts < USE)
                        {

                            bullet.shot[i] = SetShotImage(8, spot, angle + 45 * j, bullet.color);
                            k = i; //ここまで探した
                            break;
                        }
                    }
                }
            }
            for (int r = 0; r < 4; r++) //発生場所ずらし
            {
                FLOATPOINT spot;
                spot.x = emiter.x + 50 * 1.41421356 * cosf(FREQUENCY_2_RADIAN(angle + 90 * r + 45));
                spot.y = emiter.y + 50 * 1.41421356 * sinf(FREQUENCY_2_RADIAN(angle + 90 * r + 45));
                for (int j = 0; j < 8; j++) //8方向に撃つ
                {
                    int k = 0;
                    for (int i = k; i < MAX_SHOT; i++) //弾の生成
                    {
                        if (bullet.shot[i].stts < USE)
                        {

                            bullet.shot[i] = SetShotImage(8, spot, angle + 45 * j, bullet.color);
                            k = i; //ここまで探した
                            break;
                        }
                    }
                }
            }
        }
        if (bullet.count == 324 + 370) //三列目生成
        {
            for (int r = 0; r < 4; r++) //発生場所ずらし
            {
                FLOATPOINT spot;
                spot.x = emiter.x + 150 * cosf(FREQUENCY_2_RADIAN(angle + 90 * r));
                spot.y = emiter.y + 150 * sinf(FREQUENCY_2_RADIAN(angle + 90 * r));
                for (int j = 0; j < 8; j++) //8方向に撃つ
                {
                    int k = 0;
                    for (int i = k; i < MAX_SHOT; i++) //弾の生成
                    {
                        if (bullet.shot[i].stts < USE)
                        {

                            bullet.shot[i] = SetShotImage(8, spot, angle + 45 * j, bullet.color);
                            k = i; //ここまで探した
                            break;
                        }
                    }
                }
            }
            for (int r = 0; r < 4; r++) //発生場所ずらし
            {
                for (int l = 0; l < 2; l++)
                {
                    FLOATPOINT spot;
                    spot.x = emiter.x + 50 * 2.2360679 * cosf(FREQUENCY_2_RADIAN(angle + 90 * r + 30 * (l + 1)));
                    spot.y = emiter.y + 50 * 2.2360679 * sinf(FREQUENCY_2_RADIAN(angle + 90 * r + 30 * (l + 1)));
                    for (int j = 0; j < 8; j++) //8方向に撃つ
                    {
                        int k = 0;
                        for (int i = k; i < MAX_SHOT; i++) //弾の生成
                        {
                            if (bullet.shot[i].stts < USE)
                            {

                                bullet.shot[i] = SetShotImage(8, spot, angle + 45 * j, bullet.color);
                                k = i; //ここまで探した
                                break;
                            }
                        }
                    }
                }
            }
        }
        break;

    case 11: //レーザー試験
        if (bullet.count >= 250)
        {
            for (int i = 0; i < MAX_SHOT; i++) //最後尾を出す
            {
                if (bullet.shot[i].stts < USE)
                {
                    bullet.shot[i] = SetShotImage(11, bullet.emit, bullet.angle, bullet.color);
                    break;
                }
            }
            bullet.stts = FINISH;
            break;
        }
        if (!bullet.count)
        {
            for (int i = 0; i < MAX_SHOT; i++) //先頭を出す
            {
                if (bullet.shot[i].stts < USE)
                {
                    bullet.shot[i] = SetShotImage(10, bullet.emit, bullet.angle, bullet.color);
                    break;
                }
            }
        }
        else if (bullet.count >= 2 && bullet.count <= 248)
        {
            for (int i = 0; i < MAX_SHOT; i++)
            {
                if (bullet.shot[i].stts < USE)
                {
                    bullet.shot[i] = SetShotImage(9, bullet.emit, bullet.angle, bullet.color);
                    break;
                }
            }
        }
        break;

    case 12: //羽レーザー
        if (bullet.count >= 200)
        {
            for (int k = 0; k < 2; k++) //最後尾を出す
            {
                for (int i = 0; i < 5; i++)
                {
                    for (int j = 0; j < MAX_SHOT; j++)
                    {
                        if (bullet.shot[j].stts < USE)
                        {
                            bullet.shot[j] = SetShotImage(14, bullet.emit, bullet.angle + 110 - 220 * k, bullet.color);
                            bullet.shot[j].speed = i + 1;
                            bullet.shot[j].motion += k;
                            break;
                        }
                    }
                }
            }
            bullet.stts = FINISH;
            BF[bullet.number].type = 11; //BFの色を戻す
            break;
        }
        if (!bullet.count) //先頭を出す
        {
            for (int k = 0; k < 2; k++)
            {
                for (int i = 0; i < 5; i++)
                {
                    for (int j = 0; j < MAX_SHOT; j++)
                    {
                        if (bullet.shot[j].stts < USE)
                        {
                            bullet.shot[j] = SetShotImage(13, bullet.emit, bullet.angle + 110 - 220 * k, bullet.color);
                            bullet.shot[j].speed = i + 1;
                            bullet.shot[j].motion += k;
                            break;
                        }
                    }
                }
            }
        }
        if (!(bullet.count % 5) && bullet.count >= 2 && bullet.count <= 198)
        {
            for (int k = 0; k < 2; k++)
            {
                for (int i = 0; i < 5; i++)
                {
                    for (int j = 0; j < MAX_SHOT; j++)
                    {
                        if (bullet.shot[j].stts < USE)
                        {
                            bullet.shot[j] = SetShotImage(12, bullet.emit, bullet.angle + 110 - 220 * k, bullet.color);
                            bullet.shot[j].speed = i + 1;
                            bullet.shot[j].motion += k;
                            break;
                        }
                    }
                }
            }
        }
        break;

    case 13: //マーキュリーポイズン
        if (bullet.count > 300)
        {
            bullet.stts = FINISH;
            break;
        }
        if (!(bullet.count % 50)) //時計回り
        {
            int l = 0;
            for (int k = 0; k < 2; k++)
            {
                for (int i = 0; i < 8; i++)
                {
                    for (int j = l; j < MAX_SHOT; j++)
                    {
                        if (bullet.shot[j].stts < USE)
                        {
                            bullet.shot[j] = SetShotImage(15, bullet.emit, bullet.angle + 45 * i, bullet.color);
                            bullet.shot[j].accel *= k * 0.1 + 1;
                            bullet.shot[j].Aspeed *= k * 0.09 + 1;
                            l = j; //ここまで探した
                            break;
                        }
                    }
                }
            }
        }
        if (!((bullet.count + 25) % 50)) //半時計回り
        {
            int l = 0;
            for (int k = 0; k < 2; k++)
            {
                for (int i = 0; i < 8; i++)
                {
                    for (int j = l; j < MAX_SHOT; j++)
                    {
                        if (bullet.shot[j].stts < USE)
                        {
                            bullet.shot[j] = SetShotImage(15, bullet.emit, bullet.angle + 45 * i, bullet.color);
                            bullet.shot[j].accel *= k * 0.1 + 1;
                            bullet.shot[j].Aspeed *= k * 0.09 + 1;
                            bullet.shot[j].Aspeed *= -1;
                            l = j; //ここまで探した
                            break;
                        }
                    }
                }
            }
        }
        break;

    case 14: //マーキュリーポイズン　試作2
        if (bullet.count > 300)
        {
            bullet.stts = FINISH;
            break;
        }
        if (!(bullet.count % 50)) //時計回り
        {
            int l = 0;
            for (int k = 0; k < 2; k++)
            {
                for (int i = 0; i < 8; i++)
                {
                    for (int j = l; j < MAX_SHOT; j++)
                    {
                        if (bullet.shot[j].stts < USE)
                        {
                            bullet.shot[j] = SetShotImage(15, bullet.emit, bullet.angle + 45 * i, bullet.color);
                            bullet.shot[j].accel *= k * 0.1 + 1;
                            bullet.shot[j].motion = 10;
                            bullet.shot[j].Aspeed *= k * 0.09 + 1;
                            l = j; //ここまで探した
                            break;
                        }
                    }
                }
            }
        }
        if (!((bullet.count + 25) % 50)) //半時計回り
        {
            int l = 0;
            for (int k = 0; k < 2; k++)
            {
                for (int i = 0; i < 8; i++)
                {
                    for (int j = l; j < MAX_SHOT; j++)
                    {
                        if (bullet.shot[j].stts < USE)
                        {
                            bullet.shot[j] = SetShotImage(15, bullet.emit, bullet.angle + 45 * i, bullet.color);
                            bullet.shot[j].accel *= k * 0.1 + 1;
                            bullet.shot[j].motion = 10;
                            bullet.shot[j].Aspeed *= k * 0.09 + 1;
                            bullet.shot[j].Aspeed *= -1;
                            l = j; //ここまで探した
                            break;
                        }
                    }
                }
            }
        }
        break;

    case 15: //破裂弾
        if (bullet.count == 500)
        {
            bullet.stts = FINISH;
            BF[bullet.number].type = 11;
            break;
        }
        if (!(bullet.count % 5))
        {
            int k = 0;
            double randomAngle = (rand() % 20) - 10;
            float randomSpeed = rand() % 400000;
            for (int i = 0; i < 6; i++)
            {
                for (int j = k; j < MAX_SHOT; j++)
                {
                    if (bullet.shot[j].stts < USE)
                    {
                        bullet.shot[j] = SetShotImage(16, bullet.emit, (bullet.angle + randomAngle) - 100 + (bullet.count / 1), bullet.color);
                        bullet.shot[j].RenderData.angle += 60 * i;
                        bullet.shot[j].speed = randomSpeed / 100000;
                        bullet.shot[j].motion += i;
                        k = j;
                        break;
                    }
                }
            }
        }
        break;

    case 16:;                   //逆回転発狂弾幕 必殺技
        static float shift = 0; //一度の回転量
        static int enter;       //弾の発生速度
        if (bullet.count == 2800)
        {
            bullet.count = 0;
            //bullet.stts = FINISH;
            //break;
        }
        //回転の速さを決める
        if (!bullet.count)
        {
            bullet.angle -= 5;
            shift = 0.15254684 * 8;
            enter = 1024 / 8;
            bullet.emit.x = WINDOW_WIDE / 2;
            bullet.emit.y = WINDOW_HIGH / 2;
        }
        if (!(bullet.count % 400))
        {
            shift *= -2;
            shift += (rand() % 10) / 100;
            enter /= 2;
        }
        bullet.angle += shift;
        if (!(bullet.count % enter))
        {
            int k = 0;
            for (int j = 0; j < 4; j++)
            {
                for (int i = k; i < MAX_SHOT; i++)
                {
                    if (bullet.shot[i].stts < USE)
                    {
                        bullet.shot[i] = SetShotImage(17, bullet.emit, bullet.angle + 90 * j, bullet.color);
                        k = i;
                        break;
                    }
                    if (i == MAX_SHOT)
                        printf("error!!\n");
                }
            }
        }
        break;

    case 17:; //一定回転発狂弾幕
        // static float shift = 0; //一度の回転量
        // static int enter;       //弾の発生速度
        if (bullet.count == 1500)
        {
            bullet.stts = FINISH;
            break;
        }
        //回転の速さを決める
        if (!bullet.count)
        {
            shift = 0.1;
            enter = 64;
        }
        if (!(bullet.count % 300))
        {
            enter /= 2;
        }
        bullet.angle += shift += 0.01;
        if (!(bullet.count % enter))
        {
            int k = 0;
            for (int j = 0; j < 6; j++)
            {
                for (int i = k; i < MAX_SHOT; i++)
                {
                    if (bullet.shot[i].stts < USE)
                    {
                        bullet.shot[i] = SetShotImage(17, bullet.emit, bullet.angle + 60 * j, bullet.color);
                        k = i;
                        break;
                    }
                }
            }
        }
        break;

    case 18: //軌道砲弾
        if (bullet.count == 500)
        {
            bullet.stts = FINISH;
            BF[bullet.number].type = 11;
            break;
        }
        if (!bullet.count) //エミッタ初期位置
        {
            bullet.emit.x = bullet.base.x + cosf(FREQUENCY_2_RADIAN(bullet.angle + 135)) * 300;
            bullet.emit.y = bullet.base.y + sinf(FREQUENCY_2_RADIAN(bullet.angle + 135)) * 300;
        }
        else if (bullet.count == 251)
        {
            bullet.emit.x = bullet.base.x + cosf(FREQUENCY_2_RADIAN(bullet.angle + 45)) * 300;
            bullet.emit.y = bullet.base.y + sinf(FREQUENCY_2_RADIAN(bullet.angle + 45)) * 300;
        }
        else if (bullet.count <= 250)
        {
            bullet.emit.x += cosf(FREQUENCY_2_RADIAN(bullet.angle - 60)) * 4; //毎回の移動
            bullet.emit.y += sinf(FREQUENCY_2_RADIAN(bullet.angle - 60)) * 4; //毎回の移動
        }
        else
        {
            bullet.emit.x += cosf(FREQUENCY_2_RADIAN(bullet.angle - 120)) * 4; //毎回の移動
            bullet.emit.y += sinf(FREQUENCY_2_RADIAN(bullet.angle - 120)) * 4; //毎回の移動
        }
        if (!(bullet.count % 10))
        {
            int k = 0;
            for (int i = 0; i < 20; i++)
            {
                for (int j = k; j < MAX_SHOT; j++)
                {
                    if (bullet.shot[j].stts < USE)
                    {
                        bullet.shot[j] = SetShotImage(17, bullet.emit, bullet.angle + 18 * i + (bullet.count / 4), bullet.color);
                        break;
                    }
                }
            }
        }
        break;
    case 19: //扇状まっすぐ飛ぶ氷弾
        if (bullet.count == 151)
        {
            bullet.stts = FINISH;
            BF[bullet.number].type = 11;
            break;
        }
        if (!bullet.count)
            bullet.emit = bullet.base; //弾の生成場所設定
        if (!(bullet.count % 15))
        {
            int k = 0;
            for (int i = 0; i < 9; i++) //10発撃つ
            {
                for (int l = 0; l < 3; l++)
                {
                    for (int j = k; j < MAX_SHOT; j++)
                    {
                        if (bullet.shot[j].stts < USE)
                        {
                            bullet.shot[j] = SetShotImage(18, bullet.emit, bullet.angle + ((i - 4) * 15) + ((l - 1) * 3), bullet.color);
                            k = j; //ここまで調べた
                            break;
                        }
                    }
                }
            }
        }
        break;

    case 20: //扇状まっすぐ飛ぶ氷弾
        if (bullet.count == 151)
        {
            bullet.stts = FINISH;
            BF[bullet.number].type = 11;
            break;
        }
        if (!bullet.count)
        {
            int k = 0;
            for (int l = 0; l < 3; l++)
            {
                for (int j = 0; j < 72; j++)
                {
                    for (int i = k; i < MAX_SHOT; i++)
                    {
                        if (bullet.shot[i].stts < USE)
                        {
                            bullet.shot[i] = SetShotImage(19, bullet.emit, bullet.angle + 5 * j, bullet.color);
                            bullet.shot[i].speed += l;
                            k = i;
                            break;
                        }
                    }
                }
            }
        }
        break;

    case 21: //すわこ弾
        if (bullet.count == 950)
        {
            bullet.stts = FINISH;
            BF[bullet.number].type = 11;
            break;
        }

        if (!(bullet.count % 200))
        {
            int l = 0;
            for (int k = 0; k < 2; k++)
            {
                for (int j = 0; j < 64; j++)
                {
                    for (int i = l; i < MAX_SHOT; i++)
                    {
                        if (bullet.shot[i].stts < USE)
                        {
                            bullet.shot[i] = SetShotImage(20, bullet.emit, bullet.angle, bullet.color);
                            bullet.shot[i].motion += j + (64 * k);
                            l = i; //ここまで探した
                            break;
                        }
                    }
                }
            }
        }
        break;

    case 22: //反射弾
        if (bullet.count == 300)
        {
            bullet.stts = FINISH;
            BF[bullet.number].type = 11;
            break;
        }
        if (!bullet.count)
        {
            int k = 0;
            for (int j = 0; j < 18; j++) //360/18=20
            {

                for (int i = k; i < MAX_SHOT; i++)
                {
                    if (bullet.shot[i].stts < USE)
                    {
                        bullet.shot[i] = SetShotImage(21, bullet.emit, bullet.angle + (rand() % 20 - 10) + 20 * j, bullet.color);
                        k = i; //ここまで探した
                        break;
                    }
                }
            }
        }
        break;

    case 23: //ばら撒き弾
        if (bullet.count == 300)
        {
            bullet.stts = FINISH;
            BF[bullet.number].type = 11;
            break;
        }
        if (!bullet.count)
        {
            int k = 0;
            for (int i = 0; i < 200; i++)
            {
                for (int j = k; j < MAX_SHOT; j++)
                {
                    if (bullet.shot[j].stts < USE)
                    {
                        bullet.shot[j] = SetShotImage(22, bullet.emit, bullet.angle - 30 + (rand() % 60), bullet.color);
                        k = j;
                        break;
                    }
                }
            }
        }
        break;

    case 24: //感知式地雷用　時限も一緒でいいかな
    case 25: //ダメージが違うので無理
        if (bullet.count == 61)
        {
            bullet.stts = FINISH;
            break;
        }
        if (!(bullet.count % 20))
        {
            int k = 0;
            for (int j = 0; j < 5; j++)
            {
                for (int i = k; i < MAX_SHOT; i++)
                {
                    if (bullet.shot[i].stts < USE)
                    {
                        bullet.shot[i] = SetShotImage(0, bullet.emit, bullet.angle + 72 * j, bullet.color);
                        bullet.shot[i].pow += 5 * (bullet.type - 24);
                        k = i;
                        break;
                    }
                }
            }
            bullet.angle += 36;
        }
        break;

    case 26: //砲台　敵機狙い
        if (bullet.count == 1001)
        {
            bullet.stts = FINISH;
            break;
        }
        if (!(bullet.count % 50))
        {
            for (int i = 0; i < MAX_SHOT; i++)
            {
                if (bullet.shot[i].stts < USE)
                {
                    bullet.shot[i] = SetShotImage(23, bullet.emit, bullet.angle, bullet.color);
                    break;
                }
            }
        }
        break;

    case 27: //砲台　円状
        if (bullet.count == 1001)
        {
            bullet.stts = FINISH;
            break;
        }
        if (!(bullet.count % 100))
        {
            int k = 0;
            double random = rand() % 3600;
            for (int i = 0; i < 32; i++)
            {
                for (int j = k; j < MAX_SHOT; j++)
                {
                    if (bullet.shot[j].stts < USE)
                    {
                        bullet.shot[j] = SetShotImage(23, bullet.emit, bullet.angle + 11.25 * i + random / 10, bullet.color);
                        bullet.shot[i].pow = 5;
                        break;
                    }
                }
            }
        }
        break;

    case 28: //小弾幕
        if (bullet.count == 201)
        {
            bullet.stts = FINISH;
        }
        if (!(bullet.count % 30) && bullet.count <= 100)
        {
            for (int i = 0; i < MAX_SHOT; i++)
            {
                if (bullet.shot[i].stts < USE)
                {
                    bullet.shot[i] = SetShotImage(24, bullet.emit, bullet.angle, bullet.color);
                    break;
                }
            }
        }
        break;

    default:
        break;
    }

    bullet.count++; //カウントアップ
    return bullet;
}