/*
angle　は0~360
point　は回転の軸を貼り付けた画像の右上からどれだけの位置に置くか
flip　はSDL_FLIP_NONEで反転なし
    　 SDL_FLIP_HORIZONTALで画像反転

操作可能にした

ヘッダファイル　game.h　を作った

星が相手の方向を向くようにした
アリスも向くように変更
キャラ２体分の配列の構造体に変更
左シフトキーでアリス操作
右シフトキーで星操作に切り替え可能
スレッド挑戦（いけそう）

スレッドが操作をメイン、スレッドをキー操作にするとうまくいった
ボタン操作の整理
キャラ、サポートは人数分（４つ）用意していた方がやりやすいかも

SDL_Pointは常にキャラの中心を指している
キャラの周りに線を引けた

一辺についての右側だけ書けた
あとはこれを各ベクトルに行う

当たり判定を全く別にした
各辺の法線ベクトルに各頂点を投影する
できた！！

電算室環境で動かなかった
主にSDL_RenderPresentが動いていない
イベント関数を入れ替えたら行けた
しかたなし変数kankyoで切り替える

タイマー関数できた？
多分
タイマー関数でウィンドウ表示ができなかった
タイマーの秒数の変更はできた
macでは未検証！！

10/30
makefileで自動に分けるようにした
ファイル整理をした
なんとなくやけどいけるやろう
これはメインファイルになる

10/31
作り直した構造体で作成をしていく
メインは直した
ウィンドウも終わった
システムも終わったぽい
あとはヒット
ヒット終わった
イベント忘れてた
イベントのCHARAいらなかった
できたが画像読み込みが謎
もしかしたら実行した時の位置なのかもしれない
画面サイズを1850*1000に仮決定
弾の表示した
弾の謎バグ発生
色が変わる
意味がわからん！！
しらん

11/2
DelayつけたらPCの負荷が下がった？
当たり判定の点と四角を作った
弾動かした

11/5
SDL2の仕様として
・画像の一部を切り取る
・切り取った画像の元の縮尺のまま貼り付ける
を行うと不具合が生じる
弾をキャラから出るようにした
弾幕を作る部分の整理
弾の画面外判定をつくる

11/7
弾幕2が出ない理由判明
画像の大きさいじってなかった
角速度、各加速度の設定をしなかったら0にならないことがある
きちんと初期設定する
idを自分、味方、敵の3つに変更
サポートは敵のidを−1にしておくことで判別
ネットにつなげる以外の分割はできたと思う
次はクライアント側
makefileはコンパイルのみにした

11/11
サーバ側はタイマー関数1つ、スレッド1つの予定
タイマー関数でネット送信
スレッドでネット受信
メイン関数で処理の予定
ネット部分は作ったからあとは合わせるだけ
kadaiから切り取っていく

11/12
ネットと実際に合わせる
とりあえず合わせてコンパイルできるようにmakefile直した
初期設定をする
つなげるだけはできた
データが送れているのを確認
クライアントの終了でサーバも終わるようにした
タイマー関数でコマンドを送ることに成功した
RenderData型を送ることに成功した

11/15
画面にアリスを表示させた
サーバ側で変えたら位置が変わるのも確認済み
表示がチカチカする
一度貼り付けてから次が送られてくる前にもう一度貼り付けているのが原因？
送るものが多くなるとなくなるかも
何個か送ってみて様子見る
FPSを早めたらよりチカチカした
逆に遅めたらなくなった
原因違うかも？

11/16.
チカチカは止めた
終了処理でコアダンプが起こる
ウィンドウ終了とキャラのテクスチャ破棄で起こる
とりあえずはなくていいかな？              TAに聞くかも？
問題多発
・クライアント側のスレッドが勝手に終了する　メインループも抜ける
・ボタンが正しく送れていない　途中から送れる場合がある
・一定時間後から遅延の発生が著しい
わからん！！
過去データ検証
yabai
・長時間続けてもメインループの抜けはない
・ボタンを複数回押していると終了時にクライアントだけ終わらない
・サーバ側は通常通り終わるのでデータの送信はできていると思う
net
・ボタンを何回押しても通常通り終了する
・そもそもネットに繋がっていないし比較にならないかな？

11/17
・クライアントが落ちる前に終了してもサーバ側が終了しない　
・クライアントのウィンドウを終了、もしくは強制終了させるとサーバ側が終わらずに受信を繰り返している
解決項目
・RenderData配列の範囲外まで参照していたのが勝手に終了した原因ぽい
・同時にボタンを押してもクライアントが終了しなくなることもなくなった
・押したボタンが初めは正しく通信できていないのは直っていない
・dataSizeの増加をBOTTAN型に直していなかった
・BOTTANも送れていること確認
・あとは終了時にサーバ側が終わらない問題
・終了コマンドをクライアントが送ってもchar型の読み込みをしていなかったので意味がなかったのが問題
・解決
終了コマンドは-1としておく
ネット繋いでキャラが動くようにした！！！！！
やったーーーー
必要なもの全部送ったらラグ大発生
しかも時間が立つごとに大きくなっていく
一体だけしか送っていないときは60fpsでもヌルヌル動く
でも終了コマンドはすぐに反応しているからただのネットでのラグとは違うかもしれない
printfにて調べたところ
クライアントのボタン送信、サーバでの受信、演算にラグは無い
タイマー関数の中もラグはなさそう
サーバから送るところにラグ発生の原因がある？
もしくはクライアントの受信、表示
一応弾が送れていることも確認
クライアント側が受信するときにはラグが発生している
ボタンのやり取りにはラグが無いのになぜ？？
追いついているときはちらつきもないからなにか無駄なものが大量に送られている？

11/18
rendererを送る挑戦
失敗
SDL_Rendererが可変長型？
dataの容量を増やせなかった
クライアントのネットワーク受信に1ループごとにDelayが入っていた
これが原因


11/19
ネットにつなげた
クライアント数とサーバのIPアドレス入力を引数でとる
引数なしのときはクライアント1人、localhostでやる
_Boolのバイト数1だった
SDL_boolを_Boolに変更した
マクロでtrue,falseを使えるようにした

11/22
必要画像の読み込みをした
バトル背景だけ表示させた
謎のラグ再発生

11/23
謎のラグは背景が大きすぎるのが原因と判明
小さく切り取っても回転させたらだめだった
回転をさせなければラグはなくなるがそれならば一個の背景と変わらない
とりあえず背景は今は一個で回転無しで行く
終了フラグ関係を調整
escキーでモード移行
最後まで行くとクライアントから終了合図が送られるようにした
メインピック画面は作った
カーソルはまだ動かない

11/24
バトル画面の作成はできた
当たり判定等はないので動きはしない
あとはサポートピックとカーソルを動かす
それとクライアント別に送るものを決める方法
サポートピックはできた
カーソルが動くようにした
決まったクライアントにだけ送るもできた
ピック画面のカーソルをプレイヤーそれぞれで動かせるようにした
フレーム等の計算方法を変更
ifを使わずに計算だけでやった
いみはあるのかな？
短くなって見やすくはなった
けどひと目ではわかりにくいかも
弾幕の構造を変更
macと同じに直した
一度に弾を2つだすやり方を模索中
SPバーを減少、回復をするようにした
まだ１P側しか変化はしない

11/25
ピック画面を自動で分けるようにした
星のスタミナも減るようにした
弾幕の種類
・円状
・バラマキ
・扇
・3way
・サイン波弾
までできた
パーフェクトフリーズもどきができた
やっぱり完全なパーフェクトフリーズを作るにはSetShotに引数を追加せざるをえないか
追加せずにできたわ


11/26
魔法陣作成（仮）
エミッタの実験成功？
ハート型弾幕できた
これはこれでいいかも？
エミッタはできてた
弾の移動方法を修正
横向きながら移動していた

11/27
五芒星作った
動き出しを弾幕操作で制御している
星を作ってからもう一度星型に変化する
別パターンもつくってみたい

11/28
五芒星を連続で出すと弾が歪む
弾幕全体の操作で弾を移動させているからだと思う
motinに-1の場合を追加して解決
-1のときは0と同じだが弾幕を動かすときのフラグ等で使う
ManageShotでは0と同じ、ManageBullet用のフラグとする
ゾンビクロー作った
あと速度制限と二回発射を作れば完成
調整は難しかった

11/30
ゾンビクローのスピード上限と2回のループを作った

12/2
弾から弾発射の藍様の弾幕作った


12/3
五芒星を5個に分裂するようにした

12/5
キャラの移動制限を作った
マクロを作ってM_PIをなくした

12/6
当たり判定を有効にした
サポートが敵のメインをわからないのが問題になった
サポートを作ってないからまだメインのid_fがいるかもわからない
とりあえずサポートは
味方の敵を見ることで無理やりやった
素直にサポートのid_eを使ってメイン・サポートを分けるフラグをつくるか、
サポートのid_eを-1固定ではなく、クライアント番号の符号を変えて使うときは絶対値をとるか
0やったらできないやーん
とりあえず試験的にフラグを新しく作った
まだ宣言のとこ以外は直していない
このままやとおそらくバグる
直した
当たり判定完成？
当たっても弾は消えないようにした
消えたら不格好
HPの減少によってバーはまだ変わらない

12/7
当たり判定によりHPバーが変化するようにした
色の変化もつけた
HPがマイナスまで行くと吹っ切れていくが実際は０になったらゲームが終了なので関係ないかな
へにょりレーザーぽいの作った
大玉の中央を切り取って連続で貼り付けただけ
見た目はおかしくない
先頭と最後尾にはそれぞれ大玉の上半分、下半分を出している
ピック画面のフレーム選択を作る

12/9
ピック画面関係を作った
まだメインのところだけ
0番目しか色は変わらない
サポート用に新しく変数が必要かも
とりあえず置いておくことにする

12/10
五芒星と藍様の弾幕を合わせて一つにした
結構強そう
そして特攻作業
エミッタの動きとかこだわってみたい
こだわった

12/12
羽レーザー作った
レーザー使うと弾数が足りない
角度調整
先頭と最後尾を追加した
マーキュリーポイズン作りたかった
違うのができたけどこれはこれで

12/15
破裂弾をつくった
なんか物足りない
弾数が足りなくなったのでもう1000にした
見栄えは上がったけど見にくいかな？
発狂弾幕を2種類作った
発狂弾幕がよくみたらフィットフルナイトメアだった
こっち使いたい
軌道砲弾とかいうよくわからないのもできた
名前は適当
敵側がほとんどつんでるけど
まいっか

12/17
キーボードのイベント取得を改良した
ばかじゃねーの！
色分け作った
フィットフルナイトメアは必殺技になった
あともう一つは、火の鳥
もしくは、すわこの交差弾にしたい
makefileの合わせは、serverとclientで分けてなかったので無理だった

12/18
スクショでアイコンと見本用の画像を作った
花火と扇をちょうせいした

12/19
クライアントの接続時に名前を打たなくとも良くした

12/20
support.cをつくった
サポート用の構造体を12個作った
サポートの回復エリアを試験的に作った
行けそう
味方攻撃力アップ作った
敵の移動速度ランダム化作った
SP回復速度UP、DOWN作った
調整しなければDOWNがわかりづらいかもしれない

12/21
敵の移動反転を作った
相手の周り暗闇化作った
書き方が汚いけどこれ以上きれいに仕方がわからない
すわこ弾作った
調整がめっちゃむずい
一応これでいけそうやけど大弾幕にしては弱いかな
弾数をこれ以上増やすのは苦労しそう

12/22
すわこ弾調整
スピード落としたら結構良さげ
必殺技つけた
即撃ちでいいかな？
回復エリア内のダメージ増加を作った
必殺技の条件をつけた
10％は厳しすぎるかも
赤ゲージになったらでもいいかもしれない

12/23
反射弾作った
ジョイコンに変更した
ついに
メインピック作った
サポートピック作った

12/25
クリスマスぼっち電算室ｗｗｗ
バトル画面にアイコンが合うようにした
ピック終了時に自動的にバトル画面に進むようにした

12/27
勝敗をつけれるようにした

12/28
ピック画面の方法を大幅に変えた
少しはマシになった
文字を出せるようにした
空白で文字数を合わせたらうまくできそう
自分の番かどうかを出せるようにした
バトル前のカウントダウンも作った
スタート画面でどのクライアントが押しても次に行くようにした

12/29
勝敗の文字が最後に出るようにした
文字が出たあとボタンを押すと終了するようにしている
再戦はまだできていない
ついでに最初の画面にもボタンを押すようにの催促の文字を出した
低速移動ができるようにした

1/7
必殺技の条件をHP20％以下にした
反射弾、ばら撒き弾を仕上げた

1/8
プレイヤーを簡単に変えれるようにした
サポートの分類文字を送れるようにしたけど文字の色を変えるもしれないので保留
多分1つじゃなくて分けたほうがいいかも
とりあえず感知式の地雷が出せるようにした
ATフィールド
当たった判定はつけた
感知式の地雷だからもう少し広めの感知でもいいかも

1/9
地雷の感知式を作った
通信バグを直した

1/10
時限爆弾も作った
謎のコアダンプがあった
多分画像の種類が範囲外のものを指定していた
どこでかがわからない
100番を指定していたときがあった
何故かなくなった
範囲外のものは受け取らないようにしているのでコアダンプはなくなると思う
バレットフレームを弾幕再生中に赤くするなら結局全部初期化しなければいけない
めんどい
やるかは検討中
固定砲台2つ作った
花火のほうが強ない？
やっぱりフォントによって大きさが全く違った
フォントを日本語対応にしたらバラバラになった
やばいかも

1/11
文字の整理
サポートの表示順を変えた
ピック画面の表示も作り終わった
あとは弾幕の調整と説明文だけかな？

1/16
残りやること
・弾幕フレームの色を変えれるようにする
・地雷の色を分ける
・弾幕コストを全体的に上げる
・クールタイムも上げる
サポートの移動妨害のバグは直した

1/17
地雷の色が分けれるようにした

1/21
弾幕の説明文を作成
フォントをそれぞれ変えるかもしれない
それによって文字の大きさがかわるよね
サポートの文字も作った
弾幕のコストを上げた
サポートのクールタイムも作った
一台で4人+サーバを立ち上げると相当重い
やっぱりすわこ弾がひどい
あと必殺技もきつい
フィットフルナイトメアはまだいけるかも
星弾も怪しい
結構問題

1/22
タイトルつけた
弾幕フレームの回転を止めた
とりあえずすわこ弾改良
スピードを早くして、画面外に出る時間も早めた
これでも無理なら弾数減らすかな
stillも減らした
バトル前のカウントを3に変更
バトル前の画面にyouの文字を表示させた
片方のピック終了時にどちらにも待ての文字が出るようにした
ピック画面にどちらのチームかがわかるように文字を出した
位置や大きさはまた調整する
VSの文字を表示させた

1/24
プレイヤー番号をランダムにつけれるようにした
五芒星の座布団弾を大きくした

1/25
ネットの通信が正しく送られていないことが判明
コアダンプの原因はおそらくそこ
直し方が全くわからん
パソコン間の通信のみ起こる？
PFの再使用可能時間の延ばした
40FPS程度ならクライアント1台だけつけているパソコンでは動く
サーバ用に5台目のパソコンが必要かも？
最小化した場合にtype errorが発生する
最小化していなくとも起こることもある
処理落ちしていないときはほとんど起きない
現在は無理やりエラーを弾いている
一応これでできるかも
パソコンの通信台数によっても重くなるかも？
5台目が厳しいかもしれない
HPSPバーを色付きに変更した

1/26
ゲームの最終調整した
 ・タイトル画像変更と調整
 ・チーム表示変更
 ・青SPの文字にバグ
 ・フォントの変更
 ・ピック画面での文字に「your turn」,「enemy turn」最後に「please wait」
ファイル構造を変更
全部直し終了
あとはもしかしたらつける音楽のみ

1/27
音楽をつけれるようにした
ピック画面までと、バトル用の音楽を変えられるようにした
*/

