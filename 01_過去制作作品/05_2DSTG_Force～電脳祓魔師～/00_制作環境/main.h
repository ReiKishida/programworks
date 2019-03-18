//=============================================================================
//
// メイン処理 [main.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_
#define DIRECTINPUT_VERSION (0x0800)	//警告対処用

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <windows.h>
#include <dinput.h>
#include <Xinput.h>
#include <time.h>

#include "d3dx9.h"
#include "dinput.h"
#include "xaudio2.h"

//前方宣言(名称に注意！なんでも通るから！)
class CRenderer;
class CScene2D;
class CScene;
class CInputKeyboard;
class CPlayer;
class CBullet;
class CExplosion;
class CBg;
class CEnemy;
class CEffect;
class CSound;
class CNumber;
class CScore;
class CPolygon;
class CTitle;
class CGame;
class CResult;
class CMenu;
class CRanking;
//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment (lib,"d3d9.lib")		   //描画処理に必要
#pragma comment (lib,"d3dx9.lib")		   //[d3d9.lib]の拡張ライブラリ
#pragma comment (lib, "dxguid.lib")		   //DirectXコンポーネント(部品)使用に必要
#pragma comment (lib, "winmm.lib")		   //システム時刻取得に必要
#pragma comment (lib, "dinput8.lib")	   //入力処理に必要
#pragma comment (lib, "xinput.lib")			// Xinputのライブラリ

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)
// ２Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 頂点カラー / テクスチャ座標 )
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//*****************************************************************************
// 構造体定義
//*****************************************************************************

// ３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

// ３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_3D;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#ifdef _DEBUG
int GetFPS(void);
#endif


#endif