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
#include "combogauge.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CComboGauge::m_pTexture = NULL;
int CComboGauge::m_nCombo = 0;
float  CComboGauge::m_fComboGauge = 0.0f;					//�R���{��
float  CComboGauge::m_fComboGaugeMax = 0.0f;					//�R���{��
//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CComboGauge::CComboGauge(int nPriority, OBJTYPE type) : CSceneGauge(nPriority, type)
{
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CComboGauge::~CComboGauge()
{

}

//=============================================================================
// �֐����F�g�p����e�N�X�`���̓ǂݍ���
// �֐��̊T�v�F--
//=============================================================================
HRESULT CComboGauge::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/1_lifegauge.png", &m_pTexture);		// ���C�t�Q�[�W
	return S_OK;

}

//=============================================================================
// �֐����F�g�p�����e�N�X�`���̔j��
// �֐��̊T�v�F--
//=============================================================================
void CComboGauge::Unload(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

}


//=============================================================================
// �֐����F�Q�[�W�̐���
// �֐��̊T�v�F�Q�[�W�𐶐�����
//=============================================================================
CComboGauge *CComboGauge::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fCurrentGauge, float fMaxGauge, GAUGETYPE type)
{
	CComboGauge  *pSceneGauge;
	pSceneGauge = new CComboGauge;	//�V�[���N���X�̐���
	if (pSceneGauge != NULL)
	{
		pSceneGauge->m_pos = pos;						//�ʒu���̑��
		pSceneGauge->m_fWidth = fWidth;					//���̑��
		pSceneGauge->m_fHeight = fHeight;				//�����̑��
		pSceneGauge->m_fCurrentGauge = fCurrentGauge;	//���݂̒l�̑��
		pSceneGauge->m_fMaxGauge = fMaxGauge;			//�ő�l�̑��
		pSceneGauge->m_col = col;						//�Q�[�W�̐F���
		pSceneGauge->m_type = type;						//�Q�[�W�̎�ނ̑��
		pSceneGauge->m_nCombo = 0;						//�R���{��
		pSceneGauge->m_fComboGauge = 0.0f;				//�R���{��
		pSceneGauge->Init();
	}
	return pSceneGauge;
}


//=============================================================================
// �֐����F�Q�[�W�̏���������
// �֐��̊T�v�F�|���S���̐ݒ�
//=============================================================================
HRESULT CComboGauge::Init(void)
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
		BindTexture(m_pTexture);
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
void CComboGauge::Uninit(void)
{
	CSceneGauge::Uninit();
}

//=============================================================================
// �֐����F�Q�[�W�̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CComboGauge::Update(void)
{
	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();


	//�ʒu���̎擾
	D3DXVECTOR3 pos = GetPosGauge();



	//�t���[���̉��Z
	m_nCntFrame++;

	if (m_type == GAUGETYPE_COMBO)
	{


		AddValueSlip(-0.10f * ((m_nCombo + 1) * 1.1f));
		//m_fCurrentGauge -= (0.09f * ((m_nCombo + 1) * 1.0f));
		//m_fComboGauge -= (0.09f * ((m_nCombo + 1) * 1.0f));

		m_nCntFrame = m_nCntFrame % 120;


		//�_�ł̏���
		if (m_nCntFrame < 60)
		{
			m_col.a -= 0.01f;
			if (m_col.a <= 0.0f)
			{
				m_col.a = 0.0f;
			}
		}
		if (m_nCntFrame > 60)
		{
			m_col.a += 0.01f;
			if (m_col.a >= 1.0f)
			{
				m_col.a = 1.0f;
			}
		}

		if (m_fCurrentGauge < m_fDestValue)
		{//�ړI�̃Q�[�W
			m_fCurrentGauge += m_fValue * 0.035f;
			if (m_fCurrentGauge <= 0.0f)
			{
				if (m_nCombo > 0)
				{
					//�R���{���̉��Z
					m_nCombo--;
					if (m_nCombo <= 0)
					{
						m_nCombo = 0;
					}
					m_fCurrentGauge = m_fMaxGauge;
				}
			}
			if (m_fCurrentGauge >= m_fMaxGauge)
			{
				m_fCurrentGauge = 0.0f;
				m_fDestValue = 0.0f;
				//�R���{���̉��Z
				m_nCombo++;
				m_fComboGauge += 5.0f;
				if (m_nCombo >= 5)
				{
					m_nCombo = 5;
				}
			}
		}
		else if (m_fCurrentGauge > m_fDestValue)
		{//�ړI�̃Q�[�W


			m_fCurrentGauge += m_fValue * 0.035f;

			if (m_fCurrentGauge <= 0.0f)
			{
				//if (m_nCombo > 0)
				//{
					//�R���{���̉��Z
				m_nCombo--;
				if (m_nCombo <= 0)
				{
					m_nCombo = 0;
				}
				m_fCurrentGauge = m_fMaxGauge;
				//}
			}
		}

		if (m_fDestValue == 0.0f)
		{
			m_fDestValue = 0.0f;
			m_fCurrentGauge = 0.0f;
		}


	}



	//���݂̒l�̉��Z
	SetCurrentValue(m_fCurrentGauge);




	//�ʒu���̐ݒ�
	SetPosGauge(m_pos);

	//�F���̐ݒ�
	SetColor(m_col);


#ifdef _DEBUG
	//if (m_type == GAUGETYPE_COMBO)
	//{//�΃Q�[�W�̂Ƃ�
	//	CDebugProc::Print("GAUGETYPE_COMBO\n");
	//	CDebugProc::Print("m_fComboGauge: %.1f\n", m_fComboGauge);
	//	CDebugProc::Print("m_fComboGaugeMax: %.1f\n", m_fComboGaugeMax);

	//	CDebugProc::Print("m_fCurrentGauge: %.1f\n", m_fCurrentGauge);
	//	CDebugProc::Print("m_fDestValue: %.1f\n", m_fDestValue);
	//}



#endif
}