#include "../game.h"

//必要な関数
//bool Event(void); //イベント操作
Uint32 my_callbackfunc(Uint32 interval, void *param); //タイマー関数
bool PushBottun(void);                                //ボタンを押した判定

//取ってくる関数
extern void SystemGame(void);                     //必要な演算をまとめておく
extern RENDERDATA SetCharaImage(int type);        //画像の準備をする
extern int SetUpServer(int num);                  //ネットの初期設定
extern void Ending(void);                         //ネットの終了処理
extern int SendRecvManager(void);                 //ネットのメイン処理
extern int SendRenderData(RENDERDATA rd, int id); //RenderDataをクライアントに送る
extern void SetBackGround(void);                  //背景関係の準備
extern void SystemPick(void);                     //ピック画面に必要な演算
extern void WordSystem(int num, int count);       //送る文字を決め移動させる
extern void SystemCount(void);                    //カウントダウンをする
extern void WordMatch(int id);                    //勝敗の送る文字を決める
extern void SetPleyerNumber(void);                //プレイヤーのナンバーを決める

//必要な変数
GAME game;            //ゲームの変数
int endflag;          //エンドフラグ
int timer;            //一旦タイマー関数の練習で作る
Uint32 delay;         //タイマーの秒数
RENDERDATA RenderEnd; //最後に送るデータ
RENDERDATA GameEnd;   //ゲームの終了合図を送る
int num;              //通信するクライアント人数

