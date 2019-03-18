//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : RyotaKinoshita
//
//=============================================================================
#include "light.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "DebugProc.h"
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLight::CLight()
{

}
//=============================================================================
// �f�X�X�g���N�^
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
// ���C�g�̏���������
//=============================================================================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// �f�o�C�X�̎擾
	D3DXVECTOR3 vecDir;											// ���C�g�̊p�x������ϐ�

// -----���-----------------------
	// ���C�g���N���A����
	ZeroMemory(&m_aLight[0], sizeof(D3DLIGHT9));

	// ���C�g�̎�ނ�ݒ�
	m_aLight[0].m_Light.Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g�̊g�U����ݒ�
	m_aLight[0].m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ���C�g�̐F��ݒ�
	// ���C�g�̎��͐F��ݒ�
	m_aLight[0].m_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ���C�g�̎��͐F��ݒ�
	// ���C�g�̋��ʐF��ݒ�
	m_aLight[0].m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ���C�g�̋��ʐF��ݒ�


	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.7f, -0.5f, 0.8f);
	D3DXVec3Normalize(&vecDir, &vecDir);								// ���K������
	m_aLight[0].m_Light.Direction = vecDir;								// ���K�����ꂽ������������

	// ���C�g��ݒ肷��
	pDevice->SetLight(0, &m_aLight[0].m_Light);
	// ���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);
	// �g�p���Ă��邩�ǂ�����ݒ肷��
	m_aLight[0].m_bLight = TRUE;											// �I��
	m_aLight[0].m_bUse = TRUE;												// �g�p���Ă����Ԃɂ���

// ----���------------------------
	// ���C�g���N���A����
	ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));

	// ���C�g�̎�ނ�ݒ�
	m_aLight[1].m_Light.Type = D3DLIGHT_DIRECTIONAL;
	// ���C�g�̐F��ݒ�
	m_aLight[1].m_Light.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ���C�g�̊g�U����ݒ�
	m_aLight[1].m_Light.Ambient  = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);		// ���C�g�̎��͌���ݒ�
	m_aLight[1].m_Light.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);		// ���C�g�̔��ˌ���ݒ�
	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.88f, 0.88f, -0.94f);
	D3DXVec3Normalize(&vecDir, &vecDir);								// ���K������
	m_aLight[1].m_Light.Direction = vecDir;								// ���K�����ꂽ������������
	// ���C�g��ݒ肷��
	pDevice->SetLight(1, &m_aLight[1].m_Light);
	// ���C�g��L���ɂ���
	pDevice->LightEnable(1, TRUE);
	// �g�p���Ă��邩�ǂ�����ݒ肷��
	m_aLight[1].m_bLight = TRUE;											// �I��
	m_aLight[1].m_bUse = TRUE;											// �g�p���Ă����Ԃɂ���
// ----�O��------------------------
	// ���C�g���N���A����
	ZeroMemory(&m_aLight[2], sizeof(D3DLIGHT9));
	// ���C�g�̎�ނ�ݒ�
	m_aLight[2].m_Light.Type = D3DLIGHT_DIRECTIONAL;
	// ���C�g�̊g�U����ݒ�
	m_aLight[2].m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ���C�g�̐F��ݒ�
	// ���C�g�͈̔͂�ݒ�
//	m_aLight[2].Range = 70.75f;
	// ���C�g�̈ʒu��ݒ�
	m_aLight[2].m_Light.Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.89f, -0.1f, 0.94f);
	D3DXVec3Normalize(&vecDir, &vecDir);								// ���K������
	m_aLight[2].m_Light.Direction = vecDir;								// ���K�����ꂽ������������
	// ���C�g��ݒ肷��
	pDevice->SetLight(2, &m_aLight[2].m_Light);
	// ���C�g��L���ɂ���
	pDevice->LightEnable(2, TRUE);
	// �g�p���Ă��邩�ǂ�����ݒ肷��
	m_aLight[2].m_bLight = TRUE;											// �I��
	m_aLight[2].m_bUse = TRUE;												// �g�p���Ă����Ԃɂ���
	return S_OK;
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void CLight::Update(void)
{
	CDebugProc::print("���C�g�̕����F(%.1f,%.1f,%.1f)\n", m_aLight[0].m_Light.Direction.x, m_aLight[0].m_Light.Direction.y, m_aLight[0].m_Light.Direction.z);
}

//=============================================================================
// �|�C���g���C�g�̐ݒ�
//=============================================================================
int CLight::SetPoint(D3DXVECTOR3 pos, D3DXCOLOR Diffuse,float Attenuation,float Range)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// �f�o�C�X�̎擾
	int nCntLight = 0;							// ���C�g�̃J�E���g�ϐ�

	for (nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ���C�g�̍ő吔�����[�v
		if (m_aLight[nCntLight].m_bUse == false)
		{// ���C�g���g���Ă��Ȃ���ԂȂ�
			// ���C�g���N���A����
			ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

			// ���C�g�̎�ނ�ݒ�
			m_aLight[nCntLight].m_Light.Type = D3DLIGHT_POINT;

			// ���C�g�̈ʒu�̐ݒ�
			m_aLight[nCntLight].m_Light.Position = pos;									// ���C�g�̈ʒu��ݒ�

			// ���C�g�̐F��ݒ�
			m_aLight[nCntLight].m_Light.Diffuse  = Diffuse;								// ���C�g�̊g�U����ݒ�
			m_aLight[nCntLight].m_Light.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ���C�g�̎��͐F��ݒ�
			m_aLight[nCntLight].m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ���C�g�̋��ʐF��ݒ�

			// �����̐ݒ�
			m_aLight[nCntLight].m_Light.Attenuation0 = Attenuation;						// ���`�����W��(��{�I�ɂ����ŕς��)
			m_aLight[nCntLight].m_Light.Attenuation1 = 0.0f;								// 2�������W��(��{�I��0.0f�ő��v)
			m_aLight[nCntLight].m_Light.Attenuation2 = 0.0f;								// �w�������W��(��{�I��0.0f�ő��v)

			// �����͈̔͂̐ݒ�
			m_aLight[nCntLight].m_Light.Range = Range;										// �͈�

			// ���C�g��ݒ肷��
			pDevice->SetLight(nCntLight, &m_aLight[nCntLight].m_Light);

			// ���C�g��L���ɂ���
			pDevice->LightEnable(nCntLight, TRUE);

			m_aLight[nCntLight].m_bLight = true;											// ���C�g���I���ɂ���
			m_aLight[nCntLight].m_bUse	  = true;											// ���C�g���g�p��Ԃɂ���

			break;
		}
	}
	return nCntLight;																	// ������ԍ��𑗂�
}

