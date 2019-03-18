//===========================================================================//
//																			 //
//						�@�v�Z���� [calculation.cpp]						 //
//						�@�@Author :Yuki Tanaka							     //
//																			 //
//===========================================================================//
//***************************************************************************//
//							�C���N���[�h�t�@�C��							 //
//***************************************************************************//
#include "main.h"
#include "calculation.h"
#include "debugProc.h"
//***************************************************************************//
//								�}�N����`									 //
//***************************************************************************//

//***************************************************************************//
//						��{�N���X[ CCalculation ]							 //
//***************************************************************************//
//===========================================================================//
//					�@�@	�@�@�R���X�g���N�^								 //
//===========================================================================//
CCalculation::CCalculation()
{
}
//===========================================================================//
//					�@�@		�f�X�X�g���N�^								 //
//===========================================================================//
CCalculation::~CCalculation()
{

}
//===========================================================================//
//					�@�@	�����_���v�Z( int�^ )							 //
//					�@�@	 Author :Yuki Tanaka							 //
//===========================================================================//
int CCalculation::RandInt(const int nUpper, const int nLower)
{
	// �ꎞ�I�ɒl��ۊǂ���ϐ�
	int nRand = 0;
	// �����_���l���Z�o
	nRand = rand() % nUpper + nLower;
	// �v�Z���ʂ�Ԃ�
	return nRand;
}
//===========================================================================//
//					�@�@	�����_���v�Z( float�^ )							 //
//					�@�@	 Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::RandAngle(const float fStart, const float fEnd)
{
	// �ꎞ�I�ɒl��ۊǂ���ϐ�
	float fRand = 0;
	// �v�Z�p�\�ɂ��邽�߈ꎞ�I��int�^�ɏC��
	int nEnd = int(fEnd * 100 * 2);
	// �����_���l���Z�o
	if (nEnd != 0)
	{
		fRand = float(rand() % nEnd + 1) / 100;
	}
	else
	{
		fRand = float(rand() % 314 + 1) / 100;
	}
	// �~�����ɏC��
	fRand = fRand - fStart;
	// �v�Z���ʂ�Ԃ�
	return fRand;
}
//===========================================================================//
//					�@�@	 float�������_���ŎZ�o							 //
//					�@�@	 Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::Randfloat(const float fUpper, const float fLower)
{
	// �ꎞ�I�ɒl��ۊǂ���ϐ�
	float ffloat = 0.0f;
	//
	int nUpper = 0;
	int nLower = 0;
	//nUpper = int(fUpper * 100.0f);
	//nLower = int(fLower * 100.0f);
	// �����_���l���Z�o
	ffloat = float(rand() % int(fUpper * 100.0f) + int(fLower * 100.0f));
	//
	ffloat = ffloat / 100.0f;
	// �v�Z���ʂ�Ԃ�
	return ffloat;
}
//===========================================================================//
//					�@�@		�������]( int�^ )							 //
//					�@�@	   Author :Yuki Tanaka							 //
//===========================================================================//
int CCalculation::SignIntReverse(const int nBase)
{
	// �����𔽓]����
	return nBase * -1;
}
//===========================================================================//
//					�@�@	�@  �������]( float�^ )							 //
//					�@�@	    Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::SignFloatReverse(const float fBase)
{
	// �����𔽓]����
	return fBase * -1;
}
//===========================================================================//
//					�@�@�p�^�[���������Z( true:���Z , false:���Z )			 //
//					�@�@	    Author :Yuki Tanaka							 //
//===========================================================================//
int CCalculation::PatternAdd(const int nNowPattern, const int nPatternMax,const bool bDirect)
{
	// �ꎞ�I�Ƀp�^�[������ۊǂ���ϐ�
	int nPattern = nNowPattern;
	// �p�^�[���������Z
	switch (bDirect)
	{
	case true:
		nPattern = (nPattern + 1) % nPatternMax;
		break;
	case false:
		nPattern = (nPattern + nPatternMax - 1) % nPatternMax;
		break;
	}
	// �v�Z���ʂ�Ԃ�
	return nPattern;
}
//===========================================================================//
//					�@�@			�ϓ��z�u�v�Z							 //
//					�@�@	    Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::Equality(const float pos,const float fBlankSize,const int nCnt)
{
	// �ꎞ�I�ɕۊǂ���ϐ�
	float fEquality = 0.0f;
	// �ϓ��v�Z
	fEquality = pos - (nCnt * fBlankSize);
	// �v�Z���ʂ�Ԃ�
	return fEquality;
}
//===========================================================================//
//					�@�@		 �Q�_�Ԃ̋����v�Z							 //
//					�@�@	    Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::PointLength(D3DXVECTOR3 posA, D3DXVECTOR3 posB)
{
	// �ꎞ�I�ɋ�����ۊǂ���ϐ�
	float fLength = 0.0f;
	// �������Z�o
	fLength = (float)pow((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y), 0.5f);
	// �v�Z���ʂ�Ԃ�
	return fLength;
}
//===========================================================================//
//					�@�@		  �Ǐ]����( 2D )							 //
//					�@�@		Author :Yuki Tanaka							 //
//===========================================================================//
void CCalculation::Following2D(D3DXVECTOR3 *move,const D3DXVECTOR3 targetPos, const D3DXVECTOR3 mainPos, const float fSpeed)
{
	// �p�x���Z�o
	float fAngle = atan2f(targetPos.x - mainPos.x, targetPos.y - mainPos.y);
	// �ړ��ʂ��v�Z
	*move += D3DXVECTOR3(sinf(fAngle) * fSpeed, cosf(fAngle) * fSpeed,0.0f);
}
//===========================================================================//
//					�@�@		  �Ǐ]����( 3D )							 //
//					�@�@		Author :Yuki Tanaka							 //
//===========================================================================//
void CCalculation::Following3D(const D3DXVECTOR3 pos, D3DXVECTOR3 *move, D3DXVECTOR3 *rot, const D3DXVECTOR3 targetPos, const float fSpeed)
{
	// �ߋ��̉�]����ێ�
	float fRotOld = rot->y;
	// �p�x���Z�o
	float fAngle = atan2f(targetPos.x - pos.x, targetPos.z - pos.z);
	// �ړ��ʂ��v�Z
	*move += D3DXVECTOR3(sinf(fAngle) * fSpeed, 0.0f, cosf(fAngle) * fSpeed);
	// ��]�ʂ�ݒ�
	rot->y = CCalculation::PiCheck(D3DX_PI + fAngle);
	// ��]����
	rot->y = CCalculation::RotUpdate(rot->y, fRotOld);
}

