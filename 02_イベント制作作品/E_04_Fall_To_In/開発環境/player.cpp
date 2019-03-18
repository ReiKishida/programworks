//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// �v���C���[���� [player.cpp]
// Author:�������Y
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "debugProc.h"
#include "model.h"
#include "motion.h"
#include "PanelHole.h"
#include "meshWall.h"
#include "buttonMotion.h"
#include "sound.h"
#include "result.h"
#include "fade.h"
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MOVE_SPEED			(1.0f)					// �ʏ펞�̑��鑬�x
#define ORDER_SPEED			(0.4f)					// �񋟎��̑��鑬�x
#define COOK_SPEED			(0.2f)					// �񋟎��̕������x
#define MOVE_INERTIA		(0.1f)					//

#define RANGE			(D3DXVECTOR3(100.0f,10.0f,100.0f))		// �ǂƂ̓����蔻��
#define WALL_COLLISION	(D3DXVECTOR3(40.0f,40.0f,40.0f))		// �����������o���͈�
#define CHARA_COLLISION	(D3DXVECTOR3(3.0f,3.0f,3.0f))		// �v���C���[���g�Ɠ����蔻�肷��ۂ̓����̔��a
#define KUSHI_COLLISION	(100.0f)
#define MAX_LOAD_LINE (2560)
#define MAX_KISHI_TAKE (2)
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �ÓI�����o�ϐ��錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
CMotionData *CPlayer::m_apMotion[MOTION_DATA_TYPE_MAX] = {};			// ���[�V�����ւ̃|�C���^
char *CPlayer::m_apMotionInfo[MOTION_DATA_TYPE_MAX] =
{
	// �A�h���X
	"data//TEXT//motion_takeru.txt",			// �T���v��00
	"data//TEXT//motion_zozo_red.txt",			// �T���v��00
	"data//TEXT//motion_zozo_blue.txt",			// �T���v��00
};
CPlayer::MOTION_TYPE CPlayer::m_buttonMotionType[POSE_BUTTON_MAX] = {};
CButtonMotion *CPlayer::m_buttonMotionDisp = NULL;
bool CPlayer::m_bBottonCommand[POSE_BUTTON_MAX] = {};	// �{�^���p�u�[��
bool CPlayer::m_bControl = false;
int	CPlayer::m_nControlLevel = 0;						// ����Level
int CPlayer::m_n1PlayerMotion = -1;
int CPlayer::m_nNumAll = 0;
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �R���X�g���N�^
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CPlayer::CPlayer() : CMotion(CScene::PRIORITY_PLAYER)
{
	m_nNumAll++;

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �f�X�g���N�^
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CPlayer::~CPlayer()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ����������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CPlayer::Init(void)
{

	// ���[�V�������v���C���[�ɐݒ肷��
	if (m_nId == 0)
	{
		m_nType = (MOTION_DATA_TYPE)(rand() % MOTION_DATA_TYPE_MAX);

		m_n1PlayerMotion = m_nType;

		CResult::Set1P(m_nType);
	}
	else
	{
		bool bLoop = true;
		while (bLoop)
		{
			m_nType = (MOTION_DATA_TYPE)(rand() % MOTION_DATA_TYPE_MAX);

			if (m_nType != m_n1PlayerMotion)
			{
				bLoop = false;
				CResult::Set2P(m_nType);
			}
		}
	}


	// ���[�V�����ݒ�
	CMotion::BindMotion(m_apMotion[m_nType]);


	// ���[�V����������
	CMotion::Init();
	// �I�u�W�F�N�g�^�C�v��ݒ�
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);
	// ���[�V������BOOL������������
	for (int nCntBool = 0; nCntBool < BOOL_MAX; nCntBool++)
	{
		m_bBool[nCntBool] = false;
		m_bBoolOld[nCntBool] = false;
	}
	// �{�^���p��BOOL������������
	for (int nCntBool = 0; nCntBool < BOOL_MAX; nCntBool++)
	{
		m_bBotton[nCntBool] = false;
	}

	// ���a����ݒ�
	m_fRadius = 20.0f;
	m_fHeifht = 45.0f;

	// �ړ����x�ݒ�
	CMotion::SetSpeed(MOVE_SPEED);

	CMotion::SetPos(D3DXVECTOR3(-100.0f + m_nId * 200.0f, 12000.0f, 0.0f));
	//CMotion::SetRot(D3DXVECTOR3(-D3DX_PI / 2.0f, 0.0f, D3DX_PI / 2.0f));


	// ID��������

	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �I������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::Uninit(void)
{
	if (m_nId == 0)
	{


		CResult::Set1P(m_nType);
	}
	else
	{

				CResult::Set2P(m_nType);

	}

	// ���[�V�����I������
	CMotion::Uninit();
	m_nNumAll--;
}


//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �X�V����
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::Update(void)
{
	// ���[�V�����X�V����
	CMotion::Update();

	// ���[�V������BOOL�̉ߋ��̃��[�V������ݒ�
	for (int nCntBool = 0; nCntBool < BOOL_MAX; nCntBool++)
	{
		m_bBoolOld[nCntBool] = m_bBool[nCntBool];
	}

	CManager::MODE pManager = CManager::GetMode();

	if (pManager == CManager::MODE::MODE_TITLE)
	{// �^�C�g�����[�h�̎�

	 // ���[�V�������擾
		CMotionMove *pMotion = CMotion::GetMotion();

		// ���[�V������ގ擾
		int nMotionType = CMotion::GetMotionType();
		int nMotionTypeOld = CMotion::GetMotionTypeOld();

		if (pMotion != NULL)
		{
			// ���[�V�����X�V����
			pMotion->MotionUpdate(m_apMotion[m_nType], nMotionType, nMotionTypeOld);
		}


		// ���[�V���������擾
		CMotionData::Motion *pMotionData = m_apMotion[m_nType]->GetMotion(nMotionType);

		// �t���[�������B�����ꍇ�I������
		if (pMotion->GetKeyCnt() == pMotionData->nNumKey - 1
			&& pMotionData->bLoop == false
			&& pMotionData->aKey[pMotionData->nNumKey - 1].nFrame == pMotion->GetFrameCounter() + 1)
		{ // ���[�V�����I�����Ƀu�[����false�ɂ���
		  /*m_bBool[MOTION_TYPE_ATTACK] = false;
		  m_bBool[MOTION_TYPE_RAND] = false;*/
		}

		// �t���[���J�E���^�[��������
		pMotion->FrameUpdate(m_apMotion[m_nType], nMotionType);

		D3DXVECTOR3 pos = CMotion::GetPos();
		D3DXVECTOR3 move = CMotion::GetMove();

		// �ʒu�X�V
		pos += move;

		CMotion::SetPos(pos);
		CMotion::SetMove(move);
	}
	else if (pManager == CManager::MODE::MODE_GAME)
	{// �Q�[�����[�h�̎�
	 // �R���g���[���[
		Controller();

		// ���[�V����
		Motion();

		D3DXVECTOR3 rot = CMotion::GetRot();

		rot.x = -D3DX_PI / 2.0f + (rand() % 50) / 1000.0f;
		rot.z = D3DX_PI + (rand() % 50) / 1000.0f;

		CMotion::SetRot(rot);

		CManager::GetInputGamePad()->SetVibration(100, 1, m_nId);

		// �{�^���\��
		ButtonDisp(CMotion::GetPos().y);
	}
	else if (pManager == CManager::MODE::MODE_RESULT)
	{// �^�C�g�����[�h�̎�
	 // ���[�V�������擾
		CMotionMove *pMotion = CMotion::GetMotion();

		// ���[�V������ގ擾
		int nMotionType = CMotion::GetMotionType();
		int nMotionTypeOld = CMotion::GetMotionTypeOld();

		if (pMotion != NULL)
		{
			// ���[�V�����X�V����
			pMotion->MotionUpdate(m_apMotion[m_nType], nMotionType, nMotionTypeOld);
		}


		// ���[�V���������擾
		CMotionData::Motion *pMotionData = m_apMotion[m_nType]->GetMotion(nMotionType);

		// �t���[�������B�����ꍇ�I������
		if (pMotion->GetKeyCnt() == pMotionData->nNumKey - 1
			&& pMotionData->bLoop == false
			&& pMotionData->aKey[pMotionData->nNumKey - 1].nFrame == pMotion->GetFrameCounter() + 1)
		{ // ���[�V�����I�����Ƀu�[����false�ɂ���
		  /*m_bBool[MOTION_TYPE_ATTACK] = false;
		  m_bBool[MOTION_TYPE_RAND] = false;*/
		}

		// �t���[���J�E���^�[��������
		pMotion->FrameUpdate(m_apMotion[m_nType], nMotionType);
	}

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �`�揈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::Draw(void)
{
	// �f�o�C�X�擾����
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();
	D3DXVECTOR3 pos = CMotion::GetPos();
	D3DXVECTOR3 rot = CMotion::GetRot();
	D3DXMATRIX mtx = CMotion::GetMatrix();
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}
	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);
	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	// �p�[�c�̕`��
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		if (CMotion::GetModel(nCntModel) != NULL)
		{
			CMotion::GetModel(nCntModel)->Draw(mtx, CMotion::GetMotion()->GetPos(nCntModel), CMotion::GetMotion()->GetRot(nCntModel),*m_apMotion[m_nType]->GetModel(nCntModel));
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V��������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::Motion(void)
{
	// ���[�V�����^�C�v�X�V
	MotionTypeUpdate();

	// ���[�V�������擾
	CMotionMove *pMotion = CMotion::GetMotion();

	// ���[�V������ގ擾
	int nMotionType = CMotion::GetMotionType();
	int nMotionTypeOld = CMotion::GetMotionTypeOld();

	if (pMotion != NULL)
	{
		// ���[�V�����X�V����
		pMotion->MotionUpdate(m_apMotion[m_nType], nMotionType, nMotionTypeOld);
	}


	// ���[�V���������擾
	CMotionData::Motion *pMotionData = m_apMotion[m_nType]->GetMotion(nMotionType);

	// �t���[�������B�����ꍇ�I������
	if (pMotion->GetKeyCnt() == pMotionData->nNumKey - 1
		&& pMotionData->bLoop == false
		&& pMotionData->aKey[pMotionData->nNumKey - 1].nFrame == pMotion->GetFrameCounter() + 1)
	{ // ���[�V�����I�����Ƀu�[����false�ɂ���
		/*m_bBool[MOTION_TYPE_ATTACK] = false;
		m_bBool[MOTION_TYPE_RAND] = false;*/
	}

	// �t���[���J�E���^�[��������
	pMotion->FrameUpdate(m_apMotion[m_nType], nMotionType);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �v���C���[���[�V�����̎�ނ̐ݒ�
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::MotionTypeUpdate(void)
{
	int nMotionType = 0;		// ���[�V�����^�C�v
								// �ړ��ʎ擾
	D3DXVECTOR3 move = CMotion::GetMove();

	if (m_bBool[BOOL_JUMP] == true)
	{ // �W�����v��Ԃ�
		//nMotionType = MOTION_TYPE_RAND;
	}
	else if (m_bBottonCommand[POSE_BOTTAN_A] == true)
	{ // �S�̎w��A������ꍇ
		nMotionType = m_buttonMotionType[POSE_BOTTAN_A];
	}
	else if (m_bBottonCommand[POSE_BOTTAN_B] == true)
	{ // �S�̎w��B������ꍇ
		nMotionType = m_buttonMotionType[POSE_BOTTAN_B];
	}
	else if (m_bBottonCommand[POSE_BOTTAN_X] == true)
	{ // �S�̎w��X������ꍇ
		nMotionType = m_buttonMotionType[POSE_BOTTAN_X];
	}
	else if (m_bBottonCommand[POSE_BOTTAN_Y] == true)
	{ // �S�̎w��Y������ꍇ
		nMotionType = m_buttonMotionType[POSE_BOTTAN_Y];
	}
	else if (m_bControl == false)
	{ // �S�̎w���͖���������s�̏ꍇ
		nMotionType = MOTION_TYPE_NEUTRAL;
	}
	else
	{
		if (m_bBotton[POSE_BOTTAN_A])
		{
			nMotionType = m_buttonMotionType[POSE_BOTTAN_A];
		}
		else if (m_bBotton[POSE_BOTTAN_B])
		{
			nMotionType = m_buttonMotionType[POSE_BOTTAN_B];
		}
		else if (m_bBotton[POSE_BOTTAN_X])
		{
			nMotionType = m_buttonMotionType[POSE_BOTTAN_X];
		}
		else if (m_bBotton[POSE_BOTTAN_Y])
		{
			nMotionType = m_buttonMotionType[POSE_BOTTAN_Y];
		}
		else
		{
			// �j���[�g��������Ȃ̂�
			nMotionType = MOTION_TYPE_NEUTRAL;
		}
	}
	// ���[�V�����^�C�v�ݒ�
	CMotion::SetMotionType(nMotionType);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���쏈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::Controller(void)
{
	// �J�������擾
	CCamera *pCamera = CManager::GetCameraMode();
	//D3DXVECTOR3 CameraRot = pCamera->GetRot();
	D3DXVECTOR3 CameraRot = pCamera->GetRot(0);
	// �L�[�{�[�h�擾
	CInputKeyboard *pInput = CManager::GetInputKeyboard();
	// �R���g���[���[�擾
	CInputGamePad *pGamePad = CManager::GetInputGamePad();

	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();

	// �ʒu�E�ړ��ʎ擾
	D3DXVECTOR3 pos = CMotion::GetPos();
	D3DXVECTOR3 move = CMotion::GetMove();
	D3DXVECTOR3 rot = CMotion::GetRot();
	float fSpeed = CMotion::GetSpeed();

	float fDiffAngle = 0.0f;		// ����
	float fDestAngle = 0.0f;		// �ړI�̊p�x
	float fMoveAngle = 0.0f;		// ���݂̊p�x

	// ���݂̊p�x����(Y)
	fMoveAngle = rot.y;

	if (m_bControl == true)
	{
		if (CManager::GetInputKeyboard()->GetPress(DIK_A))
		{ // ���ړ�
			if (CManager::GetInputKeyboard()->GetPress(DIK_W))
			{ // ����ړ�
				move.x += sinf(-(D3DX_PI / 4) * 1) * (fSpeed);
				move.z += cosf(-(D3DX_PI / 4) * 1) * (fSpeed);
			}
			else if (CManager::GetInputKeyboard()->GetPress(DIK_S))
			{ // �����ړ�
				move.x += sinf(-(D3DX_PI / 4) * 3) * (fSpeed);
				move.z += cosf(-(D3DX_PI / 4) * 3) * (fSpeed);
			}
			else
			{ // ���ړ�
				move.x += sinf(-(D3DX_PI / 4) * 2) * (fSpeed);
				move.z += cosf(-(D3DX_PI / 4) * 2) * (fSpeed);
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_D))
		{ // �E�ړ�
			if (CManager::GetInputKeyboard()->GetPress(DIK_W))
			{ // ����ړ�
				move.x += sinf((D3DX_PI / 4) * 1) * (fSpeed);
				move.z += cosf((D3DX_PI / 4) * 1) * (fSpeed);
			}
			else if (CManager::GetInputKeyboard()->GetPress(DIK_S))
			{ // �����ړ�
				move.x += sinf((D3DX_PI / 4) * 3) * (fSpeed);
				move.z += cosf((D3DX_PI / 4) * 3) * (fSpeed);
			}
			else
			{ // ���ړ�
				move.x += sinf((D3DX_PI / 4) * 2) * (fSpeed);
				move.z += cosf((D3DX_PI / 4) * 2) * (fSpeed);
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_W))
		{ // �O�ړ�
			move.x += sinf(0.0f) * (fSpeed);
			move.z += cosf(0.0f) * (fSpeed);
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S))
		{ // ��ړ�
			move.x += sinf(D3DX_PI) * (fSpeed);
			move.z += cosf(D3DX_PI) * (fSpeed);
		}


		if (pGamePadStick[m_nId].aGamePad.lX < -GAMEPAD_DEADZONE || pGamePadStick[m_nId].aGamePad.lX > GAMEPAD_DEADZONE || pGamePadStick[m_nId].aGamePad.lY < -GAMEPAD_DEADZONE || pGamePadStick[m_nId].aGamePad.lY> GAMEPAD_DEADZONE)
		{// �Q�[���p�b�h
			float fGamePagStickAngle = atan2f(float(pGamePadStick[m_nId].aGamePad.lX), float(-pGamePadStick[m_nId].aGamePad.lY));
			move.x += sinf(fGamePagStickAngle) * (fSpeed);
			move.z += cosf(fGamePagStickAngle) * (fSpeed);
		}

		// �{�^���𗣂��Ă�����false�ɂ���
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_A) == false)
		{
			m_bBotton[POSE_BOTTAN_A] = false;
		}
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_B) == false)
		{
			m_bBotton[POSE_BOTTAN_B] = false;
		}
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_X) == false)
		{
			m_bBotton[POSE_BOTTAN_X] = false;
		}
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_Y) == false)
		{
			m_bBotton[POSE_BOTTAN_Y] = false;
		}
		if (pGamePad->GetGamePadTrigger(m_nId, BUTTON_Y) || pGamePad->GetGamePadTrigger(m_nId, BUTTON_X) ||
			pGamePad->GetGamePadTrigger(m_nId, BUTTON_B) || pGamePad->GetGamePadTrigger(m_nId, BUTTON_A))
		{
			CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_POSE);
		}
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_A))
		{ // A����
			if (m_bBotton[POSE_BOTTAN_B] == false && m_bBotton[POSE_BOTTAN_X] == false && m_bBotton[POSE_BOTTAN_Y] == false)
			{ // A�ȊO�̓��͂������ꍇ
				m_bBotton[POSE_BOTTAN_A] = true;
			}
		}
		if (pGamePad->GetGamePadPress(m_nId, BUTTON_B))
		{ // B����
			if (m_bBotton[POSE_BOTTAN_A] == false && m_bBotton[POSE_BOTTAN_X] == false && m_bBotton[POSE_BOTTAN_Y] == false)
			{ // B�ȊO�̓��͂������ꍇ
				m_bBotton[POSE_BOTTAN_B] = true;
			}
		}
		if (m_nControlLevel >= 1)
		{ // ����Level��1�ȏ�̏ꍇ
			if (pGamePad->GetGamePadPress(m_nId, BUTTON_X))
			{ // X����
				if (m_bBotton[POSE_BOTTAN_A] == false && m_bBotton[POSE_BOTTAN_B] == false && m_bBotton[POSE_BOTTAN_Y] == false)
				{ // B�ȊO�̓��͂������ꍇ
					m_bBotton[POSE_BOTTAN_X] = true;
				}
			}
		}
		if (m_nControlLevel >= 2)
		{ // ����Level��1�ȏ�̏ꍇ
			if (pGamePad->GetGamePadPress(m_nId, BUTTON_Y))
			{ // Y����
				if (m_bBotton[POSE_BOTTAN_A] == false && m_bBotton[POSE_BOTTAN_B] == false && m_bBotton[POSE_BOTTAN_X] == false)
				{ // B�ȊO�̓��͂������ꍇ
					m_bBotton[POSE_BOTTAN_Y] = true;
				}
			}
		}
	}
	// �ʒu�X�V
	pos += move;
	pos.y -= 5.0f;
	if (CGame::GetStage() == CGame::STAGE_EARTH || CGame::GetStage() == CGame::STAGE_HELL)
	{
		pos.y -= 3.0f;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_O))
	{
		pos.y -= 10;
	}

	// ����
	move.x += float(-move.x) * MOVE_INERTIA;
	move.y += float(-move.y) * MOVE_INERTIA;
	move.z += float(-move.z) * MOVE_INERTIA;


	// �������v�Z
	fDiffAngle = (fDestAngle - fMoveAngle);

	if (fDiffAngle > D3DX_PI)
	{
		fDiffAngle += -D3DX_PI * 2;
	}
	else if (fDiffAngle < -D3DX_PI)
	{
		fDiffAngle += D3DX_PI * 2;
	}

	// ���݂̊p�x�ɍ����̉���������
	fMoveAngle += fDiffAngle * 0.5f;

	if (fMoveAngle > D3DX_PI)
	{
		fMoveAngle += -D3DX_PI * 2;
	}
	else if (fMoveAngle < -D3DX_PI)
	{
		fMoveAngle += D3DX_PI * 2;
	}

	rot.y = fMoveAngle;

	// �v���C���[���m�̉�������
	//PlayerOverLap();
	CScene *m_pSceneTarget = CScene::GetScene(PRIORITY_PLAYER);
	while (m_pSceneTarget != NULL)
	{
		CScene *pSceneNext = m_pSceneTarget->GetNext();
		if (m_pSceneTarget != NULL)
		{
			if (m_pSceneTarget->GetObjType() == OBJTYPE_PLAYER)
			{// �I�u�W�F�N�g
				CPlayer *pPlayer = ((CPlayer*)m_pSceneTarget);
				if (m_nId != pPlayer->GetId())
				{
					D3DXVECTOR3 targetPos = pPlayer->GetPos();

					/*pos.z += 40.0f;
					targetPos.z += 40.0f;*/

					// �G�Ɠ�������̂̋���
					float fPosX = (pos.x - targetPos.x);
					float fPosY = (pos.y - targetPos.y);
					float fPosZ = (pos.z - targetPos.z);
					float fRadiusA = fPosX*fPosX + fPosY*fPosY + fPosZ*fPosZ;
					float fRadiusB = 0;

					fRadiusB = (m_fRadius + m_fRadius)* (pPlayer->GetRadius() + pPlayer->GetRadius());

					if (fRadiusB > fRadiusA&&
						-fRadiusB < fRadiusA)
					{// �l�Ɛl�̂����蔻��
						float fAngle2 = atan2f(pos.x - targetPos.x, pos.z - targetPos.z);

						pos.x = targetPos.x + sinf(fAngle2) * (m_fRadius + pPlayer->GetRadius());
						pos.z = targetPos.z + cosf(fAngle2) * (m_fRadius + pPlayer->GetRadius());

						//pos.z -= 40.0f;

						move.x *= 1.2f;
						move.z *= 1.2f;

						pPlayer->SetMove(move);

						move *= -1.0f;

						// �ʒu�ݒ�
						//CMotion::SetPos(pos);
					}
				}
			}
		}
		m_pSceneTarget = pSceneNext;
	}

	//
	int motion = GetMotionType();
	int nOk = 0;
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// �D�揇�ʂ̍ő吔����
		CScene *m_pSceneTarget = CScene::GetScene(nCntP);
		while (m_pSceneTarget != NULL)
		{
			CScene *pSceneNext = m_pSceneTarget->GetNext();
			if (m_pSceneTarget != NULL)
			{
				if (m_pSceneTarget->GetObjType() == OBJTYPE_PANEL)
				{// �I�u�W�F�N�g

					if (nOk != 1)
					{
						CPanelHole *pObject = ((CPanelHole*)m_pSceneTarget);
						int nOkClone = pObject->Collision(pos + D3DXVECTOR3(0.0f, 0.0f, 40), GetPosOld(), m_fRadius, motion);
						if (nOkClone != 0)
						{
							nOk = nOkClone;
						}
						if (nOk == 1)
						{
							CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_SUCCES);

						}
					}
				}
				else if (m_pSceneTarget->GetObjType() == OBJTYPE_MESHWALL)
				{
					CMeshWall *pObject = ((CMeshWall*)m_pSceneTarget);
					if (pObject->Collision(&pos, GetPosOld(), m_fRadius, m_fHeifht))
					{

					}
				}
			}
			m_pSceneTarget = pSceneNext;
		}
	}

	if (nOk == 2)
	{
		if (m_nNumAll <= 1)
		{
			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				CResult::SetWin(CGame::m_WinId);
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
	}

	if (nOk == 2)
	{
		CGame::m_WinId = (m_nId + 1) % 2;
		CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_FAILED);

		Uninit();
		if (m_nNumAll <= 0)
		{
			CResult::WinReset();

			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);

			}
		}
		return;
	}

	// �e�픽�f
	CMotion::SetPos(pos);
	CMotion::SetMove(move);
	CMotion::SetRot(rot);
	CMotion::SetSpeed(MOVE_SPEED);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ����󋵎擾
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
bool CPlayer::GetController(CONTROLLER Controller)
{
	bool bButton = false;
	CInputKeyboard *pInput = CManager::GetInputKeyboard();

	switch (Controller)
	{
	case CONTROLLER_LEFT_MOVE:	// ���ړ�
		if (pInput->GetPress(DIK_A) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_RIGHT_MOVE:	// �E�ړ�
		if (pInput->GetPress(DIK_D) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_UP_MOVE:	// ��ړ�
		if (pInput->GetPress(DIK_W) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_LEFT_UP_MOVE:	// ����ړ�
		if (pInput->GetPress(DIK_W) == true && pInput->GetPress(DIK_A) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_RIGHT_UP_MOVE: // �E��ړ�
		if (pInput->GetPress(DIK_W) == true && pInput->GetPress(DIK_D) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_LEFT_DOWN_MOVE:	// �����ړ�
		if (pInput->GetPress(DIK_S) == true && pInput->GetPress(DIK_A) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_RIGHT_DOWN_MOVE: // �E���ړ�
		if (pInput->GetPress(DIK_S) == true && pInput->GetPress(DIK_D) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_DOWN_MOVE:	// ���ړ�
		if (pInput->GetPress(DIK_S) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_ATTACK:	// �U��
		if (pInput->GetPress(DIK_RETURN) == true)
		{
			bButton = true;
		}
		break;
	case CONTROLLER_JUMP:	// �W�����v
		if (pInput->GetPress(DIK_SPACE) == true)
		{
			bButton = true;
		}
		break;
	}
	return bButton;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �|�[�Y�̃����_���ݒ�
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::RandomPose(void)
{
	// �_�u��Check
	int nCheckMotion[POSE_BUTTON_MAX];
	for (int nCnt = 0; nCnt < POSE_BUTTON_MAX; nCnt++)
	{
		nCheckMotion[nCnt] = -1;
	}

	int nMotion = -1;

	for (int nCnt = 0; nCnt < POSE_BUTTON_MAX; nCnt++)
	{
		bool bCheck = true;

		while (bCheck)
		{
			bCheck = false;

			nMotion = rand() % POSE_BUTTON_MAX + 1;
			for (int nCntCheck = 0; nCntCheck < POSE_BUTTON_MAX; nCntCheck++)
			{
				if (nMotion == nCheckMotion[nCntCheck])
				{// ���܂ł̃��[�V�����Ɠ����ꍇ
					bCheck = true;
				}
			}
		}

		// ���Ԃ肿�������̃��[�v�𔲂����ꍇ���
		m_buttonMotionType[nCnt] = (MOTION_TYPE)nMotion;
		nCheckMotion[nCnt] = nMotion;
	}

	int nData = 0;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �{�^���\��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::ButtonDisp(const float fHeight)
{
	float fStartPos = 12000.0f;
	float fEarthStartPos = 2000.0f;
	float fHellStartPos = -7000.0f;

	CDebugProc::print("�v���C���[����: %f\n",fHeight);

	if (m_buttonMotionDisp != NULL)
	{
		D3DXVECTOR3 pos = m_buttonMotionDisp->GetPos();
		pos.y = fHeight - 200.0f;
		m_buttonMotionDisp->SetPos(pos);
		for (int nCnt = 0; nCnt < POSE_BUTTON_MAX; nCnt++)
		{
			m_bBottonCommand[nCnt] = false;
		}
	}

	if (fHeight < fHellStartPos - 800.0f)
	{//Y������
		if (m_buttonMotionDisp != NULL)
		{
			m_buttonMotionDisp->Uninit();
			m_buttonMotionDisp = NULL;
		}
		// ����\�ɂ���
		m_bControl = true;
	}
	else if (fHeight < fHellStartPos - 500.0f)
	{ // Y���[�V�����̎w��
		m_bBottonCommand[POSE_BOTTAN_Y] = true;
	}
	else if (fHeight < fHellStartPos)
	{ // Y����
		if (m_buttonMotionDisp == NULL)
		{
			m_buttonMotionDisp = CButtonMotion::Create(D3DXVECTOR3(0.0f, fHeight - 200.0f, 0.0f), 400.0f, 0.0f, 400.0f, CButtonMotion::TEX_TYPE_Y);
		}
		// ����s�ɂ���
		m_bControl = false;
		// ����Level��1��
		m_nControlLevel = 2;
	}
	else if (fHeight < fEarthStartPos - 800.0f)
	{//X������
		if (m_buttonMotionDisp != NULL)
		{
			m_buttonMotionDisp->Uninit();
			m_buttonMotionDisp = NULL;
		}
		// ����\�ɂ���
		m_bControl = true;
	}
	else if (fHeight < fEarthStartPos - 500.0f)
	{ // X���[�V�����̎w��
		m_bBottonCommand[POSE_BOTTAN_X] = true;
	}
	else if (fHeight < fEarthStartPos)
	{ // X����
		if (m_buttonMotionDisp == NULL)
		{
			m_buttonMotionDisp = CButtonMotion::Create(D3DXVECTOR3(0.0f, fHeight - 200.0f, 0.0f), 400.0f, 0.0f, 400.0f, CButtonMotion::TEX_TYPE_X);
		}
		// ����s�ɂ���
		m_bControl = false;

		// ����Level��1��
		m_nControlLevel = 1;
	}
	else if (fHeight < fStartPos - 1800.0f)
	{//B������
		if (m_buttonMotionDisp != NULL)
		{
			m_buttonMotionDisp->Uninit();
			m_buttonMotionDisp = NULL;
		}
		// ����\�ɂ���
		m_bControl = true;
	}
	else if (fHeight < fStartPos - 1500.0f)
	{ // B���[�V�����̎w��
		m_bBottonCommand[POSE_BOTTAN_B] = true;
	}
	else if (fHeight < fStartPos - 1000.0f)
	{ // B����
		if (m_buttonMotionDisp == NULL)
		{
			m_buttonMotionDisp = CButtonMotion::Create(D3DXVECTOR3(0.0f, fHeight - 200.0f, 0.0f), 400.0f, 0.0f, 400.0f, CButtonMotion::TEX_TYPE_B);
		}
		// ����s�ɂ���
		m_bControl = false;
	}
	else if (fHeight < fStartPos - 800.0f)
	{//A������
		if (m_buttonMotionDisp != NULL)
		{
			m_buttonMotionDisp->Uninit();
			m_buttonMotionDisp = NULL;
		}
	}
	else if (fHeight < fStartPos - 500.0f)
	{ // A���[�V�����̎w��
		m_bBottonCommand[POSE_BOTTAN_A] = true;
	}
	else if (fHeight < fStartPos)
	{ // A����
		if (m_buttonMotionDisp == NULL)
		{
			m_buttonMotionDisp = CButtonMotion::Create(D3DXVECTOR3(0.0f, fHeight - 200.0f, 0.0f), 400.0f, 0.0f, 400.0f, CButtonMotion::TEX_TYPE_A);
		}
		// ����s�ɂ���
		m_bControl = false;

		// ����Level��0��
		m_nControlLevel = 0;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �v���C���[�̉�������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::PlayerOverLap(void)
{
	CScene *m_pSceneTarget = CScene::GetScene(PRIORITY_PLAYER);
	while (m_pSceneTarget != NULL)
	{
		CScene *pSceneNext = m_pSceneTarget->GetNext();
		if (m_pSceneTarget != NULL)
		{
			if (m_pSceneTarget->GetObjType() == OBJTYPE_PLAYER)
			{// �I�u�W�F�N�g
				CPlayer *pPlayer = ((CPlayer*)m_pSceneTarget);
				if (m_nId != pPlayer->GetId())
				{
					D3DXVECTOR3 pos = CMotion::GetPos();
					D3DXVECTOR3 targetPos = pPlayer->GetPos();

					pos.z += 40.0f;
					targetPos.z += 40.0f;

					// �G�Ɠ�������̂̋���
					float fPosX = (pos.x - targetPos.x);
					float fPosY = (pos.y - targetPos.y);
					float fPosZ = (pos.z - targetPos.z);
					float fRadiusA = fPosX*fPosX + fPosY*fPosY + fPosZ*fPosZ;
					float fRadiusB = 0;

					fRadiusB = (m_fRadius + m_fRadius)* (pPlayer->GetRadius() + pPlayer->GetRadius());

					if (fRadiusB > fRadiusA&&
						-fRadiusB < fRadiusA)
					{// �l�Ɛl�̂����蔻��
						float fAngle2 = atan2f(pos.x - targetPos.x, pos.z - targetPos.z);

						pos.x = targetPos.x + sinf(fAngle2) * (m_fRadius + pPlayer->GetRadius());
						pos.z = targetPos.z + cosf(fAngle2) * (m_fRadius + pPlayer->GetRadius());

						pos.z -= 40.0f;

						// �ʒu�ݒ�
						CMotion::SetPos(pos);
					}
				}
			}
		}
		m_pSceneTarget = pSceneNext;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V����txt�f�[�^�̓ǂݍ���
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CPlayer::Load(void)
{
	for (int nCntType = 0; nCntType < MOTION_DATA_TYPE_MAX; nCntType++)
	{
		m_apMotion[nCntType] = CMotionData::Create();

		if (m_apMotion[nCntType] != NULL)
		{
			m_apMotion[nCntType]->Load(m_apMotionInfo[nCntType]);
		}
	}
	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V����txt�f�[�^�̊J������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CPlayer::UnLoad(void)
{
	for (int nCntType = 0; nCntType < MOTION_DATA_TYPE_MAX; nCntType++)
	{
		m_apMotion[nCntType]->UnLoad();
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ��������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CPlayer *CPlayer::Create(int nId)
{
	CPlayer *pPlayer = NULL;

	pPlayer = new CPlayer;	// CPlayer�^�̃C���X�^���X����
	if (pPlayer != NULL)
	{
		pPlayer->m_nId = nId;
		// ����������
		pPlayer->Init();
	}
	else
	{ // �������m�ێ��s
		MessageBox(NULL, "�v���C���[�̃������m�ۂɎ��s���܂���", "�G���[", (MB_OK));
	}
	return pPlayer;
}


void CPlayer::SetMotion(int nId)
{
	m_nType = (MOTION_DATA_TYPE)nId;
	BindMotion(m_apMotion[nId]);
}