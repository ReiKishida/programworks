//=============================================================================
//
// メッシュウォール処理 [meshwall.h]
// Author : 岸田怜(Kishida Rei)
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"

//*****************************************************************************
// メッシュフィールドクラスの構造体
//*****************************************************************************
class CSceneMeshCylinder : public CScene
{
public:
	typedef enum
	{
		CYLINDERTYPE_NORMAL = 0,	//通常シリンダー
		CYLINDERTYPE_GOAL,			//ゴール用のシリンダー
		CYLINDERTYPE_MAX			//シリンダーの最大数
	}CYLINDERTYPE;					//シリンダーの種類
	CSceneMeshCylinder(int nPriority = 3, OBJTYPE type = OBJTYPE_MESHCYLINDER);		//コンストラクタ
	~CSceneMeshCylinder();			//デストラクタ
	static CSceneMeshCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sprit_H, int sprit_V, float fRadius, float fHeight, CYLINDERTYPE type);//シリンダーの生成
	HRESULT Init(void);							//初期化処理
	void Uninit(void);							//終了処理
	void Update(void);							//更新処理
	void Draw(void);							//描画処理
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);	//頂点情報の生成
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);	//インデックス情報の生成
	bool CollisionCylinder(D3DXVECTOR3 *pos);	//シリンダーとの当たり判定
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;			//インデックスバッファへのポインタ
	//シリンダーの情報
	D3DXVECTOR3 m_pos;		//位置(中心座標)
	D3DXVECTOR3 m_rot;		//向き
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	D3DXCOLOR m_col;		//色情報
	D3DXVECTOR2 m_TexPos;	//テクスチャ座標
	int m_nMesh_V;			//垂直の分割
	int m_nMesh_H;			//水平の分割
	int m_nVertexNum;		//頂点数
	int m_nIndexNum;		//インデックス数
	int m_nPolygonNum;		//ポリゴン数
	float m_fRadius;		//メッシュフィールドの半径
	float m_fHeight;		//メッシュフィールドの高さ
	CYLINDERTYPE m_type;	//シリンダーの種類
};

#endif