RENDERDATA BF[16];          //バレットフレーム
RENDERDATA BF_b;            //バレットフレームの後ろ
RENDERDATA Num_i[8];        //番号
RENDERDATA BG_choice;       //背景
RENDERDATA FC[12];          //フレーム
RENDERDATA JS[4];           //カーソル
RENDERDATA HP;              //HPバー構造体
RENDERDATA HP_b1;           //HPbox1
RENDERDATA HP_b1s;          //HPbox1sub
RENDERDATA HP_b2;           //HPbox2
RENDERDATA HP_b2s;          //HPbox2sub
RENDERDATA SP_b1;           //SPbox1
RENDERDATA SP_b2;           //SPbox2
RENDERDATA area;            //回復エリアサークル
RENDERDATA blind[4];        //目隠し用の黒
RENDERDATA title;           //タイトル用
int pleyer1;                //メインプレイヤー１
int pleyer2;                //メインプレイヤー2
RENDERDATA cursor;          //弾幕位置の魔法陣？
RENDERDATA MainIcon[10];    //メインが使う弾幕のアイコン
RENDERDATA SupportIcon[12]; //サポートが使うアイコン
RENDERDATA trap;            //感知式の地雷
RENDERDATA bom;             //時限爆弾
int MainIconSend[10];       //MainIconを送る相手　-1ならどちらも、もしくは送らない
int SupportIconSend[12];    //SupportIconを送る相手　-1ならどちらも、もしくは送らない
int PickMain;               //メインピックの選択する方
int PickSub;                //サポートピックの選択する方
RENDERDATA word;            //文字
RENDERDATA wait;            //please waitの表示用
RENDERDATA you[4];          //youの文字用
RENDERDATA team[2];         //teamの文字
RENDERDATA TeamColor[4];    //teamのカラーの文字
RENDERDATA VS;              //VSの文字
RENDERDATA turn[4];         //ピック画面用
RENDERDATA count;           //カウントダウン

