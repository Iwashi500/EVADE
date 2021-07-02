/*****************************************************************
ファイル名	: common.h
機能		: サーバーとクライアントで使用する定数の宣言を行う
*****************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <math.h>

#define PORT (u_short)8888 /* ポート番号 */

#define MAX_CLIENTS 4	/* クライアント数の最大値 */
#define MAX_NAME_SIZE 10 /* ユーザー名の最大値*/

#define MAX_DATA 500 /* 送受信するデータの最大値 */

#define END_COMMAND 'E' /* プログラム終了コマンド */

#define COMMAND 'M' /* 円表示コマンド */
#define DRAW 'D'	/* 円表示コマンド */
#define WIN 'W'		/* 円表示コマンド */
#define LOSE 'L'	/* 円表示コマンド */

typedef struct
{
	int game;
	char drink;
} SAMPLE; /*sampleを設定*/

#endif
