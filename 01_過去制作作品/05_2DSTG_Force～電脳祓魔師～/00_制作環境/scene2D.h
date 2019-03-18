//=============================================================================
//
// �V�[������[2D �|���S��] [scene2D.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CManager;

class CScene2D : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,
		STATE_NORMAL,
		STATE_DAMAGE,
		STATE_DEATH,
		STATE_MAX
	}STATE;
	CScene2D::CScene2D(int nPriority = 3);	//�R���X�g���N�^
	CScene2D::~CScene2D();	//�f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void DrawEffect(void); //�`�揈��
	static CScene2D *Create(D3DXVECTOR3 pos);//�G�̐���
	int GetNum2DSceneAll(void);	//2D�V�[�����̎擾
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//�e�N�X�`���̊��蓖��
	void CheckDamage(void);
	static int GetDamage(void);
	void SetTexture(float fTex_X, float fTex_Y);//�e�N�X�`���̐ݒ�
	void SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y);//�e�N�X�`���̐ݒ�

	void SetColor(D3DXCOLOR col);	//�F�̐ݒ�
	D3DXCOLOR GetColor(void);	//�F�̐ݒ�

	void SetPriority(int nPriority);//�D�揇��
	void SetForceTexture(int nType);//�t�H�[�X�e�N�X�`���̐ݒ�
	int GetForce();//�t�H�[�X�^�C�v�̎擾
	void SetForce(int nType);//�t�H�[�X�̐ݒ�

	//�w�i�X�N���[��
	void ScrollBg(D3DXVECTOR2 Scroll_0, D3DXVECTOR2 Scroll_1, D3DXVECTOR2 Scroll_2, D3DXVECTOR2 Scroll_3);

	//�e�N�X�`���A�j���[�V����
	void SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim);
	void SetTextureAnimY(float fTex_X, float fTex_Y, int nPatternAnim, int nTex_Y);

	//�|���S���̑傫���̎擾�A�ݒ�
	void SetScene2DWidth(float fWidth);
	void SetScene2DHeight(float fWidth);

	//�|���S���̈ʒu���̎擾�A�ݒ�
	D3DXVECTOR3 GetPosScene2D(void);
	void SetPosScene2D(D3DXVECTOR3 pos);

	//�|���S���̊p�x���̐ݒ�A�ݒ�
	D3DXVECTOR3 GetRotScene2D(void);
	void SetRotScene2D(D3DXVECTOR3 rot);

	//�|���S���̒����̎擾�A�ݒ�
	float GetLengthScene2D(void);
	void SetLengthScene2D(float fLength);

	//�|���S���̑傫���̎擾�A�ݒ�
	float GetWidthScene2D(void);
	float GetHeightScene2D(void);

	//���C�t�̎���
	int GetLife(void);
	void SetLife(int nLife);
	void HitLife(int nDamage);


protected:
	STATE m_state;//���

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_posScene2D;			// �|���S���̈ʒu
	D3DXVECTOR3				m_rotScene2D;			// �|���S���̉�]
	D3DXCOLOR				m_color;				//�|���S���̐F
	float m_fLengthScene2D;	//�|���S���̒���
	float m_fAngleScene2D;	//�|���S���̊p�x
	float m_fScene2DWidth;	//�|���S���̕�
	float m_fScene2DHeight;	//�|���S���̍���
	int m_nCntframe;		//�J�E���g
	int m_nNumScene2D;	//2D�|���S����
	static CManager *m_pManager;//�}�l�[�W��
	int m_nLife;			//���C�t
	int m_nPriority;		//�D��x
	int m_nType;			//�^�C�v
	static int m_nDamage;	//�_���[�W
	int m_nDPS;				//DPS


};

#endif