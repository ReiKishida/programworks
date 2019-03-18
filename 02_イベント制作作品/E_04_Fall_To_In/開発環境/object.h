#pragma once
//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : Okabe Kazuki
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "scene.h"
#include "sceneX.h"
//====================================
// 2Dオブジェクトクラス
//====================================
class CObject : public CSceneX
{
public:
	CObject(int nPriority = CScene::PRIORITY_INIT);
	~CObject();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CObject *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type, const D3DXVECTOR3 ShadowPos);

	static void SetScan(void);		// Xファイル読み込み
protected:
private:

};
#endif