//===========================================================================//
//					�@�@			 ��]����								 //
//					�@�@		Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::RotUpdate(float rot, float fRotOld)
{
	// ����
	float fDiff = 0.0f;
	// �������v�Z
	fDiff = (rot - fRotOld);
	// 3.14�𒴂����ꍇ�̏���
	fDiff = CCalculation::PiCheck(fDiff);
	// ���݂̊p�x�ɍ����̉���������
	rot += fDiff * 0.5f;
	// 3.14�𒴂����ꍇ�̏���
	rot = CCalculation::PiCheck(rot);
	// �v�Z������]����Ԃ�
	return rot;
}
//===========================================================================//
//					�@�@			�͈͔���								 //
//					�@�@		Author :Yuki Tanaka							 //
//===========================================================================//
bool CCalculation::Range(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld, D3DXVECTOR3 Size, D3DXVECTOR3 TargetPos, D3DXVECTOR3 TargetSize)
{
	// �q�b�g��Ԃ�ێ�����ϐ�
	bool bRange = false;
	// ���g�ƑΏۂ��͈͓��ɓ����Ă��邩�̃`�F�b�N
	if (Pos.x + Size.x > TargetPos.x - TargetSize.x &&
		Pos.x - Size.x < TargetPos.x + TargetSize.x &&
		Pos.z + Size.z > TargetPos.z - TargetSize.z &&
		Pos.z - Size.z < TargetPos.z + TargetSize.z)
	{
		// �q�b�g���Ă����Ԃɂ���
		bRange = true;
	}
	// �q�b�g��Ԃ�Ԃ�
	return bRange;
}
//===========================================================================//
//					�@�@			�͈͔���								 //
//					�@�@		Author :Ebina RIku							 //
//===========================================================================//
bool CCalculation::Range2D(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld, D3DXVECTOR3 Size, D3DXVECTOR3 TargetPos, D3DXVECTOR3 TargetSize)
{
	// �q�b�g��Ԃ�ێ�����ϐ�
	bool bRange = false;
	// ���g�ƑΏۂ��͈͓��ɓ����Ă��邩�̃`�F�b�N
	if (Pos.x + Size.x > TargetPos.x - TargetSize.x &&
		Pos.x - Size.x < TargetPos.x + TargetSize.x &&
		Pos.y + Size.y > TargetPos.y - TargetSize.y &&
		Pos.y - Size.y < TargetPos.y + TargetSize.y)
	{
		// �q�b�g���Ă����Ԃɂ���
		bRange = true;
	}
	// �q�b�g��Ԃ�Ԃ�
	return bRange;
}
//===========================================================================//
//					�@�@		3.14���C�����鏈��							 //
//					�@�@		Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::PiCheck(const float fPi)
{
	// �C�������l��Ԃ��p�̕ϐ�
	float fpi = fPi;
	// �C������
	if (fpi > D3DX_PI)
	{
		fpi += -D3DX_PI * 2;
	}
	else if (fpi < -D3DX_PI)
	{
		fpi += D3DX_PI * 2;
	}
	return fpi;
}

