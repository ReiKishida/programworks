//=============================================================================
//
// ���b�V���E�H�[������ [meshwall.h]
// Author : �ݓc��(Kishida Rei)
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"

//*****************************************************************************
// ���b�V���t�B�[���h�N���X�̍\����
//*****************************************************************************
class CSceneMeshCylinder : public CScene
{
public:
	typedef enum
	{
		CYLINDERTYPE_NORMAL = 0,	//�ʏ�V�����_�[
		CYLINDERTYPE_GOAL,			//�S�[���p�̃V�����_�[
		CYLINDERTYPE_MAX			//�V�����_�[�̍ő吔
	}CYLINDERTYPE;					//�V�����_�[�̎��
	CSceneMeshCylinder(int nPriority = 3, OBJTYPE type = OBJTYPE_MESHCYLINDER);		//�R���X�g���N�^
	~CSceneMeshCylinder();			//�f�X�g���N�^
	static CSceneMeshCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sprit_H, int sprit_V, float fRadius, float fHeight, CYLINDERTYPE type);//�V�����_�[�̐���
	HRESULT Init(void);							//����������
	void Uninit(void);							//�I������
	void Update(void);							//�X�V����
	void Draw(void);							//�`�揈��
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);	//���_���̐���
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);	//�C���f�b�N�X���̐���
	bool CollisionCylinder(D3DXVECTOR3 *pos);	//�V�����_�[�Ƃ̓����蔻��
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;			//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	//�V�����_�[�̏��
	D3DXVECTOR3 m_pos;		//�ʒu(���S���W)
	D3DXVECTOR3 m_rot;		//����
	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
	D3DXCOLOR m_col;		//�F���
	D3DXVECTOR2 m_TexPos;	//�e�N�X�`�����W
	int m_nMesh_V;			//�����̕���
	int m_nMesh_H;			//�����̕���
	int m_nVertexNum;		//���_��
	int m_nIndexNum;		//�C���f�b�N�X��
	int m_nPolygonNum;		//�|���S����
	float m_fRadius;		//���b�V���t�B�[���h�̔��a
	float m_fHeight;		//���b�V���t�B�[���h�̍���
	CYLINDERTYPE m_type;	//�V�����_�[�̎��
};

#endif
