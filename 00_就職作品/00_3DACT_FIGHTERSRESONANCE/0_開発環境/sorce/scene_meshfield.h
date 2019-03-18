//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.h]
// Author :
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FIELD_TEXTURE (2)
//*****************************************************************************
// ���b�V���t�B�[���h�N���X�̍\����
//*****************************************************************************
class CSceneMeshField : public CScene
{
public:
	typedef enum
	{
		FIELDTYPE_NORMAL = 0,
		FIELDTYPE_YELLOW,
		FIELDTYPE_MAX
	}FIELDTYPE;
	CSceneMeshField(int nPriority = 3, OBJTYPE type = OBJTYPE_MESHFIELD);	//�R���X�g���N�^
	~CSceneMeshField();					//�f�X�g���N�^
	static HRESULT Load(void);									//�e�N�X�`���̓ǂݍ���
	static void Unload(void);									//�e�N�X�`���̔j��
	static CSceneMeshField *Create(D3DXVECTOR3 pos, int sprit_X, int sprit_Z, float fWidth, float fHeight, FIELDTYPE type);//�G�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);	//���_���̐���
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);	//�C���f�b�N�X���̐���
	float GetHeight(D3DXVECTOR3 pos);			//�����̎擾

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[FIELD_TEXTURE];		//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff; //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_pos;			//�ʒu(���S���W)
	D3DXVECTOR3 m_rot;			//����
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	D3DXCOLOR m_col;			//�F���
	D3DXVECTOR2 m_TexPos;		//�e�N�X�`�����W
	int m_nMesh_X;				//X���̕���
	int m_nMesh_Z;				//Y���̕���
	int m_nVertexNum;			//���_��
	int m_nIndexNum;			//�C���f�b�N�X��
	int m_nPolygonNum;			//�|���S����
	static int m_nSquareNum;	//�l�p�ʂ̐�
	static int m_nVecNum;		//�͂��x�N�g���̐�
	float m_fMeshWidth;			//���b�V���t�B�[���h�̕�
	float m_fMeshDepth;			//���b�V���t�B�[���h�̉��s��
	FIELDTYPE m_type;
	static D3DXVECTOR3 Vertexpos[9];
};

#endif
