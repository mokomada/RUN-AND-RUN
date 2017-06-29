/*******************************************************************************
* タイトル GM31 Xファイル描画
* ファイル名 sceneX.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/26
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "sceneX.h"
#include "camera.h"
#include "input.h"
#include "meshfield.h"
#include "debugProc.h"
#include "sound.h"
#include "score.h"


//*****************************************************************************
// スタティック変数の生成
//*****************************************************************************



/*******************************************************************************
* 関数名：CSceneX
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CSceneX::CSceneX(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* 関数名：~CSceneX
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CSceneX::~CSceneX( ) {

}


/*******************************************************************************
* 関数名：Init
* 引数：なし
* 戻り値：なし
* 説明：初期化処理
*******************************************************************************/

void CSceneX::Init(void) {

}


/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：終了処理
*******************************************************************************/

void CSceneX::Uninit(void) {

}


/*******************************************************************************
* 関数名：Update
* 引数：なし
* 戻り値：なし
* 説明：更新処理
*******************************************************************************/

void CSceneX::Update(void) {

}


/*******************************************************************************
* 関数名：Draw
* 引数：なし
* 戻り値：なし
* 説明：描画処理
*******************************************************************************/

void CSceneX::Draw(void) {

}


/*******************************************************************************
* 関数名：Create
* 引数：sceneX : アドレス
* 戻り値：なし
* 説明：オブジェクトの生成
*******************************************************************************/

CSceneX *CSceneX::Create(void) {
	CSceneX *sceneX;
	sceneX = new CSceneX;
	sceneX -> Init( );
	return sceneX;
}

/*******************************************************************************
* 関数名：SetNewWay
* 引数：set : 設定する値
* 戻り値：なし
* 説明：指定された方向への移動量を計算する
*******************************************************************************/

void CSceneX::SetNewWay(float set) {
	// カメラ情報取得
	CCamera *get_camera;
	get_camera = CManager::GetCamera( );
	CAMERA camera = get_camera -> GetCameraData( );

	// 進行方向の設定
	m_newRot = -camera.rot.y - set;

	// 高低差による移動量減衰計算
	// 現在位置のフィールドの法線を取得
	D3DXVECTOR3 field_norm = CMeshfield::GetFieldNorm(m_model.pos);
	// 垂直な法線とフィールドの法線が成す角度θに対し、cosθを求める。どちらも正規化されているので 内積/(長さA * 長さB) の割算以下は省略
	float norm_cosangle = D3DXVec3Dot(&field_norm, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );

	PrintDebugProc("cosθ計算前(%f)\n", norm_cosangle);


	// フィールドの傾きと進行方向から、減衰量の割合を求める
	D3DXVECTOR3 field_xynorm;
	// フィールド法線のXZ成分だけを取り出し正規化
	D3DXVec3Normalize(&field_xynorm, &D3DXVECTOR3(field_norm.x, 0.0f, field_norm.z) );

	// 移動量を計算して正規化
	D3DXVECTOR3 player_move;
	D3DXVec3Normalize(&player_move, &D3DXVECTOR3(-sinf(D3DX_PI * (camera.rot.y + set) ), 0.0f, cosf(D3DX_PI * (camera.rot.y + set) ) ) );
	
	PrintDebugProc("プレイヤー移動量(%f, %f, %f)\n", player_move.x, player_move.y, player_move.z);

	// プレイヤーの移動量とフィールド法線のxy成分の内積。共に正規化されているので上記同様後半省略
	float xynorm_cosangle = D3DXVec3Dot(&field_xynorm, &player_move);

	// 求めた減衰割合で減衰量を調整
	norm_cosangle = 1 - ( (1 - norm_cosangle) * abs(xynorm_cosangle) );

	PrintDebugProc("cosθ計算後(%f)\n", norm_cosangle);

	// 移動量加算
	m_model.move.x += sinf(D3DX_PI * (camera.rot.y + set) ) * MODEL_MOVE_SPEED * norm_cosangle;
	m_model.move.z -= cosf(D3DX_PI * (camera.rot.y + set) ) * MODEL_MOVE_SPEED * norm_cosangle;
}