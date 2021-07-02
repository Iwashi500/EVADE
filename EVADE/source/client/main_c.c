/*
11/7
こちらはクライアント側のメインとする
一応分けれた
仮でアリス表示
ネット関係を練り直す必要ありかも
スレッドで解決？

11/11
クライアント側はスレッド2つ使う予定
実験は成功済み
空の関数を作ってネットと合わせる準備をする


11/17
endflagの変更の仕方を調整


11/19
ネットで受け取ると即座に描写に変更
スレッドが一個減った
*/

#include "../game.h"
#include <SDL2/SDL_mixer.h>

//必要な関数
Uint32 my_callbackfunc(Uint32 interval, void *param); //タイマー関数
void ChangeMusic(void);                               //音楽をバトル用に変える

//取ってくる関数
extern void WindowGame(void); //ウィンドウに表示させるものをまとめる
//extern void SystemGame(void); //必要な演算をまとめておく
extern bool Event(void);                                                                 //イベント操作
extern void SetTexture(void);                                                            //テクスチャの準備をする
extern void WindowEnd(void);                                                             //ウィンドウの終了処理
extern int SetUpClient(char *hostName, int *clientID, int *num, char clientNames[][10]); //ネットに接続する
extern void CloseSoc(void);                                                              //ネットの終了処理
extern int SendRecvManager(void);                                                        //ネットのメイン処理
extern void SendCommand(int pos);                                                        //コマンドを送信する
extern void SetWord(void);                                                               //文字の準備をする

//必要な変数
SDL_Window *window;       //ウィンドウデータ
SDL_Renderer *renderer;   //レンダラー
int endflag;              //エンドフラグ
int timer;                //一旦タイマー関数の練習で作る
Uint32 delay;             //タイマーの秒数
int clientID;             //自分のクライアントID
extern SDL_Joystick *joy; //ジョイスティック
TTF_Font *font;           //文字1
TTF_Font *font2;          //文字2
Mix_Music *music;         // Music型でデータを格納

//スレッド関数
static int Thread1(void *data)
{
    int endThread1 = 0;
    while (endflag)
    {
        if (Event())
            endflag = 0;
        SDL_Delay(4);
    }
    printf("thread1 end\n");
    return 0;
}

//メイン関数
int main(int argc, char *argv[])
{
    endflag = 1; //終了フラグ
    int thrend1; //スレッド処理の終了フラグ

    //ネットの接続
    int num;
    char name[4][10];
    char localHostName[] = "localhost";
    char *serverName;

    //引数チェック
    if (argc == 1)
    {
        serverName = localHostName;
    }
    else if (argc == 2)
    {
        serverName = argv[1];
    }
    //SDL初期化
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("failed to SDL\n");
        exit(-1);
    }

    //音楽
    Mix_Init(MIX_INIT_MP3); // MP3    ファイルを読み込むための初期化
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        printf("Failed to open audio device: %s\n", Mix_GetError());
        exit(-1);
    }
    // music = Mix_LoadMUS("pick2.mp3");  // test.wavをMusic型で読み込み
    // Mix_PlayMusic(music, -1);          // Music型サウンドを無限（繰り返し）再生
    // music = Mix_LoadMUS("music/pick2.mp3"); // test.wavをMusic型で読み込み
    // Mix_PlayMusic(music, -1);               // Music型サウンドを無限（繰り返し）再生

    //ジョイスティックセット
    if (SDL_NumJoysticks() <= 0)
    {
        fprintf(stderr, "コントローラを接続してください\n");
        exit(-1);
    }
    joy = SDL_JoystickOpen(0);
    SDL_JoystickEventState(SDL_ENABLE);

    //ボタン初期値
    reset.up = false;
    reset.down = false;
    reset.left = false;
    reset.right = false;
    reset.escape = false;
    reset.one = false;
    reset.two = false;
    reset.three = false;
    reset.four = false;
    reset.six = false;

    int endmain = 1;

    //サーバ接続
    printf("%s\n", serverName);
    if (SetUpClient(serverName, &clientID, &num, name) == -1)
    {
        fprintf(stderr, "setup failed : SetUpClient\n");
        return -1;
    }
    printf("clientID = %d\n", clientID);

    //ウィンドウ生成
    window = SDL_CreateWindow("SDL2 Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1850, 1000, 0);
    printf("make window\n");

    //レンダラー生成
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    printf("make renderer\n");

    //描画する画像形式を指定して初期化
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    //画像読み込み
    SetTexture();

    //フォントセット
    TTF_Init();
    font = TTF_OpenFont(FONT_PATH, 50);
    font2 = TTF_OpenFont(FONT_PATH2, 50);
    printf("Font Set\n");
    //文字セット
    SetWord();

    music = Mix_LoadMUS("music/Battle2.mp3"); // test.wavをMusic型で読み込み
    Mix_PlayMusic(music, -1);                 // Music型サウンドを無限（繰り返し）再生

    //スレッド挑戦
    SDL_Thread *thr;
    thr = SDL_CreateThread(Thread1, "TestThread", &endflag);
    if (NULL == thr)
    {
        printf("error\n");
    }
    else
    {
        printf("Thread1 complete\n");
    }

    //メインループ///////////////////////////////////////////////////////
    printf("main Start\n\n\n\n");
    while (endflag)
    {
        endmain = SendRecvManager(); //ネット関係のメイン
        if (!endmain)
            endflag = 0;
    }
    printf("main end\n");
    SDL_WaitThread(thr, &thrend1); //スレッド終了を待つ

    //終了処理
    WindowEnd();                   //ウィンドウの終了処理
    SDL_DestroyRenderer(renderer); //レンダラーの破棄
    SDL_DestroyWindow(window);     //ウィンドウの破棄
    TTF_CloseFont(font);           //フォントの開放
    TTF_Quit();                    //フォントの終了
    Mix_FreeMusic(music);          //音楽の開放
    Mix_CloseAudio();              //ミキサーの開放
    SDL_Quit();                    //SDLの終了
    CloseSoc;                      //ネットの終了処理
    printf("finish\n");            //終了表示
}

void ChangeMusic(void)
{
    // music = Mix_LoadMUS("music/Battle2.mp3"); // test.wavをMusic型で読み込み
    // Mix_PlayMusic(music, -1);                 // Music型サウンドを無限（繰り返し）再生
}