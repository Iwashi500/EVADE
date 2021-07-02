/***************************************************************
 * ウィンドウに表示させるためのファイル
 * メインからレンダラー、ゲームの構造体を取ってくる
 * 
*****************************************************************/

#include "../../game.h"

//必要な関数
//RENDERDATA SetCharaImage(int type); //キャラの画像の準備
//BULLET SetBullet(void); //弾幕の準備
//SHOT SetShotImage(int type, RENDERDATA chara); //弾の画像の準備
void SetTexture(void); //テクスチャの準備
void SetWord(void);    //文字の準備

//取ってくる関数

//必要なグローバル変数

//取ってくるグローバル変数
extern SDL_Renderer *renderer;
extern SDL_Texture *texture[TEXTURES]; //テクスチャを入れておく配列
extern TTF_Font *font;                 //文字1
extern TTF_Font *font2;                //文字2

/****************************************************************
 * 関数名      void SetTexture(void)
 * 機能       テクスチャをまとめて読み込む
 * 引数       なし
 * 戻り値      なし
*****************************************************************/
void SetTexture(void)
{
    SDL_Surface *surface; //サーフェス
    //自機赤
    surface = IMG_Load("images/chara/red_jiki2.png");
    texture[0] = SDL_CreateTextureFromSurface(renderer, surface);
    //自機青
    surface = IMG_Load("images/chara/blue2_jiki2.png");
    texture[1] = SDL_CreateTextureFromSurface(renderer, surface);
    //カラフルな小丸弾
    surface = IMG_Load("images/shot/small_shot.png");
    texture[2] = SDL_CreateTextureFromSurface(renderer, surface);
    //青大玉
    surface = IMG_Load("images/shot/big_bule_shot.png");
    texture[3] = SDL_CreateTextureFromSurface(renderer, surface);
    //座布団
    surface = IMG_Load("images/shot/PShot.png");
    texture[4] = SDL_CreateTextureFromSurface(renderer, surface);
    //背景大
    //surface = IMG_Load("images/bg/circle2.png");
    surface = IMG_Load("images/bg/background_l.png");
    texture[5] = SDL_CreateTextureFromSurface(renderer, surface);
    // //背景中
    // surface = IMG_Load("images/bg/background_m.png");
    // texture[6] = SDL_CreateTextureFromSurface(renderer, surface);
    //背景小
    //surface = IMG_Load("images/bg/background_s.png");
    //texture[7] = SDL_CreateTextureFromSurface(renderer, surface);
    //背景ピック
    surface = IMG_Load("images/bg/background.png");
    texture[8] = SDL_CreateTextureFromSurface(renderer, surface);
    //弾幕選択背景
    surface = IMG_Load("images/bg/Choiceske.png");
    texture[9] = SDL_CreateTextureFromSurface(renderer, surface);
    //弾幕選択背景ピック
    surface = IMG_Load("images/bg/frame_choice.png");
    texture[10] = SDL_CreateTextureFromSurface(renderer, surface);
    //弾幕フレーム
    surface = IMG_Load("images/frame/BulletFrame.png");
    texture[11] = SDL_CreateTextureFromSurface(renderer, surface);
    //弾幕フレーム赤
    surface = IMG_Load("images/frame/BulletFrame_choice.png");
    texture[12] = SDL_CreateTextureFromSurface(renderer, surface);
    //弾幕フレーム大
    surface = IMG_Load("images/frame/BulletFrame_cover.png");
    texture[13] = SDL_CreateTextureFromSurface(renderer, surface);
    //プレイヤーカーソル
    surface = IMG_Load("images/frame/Cursor.png");
    texture[14] = SDL_CreateTextureFromSurface(renderer, surface);
    //HP、SPバー
    surface = IMG_Load("images/frame/HP&SP.png");
    texture[15] = SDL_CreateTextureFromSurface(renderer, surface);
    //アイコン
    surface = IMG_Load("images/icon/Numberske.png");
    texture[16] = SDL_CreateTextureFromSurface(renderer, surface);
    //赤大玉
    surface = IMG_Load("images/shot/big_red_shot.png");
    texture[17] = SDL_CreateTextureFromSurface(renderer, surface);
    //氷弾
    surface = IMG_Load("images/shot/ice_shot.png");
    texture[18] = SDL_CreateTextureFromSurface(renderer, surface);
    //楔弾
    surface = IMG_Load("images/shot/kusabi.png");
    texture[19] = SDL_CreateTextureFromSurface(renderer, surface);
    //回復エリアサークル
    surface = IMG_Load("images/frame/area.png");
    texture[20] = SDL_CreateTextureFromSurface(renderer, surface);
    //地雷　赤
    surface = IMG_Load("images/frame/zirai3.png");
    texture[96] = SDL_CreateTextureFromSurface(renderer, surface);
    //地雷　青
    surface = IMG_Load("images/frame/zirai2.png");
    texture[100] = SDL_CreateTextureFromSurface(renderer, surface);
    //タイトル
    surface = IMG_Load("images/icon/title_red.png");
    texture[101] = SDL_CreateTextureFromSurface(renderer, surface);
    //プレイヤーカーソル青
    surface = IMG_Load("images/frame/Cursor_b.png");
    texture[107] = SDL_CreateTextureFromSurface(renderer, surface);
    //サークル白
    surface = IMG_Load("images/frame/Cursor_w.png");
    texture[108] = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface); //終了処理

    printf("texture complete\n");
}

