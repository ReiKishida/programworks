//=============================================================================
//
// �w�i����[2D �|���S��] [bg.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEXTURE (9)	//�e�N�X�`����
#define MAX_BG (4)	//�e�N�X�`����
//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CBg : public CScene
{
public:
	typedef enum
	{
		BGFADE_NONE = 0,		// �������Ă��Ȃ����
		BGFADE_IN,			// �t�F�[�h�C�����
		BGFADE_OUT,			// �t�F�[�h�A�E�g���
		BGFADE_MAX			//�t�F�[�h�̍ő吔
	} BGFADE;

	typedef enum
	{
		BGTYPE_NONE = 0,		// �������Ă��Ȃ����
		BGTYPE_PLAYERNORMAL,			// �v���C���[�̃n�b�N��Ԃ���m�[�}���ɖ߂��Ƃ�
		BGTYPE_BOSSNORMAL,			// �{�X�̃n�b�N��Ԃ���m�[�}���ɖ߂��Ƃ�
		BGTYPE_HACK_PLAYER,			// �n�b�L���O���(�v���C���[)
		BGTYPE_HACK_ENEMY,			// �n�b�L���O���(�G�l�~�[)
		FADE_MAX
	} BGTYPE;
	CBg(int nPriority = 7);	//�R���X�g���N�^
	~CBg();	//�f�X�g���N�^
	static CBg *Create(D3DXVECTOR3 pos);	//�w�i�̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��
	static void SetBG(BGTYPE type);
	static BGTYPE GetBG(void);

	static void SetFade(BGTYPE modeNext);//�t�F�[�h�̐ݒ�
	static BGFADE GetBgFade(void);//�t�F�[�h�̎擾
	void SetTexture(float fTex_X_0, float fTex_X_1, float fTex_Y_0, float fTex_Y_1);//�e�N�X�`���̐ݒ�


private:
	static BGTYPE m_type;//�w�i�̎��
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE]; //���L�e�N�X�`���̃|�C���^
	static CScene2D *m_apScene2D[MAX_BG];//�V�[��2D
	D3DXVECTOR2 m_aBg[MAX_BG];//�w�i�̍ő吔
	int m_nType;	//�w�i�̃^�C�v
	int m_nCntFrame;	//�t���[�����J�E���g�p
	static D3DXCOLOR m_col[MAX_BG]; //�J���[����p
	static BGTYPE m_typeNext;//���̔w�i�̎��
	static BGFADE m_BgFade;//�w�i�t�F�[�h��
	static int m_nPatternAnim;	//�A�j���[�V�����p�^�[��
	static int m_nCounterAnim;//�A�j���[�V�����J�E���^�[
	static int m_nTex_Y;//�e�N�X�`���̐ݒ�

};

#endif