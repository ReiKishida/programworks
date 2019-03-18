//=============================================================================
//
// �����L���O�p�|���S������[2D �|���S��] [rankinglogo.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _RANKINGLOGO_H_
#define _RANKINGLOGO_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANKING_LOGO_TEXTURE (5)
#define MAX_RANKING_LOGO (5)

class CManager;

class CRankingLogo : public CScene
{
public:

	CRankingLogo();	//�R���X�g���N�^
	~CRankingLogo();	//�f�X�g���N�^
	static CRankingLogo *Create(D3DXVECTOR3 pos);	//�����̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��


private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_RANKING_LOGO_TEXTURE]; //���L�e�N�X�`���̃|�C���^
	static CScene2D *m_apScene2D[MAX_RANKING_LOGO];	//����

};

#endif