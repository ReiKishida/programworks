//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// SPゲージ処理 [spGage.cpp]
// Author : 圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "spGage.h"
#include "main.h"
#include "input.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define SPGAGE_MAX					(10)				// SPゲージの最大数
#define SPGAGE_WIDTH				(500)				// 幅
#define SPGAGE_HEIGHT				(70)				// 高さ
#define SPGAGE_FRAME_WIDTH			(10)				// 枠の幅
#define SPGAGE_DEST_APPROX			(0.5f)				// 目的地の近似値
#define SPGAGE_ADD_FRAME			(10)				// SPゲージの目的の値までにかかるフレーム数
#define SPGAGE_ADD_FRAME_RED		(60)				// 赤SPゲージの目的の値までにかかるフレーム数
#define SPGAGE_RED_ADD_START_FRAME	(90)				// 赤SPゲージの減少開始までにかかるフレーム数

#define SPGAGE_MAX_VALUE			(100)				// ゲージの最大量（プレイヤー側のマクロに置き換え？？

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 構造体定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef enum
{ // テクスチャの種類の列挙体
	SPGAGE_TEX_FRAME = 0,	// フレーム
	SPGAGE_TEX_SPBLACK,		// SP黒
	SPGAGE_TEX_SPRED,		// SP赤
	SPGAGE_TEX_SP,			// SP
	SPGAGE_TEX_MAX,			// 総数
}SPGAGE_TEX_NAME;

typedef enum
{ // 状態の列挙体
	SPGAGE_STATE_NONE = 0,	// 通常状態
	SPGAGE_STATE_UP,		// 上昇状態
	SPGAGE_STATE_DOWN,		// 減少状態
	SPGAGE_STATE_MAX,		// 総数
}SPGAGE_STATE;

typedef struct
{
	char *pFilename;	// ファイル名
} SpGageTexInfo;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXCOLOR col;			// 色
	float fWidth;			// 幅
	float fHeight;			// 高さ
	float fInitWidth;		// 初期幅
	float fDestWidth;		// 目的の幅
	float fAddWidth;		// 幅の増減値
	float fFrameWidth;		// 枠の幅
	SPGAGE_TYPE type;		// 種類
	int nTexType;			// テクスチャの種類
	SPGAGE_STATE state;		// 状態
	bool bUse;				// 使用しているかどうか
}SpGage;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// プロトタイプ宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
void MakeVertexSpGage(LPDIRECT3DDEVICE9 pDevice);
void UpdateVertexSpGage(void);
void UpdateVertexSpGage(int nCntSpGage);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// グローバル変数宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9		g_pTextureSpGage[SPGAGE_TEX_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSpGage = NULL;				// 頂点バッファへのポインタ
SpGage					g_aSpGage[SPGAGE_MAX];				// SPゲージの情報
int						g_aSkillPoint[LEG_TYPE_MAX];					// ポイント
int						g_aRedSkillPointGageCounter[LEG_TYPE_MAX];		// 赤ポイントのカウンター
// 読み込むテクスチャ名の設定
SpGageTexInfo g_apTextureSpGageTInfo[SPGAGE_TEX_MAX] =
{
	{ "" },		// フレーム
	{ "" },		// SP黒
	{ "data//TEXTURE//Life_Red.jpg" },		// SP赤
	{ "data//TEXTURE//Skill_Normal.jpg" },	// SP
};

