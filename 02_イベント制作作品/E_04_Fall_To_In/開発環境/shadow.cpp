//=============================================================================
//
// �V���h�E���� [shadow.cpp]
// Author : Okabe Kazuki
//
//=============================================================================
#include "renderer.h"
#include "shadow.h"
#include "manager.h"
#include "light.h"
//=============================================================================
// �V���h�E�̕`��
//=============================================================================
void CShadow::SetShadow(LPD3DXMESH Mesh, DWORD NumMat, D3DXMATRIX mtxWorld,D3DXVECTOR3 *pPos)
{
	// �f�o�C�X�̊m��
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// �f�o�C�X�̎擾

	//�e�p�̎ˉe�}�g���N�X���쐬
	D3DXMATRIX mShadow;				// �}�g���b�N�X
	D3DXVECTOR3 vPoint= *pPos;	// ���ʂɎg���_
	D3DXVECTOR3 vNormal(0, -1, 0);	// ���ʂɎg���@��
	D3DXPLANE GroundPlane;

	// �_�Ɩ@�����畽�ʂ��쐬����
	D3DXPlaneFromPointNormal(&GroundPlane, pPos, &vNormal);

	// �W�I���g���𕽖ʂɎˉe����
	D3DXMatrixShadow(&mShadow, &D3DXVECTOR4(0.7f, -0.8f, 0.54f, 0.0f), &GroundPlane);

	mShadow = mtxWorld*mShadow;
	mShadow._42 += 1;

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);	// �X�e���V����o�b�t�@�L��

	//�e�p�}�e���A���̐ݒ�
	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));
	Material.Emissive.a = 1.0f;
	Material.Emissive.r = 0.0f;
	Material.Emissive.g = 0.0f;
	Material.Emissive.b = 0.0f;
	Material.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	//�e�Ƃ��ă����_�����O
	pDevice->SetTransform(D3DTS_WORLD, &mShadow);
	pDevice->SetMaterial(&Material);
	for (DWORD nCnt = 0; nCnt < NumMat; nCnt+=2)
	{// �`����y��
		pDevice->SetTexture(0, NULL);
		Mesh->DrawSubset(nCnt);
	}
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);	// �X�e���V����o�b�t�@����
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);



}
