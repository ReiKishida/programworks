//=============================================================================
//
// 3D���b�V���X�t�B�A�̏��� [scene3DSphere.cpp]
// Author : �T�c ����
//
//=============================================================================
#include "scene3DSphere.h"
#include "manager.h"
#include "renderer.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
CScene3DSphere *CScene3DSphere::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight, const int nCutH, const int nCutV, const D3DXCOLOR col, const int nDirection)
{
	CScene3DSphere *pScene3DSphere = NULL;

	// �N���X�̐���
	pScene3DSphere = new CScene3DSphere;

	if (pScene3DSphere == NULL)
	{// �����ł��Ȃ������ꍇ
		return NULL;
	}

	// ����������
	if (FAILED(pScene3DSphere->Init()))
	{
		return NULL;
	}
	pScene3DSphere->SetAll(pos, rot, fRadius, fHeight, nCutH, nCutV, col, nDirection);
	pScene3DSphere->MakeVertex();

	return pScene3DSphere;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene3DSphere::CScene3DSphere(int nPriority):CScene(nPriority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pVtxBuffLid = NULL;
	m_pIdxBuff = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 0.0f;
	m_fHeight = 0.0f;
	m_nCutH = 0;
	m_nCutV = 0;
	m_nDirection = 0;
	m_nNumVtx = 0;
	m_nNumPolygon = 0;
	m_nNumIdx = 0;
	m_texMove = D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene3DSphere::~CScene3DSphere()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene3DSphere::Init()
{
	// ����������
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 0.0f;
	m_fHeight = 0.0f;
	m_nCutH = 0;
	m_nCutV = 0;
	m_nDirection = 0;
	m_nNumVtx = 0;
	m_nNumPolygon = 0;
	m_nNumIdx = 0;
	m_texMove = D3DXVECTOR2(0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene3DSphere::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// ���_�o�b�t�@[�W]�̔j��
	if (m_pVtxBuffLid != NULL)
	{
		m_pVtxBuffLid->Release();
		m_pVtxBuffLid = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �J������
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene3DSphere::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene3DSphere::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}
	// �A���t�@�e�X�g�̏���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

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

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP	// �v���~�e�B�u�̎��
								, 0
								, 0
								, m_nNumVtx				// ���_��
								, 0
								, m_nNumPolygon);		// �`�悷��v���~�e�B�u�̐�

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D));

	// �|���S���̕`��[�W]
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN
						, 0						// �J�n���钸�_�̃C���f�b�N�X
						, m_nCutH);				// �`�悷��v���~�e�B�u�̐�

	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN
						, m_nCutH + 2			// �J�n���钸�_�̃C���f�b�N�X
						, m_nCutH);				// �`�悷��v���~�e�B�u�̐�
	// ���e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�𖳌���

}