//=============================================================================
// �X�|�b�g���C�g�̐ݒ�
//=============================================================================
int CLight::SetSpot(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR Diffuse)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// �f�o�C�X�̎擾
	D3DXVECTOR3 vecDir;							// ���C�g�̊p�x������ϐ�
	int nCntLight = 0;							// ���C�g�̃J�E���g�ϐ�

	for (nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ���C�g�̍ő吔�����[�v
		if (m_aLight[nCntLight].m_bUse == false)
		{// ���C�g���g���Ă��Ȃ���ԂȂ�
		 // ���C�g���N���A����
			ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

			// ���C�g�̎�ނ�ݒ�
			m_aLight[nCntLight].m_Light.Type = D3DLIGHT_SPOT;

			// ���C�g�̈ʒu�̐ݒ�
			m_aLight[nCntLight].m_Light.Position = pos;									// ���C�g�̈ʒu��ݒ�

			// ���C�g�̐F��ݒ�
			m_aLight[nCntLight].m_Light.Diffuse  = Diffuse;								// ���C�g�̊g�U����ݒ�
			m_aLight[nCntLight].m_Light.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ���C�g�̎��͐F��ݒ�
			m_aLight[nCntLight].m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ���C�g�̋��ʐF��ݒ�

			// �����̐ݒ�
			m_aLight[nCntLight].m_Light.Attenuation0 = 0.1f;								// ���`�����W��(��{�I��0.1f���炢�ő��v)
			m_aLight[nCntLight].m_Light.Attenuation1 = 0.0f;								// 2�������W��(��{�I��0.0f�ő��v)
			m_aLight[nCntLight].m_Light.Attenuation2 = 0.0f;								// �w�������W��(��{�I��0.0f�ő��v)

			// �����͈̔͂̐ݒ�
			m_aLight[nCntLight].m_Light.Range = 220.0f;										// �͈�

			// �X�|�b�g���C�g�̃t�H�[���I�t�̐ݒ�
			m_aLight[nCntLight].m_Light.Falloff = 1.0f;

			// �X�|�b�g���C�g�̓����R�[���̊p�x�̐ݒ�
			m_aLight[nCntLight].m_Light.Theta = 0.5f;

			// �X�|�b�g���C�g�̊O���R�[���̊p�x�̐ݒ�
			m_aLight[nCntLight].m_Light.Phi = D3DX_PI / 2;


			// ���C�g�̕����̐ݒ�
			vecDir = rot;														// ���C�g�̊p�x����
			D3DXVec3Normalize(&vecDir, &vecDir);								// ���C�g�̊p�x�𐳋K������
			m_aLight[nCntLight].m_Light.Direction = vecDir;								// ���K�����ꂽ�����Ŋp�x��ݒ肷��

			// ���C�g��ݒ肷��
			pDevice->SetLight(nCntLight, &m_aLight[nCntLight].m_Light);

			// ���C�g��L���ɂ���
			pDevice->LightEnable(nCntLight, TRUE);

			m_aLight[nCntLight].m_bLight = true;											// ���C�g���I���ɂ���
			m_aLight[nCntLight].m_bUse = true;												// ���C�g���g�p��Ԃɂ���

			break;
		}
	}
	return nCntLight;																	// ������ԍ��𑗂�
}

//=============================================================================
// ���C�g�̈ʒu�X�V
//=============================================================================
void CLight::SetPosition(int nIdxLight, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{

	if (nIdxLight != -1)
	{// -1�Ȃ珈�����Ȃ�
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// �f�o�C�X�̎擾
		D3DXVECTOR3 vecDir;											// ���C�g�̊p�x������ϐ�

		// ���C�g�̈ʒu��ݒ�
		m_aLight[nIdxLight].m_Light.Position = pos;					// ���C�g�̈ʒu��ݒ�

		// ���C�g�̕����̐ݒ�
		vecDir = rot;												// ���C�g�̊p�x������
		D3DXVec3Normalize(&vecDir, &vecDir);						// ���K������
		m_aLight[nIdxLight].m_Light.Direction = vecDir;				// ���K�����ꂽ������������

		// ���C�g��ݒ肷��
		pDevice->SetLight(nIdxLight, &m_aLight[nIdxLight].m_Light);

	}
}

//=============================================================================
// ���C�g�̃I���I�t�؂�ւ�
//=============================================================================
void CLight::Switching(int nIdxLight)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// �f�o�C�X�̎擾

	// ���C�g�̃I���I�t��؂�ւ�
	m_aLight[nIdxLight].m_bLight = m_aLight[nIdxLight].m_bLight ? false : true;

	// ���C�g��L���ɂ���
	pDevice->LightEnable(nIdxLight, m_aLight[nIdxLight].m_bLight);

}