//===========================================================================//
//					�@�@		3.14���C�����鏈��							 //
//					�@�@		Author :�� �a��								 //
//===========================================================================//
D3DXVECTOR3 CCalculation::PiCheck(const D3DXVECTOR3 Pi)
{
	// �C�������l��Ԃ��p�̕ϐ�
	D3DXVECTOR3 pi = Pi;
	// �C������
	if (pi.x > D3DX_PI)
	{
		pi.x += -D3DX_PI * 2;
	}
	else if (pi.x < -D3DX_PI)
	{
		pi.x += D3DX_PI * 2;
	}
	if (pi.y > D3DX_PI)
	{
		pi.y += -D3DX_PI * 2;
	}
	else if (pi.y < -D3DX_PI)
	{
		pi.y += D3DX_PI * 2;
	}
	if (pi.z > D3DX_PI)
	{
		pi.z += -D3DX_PI * 2;
	}
	else if (pi.z < -D3DX_PI)
	{
		pi.z += D3DX_PI * 2;
	}

	return pi;
}

//===========================================================================//
//					�@�@	���𒴂�������TRUE��Ԃ�����					 //
//					�@�@		Author :Yuki Tanaka							 //
//===========================================================================//
bool CCalculation::LineOver(const D3DXVECTOR3 pos, const float linepos, CCalculation::AXIS_TYPE type)
{
	// ���𒴂��Ă��邩��Ԃ��ϐ�
	bool bOver = false;
	switch (type)
	{
	case CCalculation::AXIS_TYPE::AXIS_X_RIGHT_OVER:
		if (pos.x >= linepos)
		{/* { �Ώۂ��w���́{�����𒴂��Ă��邩 } */
			bOver = true;
		}
		break;
	case CCalculation::AXIS_TYPE::AXIS_X_LEFT_OVER:
		if (pos.x <= linepos)
		{/* { �Ώۂ��w���́|�����𒴂��Ă��邩 } */
			bOver = true;
		}
		break;
	case CCalculation::AXIS_TYPE::AXIS_Y_UP_OVER:
		if (pos.y >= linepos)
		{/* { �Ώۂ��x���́{�����𒴂��Ă��邩 } */
			bOver = true;
		}
		break;
	case CCalculation::AXIS_TYPE::AXIS_Y_DOWN_OVER:
		if (pos.y <= linepos)
		{/* { �Ώۂ��x���́|�����𒴂��Ă��邩 } */
			bOver = true;
		}
		break;
	case CCalculation::AXIS_TYPE::AXIS_Z_FRONT_OVER:
		if (pos.z >= linepos)
		{/* { �Ώۂ��y���́{�����𒴂��Ă��邩 } */
			bOver = true;
		}
		break;
	case CCalculation::AXIS_TYPE::AXIS_Z_BACK_OVER :
		if (pos.z <= linepos)
		{/* { �Ώۂ��y���́|�����𒴂��Ă��邩 } */
			bOver = true;
		}
		break;
	default:
		break;
	}
	return bOver;
}
//===========================================================================//
//					�@�@			�ړ��v�Z								 //
//					�@�@		Author :Yuki Tanaka							 //
//===========================================================================//
D3DXVECTOR3 CCalculation::Moving(D3DXVECTOR3 *move, const D3DXVECTOR3 Angle, const D3DXVECTOR3 Speed)
{
	// �ړ����Z
	move->x += sinf(float(D3DX_PI * Angle.x)) * Speed.x;
	move->y += cosf(float(D3DX_PI * Angle.y)) * Speed.y;
	move->z += cosf(float(D3DX_PI * Angle.z)) * Speed.z;
	// �v�Z���ʂ�Ԃ�
	return *move;
}