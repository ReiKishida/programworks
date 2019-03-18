//=============================================================================
//
// シーン3D処理[3D ポリゴン] [scene3D.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _PR_H_
#define _PR_H_

#include "main.h"
#include "scene3D.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE_PR (7)
//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
// シーン3Dのクラス構造体
//*****************************************************************************
class CPr : public CScene3D
{
public:
	typedef enum
	{
		PRTYPE_BOARD_STREAM = 0,			//電光掲示板（流れる文字）
		PRTYPE_BOARD_SWITCH_0,		//電光掲示板（切り換え1）
		PRTYPE_BOARD_SWITCH_1,		//電光掲示板（切り換え2）
		PRTYPE_BOARD_ADVERTISE_0,	//広告（0）
		PRTYPE_BOARD_ADVERTISE_1,	//広告（1）
		PRTYPE_BOARD_ADVERTISE_2,	//広告（2）
		PRTYPE_BOARD_ADVERTISE_3,	//広告（2）
		PRTYPE_MAX
	}PRTYPE;
	static HRESULT Load(void);
	static void Unload(void);
	CPr::CPr(int nPriority = 4, OBJTYPE type = OBJTYPE_PR);	//コンストラクタ
	CPr::~CPr();	//デストラクタ
	static CPr *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, PRTYPE type);//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理


private:
	static LPDIRECT3DTEXTURE9	m_apTexture[MAX_TEXTURE_PR];		// テクスチャへのポインタ
	D3DXVECTOR3				m_pos;			// ポリゴンの位置
	D3DXVECTOR3				m_rot;			// ポリゴンの回転
	D3DXVECTOR2 m_aPr[4];	//背景の最大数
	D3DXCOLOR m_col;		//色情報
	float m_fWidth;			//幅
	float m_fHeight;		//高さ
	PRTYPE m_type;
	int m_nCntFrame;
	int m_nRand;
};

#endif