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
#include "player.h"
#include "camera.h"
#include "input.h"
#include "meshfield.h"
#include "meshwater.h"
#include "debugProc.h"
#include "sound.h"
#include "score.h"
#include "enemy.h"
#include "game.h"

#include "object.h"
#include "airgauge.h"


//*****************************************************************************
// スタティック変数の生成
//*****************************************************************************

//MODEL CPlayer::m_model = {D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 0};


/*******************************************************************************
* 関数名：CPlayer
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CPlayer::CPlayer(int priority, OBJTYPE objType):CSceneX(priority, objType) {

}


/*******************************************************************************
* 関数名：~CPlayer
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CPlayer::~CPlayer( ) {

}


/*******************************************************************************
* 関数名：Init
* 引数：なし
* 戻り値：なし
* 説明：初期化処理
*******************************************************************************/

void CPlayer::Init(void) {
	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得
	m_pTexture = NULL;
	float field_height;

	D3DXLoadMeshFromX(PLAYER_NAME,
		D3DXMESH_SYSTEMMEM,
		device,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_numMat,
		&m_pMesh);

	// 変数の初期化
	m_model.pos.x = PLAYER_POS_X;
	m_model.pos.y = PLAYER_POS_Y;
	m_model.pos.z = PLAYER_POS_Z;

	field_height = CMeshfield::GetFieldHeight(m_model.pos);
	m_model.pos.y = field_height;

	D3DXQuaternionIdentity(&m_model.rot);

	m_model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_model.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_model.jump = false;

	m_model.onObject = false;

	m_attackFlag = false;

	m_attackCooltime = 0;

	m_newRot = 0;

	m_airLeft = 100.0f;

	m_moveReduce = D3DXVECTOR3(PLAYER_MOVE_REDUCE, PLAYER_FALL_REDUCE, PLAYER_MOVE_REDUCE);

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, PLAYER_TEXTURE_NAME, &m_pTexture);
}


/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：終了処理
*******************************************************************************/

void CPlayer::Uninit(void) {
	if(m_pTexture != NULL) {
		m_pTexture -> Release( ); // テクスチャの開放
		m_pTexture = NULL;
	}

	if(m_pMesh != NULL) {
		m_pMesh -> Release( );
		m_pMesh = NULL;
	}

	if(m_pBuffMat != NULL) {
		m_pBuffMat -> Release( );
		m_pBuffMat = NULL;
	}
}


/*******************************************************************************
* 関数名：Update
* 引数：なし
* 戻り値：なし
* 説明：更新処理
*******************************************************************************/

