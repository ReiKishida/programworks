//=============================================================================
//
// UI�̏���[2D�|���S��] [ui.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "score.h"
#include "number.h"
#include "force.h"
#include "polygon.h"
#include "input.h"
#include "effect.h"
#include "sound.h"
#include "log.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WIDTH (900)	//�|���S���̕�
#define BG_HEIGHT (700)	//�|���S����Y�̍���
#define LOG_WIDTH (210.0f)	//���O�̕�
#define LOG_HEIGHT (70.0f)	//���O�̍���
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CLog::m_apTexture[MAX_LOGTEXTURE_NUM] = {}; //���L�e�N�X�`���̃|�C���^
int CLog::m_nCntLog = 0;	//���O�̐�
int CLog::m_nCntLogOld = 0;	//�ߋ��̃��O�̐�
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CLog::CLog() : CScene2D(7)
{//�N���A�Ȓl����

}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CLog::~CLog()
{
	m_nCntLog = 0;

}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CLog::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/normalenemy_d.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/attackenemy_d.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spreadenemy_d.png", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/speedenemy_d.png", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/defenceenemy_d.png", &m_apTexture[4]);

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CLog::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_LOGTEXTURE_NUM; nCntTex++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}





//=============================================================================
//�X�R�A�̐���
//=============================================================================
CLog *CLog::Create(D3DXVECTOR3 pos, LOGTYPE type)
{
	//�X�R�A�̐���
	CLog *pLog;
	pLog = new CLog;

	if (pLog != NULL)
	{
		m_nCntLog++;
		pLog->m_type = type;
		//pLog->m_MoveState = 0;
		//�X�R�A�̏���������
		pLog->Init(pos);
		m_nCntLogOld = m_nCntLog;
		m_nCntLog = m_nCntLog % MAX_LOG_NUM;


	}

	return pLog;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CLog::Init(D3DXVECTOR3 pos)
{


	//�v���C���[�̕��A�����ݒ�
	SetScene2DWidth(LOG_WIDTH);
	SetScene2DHeight(LOG_HEIGHT);

	switch (m_type)
	{
	case LOGTYPE_NORMAL:
		BindTexture(m_apTexture[LOGTYPE_NORMAL]);
		break;
	case LOGTYPE_ATTACK:
		BindTexture(m_apTexture[LOGTYPE_ATTACK]);
		break;
	case LOGTYPE_SPREAD:
		BindTexture(m_apTexture[LOGTYPE_SPREAD]);
		break;
	case LOGTYPE_SPEED:
		BindTexture(m_apTexture[LOGTYPE_SPEED]);
		break;
	case LOGTYPE_DEFENSE:
		BindTexture(m_apTexture[LOGTYPE_DEFENSE]);
		break;
	}

	//�������ʒu
	D3DXVECTOR3 InitPos;


	//2D�V�[���̏���������
	CScene2D::Init(D3DXVECTOR3(pos.x, pos.y, pos.z));

	//�ʒu���̎擾
	m_pos = GetPosScene2D();
	MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 - 120.0f, 1);

	SetTexture(TEX_X, TEX_Y);
	//�v���C���[�̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_LOG);

	return S_OK;

}


//=============================================================================
//�I������
//=============================================================================
void CLog::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
//�X�V����
//=============================================================================
void CLog::Update(void)
{

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();





	//�ʒu���̎擾
	m_pos = GetPosScene2D();
	//�F���̎擾
	m_col = GetColor();

	m_col.a -= 0.0015f;

	MoveLog();

	//�F���̐ݒ�
	SetColor(m_col);
	//�ʒu���̐ݒ�
	SetPosScene2D(m_pos);
	if (m_pos.y >= SCREEN_HEIGHT / 2 + 360.0f)
	{
		Uninit();
	}

}

//=============================================================================
//�`�揈��
//=============================================================================
void CLog::Draw(void)
{
	//2D�V�[���̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//���O�̈ړ�
//=============================================================================
void CLog::MoveLog(void)
{
	float fAngle;//�p�x�v�Z�p
	m_fAngle += 1.0;


	fAngle = sinf(m_fAngle * D3DX_PI / 180);


	float fMove_X = m_posold.x + fAngle * m_Destpos.x;
	float fMove_Y = m_posold.y + fAngle * m_Destpos.y;


	m_pos.x = fMove_X;
	m_pos.y = fMove_Y;


	if (m_fAngle == 90.0f)
	{
		switch (m_MoveState)
		{
		case 0:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 - 60.0f, 1);
			break;
		case 1:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 , 2);
			break;
		case 2:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 120.0f, 3);
			break;
		case 3:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 180.0f, 4);
			break;
		case 4:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 240.0f, 5);
			break;
		case 5:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 300.0f, 6);
			break;
		case 6:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 360.0f, 7);
			break;
		case 7:
			MoveInit(SCREEN_WIDTH / 2 + 520.0f, SCREEN_HEIGHT / 2 + 440.0f, 8);
			break;
		}

	}

}

//=============================================================================
//���O�̈ړ����̑��
//=============================================================================
void CLog::MoveInit(float dest_x, float dest_y, int state)
{
	//�ߋ��̈ʒu���
	m_posold.x = m_pos.x;
	m_posold.y = m_pos.y;

	//�ړI�̈ʒu
	m_Destpos.x = dest_x - m_pos.x;
	m_Destpos.y = dest_y - m_pos.y;


	m_fAngle = 0.0f;

	m_MoveState = state;
}