//=============================================================================
// �e�N�X�`�����蓖��
//=============================================================================
void CScene3DSphere::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CScene3DSphere::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_3D
		, D3DPOOL_MANAGED
		, &m_pVtxBuff
		, NULL);

	// ���_�o�b�t�@�𐶐�[�W]
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nCutH + 2) * 2
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_3D
		, D3DPOOL_MANAGED
		, &m_pVtxBuffLid
		, NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx
		, D3DUSAGE_WRITEONLY
		, D3DFMT_INDEX16
		, D3DPOOL_MANAGED
		, &m_pIdxBuff
		, NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx, *pVtxLid;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	m_pVtxBuffLid->Lock(0, 0, (void**)&pVtxLid, 0);

	int nCntVtx = 0, nCntVtxLid = 0;
	float fAngleV, fAngleH, fRadius, fHeight;
	for (int nCntV = 0; nCntV < m_nCutV - 1; nCntV++)
	{
		if (nCntV == 0 || nCntV == m_nCutV - 2)
		{// ���_���̐ݒ�[�W]
			pVtxLid[nCntVtxLid].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
			if (nCntV == m_nCutV - 2)
			{// ��W�̏ꍇ
				pVtxLid[nCntVtxLid].pos.y *= -1;
			}
			D3DXVec3Normalize(&pVtxLid[nCntVtxLid].nor, &pVtxLid[nCntVtxLid].pos);
			pVtxLid[nCntVtxLid].col = m_col;
			pVtxLid[nCntVtxLid].tex = D3DXVECTOR2(0.0f, 0.0f);
			nCntVtxLid++;
		}

		for (int nCntH = 0; nCntH < m_nCutH + 1; nCntH++, nCntVtx++)
		{
			fAngleH = D3DX_PI * 2.0f / m_nCutH * nCntH;
			if (fAngleH > D3DX_PI)
			{
				fAngleH -= D3DX_PI * 2.0f;
			}

			if (m_nDirection == 1)
			{// �������̏ꍇ
				fAngleH *= -1;
			}

			fAngleV = D3DX_PI / m_nCutV + D3DX_PI / m_nCutV * nCntV;
			fRadius = sinf(fAngleV) * m_fRadius;
			fHeight = cosf(fAngleV) * m_fHeight;

			if (nCntV == 0 || nCntV == m_nCutV - 2)
			{// ���_���̐ݒ�[�W]
				pVtxLid[nCntVtxLid].pos = D3DXVECTOR3(sinf(fAngleH) * fRadius, fHeight, cosf(fAngleH) * fRadius);
				if (nCntV == m_nCutV - 2)
				{// ��W�̏ꍇ
					pVtxLid[nCntVtxLid].pos.z *= -1;
				}
				D3DXVec3Normalize(&pVtxLid[nCntVtxLid].nor, &pVtxLid[nCntVtxLid].pos);
				pVtxLid[nCntVtxLid].col = m_col;
				pVtxLid[nCntVtxLid].tex = D3DXVECTOR2(0.0f, 1.0f);
				nCntVtxLid++;
			}

			// ���_���̐ݒ�
			pVtx[nCntVtx].pos = D3DXVECTOR3(sinf(fAngleH) * fRadius, fHeight, cosf(fAngleH) * -fRadius);
			D3DXVec3Normalize(&pVtx[nCntVtx].nor, &pVtx[nCntVtx].pos);
			pVtx[nCntVtx].col = m_col;
			pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f / m_nCutH * nCntH, 1.0f / m_nCutV * (nCntV - 2));
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
	m_pVtxBuffLid->Unlock();

	// �C���f�b�N�X�f�[�^�̐ݒ�
	WORD *pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	int nNumIdx = m_nCutH + 1;
	for (int nCntIdx = 0; nCntIdx < m_nNumIdx; nCntIdx += 2, nNumIdx++)
	{
		if (nNumIdx % (m_nCutH + 1) == 0 && nNumIdx != m_nCutH + 1)
		{// �܂�Ԃ��n�_�̏ꍇ
			pIdx[nCntIdx] = nNumIdx - (m_nCutH + 2);
			pIdx[nCntIdx + 1] = nNumIdx;
			nCntIdx += 2;
		}
		pIdx[nCntIdx] = nNumIdx;
		pIdx[nCntIdx + 1] = nNumIdx - (m_nCutH + 1);
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//=============================================================================
// �S�Ă̏��̐ݒ�
//=============================================================================
void CScene3DSphere::SetAll(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight, const int nCutH, const int nCutV, const D3DXCOLOR col, const int nDirection)
{
	// ����ݒ�
	m_pos = pos;
	m_rot = rot;
	m_col = col;
	m_fRadius = fRadius;
	m_fHeight = fHeight;
	m_nCutH = nCutH;
	m_nCutV = nCutV;
	m_nDirection = nDirection;
	m_nNumVtx = (m_nCutH + 1) * (m_nCutV - 1);
	m_nNumPolygon = m_nCutH * (m_nCutV - 2) * 2 + (m_nCutV - 3) * 4;
	m_nNumIdx = m_nNumPolygon + 2;
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CScene3DSphere::SetPosition(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//=============================================================================
// �e�N�X�`���ړ�
//=============================================================================
void CScene3DSphere::SetTexMove(const D3DXVECTOR2 tex)
{
	m_texMove = tex;

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

								// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtx = 0;
	for (int nCntV = 0; nCntV < m_nCutV - 1; nCntV++)
	{
		for (int nCntH = 0; nCntH < m_nCutH + 1; nCntH++, nCntVtx++)
		{
			pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f / m_nCutH * nCntH, 1.0f / m_nCutV * (nCntV - 2))+ m_texMove;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}