extern bool AreaIn;      //エリアに入っているフラグ
extern int TouchIcon[4]; //送る文字の種類

//スレッド関数
static int Thread1(void *data)
{
    while (endflag)
    {
        endflag = SendRecvManager(); //ネット関係
        SDL_Delay(4);
    }
    return 0;
}

//メイン関数
int main(int argc, char *argv[])
{
    mode = 0; //ゲームモードの開始位置

    if (argc == 2)
    {
        num = atoi(argv[1]); //引数代入
    }
    else
    {
        printf("一人用ですね\n");
        num = 1;
    }
    //num = 4; //四人用
    printf("num = %d\n", num);
    endflag = 1; //終了フラグ
    int thrend;  //スレッド処理の終了フラグ

    //ネットの初期設定
    if (SetUpServer(num) == -1)
    {
        printf("error net\n");
        exit(-1); //終了
    }
    printf("net OK\n");

    //SDL初期化
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("failed to SDL\n");
        exit(-1);
    }

    //ランダム関数初期化
    srand((unsigned)time(NULL));

    //プレイヤー番号の決定
    SetPleyerNumber(); //ここに移行させた

    //画像貼り付け準備（１体目）
    game.chara[pleyer1].RenderData = SetCharaImage(0);
    game.chara[pleyer1].floatpoint.x = game.chara[pleyer1].RenderData.dst.x; //FPointに座標を代入
    game.chara[pleyer1].floatpoint.y = game.chara[pleyer1].RenderData.dst.y; //FPointに座標を代入
    //星の画像準備(２体目)
    game.chara[pleyer2].RenderData = SetCharaImage(1);
    game.chara[pleyer2].floatpoint.x = game.chara[pleyer2].RenderData.dst.x; //FPointに座標を代入
    game.chara[pleyer2].floatpoint.y = game.chara[pleyer2].RenderData.dst.y; //FPointに座標を代入
    //サポート1の画像準備
    game.chara[game.chara[pleyer1].id_f].RenderData = SetCharaImage(2);
    game.chara[game.chara[pleyer1].id_f].floatpoint.x = game.chara[game.chara[pleyer1].id_f].RenderData.dst.x;
    game.chara[game.chara[pleyer1].id_f].floatpoint.y = game.chara[game.chara[pleyer1].id_f].RenderData.dst.y;
    //サポート2の画像準備
    game.chara[game.chara[pleyer2].id_f].RenderData = SetCharaImage(3);
    game.chara[game.chara[pleyer2].id_f].floatpoint.x = game.chara[game.chara[pleyer2].id_f].RenderData.dst.x;
    game.chara[game.chara[pleyer2].id_f].floatpoint.y = game.chara[game.chara[pleyer2].id_f].RenderData.dst.y;

    //その他キャラの初期設定
    for (int i = 0; i < 4; i++)
    {
        game.chara[i].hp = MAX_HP; //体力設定
        game.chara[i].sp = MAX_SP; //スタミナ設定
        game.chara[i].speed = 5;   //初期値
    }
    //game.chara[pleyer1].hp = MAX_HP * 0.2;
    //game.chara[pleyer2].hp = MAX_HP * 0.15;

    //背景関係の準備
    SetBackGround();

    //弾幕の設定
    for (int i = 0; i < MAX_BULLET; i++)
    {
        game.chara[0].bullet[i].type = -1;
        game.chara[1].bullet[i].type = -1;
        game.chara[2].bullet[i].type = -1;
        game.chara[3].bullet[i].type = -1;
    }
    PickMain = pleyer1; //初めに選ぶプレイヤー
    //サポートの設定
    for (int i = 0; i < 4; i++)
    {
        game.chara[game.chara[pleyer1].id_f].support_num[i] = -1;
        game.chara[game.chara[pleyer2].id_f].support_num[i] = -1;
    }
    PickSub = game.chara[pleyer2].id_f; //初めに選ぶプレイヤー
    //ターン表示の初期化
    turn[PickMain].src = (SDL_Rect){0, 0, FONT_WIDE2 * 19, FONT_HIGH2};
    turn[PickSub].src = (SDL_Rect){0, 0, FONT_WIDE2 * 19, FONT_HIGH2};

    //弾初期化
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < MAX_BULLET; j++)
        {
            for (int k = 0; k < MAX_SHOT; k++)
            {
                game.chara[i].bullet[j].shot[k].stts = UNUSED;
            }
        }
    }

    //サポート関係初期化
    AreaIn = false;
    MainPickCount = 0;
    SubPickCount = 0;

    //スレッド挑戦
    SDL_Thread *thr;
    int threadReturnValue;
    thr = SDL_CreateThread(Thread1, "TestThread", &endflag);
    if (NULL == thr)
    {
        printf("error\n");
    }
    else
    {
        printf("Thread complete\n");
    }

    timer = 0;
    RenderEnd.type = -1; //最後の合図
    RenderEnd.angle = -1;
    GameEnd.type = -2; //ゲーム終了の合図
    GameEnd.angle = -2;

    //タイマー関数挑戦
    delay = 1000 / 30; //タイマーの秒数 おそらくms
    SDL_TimerID my_timer_id = SDL_AddTimer(delay, my_callbackfunc, NULL);

    //printf("size = %d\n", (int)sizeof(RENDERDATA));

    //メインループ//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("main Start\n\n\n\n");
    while (endflag)
    {
        //modeによって処理を分ける
        switch (mode)
        {
        case 0: //タイトル画面
            if (PushBottun())
            {
                //please waitに変更
                wait.type = 87;
                wait.src = (SDL_Rect){0, 0, FONT_WIDE2 * 21, FONT_HIGH2};
                wait.angle = 0;
                wait.dst = (SDL_Rect){600, 500, 30 * 11 * 2, 40 * 2};

                mode++;
                game.chara[0].bottan.escape = false;
            }
            break;
        case 1: //プレイヤー選択　今はなし
            break;
        case 2:           //ピック画面　カーソルの移動
            SystemPick(); //ピックの操作
            break;
        case 3: //バトル前のカウントダウン
            SystemCount();
            break;
        case 4:           //バトル画面　色々演算
            SystemGame(); //バトル画面の演算
            break;
        case 5: //終了画面　分岐選択
            if (game.count == 500)
            {
                wait.type = 87;
                wait.src = (SDL_Rect){FONT_WIDE2 * 31, 0, FONT_WIDE2 * 33, FONT_HIGH2};
                wait.dst = (SDL_Rect){400, 600, 30 * 18 * 2, 50 * 2};
                wait.angle = 0;
                mode++;
            }
            if (game.count > 500 && PushBottun())
                mode++;
            game.count++; //カウントアップ
            break;
        case 6: //終了
            break;
        }
        SDL_Delay(4);
    }
    SDL_WaitThread(thr, &thrend); //スレッド終了を待つ

    //終了処理
    Ending();           //ネットの終了処理
    SDL_Quit();         //SDLの終了
    printf("finish\n"); //終了表示
}

