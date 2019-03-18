//=============================================================================
//
// �f�o�b�O���� [DebugProc.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "DebugProc.h"
#include "renderer.h"
#include "manager.h"
//*********************************************************************************
// �ÓI�����o�ϐ��錾
//*********************************************************************************
LPD3DXFONT CDebugProc::m_pFont;			// �t�H���g�ւ̃|�C���^
char CDebugProc::m_aStr[MAX_CHAR_NAME];			// �����i�[
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDebugProc::CDebugProc()
{
	m_pFont = NULL;			// �t�H���g�ւ̃|�C���^
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDebugProc::~CDebugProc()
{

}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CDebugProc::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �f�o�b�O�\���p�t�H���g��ݒ�
	// pDevice         : �f�o�C�X�ւ̃|�C���^
	// Height          : �����̍���
	// Width           : �����̕�
	// Weight          : �t�H���g�̑���
	// MipLevels       : �~�b�v�}�b�v���x����
	// Italic          : �C�^���b�N�t�H���g
	// CharSet         : �t�H���g�̕����Z�b�g
	// OutputPrecision : ���ۂ̃t�H���g�ƖړI�̃t�H���g�̃T�C�Y����ѓ����̈�v���@���w��
	// Quality         : ���ۂ̃t�H���g�ƖړI�̃t�H���g�Ƃ̈�v���@���w��
	// PitchAndFamily  : �s�b�`�ƃt�@�~���C���f�b�N�X
	// pFacename       : �t�H���g�̖��O��ێ����镶����
	// ppFont          : �t�H���g�ւ̃|�C���^

	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 20, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "�l�r�o�S�V�b�N", &m_pFont);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CDebugProc::Uninit(void)
{
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}

}


//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, 1000, 700 };
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 255));
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}

//=============================================================================
// �f�o�b�O�t�H���g�\��
//=============================================================================
void CDebugProc::print(char *fmt, ...)
{
#ifdef _DEBUG
	int Len = strlen(m_aStr);
	if (Len >= MAX_CHAR_NAME)
	{
		return;
	}
	char aStr[1028];
	va_list ap ;
	// �ϒ��������P�̕ϐ��ɂ܂Ƃ߂�
	va_start(ap, fmt);
	// �܂Ƃ߂�ꂽ�ϐ��ŏ�������
	vsprintf(aStr, fmt, ap);
	va_end(ap);
	strcat(m_aStr, aStr);
#endif // _DEBUG
}