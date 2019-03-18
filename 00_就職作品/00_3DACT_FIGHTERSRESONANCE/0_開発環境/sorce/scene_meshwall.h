//=============================================================================
//
// ���b�V���E�H�[������ [meshwall.h]
// Author : �ݓc��(Kishida Rei)
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"

//*****************************************************************************
// ���b�V���t�B�[���h�N���X�̍\����
//*****************************************************************************
class CSceneMeshWall : public CScene
{
public:
	typedef enum
	{
		WALLTYPE_NORMAL = 0,	//�ʏ�̕�
		WALLTYPE_INVISIBLE,		//�����̕�
		WALLTYPE_MAX			//�ǂ̍ő吔
	}WALLTYPE;					//�ǂ̎��
	CSceneMeshWall(int nPriority = 5, OBJTYPE type = OBJTYPE_MESHWALL);		//�R���X�g���N�^
	~CSceneMeshWall();			//�f�X�g���N�^
	static HRESULT Load(void);									//�e�N�X�`���̓ǂݍ���
	static void Unload(void);									//�e�N�X�`���̔j��
	static CSceneMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sprit_X, int sprit_Y, float Width, float Height, WALLTYPE type);//�ǂ̐���
	HRESULT Init(void);							//����������
	void Uninit(void);							//�I������
	void Update(void);							//�X�V����
	void Draw(void);							//�`�揈��
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);	//���_���̐���
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);	//�C���f�b�N�X���̐���
	void CollisionWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posold, float fRadius);	//��O�ʂ̓����蔻��
	static void LoadWallText(void);					//�ǂ̏��̓ǂݍ���
	static void LoadWallText_Tuto(void);					//�ǂ̏��̓ǂݍ���
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff; //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	//�ǂ̏��
	D3DXVECTOR3 m_pos;		//�ʒu(���S���W)
	D3DXVECTOR3 m_rot;		//����
	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
	D3DXCOLOR m_col;		//�F���
	D3DXVECTOR2 m_TexPos;	//�e�N�X�`�����W
	int m_nMesh_X;			//X���̕���
	int m_nMesh_Y;			//Y���̕���
	int m_nVertexNum;		//���_��
	int m_nIndexNum;		//�C���f�b�N�X��
	int m_nPolygonNum;		//�|���S����
	float m_fMeshWidth;		//���b�V���t�B�[���h�̕�
	float m_fMeshHeight;	//���b�V���t�B�[���h�̍���
	WALLTYPE m_type;		//�ǂ̎��
};

#endif