//=============================================================================
// �֐����F�Q�[�W�̕`�揈��
// �֐��̊T�v�F--
//=============================================================================
void CComboGauge::Draw(void)
{
	//�Q�[�W�̕`��
	CSceneGauge::Draw();

}


//=============================================================================
// �֐����F���l�̑��
// �֐��̊T�v�F�_���[�W��񕜂Ȃ�
//=============================================================================
void CComboGauge::AddValueSlip(float fValue)
{

	if (m_nCombo < 5)
	{
		m_fComboGauge += fValue;
		if (m_fComboGauge <= 100.0f && m_nCombo == 1)
		{
			m_nCombo--;
		}
		if (m_fComboGauge <= 0.0f)
		{
			m_fComboGauge = 0.0f;
		}
	}
	if (m_nCombo == 5)
	{
		m_fComboGauge += fValue;
		if (m_fComboGauge <= 100.0f && m_nCombo == 1)
		{
			m_nCombo--;
		}
		if (m_fComboGauge >= 600.0f)
		{
			m_fComboGauge = 600.0f;
		}
	}


	if (m_fComboGauge >= m_fMaxGauge)
	{

		m_fReminder = m_fComboGauge - (m_fMaxGauge * m_nCombo);
		m_fDestValue = m_fReminder;
	}
	else
	{
		m_fDestValue = m_fComboGauge;
	}

	m_fValue = (m_fDestValue - m_fCurrentGauge);


	float fEmpty = m_fComboGauge;
	if (m_fComboGauge >= m_fComboGaugeMax)
	{
		m_fComboGaugeMax = m_fComboGauge;
	}
}


//=============================================================================
// �֐����F���l�̑��
// �֐��̊T�v�F�_���[�W��񕜂Ȃ�
//=============================================================================
void CComboGauge::SetComboGauge(float fValue)
{
	m_fComboGauge = fValue;
}





//=============================================================================
// �֐����F�R���{�̐��l�̎擾
// �֐��̊T�v�F��ɃR���{�\�L��������Ƃ��Ɏg�p
//=============================================================================
float CComboGauge::GetComboGauge(void)
{
	return m_fComboGauge;
}

//=============================================================================
// �֐����F���݂̃Q�[�W�̐��l�̎擾
// �֐��̊T�v�F��ɃR���{�\�L��������Ƃ��Ɏg�p
//=============================================================================
float CComboGauge::GetCurrentGauge(void)
{
	return m_fCurrentGauge;
}

//=============================================================================
// �֐����F���݂̃Q�[�W�̐��l�̑��
// �֐��̊T�v�F��ɃR���{�\�L��������Ƃ��Ɏg�p
//=============================================================================
void CComboGauge::SetCurrentGauge(float fValue)
{
	m_fCurrentGauge = fValue;
}

float CComboGauge::GetComboGaugeMax(void)
{
	return m_fComboGaugeMax;
}

