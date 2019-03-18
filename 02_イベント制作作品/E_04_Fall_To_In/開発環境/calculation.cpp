//===========================================================================//
//																			 //
//						　計算処理 [calculation.cpp]						 //
//						　　Author :Yuki Tanaka							     //
//																			 //
//===========================================================================//
//***************************************************************************//
//							インクルードファイル							 //
//***************************************************************************//
#include "main.h"
#include "calculation.h"
#include "debugProc.h"
//***************************************************************************//
//								マクロ定義									 //
//***************************************************************************//

//***************************************************************************//
//						基本クラス[ CCalculation ]							 //
//***************************************************************************//
//===========================================================================//
//					　　	　　コンストラクタ								 //
//===========================================================================//
CCalculation::CCalculation()
{
}
//===========================================================================//
//					　　		デスストラクタ								 //
//===========================================================================//
CCalculation::~CCalculation()
{

}
//===========================================================================//
//					　　	ランダム計算( int型 )							 //
//					　　	 Author :Yuki Tanaka							 //
//===========================================================================//
int CCalculation::RandInt(const int nUpper, const int nLower)
{
	// 一時的に値を保管する変数
	int nRand = 0;
	// ランダム値を算出
	nRand = rand() % nUpper + nLower;
	// 計算結果を返す
	return nRand;
}
//===========================================================================//
//					　　	ランダム計算( float型 )							 //
//					　　	 Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::RandAngle(const float fStart, const float fEnd)
{
	// 一時的に値を保管する変数
	float fRand = 0;
	// 計算用可能にするため一時的にint型に修正
	int nEnd = int(fEnd * 100 * 2);
	// ランダム値を算出
	if (nEnd != 0)
	{
		fRand = float(rand() % nEnd + 1) / 100;
	}
	else
	{
		fRand = float(rand() % 314 + 1) / 100;
	}
	// 円周率に修正
	fRand = fRand - fStart;
	// 計算結果を返す
	return fRand;
}
//===========================================================================//
//					　　	 floatをランダムで算出							 //
//					　　	 Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::Randfloat(const float fUpper, const float fLower)
{
	// 一時的に値を保管する変数
	float ffloat = 0.0f;
	//
	int nUpper = 0;
	int nLower = 0;
	//nUpper = int(fUpper * 100.0f);
	//nLower = int(fLower * 100.0f);
	// ランダム値を算出
	ffloat = float(rand() % int(fUpper * 100.0f) + int(fLower * 100.0f));
	//
	ffloat = ffloat / 100.0f;
	// 計算結果を返す
	return ffloat;
}
//===========================================================================//
//					　　		符号反転( int型 )							 //
//					　　	   Author :Yuki Tanaka							 //
//===========================================================================//
int CCalculation::SignIntReverse(const int nBase)
{
	// 符号を反転する
	return nBase * -1;
}
//===========================================================================//
//					　　	　  符号反転( float型 )							 //
//					　　	    Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::SignFloatReverse(const float fBase)
{
	// 符号を反転する
	return fBase * -1;
}
//===========================================================================//
//					　　パターン数を加算( true:加算 , false:減算 )			 //
//					　　	    Author :Yuki Tanaka							 //
//===========================================================================//
int CCalculation::PatternAdd(const int nNowPattern, const int nPatternMax,const bool bDirect)
{
	// 一時的にパターン数を保管する変数
	int nPattern = nNowPattern;
	// パターン数を加算
	switch (bDirect)
	{
	case true:
		nPattern = (nPattern + 1) % nPatternMax;
		break;
	case false:
		nPattern = (nPattern + nPatternMax - 1) % nPatternMax;
		break;
	}
	// 計算結果を返す
	return nPattern;
}
//===========================================================================//
//					　　			均等配置計算							 //
//					　　	    Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::Equality(const float pos,const float fBlankSize,const int nCnt)
{
	// 一時的に保管する変数
	float fEquality = 0.0f;
	// 均等計算
	fEquality = pos - (nCnt * fBlankSize);
	// 計算結果を返す
	return fEquality;
}
//===========================================================================//
//					　　		 ２点間の距離計算							 //
//					　　	    Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::PointLength(D3DXVECTOR3 posA, D3DXVECTOR3 posB)
{
	// 一時的に距離を保管する変数
	float fLength = 0.0f;
	// 距離を算出
	fLength = (float)pow((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y), 0.5f);
	// 計算結果を返す
	return fLength;
}
//===========================================================================//
//					　　		  追従処理( 2D )							 //
//					　　		Author :Yuki Tanaka							 //
//===========================================================================//
void CCalculation::Following2D(D3DXVECTOR3 *move,const D3DXVECTOR3 targetPos, const D3DXVECTOR3 mainPos, const float fSpeed)
{
	// 角度を算出
	float fAngle = atan2f(targetPos.x - mainPos.x, targetPos.y - mainPos.y);
	// 移動量を計算
	*move += D3DXVECTOR3(sinf(fAngle) * fSpeed, cosf(fAngle) * fSpeed,0.0f);
}
//===========================================================================//
//					　　		  追従処理( 3D )							 //
//					　　		Author :Yuki Tanaka							 //
//===========================================================================//
void CCalculation::Following3D(const D3DXVECTOR3 pos, D3DXVECTOR3 *move, D3DXVECTOR3 *rot, const D3DXVECTOR3 targetPos, const float fSpeed)
{
	// 過去の回転情報を保持
	float fRotOld = rot->y;
	// 角度を算出
	float fAngle = atan2f(targetPos.x - pos.x, targetPos.z - pos.z);
	// 移動量を計算
	*move += D3DXVECTOR3(sinf(fAngle) * fSpeed, 0.0f, cosf(fAngle) * fSpeed);
	// 回転量を設定
	rot->y = CCalculation::PiCheck(D3DX_PI + fAngle);
	// 回転処理
	rot->y = CCalculation::RotUpdate(rot->y, fRotOld);
}

