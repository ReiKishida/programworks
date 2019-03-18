//=============================================================================
//
// 計算処理 [calculation.h]
// Author : yuki tanaka
//
//=============================================================================
#ifndef _CALCULATION_H_
#define _CALCULATION_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCalculation
{
public:
	//************************************//
	//		　	   列挙体定義			  //
	//************************************//
	/* [ 軸の種類 ] */
	typedef enum
	{
		AXIS_X_RIGHT_OVER = 0,		// Ｘ軸を＋方向
		AXIS_X_LEFT_OVER,			// Ｘ軸を－方向
		AXIS_Y_UP_OVER,				// Ｙ軸を＋方向
		AXIS_Y_DOWN_OVER,			// Ｙ軸を－方向
		AXIS_Z_FRONT_OVER,			// Ｚ軸を＋方向
		AXIS_Z_BACK_OVER,			// Ｚ軸を－方向
		AXIS_MAX					// 最大数
	}AXIS_TYPE;
	CCalculation();
	~CCalculation();
	static int RandInt(const int nUpper, const int nLower);										// int型のランダム値を算出 ( 上限値 , 下限値 )
	static float RandAngle(const float fStart, const float fEnd);								// 角度のランダム値を算出 ( 左弦 , 右弦 )
	static float Randfloat(const float fUpper, const float fLower);
	static int SignIntReverse(const int nBase);													// 符号反転 ( int型 )
	static float SignFloatReverse(const float fBase);											// 符号反転 ( float型 )
	static int PatternAdd(const int nNowPattern, const int nPatternMax, const bool bDirect);	// パターン数を加算( 現在のパターン , パターン数の最大数 , 加算か減算なのかどうか )
	static float Equality(const float pos, const float fBlankSize, const int nCnt);				// 均等計算 ( 現在位置 , 位置と位置の幅 , 現在計算中の番号 )
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