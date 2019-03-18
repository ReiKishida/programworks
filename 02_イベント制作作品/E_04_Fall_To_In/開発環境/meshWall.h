////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���b�V���Ǐ��� [meshWall.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"
#include "scene.h"
//******************************************************************************************************************************
// ���b�V�����̃N���X
//******************************************************************************************************************************
class CMeshWall : public CScene
{
public:

	CMeshWall(int nPriority = CScene::PRIORITY_INIT);
	~CMeshWall();
	static HRESULT Load(void);	// �e�N�X�`������
	static void UnLoad(void);	// �e�N�X�`���j��

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMeshWall *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,const D3DXCOLOR col, const float fWidth, const float fHeight, const float fDepth,const int nBlockX,const int nBlockY,int type);

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// ���W�擾
	float GetWidth(void) { return m_fWidth; }			// ���擾
	float GetHeight(void) { return m_fHeight; }			// �����擾
	float GetDepth(void) { return m_fDepth; }			// ���s���擾
	D3DXVECTOR3 GetRot(void) { return m_rot; };			// ��]�擾
	D3DXMATRIX	GetMtx(void) { return m_mtxWorld; };	// �}�g���b�N�X�擾

	void SetPos(const D3DXVECTOR3 pos);											// ���W�ݒ�
	void SetSize(const float fWidth, const float fHeight, const float fDepth);	// �傫���ݒ�
	void SetBlock(const int nBlockX, const int nBlockY);						// �������ݒ�
	void SetRot(const D3DXVECTOR3 fRot);										// ��]�ݒ�
	void SetColor(const D3DXCOLOR col);										// �F�ݒ�
	void SetVtx(void);															// ���_�ݒ�
	void MakeVtx(void);														// �쐬

	static void SetScan(void);		// X�t�@�C���ǂݍ���
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld,const float fWidth,const float fHeight);
private:
	static LPDIRECT3DTEXTURE9	*m_pTextureThis;		// �e�N�X�`���ւ̃|�C���^

	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_rot;			// ����
	D3DXVECTOR3 *m_pNor;		// �@��
	D3DXCOLOR	m_col;			// �F
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	float		m_fWidth;		// ��
	float		m_fHeight;		// ����
	float		m_fDepth;		// ���s��
	int         m_type;			// ���
	int			m_nBlock_X;		// �u���b�NX
	int			m_nBlock_Y;		// �u���b�NZ
	int			m_nNumVertex;	// ���_��
	int			m_nNumIndex;	// �C���f�b�N�X��
	int			m_nNumPolygon;	// �|���S����
	char		m_pFileName;					// �t�@�C���̖��O

	float		m_afAngle[4];
	float		m_afLength[4];

};
#endif
