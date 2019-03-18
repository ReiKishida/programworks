//=============================================================================
//
// �A���̏��� [Environment.h]
// Author : �؉��@�̑�
//
//=============================================================================
#ifndef _ENVIROMENT_H_
#define _ENVIROMENT_H_

#include "main.h"
#include "scene.h"
#include "sceneBillboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENVIROMENT (32)	// �ő吔

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CEnvironmentBillboard : public CBillboard
{// �A���r���{�[�h�N���X(CBillboard�̔h���N���X)
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
	static LPDIRECT3DTEXTURE9	*m_apTexture;	// ���L�e�N�X�`���ւ̃|�C���^
	static int					m_nNumTexture;
	int							m_type;					// ���
	bool						m_bUse;					// �g�p����Ă��邩�ǂ���
};

class CEnvironment : public CScene
{// �N���X(CScene�̔h���N���X)
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
	static CEnvironmentBillboard *m_apEnviroment[MAX_ENVIROMENT];	// �N���X�̃|�C���^�z��
};

#endif