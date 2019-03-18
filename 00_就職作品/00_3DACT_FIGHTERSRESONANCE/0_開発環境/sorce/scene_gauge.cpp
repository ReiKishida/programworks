//=============================================================================
//
// �V�[������[2D�|���S��] [scene_gauge.cpp]
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
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DIRE_WIDTH (10.0f)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
// �֐����F�Q�[�W�̐���
// �֐��̊T�v�F�Q�[�W�𐶐�����
//=============================================================================
CSceneGauge *CSceneGauge::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CSceneGauge  *pSceneGauge;
	pSceneGauge = new CSceneGauge;//�V�[���N���X�̐���
	if (pSceneGauge != NULL)
	{
		pSceneGauge->m_posGauge = pos;//�ʒu���̑��
		pSceneGauge->m_fGaugeWidth = fWidth;
		pSceneGauge->m_fGaugeHeight = fHeight;
		pSceneGauge->m_fCurrentValue = fWidth;
		pSceneGauge->m_fMaxValue = fWidth;
		pSceneGauge->m_bDamage = false;
		pSceneGauge->Init();
	}
	return pSceneGauge;
}

//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CSceneGauge::CSceneGauge(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CSceneGauge::~CSceneGauge()
{

}

//=============================================================================
// �֐����F�Q�[�W�̏���������
// �֐��̊T�v�F�|���S���̐ݒ�
//=============================================================================
HRESULT CSceneGauge::Init(void)
{


	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	VERTEX_2D *pVtx; // ���_���̍쐬
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_posGauge.x + DIRE_WIDTH,
		m_posGauge.y - m_fGaugeHeight,
		0.0f);

	pVtx[1].pos = D3DXVECTOR3(m_posGauge.x + ((m_fCurrentValue / m_fMaxValue) * m_fGaugeWidth) + DIRE_WIDTH,
		m_posGauge.y - m_fGaugeHeight,
		0.0f);

	pVtx[2].pos = D3DXVECTOR3(m_posGauge.x - DIRE_WIDTH,
		m_posGauge.y + m_fGaugeHeight,
		0.0f);

	pVtx[3].pos = D3DXVECTOR3(m_posGauge.x + ((m_fCurrentValue / m_fMaxValue) * m_fGaugeWidth) - DIRE_WIDTH,
		m_posGauge.y + m_fGaugeHeight,
		0.0f);


	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();


	return S_OK;
}

//=============================================================================
// �֐����F�Q�[�W�̏I������
// �֐��̊T�v�F�e�N�X�`���A���_�o�b�t�@�̔j���A���g�̔j��
//=============================================================================
void CSceneGauge::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g(�������g)�̔j��
	Release();

}

//=============================================================================
// �֐����F�Q�[�W�̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CSceneGauge::Update(void)
{

}

//=============================================================================
// �֐����F�Q�[�W�̕`�揈��
// �֐��̊T�v�F--
//=============================================================================
void CSceneGauge::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	// �f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));


	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�(�g��Ȃ��Ă������I)
	pDevice->SetTexture(0, m_pTexture);


	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}


//=============================================================================
// �֐����F�|���S���̈ʒu���擾
// �֐��̊T�v�F�|���S���̈ʒu��Ԃ�
//=============================================================================
D3DXVECTOR3 CSceneGauge::GetPosGauge(void)
{
	return m_posGauge;
}

//=============================================================================
// �֐����F�|���S���̈ʒu��ݒ�
// �֐��̊T�v�F�|���S���̈ʒu��ݒ肷��
//=============================================================================
void CSceneGauge::SetPosGauge(D3DXVECTOR3 pos)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;

	//�ʒu������
	m_posGauge = pos;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_posGauge.x + (DIRE_WIDTH +10.0f),
		m_posGauge.y - m_fGaugeHeight,
		0.0f);

	pVtx[1].pos = D3DXVECTOR3(m_posGauge.x + ((m_fCurrentValue / m_fMaxValue) * m_fGaugeWidth) + (DIRE_WIDTH + 10.0f),
		m_posGauge.y - m_fGaugeHeight,
		0.0f);

	pVtx[2].pos = D3DXVECTOR3(m_posGauge.x - DIRE_WIDTH,
		m_posGauge.y + m_fGaugeHeight,
		0.0f);

	pVtx[3].pos = D3DXVECTOR3(m_posGauge.x + ((m_fCurrentValue / m_fMaxValue) * m_fGaugeWidth) - DIRE_WIDTH,
		m_posGauge.y + m_fGaugeHeight,
		0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}


