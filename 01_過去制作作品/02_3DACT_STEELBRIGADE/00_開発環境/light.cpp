//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LIGHT

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DLIGHT9 g_Light[5];

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXVECTOR3 vecDir0;
	D3DXVECTOR3 vecDir1;
	D3DXVECTOR3 vecDir2;
	D3DXVECTOR3 vecDir3;
	D3DXVECTOR3 vecDir4;

	//// ���Z�����̐ݒ�
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���C�g���N���A����
	ZeroMemory(&g_Light[0], sizeof(D3DLIGHT9));
	ZeroMemory(&g_Light[1], sizeof(D3DLIGHT9));
	ZeroMemory(&g_Light[2], sizeof(D3DLIGHT9));
	ZeroMemory(&g_Light[3], sizeof(D3DLIGHT9));
	ZeroMemory(&g_Light[4], sizeof(D3DLIGHT9));


	// ���C�g�̎�ނ�ݒ�
	g_Light[0].Type = D3DLIGHT_DIRECTIONAL;
	g_Light[1].Type = D3DLIGHT_DIRECTIONAL;
	g_Light[2].Type = D3DLIGHT_DIRECTIONAL;
	g_Light[3].Type = D3DLIGHT_DIRECTIONAL;
	g_Light[4].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g�̊g�U����ݒ�
	g_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Light[1].Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	g_Light[2].Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	g_Light[3].Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	g_Light[4].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir0 = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	D3DXVec3Normalize(&vecDir0, &vecDir0);

	vecDir1 = D3DXVECTOR3(-0.2f, 0.8f, -0.4f);
	D3DXVec3Normalize(&vecDir1, &vecDir1);

	vecDir2 = D3DXVECTOR3(0.0f, -0.8f, 0.0f);
	D3DXVec3Normalize(&vecDir2, &vecDir2);

	vecDir3 = D3DXVECTOR3(0.3f, -0.8f, 0.0f);
	D3DXVec3Normalize(&vecDir3, &vecDir3);

	vecDir4 = D3DXVECTOR3(-0.3f, -0.8f, 0.0f);
	D3DXVec3Normalize(&vecDir4, &vecDir4);


	g_Light[0].Direction = vecDir0;
	g_Light[1].Direction = vecDir1;
	g_Light[2].Direction = vecDir2;
	g_Light[3].Direction = vecDir3;
	g_Light[4].Direction = vecDir4;

	// ���C�g��ݒ肷��
	pDevice->SetLight(0, &g_Light[0]);
	pDevice->SetLight(1, &g_Light[1]);
	pDevice->SetLight(2, &g_Light[2]);
	pDevice->SetLight(3, &g_Light[3]);
	pDevice->SetLight(4, &g_Light[4]);

	// ���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);
	pDevice->LightEnable(3, TRUE);
	pDevice->LightEnable(4, TRUE);

	//// ���u�����f�B���O�����ɖ߂�
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void UpdateLight(void)
{
}

