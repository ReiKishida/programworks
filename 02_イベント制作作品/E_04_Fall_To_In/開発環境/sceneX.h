////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// X�t�@�C������ [sceneX.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "main.h"
#include "scene.h"
class CLine;
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_MODEL_TYPE	(30)		// ���f���̃^�C�v�̍ő吔
#define MAX_VTX			(4)
#define MAX_VTX_LINE	(12)
//******************************************************************************************************************************
// X�t�@�C���̃N���X
//******************************************************************************************************************************
class CSceneX : public CScene
{
public:
	typedef struct
	{

		D3DXVECTOR3 m_ofset;					// �I�t�Z�b�g
		float m_fWidth;							// ��
		float m_fHeight;						// ����
		float m_fDepth;							// ���s��
		D3DXVECTOR3 m_vtxMax;					// �ő咸�_
		D3DXVECTOR3 m_vtxMin;					// �ŏ����_

		float m_afAngle[MAX_VTX];				// �S���_�p�x
		float m_afLength[MAX_VTX];				// �S���_����
	}CollisionData;

	typedef struct
	{
		LPD3DXMESH m_pMesh;						// ���b�V�����ւ̃|�C���^
		LPD3DXBUFFER m_pBuffMat;				// �}�e���A�����ւ̃|�C���^
		DWORD m_nNumMat;						// �}�e���A�����̐�
		LPDIRECT3DTEXTURE9 *m_pTextuer;			// �e�N�X�`��
		char *m_pFileName;						// �t�@�C����
		int m_nNumCollision;					// ���萔
		CollisionData *m_pCollision;			// ����
	}XData;


	CSceneX(int nPriority = CScene::PRIORITY_INIT);
	~CSceneX();
	static HRESULT Load(void);	// ���f������
	static void UnLoad(void);	// ���f���j��

	void BindModel(const int type);
	static XData *GetModelData(const int type) { return &m_pModelData[type]; };
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneX *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type);
	D3DXVECTOR3 GetPos(void) { return m_pos; };
	D3DXVECTOR3 GetMove(void) { return m_move; };
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	D3DXMATRIX	GetMtx(void) { return m_mtxWorld; };
	D3DXCOLOR	GetColor(void) { return m_col; };
	int			GetType(void) { return m_type; };
	static int			GetNumFile(void) { return m_nNumFile; };

	void SetPos(const D3DXVECTOR3 pos);
	void SetMove(const D3DXVECTOR3 move);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtx(const D3DXMATRIX mtx);
	void SetColor(const D3DXCOLOR col);
	void SetType(const int type);
	void SetCameraID(const int nID);
	void SetShadowPos(const D3DXVECTOR3 pos);
	bool Collision(D3DXVECTOR3 *pPos, const D3DXVECTOR3 posOld, D3DXVECTOR3 *move, const float fHeight);
	bool CollisionReflection(D3DXVECTOR3 *pPos, const D3DXVECTOR3 posOld, D3DXVECTOR3 *move, const float fHeight);
	float GetHeight(D3DXVECTOR3 *pPos, bool *bIn);
	static void ScanCollision(void);

private:
	void SetCollision(void);
	void DeleteCollision(void);
	static XData m_pModelData[MAX_MODEL_TYPE];
	LPD3DXBUFFER m_pBuffMat;
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXVECTOR3 m_posOld;				// �O��̈ʒu
	D3DXVECTOR3 m_Initpos;				// �����ʒu
	D3DXVECTOR3 m_move;					// �ړ���
	D3DXCOLOR	m_col;					// �J���[
	D3DXVECTOR3 m_Diffrot;				// �p�x�̍���
	D3DXVECTOR3 m_Destrot;				// �ړI�̊p�x
	D3DXVECTOR3 m_rot;					// �p�x
	D3DXMATRIX  m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_ShadowPos;			// �e�̈ʒu
	int m_type;							// �^�C�v
	static int m_nNumFile;						// �ǂݍ��񂾃t�@�C����
	D3DXVECTOR3 **m_aMeshPos;			// �S���_���W�v�Z�p�|�C���^
	CLine ***m_apLine;					// �l�p�`�����p�|�C���^
	int m_nCameraID;					// �J����
};
#endif
