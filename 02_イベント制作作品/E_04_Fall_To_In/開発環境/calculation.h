//=============================================================================
//
// �v�Z���� [calculation.h]
// Author : yuki tanaka
//
//=============================================================================
#ifndef _CALCULATION_H_
#define _CALCULATION_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCalculation
{
public:
	//************************************//
	//		�@	   �񋓑̒�`			  //
	//************************************//
	/* [ ���̎�� ] */
	typedef enum
	{
		AXIS_X_RIGHT_OVER = 0,		// �w�����{����
		AXIS_X_LEFT_OVER,			// �w�����|����
		AXIS_Y_UP_OVER,				// �x�����{����
		AXIS_Y_DOWN_OVER,			// �x�����|����
		AXIS_Z_FRONT_OVER,			// �y�����{����
		AXIS_Z_BACK_OVER,			// �y�����|����
		AXIS_MAX					// �ő吔
	}AXIS_TYPE;
	CCalculation();
	~CCalculation();
	static int RandInt(const int nUpper, const int nLower);										// int�^�̃����_���l���Z�o ( ����l , �����l )
	static float RandAngle(const float fStart, const float fEnd);								// �p�x�̃����_���l���Z�o ( ���� , �E�� )
	static float Randfloat(const float fUpper, const float fLower);
	static int SignIntReverse(const int nBase);													// �������] ( int�^ )
	static float SignFloatReverse(const float fBase);											// �������] ( float�^ )
	static int PatternAdd(const int nNowPattern, const int nPatternMax, const bool bDirect);	// �p�^�[���������Z( ���݂̃p�^�[�� , �p�^�[�����̍ő吔 , ���Z�����Z�Ȃ̂��ǂ��� )
	static float Equality(const float pos, const float fBlankSize, const int nCnt);				// �ϓ��v�Z ( ���݈ʒu , �ʒu�ƈʒu�̕� , ���݌v�Z���̔ԍ� )
	static float PointLength(const D3DXVECTOR3 posA, const D3DXVECTOR3 posB);
	static void Following2D(D3DXVECTOR3 *move, const D3DXVECTOR3 targetPos, const D3DXVECTOR3 mainPos, const float fSpeed);
	static void Following3D(const D3DXVECTOR3 pos, D3DXVECTOR3 *move, D3DXVECTOR3 *rot, const D3DXVECTOR3 targetPos,const float fSpeed);
	static float RotUpdate(float rot, float fDest);
	static D3DXVECTOR3 Moving(D3DXVECTOR3 *move, const D3DXVECTOR3 Angle, const D3DXVECTOR3 Speed);
	static bool Range(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld, D3DXVECTOR3 Size, D3DXVECTOR3 TargetPos, D3DXVECTOR3 TargetSize);
	static bool Range2D(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld, D3DXVECTOR3 Size, D3DXVECTOR3 TargetPos, D3DXVECTOR3 TargetSize);
	static bool LineOver(const D3DXVECTOR3 pos, const float linepos, CCalculation::AXIS_TYPE type);
	static float PiCheck(const float fPi);
	static D3DXVECTOR3 PiCheck(const D3DXVECTOR3 Pi);
private:
};

#endif