//=============================================================================
//
// 植物の処理 [Environment.h]
// Author : 木下　領太
//
//=============================================================================
#ifndef _ENVIROMENT_H_
#define _ENVIROMENT_H_

#include "main.h"
#include "scene.h"
#include "sceneBillboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENVIROMENT (32)	// 最大数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CEnvironmentBillboard : public CBillboard
{// 植物ビルボードクラス(CBillboardの派生クラス)
public:

	CEnvironmentBillboard(int nPrioritiy = PRIORITY_INIT);
	~CEnvironmentBillboard();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const int nIdy,const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int type);
	void SetUse(const bool bUse);
	bool GetUse(void) { return m_bUse; }


private:
	static LPDIRECT3DTEXTURE9	*m_apTexture;	// 共有テクスチャへのポインタ
	static int					m_nNumTexture;
	int							m_type;					// 種類
	bool						m_bUse;					// 使用されているかどうか
};

class CEnvironment : public CScene
{// クラス(CSceneの派生クラス)
public:
	CEnvironment();
	~CEnvironment();
	static CEnvironment *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static int Set(const int nIdy,const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int type);
	static void Delete(const int nID);
	static void SetScan(void);
private:
	static CEnvironmentBillboard *m_apEnviroment[MAX_ENVIROMENT];	// クラスのポインタ配列
};

#endif