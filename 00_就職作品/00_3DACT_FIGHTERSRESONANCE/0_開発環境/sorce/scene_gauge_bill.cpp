//=============================================================================
//
// �r���{�[�h����[3D�|���S��] [scene_billboard.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "scene_gauge_bill.h"
#include "camera.h"
#include "debugproc.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DIRE_WIDTH (10.0f)

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
//=============================================================================
// �֐����F�r���{�[�h�̐���
// �֐��̊T�v�F�r���{�[�h�𐶐�����
//=============================================================================
CSceneGaugeBillBoard *CSceneGaugeBillBoard::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CSceneGaugeBillBoard  *pSceneBillboard;
	pSceneBillboard = new CSceneGaugeBillBoard;//�V�[���N���X�̐���
	if (pSceneBillboard != NULL)
	{
		pSceneBillboard->m_pos = pos;
		pSceneBillboard->m_fWidth = fWidth;
		pSceneBillboard->m_fHeight = fHeight;

		pSceneBillboard->Init();

	}
	return pSceneBillboard;
}

//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CSceneGaugeBillBoard::CSceneGaugeBillBoard(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CSceneGaugeBillBoard::~CSceneGaugeBillBoard()
{

}

//=============================================================================
// �֐����F�r���{�[�h�̏���������
// �֐��̊T�v�F�|���S���̐ݒ�
//=============================================================================
HRESULT CSceneGaugeBillBoard::Init(void)
{

	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(((m_fCurrentValue / m_fMaxValue) * m_fWidth), m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(((m_fCurrentValue / m_fMaxValue) * m_fWidth), -m_fHeight, 0.0f);

	//�@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();


	return S_OK;
}

//=============================================================================
// �֐����F�r���{�[�h�̏I������
// �֐��̊T�v�F���g��j������
//=============================================================================
void CSceneGaugeBillBoard::Uninit(void)
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
// �֐����F�r���{�[�h�̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CSceneGaugeBillBoard::Update(void)
{

}

//=============================================================================
// �֐����F�r���{�[�h�̕`�揈��
// �֐��̊T�v�F�s���ς��Ă����Ƃ�����������悤��
//=============================================================================
void CSceneGaugeBillBoard::Draw(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�J�����̎擾
	CCamera *pCamera;
	pCamera = CManager::GetCamera();

	D3DXMATRIX pMtxView;
	pMtxView = pCamera->GetmtxView();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//�}�g���b�N�X���

	//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	////Z�o�b�t�@
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	pDevice->GetTransform(D3DTS_VIEW, &pMtxView);

	m_mtxWorld._11 = pMtxView._11;
	m_mtxWorld._12 = pMtxView._21;
	m_mtxWorld._13 = pMtxView._31;
	m_mtxWorld._21 = pMtxView._12;
	m_mtxWorld._22 = pMtxView._22;
	m_mtxWorld._23 = pMtxView._32;
	m_mtxWorld._31 = pMtxView._13;
	m_mtxWorld._32 = pMtxView._23;
	m_mtxWorld._33 = pMtxView._33;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, m_pTexture);


	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Z�o�b�t�@
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);



	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}






//=============================================================================
// �֐����F�r���{�[�h�Ƀe�N�X�`�������蓖��
// �֐��̊T�v�Fm_pTexture�Ƀe�N�X�`����ݒ肷��
//=============================================================================
void CSceneGaugeBillBoard::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//=============================================================================
// �֐����F�r���{�[�h�̐F��ݒ�
// �֐��̊T�v�F�r���{�[�h�̐F��ݒ肷��
//=============================================================================
void CSceneGaugeBillBoard::SetColor(D3DXCOLOR col)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx; // ���_���̍쐬


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
// �֐����F�|���S���̕���ݒ�
// �֐��̊T�v�F�|���S���̕���ݒ肷��
//=============================================================================
void CSceneGaugeBillBoard::SetGaugeWidth(float fWidth)
{
	m_fWidth = fWidth;
}
//=============================================================================
// �֐����F�|���S���̍�����ݒ�
// �֐��̊T�v�F�|���S���̍�����ݒ肷��
//=============================================================================
void CSceneGaugeBillBoard::SetGaugeHeight(float fHeight)
{
	m_fHeight = fHeight;

}
//=============================================================================
// �֐����F���݂̒l�̑��
// �֐��̊T�v�F���݂̒l�̑������
//=============================================================================
void CSceneGaugeBillBoard::SetCurrentValue(float fCurrentValue)
{
	m_fCurrentValue = fCurrentValue;

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	if (m_pVtxBuff != NULL)
	{
		//���_�o�b�t�@�����b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(((m_fCurrentValue / m_fMaxValue) * m_fWidth), m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, -m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(((m_fCurrentValue / m_fMaxValue) * m_fWidth), -m_fHeight, 0.0f);

		//���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �֐����F�ő�l�̑��
// �֐��̊T�v�F�ő�l�̑������
//=============================================================================
void CSceneGaugeBillBoard::SetMaxValue(float fMaxValue)
{
	m_fMaxValue = fMaxValue;
}

//=============================================================================
// �֐����F�|���S���̈ʒu��ݒ�
// �֐��̊T�v�F�|���S���̈ʒu��ݒ肷��
//=============================================================================
void CSceneGaugeBillBoard::SetPosGauge(D3DXVECTOR3 pos)
{
	//�ʒu������
	m_pos = pos;


}
