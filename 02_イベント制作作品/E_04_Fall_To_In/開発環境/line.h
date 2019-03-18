////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����p���C���̏��� [line.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _LINE_H_
#define _LINE_H_

#include "main.h"
#include "scene.h"
//******************************************************************************************************************************
// �\���̒�`
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
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_posStart;	// �ŏ��̈ʒu
	D3DXVECTOR3 m_posEnd;	// �Ō�̈ʒu
	D3DXCOLOR	m_col;		// �F
	D3DXVECTOR3 m_move;		// ����
	D3DXVECTOR3 m_rot;		// ����
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
	static bool		m_bUse;		// �g�p����Ă��邩�ǂ���

};

#endif
