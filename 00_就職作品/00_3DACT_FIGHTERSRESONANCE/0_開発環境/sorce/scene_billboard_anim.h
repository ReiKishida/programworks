//=============================================================================
//
// �r���{�[�h����[3D �|���S��] [billboard.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _SCENEBILLBOARDANIM_H_
#define _SCENEBILLBOARDANIM_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
//*****************************************************************************
// �V�[��3D�̃N���X�\����
//*****************************************************************************
class CSceneBillBoardAnim : public CScene
{
public:
	CSceneBillBoardAnim(int nPriority = 5, OBJTYPE type = OBJTYPE_SCENEBILLBOARDANIM);	//�R���X�g���N�^
	~CSceneBillBoardAnim();	//�f�X�g���N�^
	static CSceneBillBoardAnim *Create(D3DXVECTOR3 pos);//�G�̐���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void DrawEffect(void); //�`�揈��
	int GetNumSceneNillBoardAll(void);	//2D�V�[�����̎擾
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//�e�N�X�`���̊��蓖��
	void SetColor(D3DXCOLOR col);	//�F�̐ݒ�
	void SetTexture(float fTex_X, float fTex_Y);//�e�N�X�`���̐ݒ�

	//�e�N�X�`���A�j���[�V����
	void SetTextureAnim(float fTex_X, int nPatternAnim);
	//void SetTextureAnimY(float fTex_X, float fTex_Y, int nPatternAnim, int nTex_Y);

	//�|���S���̈ʒu���̎擾�A�ݒ�
	D3DXVECTOR3 GetPos(void);
	void SetPos(D3DXVECTOR3 pos);

	//�|���S���̊p�x���̎擾�A�ݒ�
	D3DXVECTOR3 GetRot(void);
	void SetRot(D3DXVECTOR3 pos);
	//���a�̐ݒ�
	void SetRadius(float fRadius);

private:
	LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;			//�|���S���̃}�g���b�N�X���
	D3DXVECTOR3				m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;			// �|���S���̉�]
	static CManager *m_pManager;//�}�l�[�W��
	int m_nCntflame;		//�J�E���g
	int m_nNumSceneBillBoard;	//2D�|���S����
	int m_nPriority;//�D��x
	float m_fWidth;	//�|���S���̕�
	float m_fHeight; //�|���S���̍���
	float m_fRadius; //�|���S���̔��a
	float m_fnor; //�|���S���̖@��
};

#endif