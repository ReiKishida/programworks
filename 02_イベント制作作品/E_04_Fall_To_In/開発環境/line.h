////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 判定用ラインの処理 [line.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _LINE_H_
#define _LINE_H_

#include "main.h"
#include "scene.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
class CLine :public CScene
{
public:
	CLine(int nPriority = CScene::PRIORITY_INIT);
	~CLine();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLine *Create(const D3DXVECTOR3 posStart, const D3DXVECTOR3 posEnd);
	void SetPos(const D3DXVECTOR3 start, const D3DXVECTOR3 end);
	D3DXVECTOR3 GetPosStart(void) { return m_posStart; }
	D3DXVECTOR3 GetPosEnd(void) { return m_posEnd; }

	static bool GetDisp(void){return m_bUse;};
	static void SetDisp(bool bDisp);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_posStart;	// 最初の位置
	D3DXVECTOR3 m_posEnd;	// 最後の位置
	D3DXCOLOR	m_col;		// 色
	D3DXVECTOR3 m_move;		// 速さ
	D3DXVECTOR3 m_rot;		// 向き
	D3DXMATRIX	m_mtxWorld;	// ワールドマトリックス
	static bool		m_bUse;		// 使用されているかどうか

};

#endif