/****************************************************************
 * 関数名      SetWord
 * 機能       文字の準備を行う
 * 引数       なし
 * 戻り値     なし
*****************************************************************/
void SetWord(void)
{
    //弾幕の説明文は、EX: [20文字]　となるように最後に空白で調整している
    //サポートは21文字

    SDL_Surface *surface; //サーフェス

    //4321 カウントダウン用
    surface = TTF_RenderUTF8_Blended(font, "321", (SDL_Color){0, 0, 0, 255});
    texture[6] = SDL_CreateTextureFromSurface(renderer, surface);
    //ピック画面のターン表示
    surface = TTF_RenderUTF8_Blended(font2, "your turn enemy turn", (SDL_Color){0, 0, 0, 255});
    texture[7] = SDL_CreateTextureFromSurface(renderer, surface);

    //弾幕1　翼　17
    surface = TTF_RenderUTF8_Blended(font, "SP: 小", (SDL_Color){0, 0, 0, 255});
    texture[21] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "PW: 小", (SDL_Color){0, 0, 0, 255});
    texture[22] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 相手に向かって2本のレーザーを発射   ", (SDL_Color){0, 0, 0, 255});
    texture[23] = SDL_CreateTextureFromSurface(renderer, surface);

    //弾幕2　扇　15
    surface = TTF_RenderUTF8_Blended(font, "SP: 中", (SDL_Color){0, 0, 0, 255});
    texture[24] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "PW: 中", (SDL_Color){0, 0, 0, 255});
    texture[25] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 相手に向かって扇状の弾幕を発射     ", (SDL_Color){0, 0, 0, 255});
    texture[26] = SDL_CreateTextureFromSurface(renderer, surface);

    //弾幕3　拡　14
    surface = TTF_RenderUTF8_Blended(font, "SP: 中", (SDL_Color){0, 0, 0, 255});
    texture[27] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "PW: 大", (SDL_Color){0, 0, 0, 255});
    texture[28] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 円状に拡散していく弾幕を発射      ", (SDL_Color){0, 0, 0, 255});
    texture[29] = SDL_CreateTextureFromSurface(renderer, surface);

    //弾幕4　反　18
    surface = TTF_RenderUTF8_Blended(font, "SP: 中", (SDL_Color){0, 0, 0, 255});
    texture[30] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "PW: 大", (SDL_Color){0, 0, 0, 255});
    texture[31] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 壁で反射する弾をランダムな方向に発射  ", (SDL_Color){0, 0, 0, 255});
    texture[32] = SDL_CreateTextureFromSurface(renderer, surface);

    //弾幕5　散　16
    surface = TTF_RenderUTF8_Blended(font, "SP: 小", (SDL_Color){0, 0, 0, 255});
    texture[33] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "PW: 小", (SDL_Color){0, 0, 0, 255});
    texture[34] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 相手に向かってばらまき弾幕を発射    ", (SDL_Color){0, 0, 0, 255});
    texture[35] = SDL_CreateTextureFromSurface(renderer, surface);

    //弾幕6　止　19
    surface = TTF_RenderUTF8_Blended(font, "SP: 中", (SDL_Color){0, 0, 0, 255});
    texture[36] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "PW: 中", (SDL_Color){0, 0, 0, 255});
    texture[37] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 一定時間経つと静止する弾を全方向に発射 ", (SDL_Color){0, 0, 0, 255});
    texture[38] = SDL_CreateTextureFromSurface(renderer, surface);

    //弾幕7　星　17
    surface = TTF_RenderUTF8_Blended(font, "SP: 大", (SDL_Color){0, 0, 0, 255});
    texture[39] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "PW: 大", (SDL_Color){0, 0, 0, 255});
    texture[40] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 前方に爆発する弾と星状の弾幕を発射   ", (SDL_Color){0, 0, 0, 255});
    texture[41] = SDL_CreateTextureFromSurface(renderer, surface);

    //弾幕8　爆　12
    surface = TTF_RenderUTF8_Blended(font, "SP: 中", (SDL_Color){0, 0, 0, 255});
    texture[42] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "PW: 大", (SDL_Color){0, 0, 0, 255});
    texture[43] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 爆発する弾を全方向に発射        ", (SDL_Color){0, 0, 0, 255});
    texture[44] = SDL_CreateTextureFromSurface(renderer, surface);

    //弾幕9　流　18
    surface = TTF_RenderUTF8_Blended(font, "SP: 大", (SDL_Color){0, 0, 0, 255});
    texture[45] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "PW: 中", (SDL_Color){0, 0, 0, 255});
    texture[46] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 拡散弾を流れるように生成する弾を発射  ", (SDL_Color){0, 0, 0, 255});
    texture[47] = SDL_CreateTextureFromSurface(renderer, surface);

    //弾幕10　交　16
    surface = TTF_RenderUTF8_Blended(font, "SP: 大", (SDL_Color){0, 0, 0, 255});
    texture[48] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "PW: 中", (SDL_Color){0, 0, 0, 255});
    texture[49] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 交差しながら円状に広がる弾を発射    ", (SDL_Color){0, 0, 0, 255});
    texture[50] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート1　罠　1　11
    surface = TTF_RenderUTF8_Blended(font, "CT: 小", (SDL_Color){0, 0, 0, 255});
    texture[51] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: ー", (SDL_Color){0, 0, 0, 255});
    texture[52] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 近づくと作動し爆発する           ", (SDL_Color){0, 0, 0, 255});
    texture[53] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート2　爆弾　2　15
    surface = TTF_RenderUTF8_Blended(font, "CT: 中", (SDL_Color){0, 0, 0, 255});
    texture[54] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: 中", (SDL_Color){0, 0, 0, 255});
    texture[55] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 一定時間経過後に作動し爆発する      ", (SDL_Color){0, 0, 0, 255});
    texture[56] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート3　砲台(自機) 7　20
    surface = TTF_RenderUTF8_Blended(font, "CT: 小", (SDL_Color){0, 0, 0, 255});
    texture[69] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: 大", (SDL_Color){0, 0, 0, 255});
    texture[70] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 相手に向かって弾を発射する砲台を設置する ", (SDL_Color){0, 0, 0, 255});
    texture[71] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート4　砲台(円) 8　18
    surface = TTF_RenderUTF8_Blended(font, "CT: 小", (SDL_Color){0, 0, 0, 255});
    texture[72] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: 大", (SDL_Color){0, 0, 0, 255});
    texture[73] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 円状に弾を発射する砲台を設置する     ", (SDL_Color){0, 0, 0, 255});
    texture[74] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート5　味方SP 3　13
    surface = TTF_RenderUTF8_Blended(font, "CT: 小", (SDL_Color){0, 0, 0, 255});
    texture[57] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: 小", (SDL_Color){0, 0, 0, 255});
    texture[58] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: スタミナの回復速度を上げる        ", (SDL_Color){0, 0, 0, 255});
    texture[59] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート6　味方弾 4　13
    surface = TTF_RenderUTF8_Blended(font, "CT: 小", (SDL_Color){0, 0, 0, 255});
    texture[60] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: 小", (SDL_Color){0, 0, 0, 255});
    texture[61] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 味方弾のダメージ量を増やす        ", (SDL_Color){0, 0, 0, 255});
    texture[62] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート7　範囲 9 20
    surface = TTF_RenderUTF8_Blended(font, "CT: 中", (SDL_Color){0, 0, 0, 255});
    texture[75] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: 中", (SDL_Color){0, 0, 0, 255});
    texture[76] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 味方のHPが回復するが被ダメージが増える ", (SDL_Color){0, 0, 0, 255});
    texture[77] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート8　必殺技 10 21
    surface = TTF_RenderUTF8_Blended(font, "CT: ー", (SDL_Color){0, 0, 0, 255});
    texture[78] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: ー", (SDL_Color){0, 0, 0, 255});
    texture[79] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 味方のHP50％以下の時に強力な弾幕が発動", (SDL_Color){0, 0, 0, 255});
    texture[80] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート9　敵SP 5　16
    surface = TTF_RenderUTF8_Blended(font, "CT: 小", (SDL_Color){0, 0, 0, 255});
    texture[63] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: 小", (SDL_Color){0, 0, 0, 255});
    texture[64] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 相手のスタミナの回復速度を下げる     ", (SDL_Color){0, 0, 0, 255});
    texture[65] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート10　暗闇 6　14
    surface = TTF_RenderUTF8_Blended(font, "CT: 大", (SDL_Color){0, 0, 0, 255});
    texture[66] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: 中", (SDL_Color){0, 0, 0, 255});
    texture[67] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 相手の周囲以外を見えなくする       ", (SDL_Color){0, 0, 0, 255});
    texture[68] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート11　移動変化　11　18
    surface = TTF_RenderUTF8_Blended(font, "CT: 大", (SDL_Color){0, 0, 0, 255});
    texture[81] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: 中", (SDL_Color){0, 0, 0, 255});
    texture[82] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 相手の移動速度をランダムに変化させる   ", (SDL_Color){0, 0, 0, 255});
    texture[83] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポート12　移動反転　12　13
    surface = TTF_RenderUTF8_Blended(font, "CT: 特", (SDL_Color){0, 0, 0, 255});
    texture[84] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "ET: 中", (SDL_Color){0, 0, 0, 255});
    texture[85] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "EX: 相手の移動方向を反転させる        ", (SDL_Color){0, 0, 0, 255});
    texture[86] = SDL_CreateTextureFromSurface(renderer, surface);

    //please wait & countdown
    surface = TTF_RenderUTF8_Blended(font2, "please wait 4321 press any button", (SDL_Color){0, 0, 0, 255});
    texture[87] = SDL_CreateTextureFromSurface(renderer, surface);

    //勝敗の結果
    surface = TTF_RenderUTF8_Blended(font2, "you win you lose", (SDL_Color){0, 0, 0, 255});
    texture[88] = SDL_CreateTextureFromSurface(renderer, surface);

    //サポートの分類文字 Attack
    surface = TTF_RenderUTF8_Blended(font, "Attack", (SDL_Color){255, 255, 255, 255});
    texture[89] = SDL_CreateTextureFromSurface(renderer, surface);
    //サポートの分類文字 Support
    surface = TTF_RenderUTF8_Blended(font, "Support", (SDL_Color){0, 0, 255, 255});
    texture[90] = SDL_CreateTextureFromSurface(renderer, surface);
    //サポートの分類文字 Jamming
    surface = TTF_RenderUTF8_Blended(font, "Jamming", (SDL_Color){255, 0, 0, 255});
    texture[91] = SDL_CreateTextureFromSurface(renderer, surface);

    //弾幕文字中
    surface = TTF_RenderUTF8_Blended(font, "翼扇拡反散", (SDL_Color){0, 255, 255, 255});
    texture[92] = SDL_CreateTextureFromSurface(renderer, surface);
    //弾幕文字大
    surface = TTF_RenderUTF8_Blended(font, "止星爆流交", (SDL_Color){255, 0, 255, 255});
    texture[93] = SDL_CreateTextureFromSurface(renderer, surface);
    //サポート攻撃文字
    //surface = TTF_RenderUTF8_Blended(font, "罠爆単円", (SDL_Color){255, 255, 255, 255});
    surface = TTF_RenderUTF8_Blended(font, "地雷砲台", (SDL_Color){255, 255, 255, 255});
    texture[97] = SDL_CreateTextureFromSurface(renderer, surface);
    //サポート援護文字
    surface = TTF_RenderUTF8_Blended(font, "SP弾範必", (SDL_Color){0, 0, 255, 255});
    texture[98] = SDL_CreateTextureFromSurface(renderer, surface);
    //サポート妨害文字
    surface = TTF_RenderUTF8_Blended(font, "SP闇変反", (SDL_Color){255, 0, 0, 255});
    texture[99] = SDL_CreateTextureFromSurface(renderer, surface);
    //youの文字
    surface = TTF_RenderUTF8_Blended(font, "you↓", (SDL_Color){255, 0, 0, 255});
    texture[102] = SDL_CreateTextureFromSurface(renderer, surface);
    //Teamの文字
    surface = TTF_RenderUTF8_Blended(font2, "Main Support", (SDL_Color){255, 255, 255, 255});
    texture[103] = SDL_CreateTextureFromSurface(renderer, surface);
    //REDの文字
    surface = TTF_RenderUTF8_Blended(font2, "RED", (SDL_Color){255, 0, 0, 255});
    texture[104] = SDL_CreateTextureFromSurface(renderer, surface);
    //BLUEの文字
    surface = TTF_RenderUTF8_Blended(font2, "BLUE", (SDL_Color){0, 0, 255, 255});
    texture[105] = SDL_CreateTextureFromSurface(renderer, surface);
    //VSの文字
    surface = TTF_RenderUTF8_Blended(font, "VS", (SDL_Color){255, 255, 255, 255});
    texture[106] = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface); //終了処理
    printf("word complete\n");
}