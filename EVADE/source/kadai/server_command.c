/*****************************************************************
ファイル名	: server_command.c
機能		: サーバーのコマンド処理
*****************************************************************/

#include "server_common.h"
#include "server_func.h"
#include "../game.h"

static SAMPLE test;
static void SetIntData2DataBlock(void *data, int intData, int *dataSize);
static void SetCharData2DataBlock(void *data, char charData, int *dataSize);
static void SetRenderDataBlock(void *data, RENDERDATA RenderData, int *dataSize); //RenderData型のデータを送る

int SendRenderData(RENDERDATA rd, int id); //RenderDataを送る
extern int num;                            //通信するクライアント人数

/*****************************************************************
関数名	: ExecuteCommand
機能	: クライアントから送られてきたコマンドを元に，
		  引き数を受信し，実行する
引数	: char	command		: コマンド
		  int	pos			: コマンドを送ったクライアント番号
出力	: プログラム終了コマンドが送られてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char command, int pos)
{
    unsigned char data[MAX_DATA];
    int dataSize, intData;
    int endFlag = 1;

    /* 引き数チェック */
    assert(0 <= pos && pos < MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("Get command %c\n", command);
#endif
    //printf("command = %c\n", command); //チェック
    switch (command)
    {
    case END_COMMAND:
        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data, command, &dataSize);
        //Settest2DataBlock(data, test, &dataSize);
        /* 全ユーザーに送る */
        SendData(ALL_CLIENTS, data, dataSize);
        endFlag = 0;
        break;
    /*
    case COMMAND:
        dataSize = 0;
        RecvIntData(pos, &intData); //それぞれの手を格納する ここを毎回しないとバグる可能性？
        /*
        //////////////////////////////////////ここは残す！！！/////////////////////////////////////////////////////////////////////////////
        printf("intData = %d\n", intData); //チェック ここだ!!!
        SetCharData2DataBlock(data, command, &dataSize);
        //Settest2DataBlock(data, test, &dataSize);
        //全ユーザーに送る
        SendData(ALL_CLIENTS, data, dataSize);
        
        break;
    */
    default:
        /* 未知のコマンドが送られてきた */
        //fprintf(stderr, "0x%02x is not command!\n", command);
        break;
    }
    return endFlag;
}

/*****
      static
*****/
/*****************************************************************
関数名	: SetIntData2DataBlock
機能	: int 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetIntData2DataBlock(void *data, int intData, int *dataSize)
{
    int tmp;
    int htonl(int);
    /* 引き数チェック */
    assert(data != NULL);
    assert(0 <= (*dataSize));

    tmp = htonl(intData);

    /* int 型のデータを送信用データの最後にコピーする */
    memcpy(data + (*dataSize), &tmp, sizeof(int));
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}

/*****************************************************************
関数名	: SetCharData2DataBlock
機能	: char 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetCharData2DataBlock(void *data, char charData, int *dataSize)
{
    /* 引き数チェック */
    assert(data != NULL);
    assert(0 <= (*dataSize));

    /* int 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData; //ここに問題あり
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}

/*****************************************************************
関数名	: SetRenderDataDataBlock
機能	: RenderData型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  RENDERDATA		RenderData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetRenderDataBlock(void *data, RENDERDATA RenderData, int *dataSize)
{
    /* 引き数チェック */
    assert(data != NULL);
    assert(0 <= (*dataSize));

    /* int 型のデータを送信用データの最後にコピーする */
    *(RENDERDATA *)(data + (*dataSize)) = RenderData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(RENDERDATA);
}

/****************************************************************
 * 関数名      SendRenderData
 * 機能       RenderDataを各クライアントに送る
 * 引数       RENDERDATA rd
 * 引数2      送るクライアントID、-1なら全員に送る
 * 戻り値      int
*****************************************************************/
int SendRenderData(RENDERDATA rd, int id)
{
    if (id >= num)
        return 0;
    if (rd.type >= 200 && rd.type <= 205)
        ;
    else if (rd.type >= TEXTURES || rd.type < -2)
        printf("type error\ntype = %d\n", rd.type);

    unsigned char data[MAX_DATA];
    int dataSize = 0;
    SetRenderDataBlock(data, rd, &dataSize); //データをセットする
    SendData(id, data, dataSize);
    // if (id == -1)
    // {
    //     //全ユーザーに送る
    //     SendData(ALL_CLIENTS, data, dataSize);
    // }
    // else
    // {
    //     SendData(id, data, dataSize); //決まったクライアントにだけ送る
    // }
    return 0;
}
