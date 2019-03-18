//=============================================================================
//
// ���C�g���� [light.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LIGHT			(10)		// ���C�g�̍ő吔

//*****************************************************************************
// ���C�g�̃N���X
//*****************************************************************************
class CLight
{
public:
	typedef struct
	{// ���C�g�̍\����
		D3DLIGHT9 m_Light;				// ���C�g�̏��
		bool m_bLight;					// ���C�g�̃I���I�t
		bool m_bUse;						// �g�p����Ă��邩�ǂ���
	}Light;

	CLight();
	~CLight();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	int SetPoint(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, float Attenuation, float Range);		// �|�C���g���C�g�̐ݒu
	int SetSpot(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR Diffuse);						// �X�|�b�g���C�g�̐ݒu
	void SetPosition(int nIdxLight, D3DXVECTOR3 pos, D3DXVECTOR3 rot);						// ���C�g�̈ʒu,��],�X�V
	void Switching(int nIdxLight);															// ���C�g�̃I���I�t��؂�ւ�

private:
	Light m_aLight[MAX_LIGHT];				// ���C�g�̏��
};


#endif
