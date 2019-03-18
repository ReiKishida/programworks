//=============================================================================
//
// ���O����[2D �|���S��] [log.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _LOG_H_
#define _LOG_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LOGTEXTURE_NUM (5)
#define MAX_LOG_NUM (8)

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CManager;

class CLog : public CScene2D
{
public:
	typedef enum
	{
		LOGTYPE_NORMAL = 0,	//�ʏ���
		LOGTYPE_ATTACK,		//�U���^�C�v
		LOGTYPE_SPREAD,		//�g�U�^
		LOGTYPE_SPEED,		//�X�s�[�h�^
		LOGTYPE_DEFENSE,	//�h��^
	}LOGTYPE;//���O�̃^�C�v
	typedef enum
	{
		LOGNUM_ZERO = 0,	//0
		LOGNUM_FIRST,		//1
		LOGNUM_SECOND,		//2
		LOGNUM_THIRD,		//3
		LOGNUM_FORTH,		//4
		LOGNUM_FIFTH,		//5
		LOGNUM_SIXTH,		//6
		LOGNUM_SEVENTH,		//7

	}LOGNUM;//���O�̃^�C�v
	CLog();	//�R���X�g���N�^
	~CLog();	//�f�X�g���N�^
	static CLog *Create(D3DXVECTOR3 pos, LOGTYPE type);	//���O�̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��
	void MoveLog(void);	//���O�̈ړ�
	void MoveInit(float dest_x, float dest_y, int state);
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_LOGTEXTURE_NUM]; //���L�e�N�X�`���̃|�C���^
	D3DXCOLOR m_col;		//�F���
	D3DXVECTOR3 m_pos;		//�ʒu���
	D3DXVECTOR3 m_Destpos;	//�ړI�̈ʒu
	D3DXVECTOR3 m_posold;	//�ړI�̈ʒu
	LOGTYPE m_type;			//���O�̃^�C�v
	LOGNUM m_lognum;		//���O�̔ԍ�
	static int m_nCntLog;	//���O�̐�
	static int m_nCntLogOld;	//�ߋ��̃��O�̐�
	int m_MoveState;	//�ړ����
	float m_fAngle;	//�p�x���
	//static bool m_bMove;

};

#endif