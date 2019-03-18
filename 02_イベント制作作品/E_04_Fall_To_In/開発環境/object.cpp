//=============================================================================
//
// オブジェクトの処理 [object.cpp]
// Author : Okabe Kazuki
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// インクルード関数
#include <string.h>		// ストリング関数
#include <stdlib.h>		// ランド関数

#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "object.h"

//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_CHAR (1024)
#define FILE_SCAN  "data/object_scan.txt"		// ファイルの名前読み込み
#define FILE_PRINT "data/object_print.txt"		// ファイルの名前書き込み

#define MOVE (2.0f)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//== コンストラクタ ==//
CObject::CObject(int nPriority) : CSceneX(PRIORITY_OBJECT)
{//クリア値にする
}

//=== デストラクタ ===//
CObject::~CObject()
{

}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject::Init(void)
{
	CSceneX::Init();	// 初期化処理
	SetObjType(OBJTYPE_OBJECT);				// オブジェクトタイプ設定
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObject::Uninit(void)
{
	CSceneX::Uninit();	// 終了処理
}

//============================================================================
// 更新処理
//=============================================================================
void CObject::Update(void)
{

	CSceneX::Update();	// 更新処理
}

//=============================================================================
// 描画処理
//=============================================================================
void CObject::Draw(void)
{
	if (GetType() == 1 || GetType() == 2 || GetType() == 3)
	{
		SetRot(GetRot() + D3DXVECTOR3(0.01f, 0.001f, 0.001f));
		D3DXVECTOR3 pos = GetPos();
		pos.y += 20;
		SetPos(pos);
		if (pos.y >= 12000)
		{
			Uninit();
			return;
		}
	}
	CSceneX::Draw();	// 描画処理
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type,  const D3DXVECTOR3 ShadowPos)
{
	CObject *pObject = NULL;
	if (!CObject::OverData())
	{// オブジェクトの生成
		pObject = new CObject;
	}
	if (pObject != NULL)
	{
		// 初期化処理
		if (FAILED(pObject->Init()))
		{
			return NULL;
		}
		pObject->SetPos(pos);		// 位置の確保
		pObject->SetRot(rot);		// 角度の確保
		pObject->SetType(type);		// 種類の確保
		pObject->SetShadowPos(ShadowPos);// 影の確保
		pObject->BindModel(type);
	}
	return pObject;
}

//=============================================================================
// ファイルの読み込み
//=============================================================================
void CObject::SetScan()
{
	// デバイスの取得
	FILE *pFile;
	char cLine[MAX_CHAR];	// 行全て読み込み
	char cData[MAX_CHAR];	// 文字判定
	pFile = fopen(FILE_SCAN, "r");		// ファイルの名前の読み込み


	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// 行の読み込み

			if (strcmp(cData, "OBJECTSET") == 0)
			{// OBJECTSETだった場合
				D3DXVECTOR3 pos = D3DXVECTOR3(0,0,0);		// 位置
				D3DXVECTOR3 rot = D3DXVECTOR3(0, 0, 0);		// 角度
				D3DXVECTOR3 shadowPos = D3DXVECTOR3(0, 0, 0);	// 影
				int type = 0;				// 種類
				while (strcmp(cData, "END_OBJECTSET") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TYPE") == 0)
					{// 種類の読み込み
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &type);
					}
					else if (strcmp(cData, "POS") == 0)
					{// 位置の読み込み
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pos.x, &pos.y, &pos.z);
						shadowPos = pos;
					}
					else if (strcmp(cData, "ROT") == 0)
					{// 角度の読み込み
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &rot.x, &rot.y, &rot.z);
					}
					else if (strcmp(cData, "SHADOWPOS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &shadowPos.x, &shadowPos.y, &shadowPos.z);

					}
				}
				if (strcmp(cData, "END_OBJECTSET") == 0)
				{// モデルの生成

					CObject::Create(pos, rot, type, shadowPos);

				}
			}
		}
		fclose(pFile);// 終了
	}
}

