//=============================================================================
//
// �V�[������[2D�|���S��] [scene2D.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "explosion.h"
#include "number.h"
#include "polygon.h"
#include "force.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ANIMATION_SPEED (8)	//�A�j���[�V�����̃X�s�[�h
#define TEX_X (0.25f)	//�e�N�X�`�����W_X
#define TEX_Y (1.0f)	//�e�N�X�`�����W_Y

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************


//=============================================================================
//�R���X�g���N�^
//=============================================================================
CPolygon::CPolygon()
{
	m_nType = CForce::FORCETYPE_ATTACK;
	m_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�p�x
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
	m_fPolygonWidth = 0.0f;	//�|���S���̕�
	m_fPolygonHeight = 0.0f;	//�|���S���̍���
	m_pVtxBuff = NULL;

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CPolygon::~CPolygon()
{

}

//=============================================================================
//2D�V�[���̐���
//=============================================================================
CPolygon *CPolygon::Create(D3DXVECTOR3 pos)
{
	CPolygon  *pPolygon;
	pPolygon = new CPolygon;//�V�[���N���X�̐���
	if (pPolygon != NULL)
	{
		pPolygon->Init(pos);

	}
	return pPolygon;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CPolygon::Init(D3DXVECTOR3 pos)
{
	m_nType = CForce::FORCETYPE_ATTACK;

	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	m_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �|���S���̈ʒu��ݒ�
	m_posPolygon = pos;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬

					 ////�|���S���̒���
	m_fLengthPolygon = sqrtf((m_fPolygonWidth / 2 + m_fPolygonWidth / 2) * (m_fPolygonHeight / 2 + m_fPolygonHeight / 2));

	//�|���S���̊p�x
	m_fAnglePolygon = atan2f(m_fPolygonWidth / 2, m_fPolygonHeight / 2);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - m_fPolygonWidth, m_posPolygon.y - m_fPolygonHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x + m_fPolygonWidth, m_posPolygon.y - m_fPolygonHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x - m_fPolygonWidth, m_posPolygon.y + m_fPolygonHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + m_fPolygonWidth, m_posPolygon.y + m_fPolygonHeight, 0.0f);


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
//�I������
//=============================================================================
void CPolygon::Uninit(void)
{


	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//=============================================================================
//�X�V����
//=============================================================================
void CPolygon::Update(void)
{



}

//=============================================================================
//�`�揈��
//=============================================================================
void CPolygon::Draw(void)
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
//�|���S���̈ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CPolygon::GetPosPolygon(void)
{
	return m_posPolygon;
}

//=============================================================================
//�|���S���̈ʒu�̐ݒ�
//=============================================================================
void CPolygon::SetPosPolygon(D3DXVECTOR3 pos)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;

	//�ʒu������
	m_posPolygon = pos;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - m_fPolygonWidth / 2, m_posPolygon.y - m_fPolygonHeight / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x + m_fPolygonWidth / 2, m_posPolygon.y - m_fPolygonHeight / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x - m_fPolygonWidth / 2, m_posPolygon.y + m_fPolygonHeight / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + m_fPolygonWidth / 2, m_posPolygon.y + m_fPolygonHeight / 2, 0.0f);



	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�|���S���̊p�x�̎擾
//=============================================================================
D3DXVECTOR3 CPolygon::GetRotPolygon(void)
{
	return m_rotPolygon;
}

//=============================================================================
//�|���S���̊p�x�̐ݒ�
//=============================================================================
void CPolygon::SetRotPolygon(D3DXVECTOR3 rot)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;

	m_rotPolygon = rot;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x + sinf(-D3DX_PI + m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(-D3DX_PI + m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x + sinf(D3DX_PI - m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(D3DX_PI - m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + sinf(-m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(-m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + sinf(m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�|���S���̒����̎擾
//=============================================================================
float CPolygon::GetLengthPolygon(void)
{
	return m_fLengthPolygon;
}

//=============================================================================
//�|���S���̒����̐ݒ�
//=============================================================================
void CPolygon::SetLengthPolygon(float fLength)
{
	m_fLengthPolygon = fLength;
}

//=============================================================================
//�|���S���̒����̐ݒ�
//=============================================================================
void CPolygon::SetPolygonWidth(float fWidth)
{
	m_fPolygonWidth = fWidth;
}
//=============================================================================
//�|���S���̒����̐ݒ�
//=============================================================================
void CPolygon::SetPolygonHeight(float fHeight)
{
	m_fPolygonHeight = fHeight;

}

//=============================================================================
//�e�N�X�`���̐ݒ�
//=============================================================================
void CPolygon::SetTexture(float fTex_X, float fTex_Y)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

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

//============================================================================================
//�F�̐ݒ�
//============================================================================================
void CPolygon::SetPolygonColor(D3DXCOLOR col)
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

//============================================================================================
//�F���̎擾
//============================================================================================
D3DXCOLOR CPolygon::GetPolygonColor(void)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬

					 // ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	return pVtx[0].col;


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}


//============================================================================================
//���̎擾
//============================================================================================
float CPolygon::GetWidthPolygon(void)
{
	return m_fPolygonWidth;
}
//============================================================================================
//���̎擾
//============================================================================================
float CPolygon::GetHeightPolygon(void)
{
	return m_fPolygonHeight;

}




//============================================================================================
//���C�t�̐ݒ�
//============================================================================================
void CPolygon::SetLife(int nLife)
{
	m_nLife = nLife;
}

//============================================================================================
//�t�H�[�X�̃e�N�X�`���ݒ�
//============================================================================================
void CPolygon::SetPolygonTexture(int nType)
{
	m_nType = nType;

	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.25f * m_nType), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f * m_nType), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (0.25f * m_nType), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f * m_nType), 1.0f);
	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================================================
//�e�N�X�`���ݒ�
//============================================================================================
void CPolygon::SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y)
{
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// ���_����ݒ�
	VERTEX_2D *pVtx; // ���_���̍쐬

					 // ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(fTex_X_0, fTex_Y_0);
	pVtx[1].tex = D3DXVECTOR2(fTex_X, fTex_Y_0);
	pVtx[2].tex = D3DXVECTOR2(fTex_X_0, fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X, fTex_Y);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}



//============================================================================================
//��Ԃ̐ݒ�
//============================================================================================
void CPolygon::SetPolygon(int nType)
{
	m_nType = nType;


}

//============================================================================================
//��Ԃ̎擾
//============================================================================================
int CPolygon::GetPolygon()
{
	return m_nType;
}

//=============================================================================
//�e�N�X�`���ݒ�
//============================================================================
void CPolygon::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}


//=============================================================================
//�e�N�X�`���A�j���[�V�����̐ݒ�
//=============================================================================
void CPolygon::SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

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
