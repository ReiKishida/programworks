////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����p���C���̏��� [libe.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "line.h"
#include "manager.h"
#include "renderer.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
bool CLine::m_bUse = false;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLine::CLine(int nPriority ):CScene(PRIORITY_LINE)
{
	m_pVtxBuff = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��̈ʒu
	m_posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ŏ��̈ʒu
	m_posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �Ō�̈ʒu
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_bUse = false;								// �g�p����Ă��邩�ǂ���]

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLine::~CLine()
{

}
//==============================================================================================================================
// ����������
//==============================================================================================================================
HRESULT CLine::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��̈ʒu
	m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// �F
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_bUse = true;								// �g�p����Ă��邩�ǂ���]
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_���W�̐ݒ�
	pVtx[0].pos = m_posStart;
	pVtx[1].pos = m_posEnd;
	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void CLine::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	Release();
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void CLine::Update(void)
{
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void CLine::Draw(void)
{
	if (!m_bUse)return;
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							// �r���{�[�h�p�}�g���b�N�X

	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, 0);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}
//=============================================================================
// ����
//=============================================================================
CLine *CLine::Create(const D3DXVECTOR3 posStart, const D3DXVECTOR3 posEnd)
{
	CLine *pLine = NULL;
	if (!CScene::OverData())
	{// �QD�|���S������
		pLine = new CLine;
	}
	if (pLine != NULL)
	{
		pLine->m_posStart = posStart;
		pLine->m_posEnd = posEnd;

		// ����������
		if (FAILED(pLine->Init()))
		{
			return NULL;
		}
	}
	return pLine;
}
//=============================================================================
// ���W�ݒ�
//=============================================================================
void CLine::SetPos(const D3DXVECTOR3 start, const D3DXVECTOR3 end)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	m_posStart = start;
	m_posEnd = end;
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_���W�̐ݒ�
	pVtx[0].pos = m_posStart;
	pVtx[1].pos = m_posEnd;
	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �\����\��
//=============================================================================
void CLine::SetDisp(bool bDisp)
{
	m_bUse = bDisp;
}