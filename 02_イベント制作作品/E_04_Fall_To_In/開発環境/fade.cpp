//=============================================================================
//
// �t�F�[�h�̏��� [fade.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFade::m_pTexture[TYPE_MAX] = {};			// �e�N�X�`��
CManager::MODE CFade::m_modeNext = CManager::MODE_TITLE;
CFade::FADE CFade::m_fade = CFade::FADE_NONE;
D3DXVECTOR3 CFade::m_pos = D3DXVECTOR3(1.0f, 1.0f, 1.0);		// ���W
D3DXCOLOR CFade::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0, 0.0f);		// �F
float CFade::m_fWidth = 0.0f;//�@��
float CFade::m_fHeight = 0.0f;//�@��
D3DXVECTOR2 CFade::m_uv = D3DXVECTOR2(0.0f, 0.0f);
CFade::TYPE CFade::m_type = CFade::TYPE_NONE;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFade::CFade()
{
	m_pVtxBuff = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0, 0,0);			// ���W
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0, 0.0f);		// �F
	m_modeNext = CManager::MODE_TITLE;
	m_fade = CFade::FADE_NONE;
	m_uv = D3DXVECTOR2(0.0f, 0.0f);
	m_type = CFade::TYPE_NONE;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{
}
//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CFade::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/.png",
		&m_pTexture[TYPE_NONE]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wipe.png",
		&m_pTexture[TYPE_WIPE]);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CFade::UnLoad(void)
{
	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CFade::Init(CManager::MODE modeNext)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_modeNext = modeNext;
	m_type = CFade::TYPE_NONE;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0);			// ���W
	m_fWidth = SCREEN_WIDTH ;
	m_fHeight = SCREEN_HEIGHT ;
	m_uv = D3DXVECTOR2(0.0f, 0.0f);
	// ���_���̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth/2 , -(m_fHeight )/2, 0.0f)+ m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_fWidth/2, -(m_fHeight )/2, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth /2, (m_fHeight)/2, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_fWidth/2, (m_fHeight)/2, 0.0f) + m_pos;
	// rhw�̐ݒ�
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
	}
	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[2].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 0.5f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFade::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void CFade::Update(void)
{
	switch (m_type)
	{
	case TYPE_NONE:
		if (m_fade == FADE_IN)			// �t�F�[�h�C����Ԃ�
		{
			m_col.a -= 0.02f;
			if (m_col.a <= 0.0f)
			{// �����ɂȂ�����
				m_col.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)	// �t�F�[�h�A�E�g��Ԃ�
		{
			m_col.a += 0.02f;
			if (m_col.a >= 1.0f)
			{// �s�����ɂȂ�����
				m_col.a = 1.0f;
				m_fade = FADE_IN;	// �t�F�[�h�C����Ԃ�
				// ���[�h�̐ݒ�
				CManager::SetMode(m_modeNext);
				CGame::SetPause(false);
			}
		}
		break;
	case TYPE_WIPE:
		if (m_fade == FADE_IN)			// �t�F�[�h�C����Ԃ�
		{
			m_uv.x += -0.1f;
			m_uv.y += -0.1f;
			if (m_uv.x <= 0.0f)
			{// �����ɂȂ�����
				m_col.a = 0.0f;
				m_uv.x = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)	// �t�F�[�h�A�E�g��Ԃ�
		{
			m_uv.x += 0.1f;
			m_uv.y += 0.1f;
			if (m_uv.x >= 5.0f)
			{// �s�����ɂȂ�����
				m_uv.x = 5.0f;
				m_fade = FADE_IN;	// �t�F�[�h�C����Ԃ�
				// ���[�h�̐ݒ�
				CManager::SetMode(m_modeNext);
				CGame::SetPause(false);
			}
		}
		break;
	case TYPE_CLOSSFADE:
		if (m_fade == FADE_IN)			// �t�F�[�h�C����Ԃ�
		{
			m_col.a -= 0.01f;
			if (m_col.a <= 0.0f)
			{// �����ɂȂ�����
				m_col.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)	// �t�F�[�h�A�E�g��Ԃ�
		{
			m_col.a += 0.01f;
			if (m_col.a >= 1.0f)
			{// �s�����ɂȂ�����
				m_col.a = 1.0f;
				m_fade = FADE_IN;	// �t�F�[�h�C����Ԃ�
			}
		}
		break;
	}

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.5f-m_uv.x, 0.5f- m_uv.y);
	pVtx[1].tex = D3DXVECTOR2(0.5f+m_uv.x, 0.5f- m_uv.y);
	pVtx[2].tex = D3DXVECTOR2(0.5f-m_uv.x, 0.5f+ m_uv.y);
	pVtx[3].tex = D3DXVECTOR2(0.5f+m_uv.x, 0.5f+ m_uv.y);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFade::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture[m_type]);
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
}
//=============================================================================
// ����
//=============================================================================
CFade *CFade::Create(CManager::MODE modeNext)
{
	CFade *pCFade = NULL;
	if (pCFade == NULL)
	{// �t�F�[�h����
		pCFade = new CFade;
	}
	if (pCFade != NULL)
	{
		// ����������
		if (FAILED(pCFade->Init(modeNext)))
		{
			return NULL;
		}

	}
	return pCFade;
}
//=============================================================================
// �t�F�[�h
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	m_type = TYPE_NONE;
	m_fade = CFade::FADE_OUT;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	m_modeNext = modeNext;
}
//=============================================================================
// wipe
//=============================================================================
void CFade::SetWipe(CManager::MODE modeNext)
{
	m_type = TYPE_WIPE;
	m_fade = CFade::FADE_OUT;
	m_uv = D3DXVECTOR2(0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_modeNext = modeNext;
}
//=============================================================================
// �Ȃɂ��Ȃ�
//=============================================================================
void CFade::SetCloss(void)
{
	m_type = TYPE_CLOSSFADE;
	m_fade = CFade::FADE_OUT;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

}
//=============================================================================
// �擾
//=============================================================================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

