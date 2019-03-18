//=============================================================================
//
// �Q�[�W����[2D�|���S��] [ui_gauge.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene_gauge.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "ui_gauge.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CUiGauge::m_apTexture[MAX_GAUGE_TEX] = {};
//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CUiGauge::CUiGauge(int nPriority, OBJTYPE type) : CSceneGauge(nPriority, type)
{
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CUiGauge::~CUiGauge()
{

}

//=============================================================================
// �֐����F�g�p����e�N�X�`���̓ǂݍ���
// �֐��̊T�v�F--
//=============================================================================
HRESULT CUiGauge::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/1_lifegauge.png", &m_apTexture[0]);		// ���C�t�Q�[�W
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/face_P.png", &m_apTexture[1]);		// ���C�t�Q�[�W
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/face_B.png", &m_apTexture[2]);		// ���C�t�Q�[�W

	return S_OK;

}

//=============================================================================
// �֐����F�g�p�����e�N�X�`���̔j��
// �֐��̊T�v�F--
//=============================================================================
void CUiGauge::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_GAUGE_TEX; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}


//=============================================================================
// �֐����F�Q�[�W�̐���
// �֐��̊T�v�F�Q�[�W�𐶐�����
//=============================================================================
CUiGauge *CUiGauge::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fCurrentGauge, float fMaxGauge, GAUGETYPE type)
{
	CUiGauge  *pSceneGauge;
	pSceneGauge = new CUiGauge;	//�V�[���N���X�̐���
	if (pSceneGauge != NULL)
	{
		pSceneGauge->m_pos = pos;						//�ʒu���̑��
		pSceneGauge->m_fWidth = fWidth;					//���̑��
		pSceneGauge->m_fHeight = fHeight;				//�����̑��
		pSceneGauge->m_fCurrentGauge = fCurrentGauge;	//���݂̒l�̑��
		pSceneGauge->m_fMaxGauge = fMaxGauge;			//�ő�l�̑��
		pSceneGauge->m_col = col;						//�Q�[�W�̐F���
		pSceneGauge->m_type = type;						//�Q�[�W�̎�ނ̑��
		pSceneGauge->Init();
	}
	return pSceneGauge;
}


//=============================================================================
// �֐����F�Q�[�W�̏���������
// �֐��̊T�v�F�|���S���̐ݒ�
//=============================================================================
HRESULT CUiGauge::Init(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//�Q�[�W�̕��̑��
	SetGaugeWidth(m_fWidth);
	//�Q�[�W�̍����̑��
	SetGaugeHeight(m_fHeight);

	//���݂̒l�̑��
	SetCurrentValue(m_fCurrentGauge);
	//�ő�l�̑��
	SetMaxValue(m_fMaxGauge);

	if (m_type == GAUGETYPE_UNDER)
	{
		//�e�N�X�`���̊��蓖��
		BindTexture(m_apTexture[0]);
	}
	if (m_type == GAUGETYPE_FACE_PLAYER)
	{
		//�e�N�X�`���̊��蓖��
		BindTexture(m_apTexture[1]);
	}
	if (m_type == GAUGETYPE_FACE_BOSS)
	{
		//�e�N�X�`���̊��蓖��
		BindTexture(m_apTexture[2]);
	}

	//�Q�[�W�̏�����
	CSceneGauge::Init();
	//�F���̑��
	SetColor(m_col);


	//�ʒu���̑��
	SetPosGauge(m_pos);

	return S_OK;
}

//=============================================================================
// �֐����F�Q�[�W�̏I������
// �֐��̊T�v�F�e�N�X�`���A���_�o�b�t�@�̔j���A���g�̔j��
//=============================================================================
void CUiGauge::Uninit(void)
{
	CSceneGauge::Uninit();
}

