/*******************************************************************************
* タイトル カメラ処理
* ファイル名 camare.cpp
* 作成者 AT-12C-245 秋間雄太
* 作成日 2016/04/27
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "light.h"
#include "manager.h"


/*******************************************************************************
* グローバル変数
*******************************************************************************/



/*******************************************************************************
* 関数名：CLight
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CLight::CLight( ) {

}


/*******************************************************************************
* 関数名：~CLight
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CLight::~CLight( ) {

}


/*******************************************************************************
* 関数名：Init
* 引数：なし
* 戻り値：なし
* 説明：カメラの初期化
*******************************************************************************/

void CLight::Init(void) {
	D3DXVECTOR3 vecDir; // ベクトル正規化用

	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

	// ライト設定
	device -> SetRenderState(D3DRS_LIGHTING, TRUE); // 主電源 FALSEで全ライトOFF

	// 第1ライト設定
	ZeroMemory(&m_aLight[0], sizeof(D3DLIGHT9) ); // 初期化
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL; // 平行光源
	m_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 光の色
	vecDir = D3DXVECTOR3(0.5f, -0.5f, 0.5f); // 光の向き（-1.0～1.0）
//	vecDir = D3DXVECTOR3(0.0f, -1.0f, -0.0f); // 光の向き（-1.0～1.0）
	D3DXVec3Normalize( (D3DXVECTOR3*) &m_aLight[0].Direction, &vecDir); // ベクトルを正規化
	device -> SetLight(0, &m_aLight[0]); // ライトの設定
	device -> LightEnable(0, TRUE); // ライトを有効にする

	// 第2ライト設定
	ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9) ); // 初期化
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL; // 平行光源
	m_aLight[1].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f); // 光の色
	vecDir = D3DXVECTOR3(-0.5f, 0.4f, 0.6f); // 光の向き（-1.0～1.0）
	D3DXVec3Normalize( (D3DXVECTOR3*) &m_aLight[1].Direction, &vecDir); // ベクトルを正規化
	device -> SetLight(1, &m_aLight[1]); // ライトの設定
	device -> LightEnable(1, TRUE); // ライトを有効にする

	// 第3ライト設定
	ZeroMemory(&m_aLight[2], sizeof(D3DLIGHT9) ); // 初期化
	m_aLight[2].Type = D3DLIGHT_DIRECTIONAL; // 平行光源
	m_aLight[2].Diffuse = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f); // 光の色
	vecDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f); // 光の向き（-1.0～1.0）
	D3DXVec3Normalize( (D3DXVECTOR3*) &m_aLight[2].Direction, &vecDir); // ベクトルを正規化
	device -> SetLight(2, &m_aLight[2]); // ライトの設定
	device -> LightEnable(2, TRUE); // ライトを有効にする
}


/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：カメラの終了処理
*******************************************************************************/

void CLight::Uninit(void) {

}

/*******************************************************************************
* 関数名：Update
* 引数：なし
* 戻り値：なし
* 説明：カメラの更新処理
*******************************************************************************/

void CLight::Update(void) {

}