void CPlayer::Update(void) {
	CAirgauge *get_airgauge;
	get_airgauge = CGame::GetAirgauge( );
	MODEL old_model = m_model; // 現在情報保持
	float field_height, water_height; // 地面/水面の高さ
	D3DXVECTOR3 axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXQUATERNION quat;
	OBJHITCHK hitchk;
	bool drown_flag = false;

	// 敵情報取得
//	CEnemy *get_enemy = CGame::GetEnemy( );
//	MODEL enemy = get_enemy -> GetEnemy( );

	// 現在のプレイヤー情報保存
	m_oldModel = m_model;

	// 入力情報取得
	CInput *input;
	input = CManager::GetInput( );

	// モデルの移動処理
	if(!m_attackFlag) {
		if(input -> GetKeyboardPress(DIK_A) ) {
			if(input -> GetKeyboardPress(DIK_W) ) { // 左上移動
				SetNewWay(-0.75f);
			}
			else if(input -> GetKeyboardPress(DIK_S) ) { // 左下移動
				SetNewWay(-0.25f);
			}
			else { // 左移動
				SetNewWay(-0.5f);
			}
		}
		else if(input -> GetKeyboardPress(DIK_D) ) {
			if(input -> GetKeyboardPress(DIK_W) ) { // 右上移動
				SetNewWay(0.75f);
			}
			else if(input -> GetKeyboardPress(DIK_S) ) { // 右下移動
				SetNewWay(0.25f);
			}
			else { // 右移動
				SetNewWay(0.5f);
			}
		} // 前移動
		else if(input -> GetKeyboardPress(DIK_W) ) {
			SetNewWay(1.0f);
		} // 後移動
		else if(input -> GetKeyboardPress(DIK_S) ) {
			SetNewWay(0.0f);
		}

		// モデルの回転処理
		if(input -> GetKeyboardPress(DIK_RSHIFT) ) {
			m_newRot += PLAYER_ROT_SPEED;
		}
		else if(input -> GetKeyboardPress(DIK_LSHIFT) ) {
			m_newRot -= PLAYER_ROT_SPEED;
		}

		// ジャンプ処理
		if(input -> GetKeyboardPress(DIK_J) && !m_model.jump) {
			m_model.move.y += PLAYER_JUMP_SPEED;
			m_model.jump = true;
			CSound::PlaySound(SOUND_LABEL_SE_JUMP);
		}

		// 突進処理
		if(input -> GetKeyboardTrigger(DIK_SPACE) && !m_model.jump && !m_attackCooltime) {
			m_attackFlag = true;
			m_attackCooltime = 60;
			m_model.move.x += sinf(D3DX_PI * (-m_newRot) ) * PLAYER_MOVE_SPEED * 3;
			m_model.move.z -= cosf(D3DX_PI * (-m_newRot) ) * PLAYER_MOVE_SPEED * 3;
			m_moveReduce = D3DXVECTOR3(0.95f, 0.0f, 0.95f);
			CSound::PlaySound(SOUND_LABEL_SE_ATTACK);
		}
	}

	// ジャンプ中であれば落下速度を加算
	if(m_model.jump) {
		m_model.move.y -= PLAYER_FALL_SPEED;
	}

	// 移動量反映
	m_model.pos.x += m_model.move.x;
	m_model.pos.y += m_model.move.y;
	m_model.pos.z += m_model.move.z;

	// 攻撃モーション終了判定
	if(m_attackFlag && D3DXVec3Length(&m_model.move) < 1.0f) {
		m_attackFlag = false;
		m_model.jump = true;
		m_moveReduce = D3DXVECTOR3(PLAYER_MOVE_REDUCE, PLAYER_FALL_REDUCE, PLAYER_MOVE_REDUCE);
	}

	// 攻撃クールタイム解消
	if(m_attackCooltime > 0) m_attackCooltime--;

	// 世界の果て判定 しかっけいっていいよね
	if(m_model.pos.x < MESHFIELD_FIELD_X + 1.0f) {
		m_model.pos.x = MESHFIELD_FIELD_X + 1.0f;
	}
	else if(m_model.pos.x > MESHFIELD_FIELD_X + MESHFIELD_FIELD_WIDTH - 1.0f) {
		m_model.pos.x = MESHFIELD_FIELD_X + MESHFIELD_FIELD_WIDTH - 1.0f;
	}

	if(m_model.pos.z > MESHFIELD_FIELD_Z - 1.0f) {
		m_model.pos.z = MESHFIELD_FIELD_Z - 1.0f;
	}
	else if(m_model.pos.z < MESHFIELD_FIELD_Z - MESHFIELD_FIELD_HEIGHT + 1.0f) {
		m_model.pos.z = MESHFIELD_FIELD_Z - MESHFIELD_FIELD_HEIGHT + 1.0f;
	}

	// フィールド高さ取得
	field_height = CMeshfield::GetFieldHeight(m_model.pos);

	// 高さ処理
	if(!m_model.jump && !m_model.onObject && !m_attackFlag) {
		m_model.pos.y = field_height;
	}

	// フィールドめり込み処理
	if(m_model.pos.y < field_height) {
		m_model.pos.y = field_height;
		m_model.move.y = 0.0f;
		m_model.jump = false;
	}

	// 水面の高さ取得
	water_height = CMeshwater::GetWaterHeight(m_model.pos);

	// 水中判定
	if(m_model.pos.y + 5 < water_height) {
		m_airLeft -= 0.3f;
		if(m_airLeft < 0) {
			m_airLeft = 0;
			CScore::AddScore(600000);
			CGame::EndGame( );
		}

	}
	else if(m_airLeft < 100) {
		m_airLeft += 0.8f;
		if(m_airLeft >= 100) {
			m_airLeft = 100;
		}
	}

	CAirgauge::SetGauge(m_airLeft);

	// オブジェクトとの当たり判定
	hitchk = CScene::HitCheckAll(m_model.pos, m_oldModel.pos);
	if(hitchk.type != 0) {
		switch(hitchk.type) {
		case 1:
//			m_model.pos.z = m_oldModel.pos.z;
//			check = D3DXVECTOR3(0.0f, 0.0f, 1.0f) * D3DXVec3Dot(&(-m_model.move), &D3DXVECTOR3(0.0f, 0.0f, 1.0f) ); 法線受け取ればこの計算でいけるはず？
//			m_model.pos += D3DXVECTOR3(0.0f, 0.0f, 1.0f) * D3DXVec2Dot(&D3DXVECTOR2(-m_model.move.x, -m_model.move.z), &D3DXVECTOR2(0.0f, 1.0f) );
			m_model.pos += hitchk.nor * D3DXVec3Dot(&(-m_model.move), &hitchk.nor);
			break;
		case 2:
//			m_model.pos.x = m_oldModel.pos.x;
//			m_model.pos += D3DXVECTOR3(-1.0f, 0.0f, 0.0f) * D3DXVec2Dot(&D3DXVECTOR2(-m_model.move.x, -m_model.move.z), &D3DXVECTOR2(-1.0f, 0.0f) );
			m_model.pos.y = hitchk.nor.y;
			m_model.move.y = 0;
			m_model.jump = false;
			m_model.onObject = true;
			break;
/*		case 3:
//			m_model.pos.x = m_oldModel.pos.x;
			m_model.pos += D3DXVECTOR3(1.0f, 0.0f, 0.0f) * D3DXVec2Dot(&D3DXVECTOR2(-m_model.move.x, -m_model.move.z), &D3DXVECTOR2(1.0f, 0.0f) );
			break;
		case 4:
//			m_model.pos.z = m_oldModel.pos.z;
			m_model.pos += D3DXVECTOR3(0.0f, 0.0f, -1.0f) * D3DXVec2Dot(&D3DXVECTOR2(-m_model.move.x, -m_model.move.z), &D3DXVECTOR2(0.0f, -1.0f) );
			break;*/
		default:
			break;
		}
	}
	else if(m_model.onObject) {
		m_model.onObject = false;
		m_model.jump = true;
	}

	// 移動量減衰
	m_model.move.x *= m_moveReduce.x;
	m_model.move.y *= m_moveReduce.y;
	m_model.move.z *= m_moveReduce.z;

	// newRot調整
	if(m_newRot > 1.0f) {
		m_newRot -= 2.0f;
	}
	else if(m_newRot < -1.0f) {
		m_newRot += 2.0f;
	}

	// モデルの方向補正
	if(abs(m_newRot - m_model.rot.y) <= 1.0f) {
		m_model.rot.y += (m_newRot - m_model.rot.y) * 0.1f;
	}
	if(abs(m_newRot - m_model.rot.y) > 1.0f) {
		m_model.rot.y += (fmod(m_newRot + 2.0f, 2.0f) - fmod(m_model.rot.y + 2.0f, 2.0f) ) * 0.1f;
	}

/*	D3DXQuaternionRotationAxis(&quat, &axis, 0.0f);
	D3DXQuaternionMultiply(&m_model.rot, &m_model.rot, &quat);*/

	// 回転角の例外処理
	if(m_model.rot.y > 1.0f) {
		m_model.rot.y -= 2.0f;
	}
	else if(m_model.rot.y < -1.0f) {
		m_model.rot.y += 2.0f;
	}

	PrintDebugProc("高さ(%f)\n", m_model.pos.y);
}

