/*****************************************************************
ファイル名	: client_command.c
機能		: クライアントのコマンド処理
*****************************************************************/
#include "../game.h"
#include "common.h"
#include "client_func.h"

void SetIntData2DataBlock(void *data, int intData, int *dataSize);
void SetBottanData(void *data, BOTTAN bottan, int *dataSize);
static void RecvCircleData(void);
static void RecvRectangleData(void);

/*****************************************************************
関数名	: SendEndCommand
機能	: プログラムの終了を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendEndCommand(void)
{
    unsigned char data[MAX_DATA];
    int dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif
    dataSize = 0;
    /* コマンドのセット */
    //SetCharData2DataBlock(data, END_COMMAND, &dataSize);
    SetIntData2DataBlock(data, -1, &dataSize);
    /* データの送信 */
    SendData(data, dataSize);
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
void SetIntData2DataBlock(void *data, int intData, int *dataSize)
{
    /* 引き数チェック */
    assert(data != NULL);
    assert(0 <= (*dataSize));

    /* BOTTA 型のデータを送信用データの最後にコピーする */
    *(int *)(data + (*dataSize)) = intData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}

/*****************************************************************
関数名	: SetBottaData
機能	: BOTTA型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
void SetBottanData(void *data, BOTTAN bottan, int *dataSize)
{
    /* 引き数チェック */
    assert(data != NULL);
    assert(0 <= (*dataSize));

    /* BOTTA 型のデータを送信用データの最後にコピーする */
    *(BOTTAN *)(data + (*dataSize)) = bottan;
    /* データサイズを増やす */
    (*dataSize) += sizeof(BOTTAN);
}