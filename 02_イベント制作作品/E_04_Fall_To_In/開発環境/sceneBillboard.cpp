//=============================================================================
//
// �r���{�[�h���� [sceneBillboard.h]
// Author : Okabe Kazuki
//
//=============================================================================
#include "sceneBillboard.h"
#include "manager.h"
#include "renderer.h"
//******************************************************************************
// �}�N����`
//******************************************************************************
LPDIRECT3DTEXTURE9	CBillboard::m_pTextureThis = NULL;
bool				CBillboard::m_bZenable = true;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBillboard::CBillboard(int nPriority) :CScene(nPriority)
{
	m_pTexture = NULL;							// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_rot = 0;									// ����
	m_fWidth = 0;								// ��
	m_fHeight = 0;								// ����
	m_fAngle = 0;								// �p�x
	m_fLength = 0;								// ����
	m_bZenable = true;
	m_VtxType = VTXTYPE_CENTER;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBillboard::~CBillboard()
{

}

//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CBillboard::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice,
		"",
		&m_pTextureThis);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void CBillboard::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_pTextureThis != NULL)
	{
		m_pTextureThis->Release();
		m_pTextureThis = NULL;
	}
}

//=============================================================================
// �e�N�X�`�����蓖��
//=============================================================================
void CBillboard::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CBillboard::Init(void)
{

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �|���S���̈ʒu��ݒ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_rot = 0;									// ����
	m_fWidth = 0;								// ��
	m_fHeight = 0;								// ����
	m_fAngle = 0;								// �p�x
	m_fLength = 0;								// ����
	m_VtxType = VTXTYPE_CENTER;
	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CBillboard::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CBillboard::Update(void)
{
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CBillboard::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;				// �v�Z�p�}�g���b�N�X

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (GetObjType() == OBJTYPE_CALLOUT&&!m_bZenable)
	{// �����o���Ȃ�
		// Z�o�b�t�@�𖳌���
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	}
	// �A���t�@�e�X�g�̏���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 130);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �t�s��(�J�����ƌ���������)
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,2);

	// �����_�[�X�e�[�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	if (GetObjType() == OBJTYPE_CALLOUT && !m_bZenable)
	{// �����o���Ȃ�
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
}
//=============================================================================
// ����
//=============================================================================
CBillboard *CBillboard::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CBillboard *pBillboard = NULL;
	if (!CBillboard::OverData())
	{// �r���{�[�h����
		pBillboard = new CBillboard;
	}
	if (pBillboard != NULL)
	{
		// ����������
		if (FAILED(pBillboard->Init()))
		{
			return NULL;
		}
		pBillboard->SetPos(pos);
		pBillboard->SetSize(fWidth, fHeight);
		pBillboard->SetVtx();
	}
	return pBillboard;
}
//=============================================================================
// ���W�ݒ�
//=============================================================================
void CBillboard::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �傫���ݒ�
//=============================================================================
void CBillboard::SetSize(const float fWidth, const float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fAngle = atan2f(m_fWidth / 2, m_fHeight / 2);
	m_fLength = sqrtf((m_fWidth / 2.0f) * (m_fWidth / 2.0f) + (m_fHeight / 2.0f) * (m_fHeight / 2.0f));
}
//==============================================================================================================================
// ��]�ݒ�
//==============================================================================================================================
void CBillboard::SetRot(const float rot)
{
	m_rot = rot;

	// ���E�n�𒴂����猳�ɖ߂�
	if (m_rot > D3DX_PI)
	{
		m_rot -= D3DX_PI * 2.0f;
	}
	else if (m_rot < -D3DX_PI)
	{
		m_rot += D3DX_PI * 2.0f;
	}

	CBillboard::SetVtx();
}

//=============================================================================
// �F�ݒ�
//=============================================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// ���_���W
//=============================================================================
void CBillboard::SetVtx(void)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	switch (m_VtxType)
	{// ���_���W�̐ݒ�
	case VTXTYPE_CENTER:
		pVtx[0].pos = D3DXVECTOR3(sinf(m_rot - m_fAngle) * m_fLength, cosf(m_rot - m_fAngle) * m_fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(sinf(m_rot + m_fAngle) * m_fLength, cosf(m_rot + m_fAngle) * m_fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(sinf(m_rot - D3DX_PI + m_fAngle) * m_fLength, cosf(m_rot - D3DX_PI + m_fAngle) * m_fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(sinf(m_rot + D3DX_PI - m_fAngle) * m_fLength, cosf(m_rot + D3DX_PI - m_fAngle) * m_fLength, 0.0f);
		break;
	case VTXTYPE_BASE:
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth/2, m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth/2, m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth/2, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth/2, 0.0f, 0.0f);
		break;
	case VTXTYPE_LEFTBASE:
		pVtx[0].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, 0.0f);
		break;
	}

	// �@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//=============================================================================
// Z�o�b�t�@�؂邩�ǂ������W
//=============================================================================
void CBillboard::SetZenable(const bool Zenable)
{
	m_bZenable = Zenable;
}
//=============================================================================
// ���_�^�C�v
//=============================================================================
void CBillboard::SetVtxType(const VTXTYPE type)
{
	m_VtxType = type;
}