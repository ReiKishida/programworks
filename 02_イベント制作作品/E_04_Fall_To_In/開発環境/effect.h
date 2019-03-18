//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : �T�c ����
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_EFFECT (1024)	// �G�t�F�N�g�̍ő吔

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CSceneEffect
{// �G�t�F�N�g�|���S���N���X
public:
	typedef enum
	{// �G�t�F�N�g�̎��
		TYPE_CIRCLE = 0,	// �~
		TYPE_SHINY,			// �L���L��
		TYPE_EXPLOSION,		// ����
		TYPE_INK,		// �C���N
		TYPE_MAX			// ��ނ̑���
	} TYPE;

	typedef enum
	{// �G�t�F�N�g�̃T�C�Y�ω��̎��
		SCALETYPE_NONE = 0,	// �ς��Ȃ�
		SCALETYPE_UP,		// �g��
		SCALETYPE_DOWN,		// �k��
		SCALETYPE_UPDOWN,	// �g�k
		SCALETYPE_MAX		// ��ނ̑���
	} SCALETYPE;
	CSceneEffect();
	~CSceneEffect();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius, const D3DXCOLOR col, const int nLife, const float fRot, const TYPE type, const SCALETYPE scaleType, const float fTrans);
	bool GetUse(void) { return m_bUse; }
private:
	static LPDIRECT3DTEXTURE9	m_apTexture[TYPE_MAX];	// ���L�e�N�X�`���ւ̃|�C���^�z��
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX					m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3					m_pos;					// �ʒu
	D3DXVECTOR3					m_move;					// �ړ���
	float						m_fRadius;				// ���a
	float						m_fRadiusInit;			// �������a
	D3DXCOLOR					m_col;					// �F
	int							m_nLife;				// �\������
	float						m_fRot;					// ��]��
	float						m_fAngle;				// �p�x
	float						m_fLength;				// �Ίp���̒���
	TYPE						m_type;					// ���
	SCALETYPE					m_scaleType;			// �T�C�Y�ω��̎��
	float						m_fTrans;				// �g�k�x
	float						m_fAlpha;				// �A���t�@�����l
	int							m_nCounter;				// �J�E���^�[
	bool						m_bUse;					// �g�p����Ă���ǂ���
};

class CEffect : public CScene
{// �G�t�F�N�g�N���X(CScene�̔h���N���X)
public:
	CEffect();
	~CEffect();
	static CEffect *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius, const D3DXCOLOR col, const int nLife, const float fRot, const CSceneEffect::TYPE type, const CSceneEffect::SCALETYPE scaleType, const float fTrans);
	static void SetExplosion(const D3DXVECTOR3 pos, const int nPlayerID);
private:
	static CSceneEffect *m_apEffect[MAX_EFFECT];	// �G�t�F�N�g�N���X�̃|�C���^�z��
};

#endif