//===========================================================================//
//					　　			 回転処理								 //
//					　　		Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::RotUpdate(float rot, float fRotOld)
{
	// 差分
	float fDiff = 0.0f;
	// 差分を計算
	fDiff = (rot - fRotOld);
	// 3.14を超えた場合の処理
	fDiff = CCalculation::PiCheck(fDiff);
	// 現在の角度に差分の何割かを代入
	rot += fDiff * 0.5f;
	// 3.14を超えた場合の処理
	rot = CCalculation::PiCheck(rot);
	// 計算した回転情報を返す
	return rot;
}
//===========================================================================//
//					　　			範囲判定								 //
//					　　		Author :Yuki Tanaka							 //
//===========================================================================//
bool CCalculation::Range(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld, D3DXVECTOR3 Size, D3DXVECTOR3 TargetPos, D3DXVECTOR3 TargetSize)
{
	// ヒット状態を保持する変数
	bool bRange = false;
	// 自身と対象が範囲内に入っているかのチェック
	if (Pos.x + Size.x > TargetPos.x - TargetSize.x &&
		Pos.x - Size.x < TargetPos.x + TargetSize.x &&
		Pos.z + Size.z > TargetPos.z - TargetSize.z &&
		Pos.z - Size.z < TargetPos.z + TargetSize.z)
	{
		// ヒットしている状態にする
		bRange = true;
	}
	// ヒット状態を返す
	return bRange;
}
//===========================================================================//
//					　　			範囲判定								 //
//					　　		Author :Ebina RIku							 //
//===========================================================================//
bool CCalculation::Range2D(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld, D3DXVECTOR3 Size, D3DXVECTOR3 TargetPos, D3DXVECTOR3 TargetSize)
{
	// ヒット状態を保持する変数
	bool bRange = false;
	// 自身と対象が範囲内に入っているかのチェック
	if (Pos.x + Size.x > TargetPos.x - TargetSize.x &&
		Pos.x - Size.x < TargetPos.x + TargetSize.x &&
		Pos.y + Size.y > TargetPos.y - TargetSize.y &&
		Pos.y - Size.y < TargetPos.y + TargetSize.y)
	{
		// ヒットしている状態にする
		bRange = true;
	}
	// ヒット状態を返す
	return bRange;
}
//===========================================================================//
//					　　		3.14を修正する処理							 //
//					　　		Author :Yuki Tanaka							 //
//===========================================================================//
float CCalculation::PiCheck(const float fPi)
{
	// 修正した値を返す用の変数
	float fpi = fPi;
	// 修正処理
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
//					　　		3.14を修正する処理							 //
//					　　		Author :圷 和也								 //
//===========================================================================//
D3DXVECTOR3 CCalculation::PiCheck(const D3DXVECTOR3 Pi)
{
	// 修正した値を返す用の変数
	D3DXVECTOR3 pi = Pi;
	// 修正処理
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
//					　　	軸を超えた時にTRUEを返す処理					 //
//					　　		Author :Yuki Tanaka							 //
//===========================================================================//
bool CCalculation::LineOver(const D3DXVECTOR3 pos, const float linepos, CCalculation::AXIS_TYPE type)
{
	// 軸を超えているかを返す変数
	bool bOver = false;
	switch (type)
	{
	case CCalculation::AXIS_TYPE::AXIS_X_RIGHT_OVER:
		if (pos.x >= linepos)
		{/* { 対象がＸ軸の＋方向を超えているか } */
			bOver = true;
		}
		break;
	case CCalculation::AXIS_TYPE::AXIS_X_LEFT_OVER:
		if (pos.x <= linepos)
		{/* { 対象がＸ軸の－方向を超えているか } */
			bOver = true;
		}
		break;
	case CCalculation::AXIS_TYPE::AXIS_Y_UP_OVER:
		if (pos.y >= linepos)
		{/* { 対象がＹ軸の＋方向を超えているか } */
			bOver = true;
		}
		break;
	case CCalculation::AXIS_TYPE::AXIS_Y_DOWN_OVER:
		if (pos.y <= linepos)
		{/* { 対象がＹ軸の－方向を超えているか } */
			bOver = true;
		}
		break;
	case CCalculation::AXIS_TYPE::AXIS_Z_FRONT_OVER:
		if (pos.z >= linepos)
		{/* { 対象がＺ軸の＋方向を超えているか } */
			bOver = true;
		}
		break;
	case CCalculation::AXIS_TYPE::AXIS_Z_BACK_OVER :
		if (pos.z <= linepos)
		{/* { 対象がＺ軸の－方向を超えているか } */
			bOver = true;
		}
		break;
	default:
		break;
	}
	return bOver;
}
//===========================================================================//
//					　　			移動計算								 //
//					　　		Author :Yuki Tanaka							 //
//===========================================================================//
D3DXVECTOR3 CCalculation::Moving(D3DXVECTOR3 *move, const D3DXVECTOR3 Angle, const D3DXVECTOR3 Speed)
{
	// 移動加算
	move->x += sinf(float(D3DX_PI * Angle.x)) * Speed.x;
	move->y += cosf(float(D3DX_PI * Angle.y)) * Speed.y;
	move->z += cosf(float(D3DX_PI * Angle.z)) * Speed.z;
	// 計算結果を返す
	return *move;
}