int						g_nSkillPointPlayerTest[LEG_TYPE_MAX];			// プレイヤーがもつ用ポイント
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitSpGage(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 情報の初期化
	for (int nCntSpGage = 0; nCntSpGage < SPGAGE_MAX; nCntSpGage++)
	{
		g_aSpGage[nCntSpGage].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSpGage[nCntSpGage].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSpGage[nCntSpGage].fWidth = 0.0f;
		g_aSpGage[nCntSpGage].fHeight = 0.0f;
		g_aSpGage[nCntSpGage].fInitWidth = 0.0f;
		g_aSpGage[nCntSpGage].fDestWidth = 0.0f;
		g_aSpGage[nCntSpGage].fFrameWidth = 0.0f;
		g_aSpGage[nCntSpGage].fAddWidth = 0.0f;
		g_aSpGage[nCntSpGage].type = SPGAGE_TYPE_1P_FRAME;
		g_aSpGage[nCntSpGage].state = SPGAGE_STATE_NONE;
		g_aSpGage[nCntSpGage].bUse = false;
	}

	for (int nCntSpGageTex = 0; nCntSpGageTex < SPGAGE_TEX_MAX; nCntSpGageTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_apTextureSpGageTInfo[nCntSpGageTex].pFilename,
			&g_pTextureSpGage[nCntSpGageTex]);
	}

	// 頂点情報の作成
	MakeVertexSpGage(pDevice);

	// SP情報の初期化
	for (int nCntType = 0; nCntType < LEG_TYPE_MAX; nCntType++)
	{ // ゲージの数分回す
		g_aSkillPoint[nCntType] = 0;
		g_nSkillPointPlayerTest[nCntType] = 0;
	}

	SetSpAsset(D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 100.0f, SCREEN_HEIGHT - SPGAGE_HEIGHT / 2.0f - 10.0f, 0.0f), LEG_TYPE_LEFT);
	SetSpAsset(D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 100.0f, SCREEN_HEIGHT - SPGAGE_HEIGHT / 2.0f - 10.0f, 0.0f), LEG_TYPE_RIGHT);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitSpGage(void)
{
	// テクスチャの開放
	for (int nCntTex = 0; nCntTex < SPGAGE_TEX_MAX; nCntTex++)
	{
		if (g_pTextureSpGage[nCntTex] != NULL)
		{
			g_pTextureSpGage[nCntTex]->Release();
			g_pTextureSpGage[nCntTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffSpGage != NULL)
	{
		g_pVtxBuffSpGage->Release();
		g_pVtxBuffSpGage = NULL;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateSpGage()
{

	// 赤ポイントのカウンター増加
	for (int nCntType = 0; nCntType < LEG_TYPE_MAX; nCntType++)
	{ // プレイヤー人数分回す
		g_aRedSkillPointGageCounter[nCntType]--;
	}

	for (int nCntSpGage = 0; nCntSpGage < SPGAGE_MAX; nCntSpGage++)
	{
		if (g_aSpGage[nCntSpGage].bUse == true)
		{
			// 赤ゲージ処理
			if (g_aRedSkillPointGageCounter[LEG_TYPE_LEFT] == 0 && g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_SPRED)
			{ // 1P用のカウンターが0になり赤ゲージの場合
			  // 目的の値を設定
				g_aSpGage[nCntSpGage].fDestWidth = g_aSpGage[nCntSpGage].fInitWidth * (g_aSkillPoint[LEG_TYPE_LEFT] / float(SPGAGE_MAX_VALUE));

				// 目的の値までの1フレームあたりの増加量を算出
				g_aSpGage[nCntSpGage].fAddWidth = (g_aSpGage[nCntSpGage].fDestWidth - g_aSpGage[nCntSpGage].fWidth) / SPGAGE_ADD_FRAME_RED;

				// 状態の設定
				if (g_aSpGage[nCntSpGage].fDestWidth < g_aSpGage[nCntSpGage].fWidth)
				{ // 元の幅の方が大きい場合
				  // 減少状態にする
					g_aSpGage[nCntSpGage].state = SPGAGE_STATE_DOWN;
				}
				else
				{
					// 上昇状態にする
					g_aSpGage[nCntSpGage].state = SPGAGE_STATE_UP;
				}
			}
			else if (g_aRedSkillPointGageCounter[LEG_TYPE_RIGHT] == 0 && g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_SPRED)
			{ // 2P用のカウンターが0になり赤ゲージの場合
			  // 目的の値を設定
				g_aSpGage[nCntSpGage].fDestWidth = g_aSpGage[nCntSpGage].fInitWidth * (g_aSkillPoint[LEG_TYPE_RIGHT] / float(SPGAGE_MAX_VALUE));

				// 目的の値までの1フレームあたりの増加量を算出
				g_aSpGage[nCntSpGage].fAddWidth = (g_aSpGage[nCntSpGage].fDestWidth - g_aSpGage[nCntSpGage].fWidth) / SPGAGE_ADD_FRAME_RED;

				// 状態の設定
				if (g_aSpGage[nCntSpGage].fDestWidth < g_aSpGage[nCntSpGage].fWidth)
				{ // 元の幅の方が大きい場合
				  // 減少状態にする
					g_aSpGage[nCntSpGage].state = SPGAGE_STATE_DOWN;
				}
				else
				{
					// 上昇状態にする
					g_aSpGage[nCntSpGage].state = SPGAGE_STATE_UP;
				}
			}

			// 幅の増減を行う
			g_aSpGage[nCntSpGage].fWidth += g_aSpGage[nCntSpGage].fAddWidth;

			if (g_aSpGage[nCntSpGage].state == SPGAGE_STATE_DOWN)
			{ // ゲージが減少中の場合
				if (g_aSpGage[nCntSpGage].fWidth < g_aSpGage[nCntSpGage].fDestWidth)
				{ // 目的の値より小さくなった場合
					// 目的の値を代入
					g_aSpGage[nCntSpGage].fWidth = g_aSpGage[nCntSpGage].fDestWidth;
					// 増減量をリセット
					g_aSpGage[nCntSpGage].fAddWidth = 0.0f;
					// 状態を通常状態にする
					g_aSpGage[nCntSpGage].state = SPGAGE_STATE_NONE;
				}
			}
			else if (g_aSpGage[nCntSpGage].state == SPGAGE_STATE_UP)
			{ // ゲージが上昇中の場合
				if (g_aSpGage[nCntSpGage].fWidth > g_aSpGage[nCntSpGage].fDestWidth)
				{ // 目的の値より大きくなった場合
				  // 目的の値を代入
					g_aSpGage[nCntSpGage].fWidth = g_aSpGage[nCntSpGage].fDestWidth;
					// 増減量をリセット
					g_aSpGage[nCntSpGage].fAddWidth = 0.0f;
					// 状態を通常状態にする
					g_aSpGage[nCntSpGage].state = SPGAGE_STATE_NONE;
				}
			}

			// 頂点情報の更新(個別)
			UpdateVertexSpGage(nCntSpGage);
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawSpGage(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffSpGage, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for(int nCntSpGage = 0; nCntSpGage < SPGAGE_MAX; nCntSpGage++)
	{
		if (g_aSpGage[nCntSpGage].bUse == true)	// SPゲージが使用されている
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureSpGage[g_aSpGage[nCntSpGage].nTexType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSpGage * 4, 2);
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// SPゲージの設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetSpGage(D3DXVECTOR3 pos, D3DXVECTOR3 rot, SPGAGE_TYPE type)
{
	for (int nCntSpGage = 0; nCntSpGage < SPGAGE_MAX; nCntSpGage++)
	{
		if (g_aSpGage[nCntSpGage].bUse == false)
		{ // SPゲージが使用されていない場合
			 // 頂点情報の設定
			g_aSpGage[nCntSpGage].pos = pos;
			g_aSpGage[nCntSpGage].rot = rot;
			g_aSpGage[nCntSpGage].fFrameWidth = SPGAGE_FRAME_WIDTH;
			g_aSpGage[nCntSpGage].type = type;

			if (g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_FRAME || g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_FRAME)
			{ // フレームの場合
				g_aSpGage[nCntSpGage].nTexType = SPGAGE_TEX_FRAME;
			}
			else if (g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_SPBLACK || g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_SPBLACK)
			{ // SP黒の場合
				g_aSpGage[nCntSpGage].nTexType = SPGAGE_TEX_SPBLACK;
			}
			else if (g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_SPRED || g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_SPRED)
			{ // SP赤の場合
				g_aSpGage[nCntSpGage].nTexType = SPGAGE_TEX_SPRED;
			}
			else if (g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_SP || g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_SP)
			{ // SPの場合
				g_aSpGage[nCntSpGage].nTexType = SPGAGE_TEX_SP;
			}


			if (g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_SPBLACK || g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_SPBLACK)
			{ // SP黒の場合色を黒に設定する
				g_aSpGage[nCntSpGage].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			}
			else if (g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_SPRED || g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_SPRED)
			{ // SP赤の場合色を青っぽやつに設定する
				g_aSpGage[nCntSpGage].col = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
			}

			// 種類別頂点位置設定
			if(g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_FRAME || g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_FRAME)
			{ // フレームの場合フレームの幅を設定
				g_aSpGage[nCntSpGage].fWidth = SPGAGE_WIDTH;
				g_aSpGage[nCntSpGage].fHeight = SPGAGE_HEIGHT;
			}
			else
			{ // フレーム以外の場合
				g_aSpGage[nCntSpGage].fWidth = SPGAGE_WIDTH - g_aSpGage[nCntSpGage].fFrameWidth * 2.0f;
				g_aSpGage[nCntSpGage].fHeight = SPGAGE_HEIGHT - g_aSpGage[nCntSpGage].fFrameWidth * 2.0f;
			}

			// 初期幅を記録
			g_aSpGage[nCntSpGage].fInitWidth = g_aSpGage[nCntSpGage].fWidth;

			// 頂点情報の更新(個別)
			UpdateVertexSpGage(nCntSpGage);

			g_aSpGage[nCntSpGage].bUse = true;	// 使用する状態にする
			break;	// このbreakはかなり大切
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// SPゲージのまとめ設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetSpAsset(D3DXVECTOR3 pos, LEG_TYPE legType)
{
	D3DXVECTOR3 rot;
	if (legType == LEG_TYPE_LEFT)
	{ // 左ゲージの場合
		rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		SetSpGage(pos, rot, SPGAGE_TYPE_1P_FRAME);
		SetSpGage(pos, rot, SPGAGE_TYPE_1P_SPBLACK);
		SetSpGage(pos, rot, SPGAGE_TYPE_1P_SPRED);
		SetSpGage(pos, rot, SPGAGE_TYPE_1P_SP);
	}
	else if (legType == LEG_TYPE_RIGHT)
	{ // 右ゲージの場合
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		SetSpGage(pos, rot, SPGAGE_TYPE_2P_FRAME);
		SetSpGage(pos, rot, SPGAGE_TYPE_2P_SPBLACK);
		SetSpGage(pos, rot, SPGAGE_TYPE_2P_SPRED);
		SetSpGage(pos, rot, SPGAGE_TYPE_2P_SP);
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// SPゲージの値設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetSpGageSkillPoint(int nSkillPoint, LEG_TYPE legType)
{
	// lifeを代入
	g_aSkillPoint[legType] = nSkillPoint;

	for (int nCntSpGage = 0; nCntSpGage < SPGAGE_MAX; nCntSpGage++)
	{
		if (g_aSpGage[nCntSpGage].bUse == true)	// SPゲージが使用されている
		{
			if (legType == LEG_TYPE_LEFT &&
				(g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_SP || g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_SPRED))
			{ // 左ゲージの場合
			  // 値を設定
				g_aSpGage[nCntSpGage].fWidth = g_aSpGage[nCntSpGage].fInitWidth * (g_aSkillPoint[legType] / float(SPGAGE_MAX_VALUE));
			}
			else if (legType == LEG_TYPE_RIGHT &&
				(g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_SP || g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_SPRED))
			{ // 右ゲージの場合
			  // 値を設定
				g_aSpGage[nCntSpGage].fWidth = g_aSpGage[nCntSpGage].fInitWidth * (g_aSkillPoint[legType] / float(SPGAGE_MAX_VALUE));
			}
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// SPゲージの目的の値設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetSpGageDestSkillPoint(int nSkillPoint, LEG_TYPE legType)
{
	SPGAGE_STATE state;

	// 状態の設定
	if (g_aSkillPoint[legType] > nSkillPoint)
	{ // 元のSkillPointの方が大きい場合
		// 減少状態にする
		state = SPGAGE_STATE_DOWN;
	}
	else
	{
		// 上昇状態にする
		state = SPGAGE_STATE_UP;
	}

	// lifeを代入
	g_aSkillPoint[legType] = nSkillPoint;

	for (int nCntSpGage = 0; nCntSpGage < SPGAGE_MAX; nCntSpGage++)
	{
		if (g_aSpGage[nCntSpGage].bUse == true)	// SPゲージが使用されている
		{
			if (legType == LEG_TYPE_LEFT && g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_SP)
			{ // 1P用の場合
				// 目的の値を設定
				g_aSpGage[nCntSpGage].fDestWidth = g_aSpGage[nCntSpGage].fInitWidth * (g_aSkillPoint[legType] / float(SPGAGE_MAX_VALUE));

				// 目的の値までの1フレームあたりの増加量を算出
				g_aSpGage[nCntSpGage].fAddWidth = (g_aSpGage[nCntSpGage].fDestWidth - g_aSpGage[nCntSpGage].fWidth) / SPGAGE_ADD_FRAME;

				// 状態を設定する
				g_aSpGage[nCntSpGage].state = state;
			}
			else if (legType == LEG_TYPE_RIGHT && g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_SP)
			{ // 2P用の場合
			  // 目的の値を設定
				g_aSpGage[nCntSpGage].fDestWidth = g_aSpGage[nCntSpGage].fInitWidth * (g_aSkillPoint[legType] / float(SPGAGE_MAX_VALUE));

				// 目的の値までの1フレームあたりの増加量を算出
				g_aSpGage[nCntSpGage].fAddWidth = (g_aSpGage[nCntSpGage].fDestWidth - g_aSpGage[nCntSpGage].fWidth) / SPGAGE_ADD_FRAME;

				// 状態を設定する
				g_aSpGage[nCntSpGage].state = state;
			}
		}
	}

	// 赤SPゲージの減少開始までにかかるフレーム数を設定
	g_aRedSkillPointGageCounter[legType] = SPGAGE_RED_ADD_START_FRAME;
}


//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 頂点情報の更新
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateVertexSpGage(void)
{
	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSpGage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSpGage = 0; nCntSpGage < SPGAGE_MAX; nCntSpGage++, pVtx += 4)
	{
		// 種類別頂点位置設定
		if (g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_FRAME || g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_FRAME)
		{ // フレームの場合フレームの幅を設定
			// 回転情報
			float fAngleUp = atan2f((g_aSpGage[nCntSpGage].fWidth), (-g_aSpGage[nCntSpGage].fHeight / 2));
			float fLength = sqrtf((g_aSpGage[nCntSpGage].fWidth) * (g_aSpGage[nCntSpGage].fWidth) + (g_aSpGage[nCntSpGage].fHeight / 2) * (g_aSpGage[nCntSpGage].fHeight / 2));
			float fAngleDown = atan2f((g_aSpGage[nCntSpGage].fWidth), (g_aSpGage[nCntSpGage].fHeight / 2));

			// 頂点情報の設定
			pVtx[0].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(D3DX_PI + g_aSpGage[nCntSpGage].rot.y), cosf(D3DX_PI + g_aSpGage[nCntSpGage].rot.y), 0.0f) * (g_aSpGage[nCntSpGage].fHeight / 2);
			pVtx[1].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngleUp + g_aSpGage[nCntSpGage].rot.y), cosf(fAngleUp + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength;
			pVtx[2].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(0.0f + g_aSpGage[nCntSpGage].rot.y), cosf(0.0f + g_aSpGage[nCntSpGage].rot.y), 0.0f) * (g_aSpGage[nCntSpGage].fHeight / 2);
			pVtx[3].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngleDown + g_aSpGage[nCntSpGage].rot.y), cosf(fAngleDown + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength;
		}
		else
		{ // フレーム以外の場合
			// 回転情報
			// 頂点情報[0]の角度
			float fAngle0 = atan2f((g_aSpGage[nCntSpGage].fFrameWidth), (-g_aSpGage[nCntSpGage].fHeight / 2));
			// 頂点情報[2]の角度
			float fAngle2 = atan2f((g_aSpGage[nCntSpGage].fFrameWidth), (g_aSpGage[nCntSpGage].fHeight / 2));
			// 頂点情報[0][2]の距離
			float fLength02 = sqrtf((g_aSpGage[nCntSpGage].fFrameWidth) * (g_aSpGage[nCntSpGage].fFrameWidth)
				+ (g_aSpGage[nCntSpGage].fHeight / 2) * (g_aSpGage[nCntSpGage].fHeight / 2));
			// 頂点情報[1]の角度
			float fAngle1 = atan2f((g_aSpGage[nCntSpGage].fWidth + g_aSpGage[nCntSpGage].fFrameWidth), (-g_aSpGage[nCntSpGage].fHeight / 2));
			// 頂点情報[3]の角度
			float fAngle3 = atan2f((g_aSpGage[nCntSpGage].fWidth + g_aSpGage[nCntSpGage].fFrameWidth), (g_aSpGage[nCntSpGage].fHeight / 2));
			// 頂点情報[1][3]の距離
			float fLength13 = sqrtf((g_aSpGage[nCntSpGage].fWidth + g_aSpGage[nCntSpGage].fFrameWidth) * (g_aSpGage[nCntSpGage].fWidth + g_aSpGage[nCntSpGage].fFrameWidth)
				+ (g_aSpGage[nCntSpGage].fHeight / 2) * (g_aSpGage[nCntSpGage].fHeight / 2));

			// 頂点情報の設定
			pVtx[0].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngle0 + g_aSpGage[nCntSpGage].rot.y), cosf(fAngle0 + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength02;
			pVtx[1].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngle1 + g_aSpGage[nCntSpGage].rot.y), cosf(fAngle1 + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength13;
			pVtx[2].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngle2 + g_aSpGage[nCntSpGage].rot.y), cosf(fAngle2 + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength02;
			pVtx[3].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngle3 + g_aSpGage[nCntSpGage].rot.y), cosf(fAngle3 + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength13;
		}
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		pVtx[0].col = g_aSpGage[nCntSpGage].col;
		pVtx[1].col = g_aSpGage[nCntSpGage].col;
		pVtx[2].col = g_aSpGage[nCntSpGage].col;
		pVtx[3].col = g_aSpGage[nCntSpGage].col;
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffSpGage->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 頂点情報の個別更新
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateVertexSpGage(int nCntSpGage)
{
	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSpGage->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を進める
	pVtx += 4 * nCntSpGage;

	// 種類別頂点位置設定
	if (g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_1P_FRAME || g_aSpGage[nCntSpGage].type == SPGAGE_TYPE_2P_FRAME)
	{ // フレームの場合フレームの幅を設定
		// 回転情報
		float fAngleUp = atan2f((g_aSpGage[nCntSpGage].fWidth), (-g_aSpGage[nCntSpGage].fHeight / 2));
		float fLength = sqrtf((g_aSpGage[nCntSpGage].fWidth) * (g_aSpGage[nCntSpGage].fWidth) + (g_aSpGage[nCntSpGage].fHeight / 2) * (g_aSpGage[nCntSpGage].fHeight / 2));
		float fAngleDown = atan2f((g_aSpGage[nCntSpGage].fWidth), (g_aSpGage[nCntSpGage].fHeight / 2));

		// 頂点情報の設定
		pVtx[0].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(D3DX_PI + g_aSpGage[nCntSpGage].rot.y), cosf(D3DX_PI + g_aSpGage[nCntSpGage].rot.y), 0.0f) * (g_aSpGage[nCntSpGage].fHeight / 2);
		pVtx[1].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngleUp + g_aSpGage[nCntSpGage].rot.y), cosf(fAngleUp + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength;
		pVtx[2].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(0.0f + g_aSpGage[nCntSpGage].rot.y), cosf(0.0f + g_aSpGage[nCntSpGage].rot.y), 0.0f) * (g_aSpGage[nCntSpGage].fHeight / 2);
		pVtx[3].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngleDown + g_aSpGage[nCntSpGage].rot.y), cosf(fAngleDown + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength;
	}
	else
	{ // フレーム以外の場合
		// 回転情報
		// 頂点情報[0]の角度
		float fAngle0 = atan2f((g_aSpGage[nCntSpGage].fFrameWidth), (-g_aSpGage[nCntSpGage].fHeight / 2));
		// 頂点情報[2]の角度
		float fAngle2 = atan2f((g_aSpGage[nCntSpGage].fFrameWidth), (g_aSpGage[nCntSpGage].fHeight / 2));
		// 頂点情報[0][2]の距離
		float fLength02 = sqrtf((g_aSpGage[nCntSpGage].fFrameWidth) * (g_aSpGage[nCntSpGage].fFrameWidth)
			+ (g_aSpGage[nCntSpGage].fHeight / 2) * (g_aSpGage[nCntSpGage].fHeight / 2));
		// 頂点情報[1]の角度
		float fAngle1 = atan2f((g_aSpGage[nCntSpGage].fWidth + g_aSpGage[nCntSpGage].fFrameWidth), (-g_aSpGage[nCntSpGage].fHeight / 2));
		// 頂点情報[3]の角度
		float fAngle3 = atan2f((g_aSpGage[nCntSpGage].fWidth + g_aSpGage[nCntSpGage].fFrameWidth), (g_aSpGage[nCntSpGage].fHeight / 2));
		// 頂点情報[1][3]の距離
		float fLength13 = sqrtf((g_aSpGage[nCntSpGage].fWidth + g_aSpGage[nCntSpGage].fFrameWidth) * (g_aSpGage[nCntSpGage].fWidth + g_aSpGage[nCntSpGage].fFrameWidth)
			+ (g_aSpGage[nCntSpGage].fHeight / 2) * (g_aSpGage[nCntSpGage].fHeight / 2));

		// 頂点情報の設定
		pVtx[0].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngle0 + g_aSpGage[nCntSpGage].rot.y), cosf(fAngle0 + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength02;
		pVtx[1].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngle1 + g_aSpGage[nCntSpGage].rot.y), cosf(fAngle1 + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength13;
		pVtx[2].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngle2 + g_aSpGage[nCntSpGage].rot.y), cosf(fAngle2 + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength02;
		pVtx[3].pos = g_aSpGage[nCntSpGage].pos + D3DXVECTOR3(sinf(fAngle3 + g_aSpGage[nCntSpGage].rot.y), cosf(fAngle3 + g_aSpGage[nCntSpGage].rot.y), 0.0f) * fLength13;
	}
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	pVtx[0].col = g_aSpGage[nCntSpGage].col;
	pVtx[1].col = g_aSpGage[nCntSpGage].col;
	pVtx[2].col = g_aSpGage[nCntSpGage].col;
	pVtx[3].col = g_aSpGage[nCntSpGage].col;
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffSpGage->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 頂点情報の作成
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexSpGage(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SPGAGE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSpGage,
		NULL);

	// 頂点情報の更新
	UpdateVertexSpGage();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// スキルポイントの増加
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void AddSkillPoint(int nPlayerID, int nSkillPoint)
{
	g_nSkillPointPlayerTest[nPlayerID] += nSkillPoint;

	if (g_nSkillPointPlayerTest[nPlayerID] > SPGAGE_MAX_VALUE)
	{// 最大値を超える場合
		g_nSkillPointPlayerTest[nPlayerID] = SPGAGE_MAX_VALUE;
	}
	else if (g_nSkillPointPlayerTest[nPlayerID] < 0)
	{
		g_nSkillPointPlayerTest[nPlayerID] = 0;
	}


	// スキルゲージの変更
	SetSpGageDestSkillPoint(g_nSkillPointPlayerTest[nPlayerID], (LEG_TYPE)nPlayerID);
}