/****************************************************************
 * 関数名    Uint32 my_callbackfunc(Uint32 interval, void *param)
 * 機能     タイマー関数
 *          ここで送信の制御をするかも
 * 引数1     多分時間
 * 引数2      設定していればここに引数を渡せる？
 * 戻り値   成功でタイマーの秒数
*****************************************************************/
Uint32 my_callbackfunc(Uint32 interval, void *param)
{
    //終了しているなら送らない
    if (!endflag)
        return interval;

    //modeによって場面を分ける
    switch (mode)
    {
    case 0:                        //一応の確認でバトル背景？
        SendRenderData(wait, -1);  //push any bottun
        SendRenderData(title, -1); //タイトル
        break;

    case 1:     //プレイヤー選択　
        mode++; //今はまだ無いので飛ばす
        break;

    case 2: //ピック画面
        //まず背景
        SendRenderData(BG_choice, -1);

        for (int j = 0; j < num; j++)
        {

            //ここからサポートピック
            if (!game.chara[j].main)
            {
                //フレーム
                for (int i = 0; i < 12; i++)
                {
                    FC[i].dst.x = 150 + 220 * (((i % 6) + 1) / 2) + 360 * ((i % 6) / 2);
                    FC[i].dst.y = 50 + 270 * (i / 6);
                    SendRenderData(FC[i], j);
                }
                //bulletframe_cover画像
                //BF_b.angle += 1; //抜きたい
                BF_b.dst.x = 1500;
                BF_b.dst.y = 650;
                SendRenderData(BF_b, j);
                //bulletframe画像と番号
                int number = (j == game.chara[pleyer1].id_f) ? 0 : 1;
                for (int i = 8 + 4 * number; i < 8 + 4 * number + 4; i++)
                {
                    //BF[i].angle -= 1; //抜きたい
                    SendRenderData(BF[i], j);
                }
                //サポートが使うアイコン
                for (int i = 0; i < 12; i++)
                {
                    if (SupportIconSend[i] == -1)
                        SendRenderData(SupportIcon[i], j);
                    else
                        SendRenderData(SupportIcon[i], SupportIconSend[i]);
                }
                //数字のアイコン
                for (int i = 4; i < 8; i++)
                    SendRenderData(Num_i[i], j);
                //サポートアイコンの分類文字を送る
                //Attack
                word.type = 89;
                word.src = (SDL_Rect){0, 0, FONT_WIDE * 7, FONT_HIGH};
                word.dst = (SDL_Rect){270, 0, FONT_WIDE * 6, FONT_HIGH};
                SendRenderData(word, j);
                //Support
                word.type = 90;
                word.dst = (SDL_Rect){850, 0, FONT_WIDE * 7, FONT_HIGH};
                SendRenderData(word, j);
                //Jamming
                word.type = 91;
                word.dst = (SDL_Rect){1430, 0, FONT_WIDE * 7, FONT_HIGH};
                SendRenderData(word, j);

                //カーソル
                SendRenderData(JS[j], j);

                //待てを送る
                if (SubPickCount == 8)
                    SendRenderData(wait, j);
                else
                    SendRenderData(turn[j], j);

                //チームを送る
                SendRenderData(team[1], j);
                SendRenderData(TeamColor[j], j);
            }

            //ここからメインピック
            else
            {
                //ピックのフレーム
                for (int i = 0; i < 10; i++)
                {
                    FC[i].dst.x = 400 + 220 * (i % 5);
                    FC[i].dst.y = 50 + 270 * (i / 5);
                    SendRenderData(FC[i], j);
                }
                //BF_b.angle += 1; //抜きたい
                BF_b.dst.x = 110;
                BF_b.dst.y = 650;
                SendRenderData(BF_b, j);
                //弾幕フレーム
                int number = (j == pleyer1) ? 0 : 1;
                for (int i = number * 4; i < number * 4 + 4; i++)
                {
                    //BF[i].angle -= 1; //抜きたい
                    SendRenderData(BF[i], j);
                }
                //メインが使う弾幕のアイコン
                for (int i = 0; i < 10; i++)
                {
                    if (MainIconSend[i] == -1)
                        SendRenderData(MainIcon[i], j);
                    else
                        SendRenderData(MainIcon[i], MainIconSend[i]);
                }
                //ナンバー
                for (int i = 0; i < 4; i++)
                    SendRenderData(Num_i[i], j);
                //カーソル
                SendRenderData(JS[j], j);

                //待てを送る
                if (MainPickCount == 8)
                    SendRenderData(wait, j);
                else
                    SendRenderData(turn[j], j);
                // else if (j != PickMain)
                //     SendRenderData(wait, j);

                //チームを送る
                SendRenderData(team[0], j);
                SendRenderData(TeamColor[j], j);
            }

            //文字を送る
            if (TouchIcon[j] != -1)
            {
                for (int i = 0; i < 3; i++)
                {
                    WordSystem(j, i); //送る文字をセットする
                    SendRenderData(word, j);
                }
            }
        }
        break;

    case 3: //バトル前待機画面
        //キャラ
        for (int i = 0; i < num; i++)
        {
            if (game.chara[i].main) //メイン
                SendRenderData(game.chara[i].RenderData, -1);
            else //サポート
                SendRenderData(game.chara[i].RenderData, i);
        }
        //先にHP、SPバーを送る
        SendRenderData(HP_b1, -1);
        SendRenderData(HP_b1s, -1);
        SendRenderData(HP_b2, -1);
        SendRenderData(HP_b2s, -1);
        SendRenderData(SP_b1, -1);
        SendRenderData(SP_b2, -1);
        //HP、SP枠を送る
        SendRenderData(HP, -1);
        //画像貼り付け準備(バレットフレームの後ろ)
        //メイン
        BF_b.dst.w = BF_b.dst.h = 200;
        //BF_b.angle += 1; //抜きたい
        BF_b.dst.x = 50;
        BF_b.dst.y = 750;
        SendRenderData(BF_b, -1);
        //サポート
        BF_b.dst.x = 1600;
        BF_b.dst.y = 750;
        SendRenderData(BF_b, -1);
        //弾幕アイコンを送る
        for (int i = 0; i < 10; i++)
        {
            if (MainIconSend[i] != -1)
            {
                //ここ問題 解決
                SendRenderData(MainIcon[i], MainIconSend[i]);
                SendRenderData(MainIcon[i], game.chara[MainIconSend[i]].id_f);
            }
        }
        //サポートアイコンを送る
        for (int i = 0; i < 12; i++)
        {
            if (SupportIconSend[i] != -1)
            {
                SendRenderData(SupportIcon[i], SupportIconSend[i]);
                SendRenderData(SupportIcon[i], game.chara[SupportIconSend[i]].id_f);
            }
        }
        //弾幕フレーム
        for (int i = 4; i < 12; i++)
        {
            //BF[i].angle -= 1;
            SendRenderData(BF[i], -1);
        }
        //VS
        SendRenderData(VS, -1);
        //youの文字
        for (int i = 0; i < num; i++)
        {
            SendRenderData(you[i], i);
        }
        //カウントダウン
        SendRenderData(count, -1);

        break;

    case 4: //バトル画面　
        /////////////キャラ/////////////////////////////////////////////////
        for (int i = 0; i < 4; i++)
        {
            if (game.chara[i].main) //ここがメインキャラ
            {
                SendRenderData(game.chara[i].RenderData, -1); //キャラのデータ送信
            }
            else //ここからサポート
            {
                SendRenderData(game.chara[i].RenderData, i);
            }
        }
        for (int i = 0; i < num; i++)
        {
            // if (game.chara[i].main) //ここがメインキャラ
            // {
            //     SendRenderData(game.chara[i].RenderData, -1); //キャラのデータ送信
            // }
            // else //ここからサポート
            // {
            //     SendRenderData(game.chara[i].RenderData, i);
            // }
            /////////////////弾幕////////////////////////////////
            for (int j = 0; j < MAX_BULLET; j++)
            {
                if (game.chara[i].bullet[j].stts == USE)
                {
                    cursor.dst.x = game.chara[i].bullet[j].emit.x;
                    cursor.dst.y = game.chara[i].bullet[j].emit.y;
                    SendRenderData(cursor, -1);
                }
                ////////////////////弾////////////////////////
                for (int k = 0; k < MAX_SHOT; k++)
                {
                    if (game.chara[i].bullet[j].shot[k].stts == UNUSED)
                    {
                        break;
                    }
                    else if (game.chara[i].bullet[j].shot[k].stts == USE)
                        SendRenderData(game.chara[i].bullet[j].shot[k].RenderData, -1); //弾のデータ送信
                }
            }
        }

        //地雷
        if (support[0].flag && support[0].count <= 10000)
            SendRenderData(trap, -1);
        if (support[1].flag && support[1].count <= 500)
            SendRenderData(bom, -1);

        //回復エリアサークル
        if (support[6].flag && support[6].count <= 300)
            SendRenderData(area, -1);

        //目隠し用の黒色
        if (support[9].flag && support[9].count <= 500)
        {
            int id = game.chara[support[9].id].id_e;
            for (int i = 0; i < 4; i++)
            {
                SendRenderData(blind[i], id);
            }
        }

        //先にHP、SPバーを送る
        SendRenderData(HP_b1, -1);
        SendRenderData(HP_b1s, -1);
        SendRenderData(HP_b2, -1);
        SendRenderData(HP_b2s, -1);
        SendRenderData(SP_b1, -1);
        SendRenderData(SP_b2, -1);
        //HP、SP枠を送る
        SendRenderData(HP, -1);
        //画像貼り付け準備(バレットフレームの後ろ)
        //main
        BF_b.dst.w = BF_b.dst.h = 200;
        //BF_b.angle += 1; //抜きたい
        BF_b.dst.x = 50;
        BF_b.dst.y = 750;
        SendRenderData(BF_b, -1);
        //support
        BF_b.dst.x = 1600;
        BF_b.dst.y = 750;
        SendRenderData(BF_b, -1);
        //画像貼り付け準備(バレットフレーム)
        //弾幕アイコンを送る
        for (int i = 0; i < 10; i++)
        {
            if (MainIconSend[i] != -1)
            {
                //ここ問題 解決
                SendRenderData(MainIcon[i], MainIconSend[i]);
                SendRenderData(MainIcon[i], game.chara[MainIconSend[i]].id_f);
            }
        }
        //サポートアイコンを送る
        for (int i = 0; i < 12; i++)
        {
            if (SupportIconSend[i] != -1)
            {
                SendRenderData(SupportIcon[i], SupportIconSend[i]);
                //if (game.chara[SupportIconSend[i]].id_f <= num)
                SendRenderData(SupportIcon[i], game.chara[SupportIconSend[i]].id_f);
            }
        }
        //弾幕フレームを送る
        for (int i = 0; i < 16; i++)
        {
            //BF[i].angle -= 1;
            int x = (i < 4 || (i >= 8 && i < 12)) ? pleyer1 : pleyer2;
            SendRenderData(BF[i], x);
            SendRenderData(BF[i], game.chara[x].id_f);
        }
        //VS
        SendRenderData(VS, -1);
        break;

    case 5: //バトル終了
        for (int i = 0; i < num; i++)
        {
            WordMatch(i); //勝敗の結果の文字に変える
            SendRenderData(word, i);
        }
        if (game.count >= 700)
            SendRenderData(wait, -1); //
        break;

    case 6: //勝敗結果画面　1にループもしくは終了になる予定
        printf("\n\n\n\n\n");
        printf("winner = %d, %d\n", winner, game.chara[winner].id_f);
        SendRenderData(GameEnd, -1); //ゲーム終了の合図
        endflag = 0;
        //mode = 1; //今はまだ無い
        break;
    }
    SendRenderData(RenderEnd, -1); //最後の合図
    return interval;
}

/****************************************************************
 * 関数名    PushBottun
 * 機能     なにかボタンを押しているかを調べる
 * 引数      なし
 * 戻り値   何か押していればtrue
*****************************************************************/
bool PushBottun(void)
{
    for (int i = 0; i < num; i++)
    {
        if (game.chara[i].bottan.one)
            return true;
        if (game.chara[i].bottan.two)
            return true;
        if (game.chara[i].bottan.three)
            return true;
        if (game.chara[i].bottan.four)
            return true;
        if (game.chara[i].bottan.six)
            return true;
        if (game.chara[i].bottan.escape)
            return true;
    }
    return false;
}