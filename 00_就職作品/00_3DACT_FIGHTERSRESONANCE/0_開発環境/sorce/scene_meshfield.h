//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author :
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FIELD_TEXTURE (2)
//*****************************************************************************
// メッシュフィールドクラスの構造体
//*****************************************************************************
class CSceneMeshField : public CScene
{
public:
	typedef enum
	{
		FIELDTYPE_NORMAL = 0,
		FIELDTYPE_YELLOW,
		FIELDTYPE_MAX
	}FIELDTYPE;
	CSceneMeshField(int nPriority = 3, OBJTYPE type = OBJTYPE_MESHFIELD);	//コンストラクタ
	~CSceneMeshField();					//デストラクタ
	static HRESULT Load(void);									//テクスチャの読み込み
	static void Unload(void);									//テクスチャの破棄
	static CSceneMeshField *Create(D3DXVECTOR3 pos, int sprit_X, int sprit_Z, float fWidth, float fHeight, FIELDTYPE type);//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);	//頂点情報の生成
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);	//インデックス情報の生成
	float GetHeight(D3DXVECTOR3 pos);			//高さの取得

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[FIELD_TEXTURE];		//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff; //インデックスバッファへのポインタ

	D3DXVECTOR3 m_pos;			//位置(中心座標)
	D3DXVECTOR3 m_rot;			//向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXCOLOR m_col;			//色情報
	D3DXVECTOR2 m_TexPos;		//テクスチャ座標
	int m_nMesh_X;				//X軸の分割
	int m_nMesh_Z;				//Y軸の分割
	int m_nVertexNum;			//頂点数
	int m_nIndexNum;			//インデックス数
	int m_nPolygonNum;			//ポリゴン数
	static int m_nSquareNum;	//四角面の数
	static int m_nVecNum;		//囲うベクトルの数
	float m_fMeshWidth;			//メッシュフィールドの幅
	float m_fMeshDepth;			//メッシュフィールドの奥行き
	FIELDTYPE m_type;
	static D3DXVECTOR3 Vertexpos[9];
};

#endif