//=============================================================================
// �֐����F�Q�[�W�̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CUiGauge::Update(void)
{
	//�ʒu���̎擾
	D3DXVECTOR3 pos = GetPosGauge();

	//�t���[���̉��Z
	m_nCntFrame++;
	if (m_type == GAUGETYPE_SKILL)
	{
		m_nCntFrame = m_nCntFrame % 80;
	}
	if (m_type == GAUGETYPE_GREEN)
	{
		m_nCntFrame = m_nCntFrame % 40;
	}

	if (m_type == GAUGETYPE_GREEN || m_type == GAUGETYPE_RED)
	{
		if (m_fCurrentGauge > m_fDestValue)
		{//�ړI�̃Q�[�W
			m_fCurrentGauge += m_fValue * 0.035f;
			if (m_fCurrentGauge <= 0.0f)
			{
				m_fCurrentGauge = 0.0f;
			}
			if (m_fCurrentGauge >= m_fMaxGauge)
			{
				m_fCurrentGauge = m_fMaxGauge;
			}
			SetCurrentValue(m_fCurrentGauge);
		}
	}
	//�X�L���Q�[�W
	if (m_type == GAUGETYPE_SKILL)
	{
		if (m_fCurrentGauge < m_fDestValue)
		{//�ړI�̃Q�[�W
			m_fCurrentGauge += m_fValue * 0.035f;
			if (m_fCurrentGauge <= 0.0f)
			{
				m_fCurrentGauge = 0.0f;
			}
			if (m_fCurrentGauge >= m_fMaxGauge)
			{
				m_fCurrentGauge = m_fMaxGauge;

			}
			SetCurrentValue(m_fCurrentGauge);
		}
		if (m_fCurrentGauge > m_fDestValue)
		{//�ړI�̃Q�[�W
			m_fCurrentGauge += m_fValue * 0.035f;
			if (m_fCurrentGauge <= 0.0f)
			{
				m_fCurrentGauge = 0.0f;
			}
			if (m_fCurrentGauge >= m_fMaxGauge)
			{
				m_fCurrentGauge = m_fMaxGauge;

			}
			SetCurrentValue(m_fCurrentGauge);
		}
		if (m_fCurrentGauge >= m_fMaxGauge -0.2f)
		{
			if (m_nCntFrame < 40)
			{
				m_col.a -= 0.01f;
				if (m_col.a <= 0.0f)
				{
					m_col.a = 0.0f;
				}
			}
			if (m_nCntFrame > 40)
			{
				m_col.a += 0.01f;
				if (m_col.a >= 1.0f)
				{
					m_col.a = 1.0f;
				}
			}
		}
	}

	if (m_type == GAUGETYPE_GREEN)
	{
		if (m_fCurrentGauge <= 120.0f)
		{
			if (m_nCntFrame < 20)
			{
				m_col.a -= 0.01f;
				if (m_col.a <= 0.0f)
				{
					m_col.a = 0.0f;
				}
			}
			if (m_nCntFrame > 20)
			{
				m_col.a += 0.01f;
				if (m_col.a >= 1.0f)
				{
					m_col.a = 1.0f;
				}
			}
		}

	}

	//�ʒu���̐ݒ�
	SetPosGauge(m_pos);

	//�F���̐ݒ�
	SetColor(m_col);


#ifdef _DEBUG
	//if (m_type == GAUGETYPE_GREEN)
	//{//�΃Q�[�W�̂Ƃ�
	//	CDebugProc::Print("GAUGETYPE_GREEN\n");
	//	CDebugProc::Print("m_fCurrentGauge: %.1f\n", m_fCurrentGauge);
	//	CDebugProc::Print("m_fDestValue: %.1f\n", m_fDestValue);

	//}
	//if (m_type == GAUGETYPE_RED)
	//{//�ԃQ�[�W�̂Ƃ�
	//	CDebugProc::Print("\n");
	//	CDebugProc::Print("GAUGETYPE_RED\n");
	//	CDebugProc::Print("m_fCurrentGauge: %.1f\n", m_fCurrentGauge);
	//	CDebugProc::Print("m_fDestValue: %.1f\n", m_fDestValue);
	//	CDebugProc::Print("\n");
	//}

	//if (m_type == GAUGETYPE_SKILL)
	//{//�ԃQ�[�W�̂Ƃ�
	//	CDebugProc::Print("\n");
	//	CDebugProc::Print("GAUGETYPE_SKILL\n");
	//	CDebugProc::Print("m_fCurrentGauge: %.1f\n", m_fCurrentGauge);
	//	CDebugProc::Print("m_fDestValue: %.1f\n", m_fDestValue);
	//	CDebugProc::Print("\n");
	//}
#endif
}

//=============================================================================
// �֐����F�Q�[�W�̕`�揈��
// �֐��̊T�v�F--
//=============================================================================
void CUiGauge::Draw(void)
{
	//�Q�[�W�̕`��
	CSceneGauge::Draw();

}

//=============================================================================
// �֐����F���l�̑��
// �֐��̊T�v�F�_���[�W��񕜂Ȃ�
//=============================================================================
void CUiGauge::AddValue(float fValue)
{
	m_fCurrentGauge = fValue;
	if (m_fCurrentGauge <= 0.0f)
	{
		m_fCurrentGauge = 0.0f;
	}
	if (m_fCurrentGauge >= m_fMaxGauge)
	{
		m_fCurrentGauge = m_fMaxGauge;
	}
	SetCurrentValue(m_fCurrentGauge);
}

//=============================================================================
// �֐����F���l�̑��
// �֐��̊T�v�F�_���[�W��񕜂Ȃ�
//=============================================================================
void CUiGauge::AddValueSlip(float fValue)
{
	m_fDestValue = fValue;
	if (m_fDestValue <= 0.0f)
	{
		m_fDestValue = 0.0f;
	}
	if (m_fDestValue >= m_fMaxGauge)
	{
		m_fDestValue = m_fMaxGauge;
	}
	m_fValue = (m_fDestValue - m_fCurrentGauge);
}