/*******************************************************************************
* 関数名：SetNewWay
* 引数：set : 設定する値
* 戻り値：なし
* 説明：移動量の設定
*******************************************************************************/

/*void CPlayer::SetNewWay(float set) {
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
	m_model.move.x += sinf(D3DX_PI * (camera.rot.y + set) ) * PLAYER_MOVE_SPEED * norm_cosangle;
	m_model.move.z -= cosf(D3DX_PI * (camera.rot.y + set) ) * PLAYER_MOVE_SPEED * norm_cosangle;
}*/


/*******************************************************************************
* 関数名：Draw
* 引数：なし
* 戻り値：なし
* 説明：描画処理
*******************************************************************************/

void CPlayer::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // 計算用
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef; // マテリアル情報保存用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// ワールドマトリックスの作成
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, PLAYER_SCALE_X, PLAYER_SCALE_Y, PLAYER_SCALE_Z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映
//	D3DXMatrixRotationQuaternion(&mtxRot, &m_model.rot); // ※※※ y, x, z の順番に注意！！！ ※※※
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_model.rot.y * D3DX_PI, m_model.rot.x * D3DX_PI, m_model.rot.z * D3DX_PI);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_model.pos.x, m_model.pos.y, m_model.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在マテリアルを取得
	device -> GetMaterial(&matDef);

	// マテリアル設定
	pMat = (D3DXMATERIAL*)m_pBuffMat -> GetBufferPointer( );
	for(int nCntMat = 0; nCntMat < (int)m_numMat; nCntMat++) {
		device -> SetMaterial(&pMat[nCntMat].MatD3D);
		device -> SetTexture(0, m_pTexture);
		m_pMesh -> DrawSubset(nCntMat);
	}

	// マテリアルの設定を戻す
	device -> SetMaterial(&matDef);
}


/*******************************************************************************
* 関数名：Create
* 引数：sceneX : アドレス
* 戻り値：なし
* 説明：オブジェクトの生成
*******************************************************************************/

CPlayer *CPlayer::Create(void) {
	CPlayer *sceneX;
	sceneX = new CPlayer;
	sceneX -> Init( );
	return sceneX;
}
