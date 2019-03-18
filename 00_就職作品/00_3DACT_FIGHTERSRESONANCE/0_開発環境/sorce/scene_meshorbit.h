//=============================================================================
//
// シーン3D処理[3D ポリゴン] [scene3D.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _SCENEMESHORBIT_H_
#define _SCENEMESHORBIT_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_VERTEX (100)
//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
// シーン3Dのクラス構造体
//*****************************************************************************
class CMeshOrbit : public CScene
{
public:
	CMeshOrbit::CMeshOrbit(int nPriority = 4, OBJTYPE type = OBJTYPE_SCENEORBIT);	//コンストラクタ
	CMeshOrbit::~CMeshOrbit();	//デストラクタ
	static CMeshOrbit *Create();//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//テクスチャの割り当て
	void SetColor(D3DXCOLOR col);	//色の設定
	void CreateOrbit(void);	//軌跡の生成

	//ポリゴンの位置情報の取得、設定
	D3DXVECTOR3 GetPosPolygon(void);
	void SetPosPolygon(D3DXVECTOR3 pos);

	//ポリゴンの角度情報の取得、設定
	D3DXVECTOR3 GetRotPolygon(void);
	void SetRotPolygon(D3DXVECTOR3 pos);

	void SetMtxParent(D3DXMATRIX *pMtx);	//親のマトリックスへのポインタ
private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;			//ポリゴンのマトリックス情報
	D3DXVECTOR3				m_posPolygon;		// ポリゴンの位置
	D3DXVECTOR3				m_rotPolygon;		// ポリゴンの回転
	int m_nCntflame;							//カウント
	int m_nNumScene3D;							//3Dポリゴン数
	int m_nPriority;							//優先度
	float m_fWidth;								//ポリゴンの幅
	float m_fHeight;							//ポリゴンの奥行き
	float m_fnor;								//ポリゴンの法線
	D3DXMATRIX *m_pMtxParent;					//ポリゴンのマトリックス情報
	D3DXVECTOR3 m_aPosVertex[MAX_VERTEX];		//算出した頂点座標
	D3DXVECTOR3 m_aOffSet[2];					//オフセット
};

#endif