//=============================================================================
// �֐����F�|���S���̕���ݒ�
// �֐��̊T�v�F�|���S���̕���ݒ肷��
//=============================================================================
void CSceneGauge::SetGaugeWidth(float fWidth)
{
	m_fGaugeWidth = fWidth;
}
//=============================================================================
// �֐����F�|���S���̍�����ݒ�
// �֐��̊T�v�F�|���S���̍�����ݒ肷��
//=============================================================================
void CSceneGauge::SetGaugeHeight(float fHeight)
{
	m_fGaugeHeight = fHeight;

}
//=============================================================================
// �֐����F�e�N�X�`���̐ݒ�
// �֐��̊T�v�F�e�N�X�`�������蓖�Ă�
//=============================================================================
void CSceneGauge::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//=============================================================================
// �֐����F�e�N�X�`���̍��W�ݒ�
// �֐��̊T�v�F�e�N�X�`���̍��W��ݒ肷��
//=============================================================================
void CSceneGauge::SetTexture(float fTex_X, float fTex_Y)
{

	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬

					 // ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTex_X, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X, fTex_Y);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}

//=============================================================================
// �֐����F�e�N�X�`���A�j���[�V����
// �֐��̊T�v�F�A�j���[�V�����𓮂���
//=============================================================================
void CSceneGauge::SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim)
{
	//�f�o�C�X�̎擾
	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬


					 // ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0 + (fTex_X*nPatternAnim), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), fTex_Y);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}


//=============================================================================
// �֐����F�e�N�X�`���A�j���[�V����(Y��)
// �֐��̊T�v�F�A�j���[�V�����𓮂���
//=============================================================================
void CSceneGauge::SetTextureAnimY(float fTex_X, float fTex_Y, int nPatternAnim, int nTex_Y)
{

	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0 + (fTex_X*nPatternAnim), (fTex_Y * nTex_Y));
	pVtx[1].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), (fTex_Y * nTex_Y));
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), fTex_Y * (nTex_Y + 1));
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), fTex_Y * (nTex_Y + 1));

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �֐����F�|���S���̐F�ݒ�
// �֐��̊T�v�F�|���S���̐F��ݒ肷��
//=============================================================================
void CSceneGauge::SetColor(D3DXCOLOR col)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}


//=============================================================================
// �֐����F�|���S���̕����擾
// �֐��̊T�v�F�|���S���̕���Ԃ�
//=============================================================================
float CSceneGauge::GetWidthGauge(void)
{
	return m_fGaugeWidth;
}
//=============================================================================
// �֐����F�|���S���̂������擾
// �֐��̊T�v�F�|���S���̕���Ԃ�
//=============================================================================
float CSceneGauge::GetHeightGauge(void)
{
	return m_fGaugeHeight;

}
//=============================================================================
// �֐����F���݂̒l�̑��
// �֐��̊T�v�F���݂̒l�̑������
//=============================================================================
void CSceneGauge::SetCurrentValue(float fCurrentValue)
{
	m_fCurrentValue = fCurrentValue;
}

//=============================================================================
// �֐����F�ő�l�̑��
// �֐��̊T�v�F�ő�l�̑������
//=============================================================================
void CSceneGauge::SetMaxValue(float fMaxValue)
{
	m_fMaxValue = fMaxValue;
}



//=============================================================================
// �֐����F�Q�[�W�̑���
// �֐��̊T�v�F�|���S���̕���Ԃ�
//=============================================================================
void CSceneGauge::SlipGauge(void)
{



}


