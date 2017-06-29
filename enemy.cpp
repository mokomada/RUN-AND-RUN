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
#include "enemy.h"
#include "camera.h"
#include "input.h"
#include "meshfield.h"
#include "debugProc.h"
#include "sound.h"
#include "score.h"
#include "player.h"
#include "game.h"
#include "object.h"


//*****************************************************************************
// スタティック変数の生成
//*****************************************************************************

//MODEL CEnemy::m_model = {D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 0};


/*******************************************************************************
* 関数名：CEnemy
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CEnemy::CEnemy(int priority, OBJTYPE objType):CSceneX(priority, objType) {

}


/*******************************************************************************
* 関数名：~CEnemy
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CEnemy::~CEnemy( ) {

}


/*******************************************************************************
* 関数名：Init
* 引数：なし
* 戻り値：なし
* 説明：初期化処理
*******************************************************************************/

void CEnemy::Init(void) {
	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得
	m_pTexture = NULL;

	D3DXLoadMeshFromX(ENEMY_NAME,
		D3DXMESH_SYSTEMMEM,
		device,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_numMat,
		&m_pMesh);

	// 変数の初期化
	m_model.pos.x = ENEMY_POS_X + rand( ) % 40 - ENEMY_START_DISTANCE;
	if(m_model.pos.x < 0) {
		m_model.pos.x -= ENEMY_START_DISTANCE;
	}
	else{
		m_model.pos.x += ENEMY_START_DISTANCE;
	}

	m_model.pos.y = ENEMY_POS_Y;

	m_model.pos.z = ENEMY_POS_Z + rand( ) % 40 - ENEMY_START_DISTANCE;
	if(m_model.pos.z < 0) {
		m_model.pos.z -= ENEMY_START_DISTANCE;
	}
	else{
		m_model.pos.z += ENEMY_START_DISTANCE;
	}

	D3DXQuaternionIdentity(&m_model.rot);

	m_model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_model.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_model.jump = false;

	m_newRot = 0;

	m_moveChange = 0;

	m_respawn = 0;

	m_moveFlag = false;

	m_jumpFlag = false;

	m_rnd = 0;

	m_use = true;

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, ENEMY_TEXTURE_NAME, &m_pTexture);
}


/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：終了処理
*******************************************************************************/

void CEnemy::Uninit(void) {
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

void CEnemy::Update(void) {
	if(m_use) {
		CCamera *get_camera;
		get_camera = CManager::GetCamera( ); // カメラ情報取得
		CAMERA camera = get_camera -> GetCameraData( );
		MODEL old_model = m_model; // 現在情報保持
		float field_height; // 地面の高さ
		D3DXVECTOR3 axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXQUATERNION quat;
		float PEDistance;
		OBJHITCHK hitchk;

		CPlayer *get_player = CGame::GetPlayer( );
		MODEL player = get_player -> GetModel( );

		// 現在の敵情報保存
		m_oldModel = m_model;

		// プレイヤーとの距離
		PEDistance = sqrt( (m_model.pos.x - player.pos.x) * (m_model.pos.x - player.pos.x) + (m_model.pos.z - player.pos.z) * (m_model.pos.z - player.pos.z) );

		// 一定距離以下なら移動開始、一定距離以上で移動終了
		if(PEDistance < ENEMY_SEARCH_START && m_moveFlag == false) {
			m_moveFlag = true;
		}
		else if(PEDistance >= ENEMY_SEARCH_STOP && m_moveFlag == true) {
			m_moveFlag = false;
		}

		// 移動中ならくっそガバガバな乱数ランダム移動を行う
		if(m_moveFlag) {
			m_moveChange--;

			if(m_moveChange <= 0) {
				m_rnd = rand( ) % 8;
				m_moveChange = ENEMY_MOVE_CHANGE;
			}

		// モデルの移動処理
			if(m_rnd < 3) {
				if(m_rnd == 1) { // 左上移動
				SetNewWay(-0.75f);
				}
				else if(m_rnd == 2) { // 左下移動
					SetNewWay(-0.25f);
				}
				else { // 左移動
					SetNewWay(-0.5f);
				}
			}
			// 上移動
			else if(m_rnd == 3) {
				SetNewWay(1.0f);
			}
			else if(m_rnd < 7) {
				if(m_rnd == 6) { // 右上移動
					SetNewWay(0.75f);
				}
				else if(m_rnd == 5) { // 右下移動
					SetNewWay(0.25f);
				}
				else { // 右移動
					SetNewWay(0.5f);
				}
			}
			// 下移動
			else if(m_rnd == 7) {
				SetNewWay(0.0f);
			}
			else if(m_rnd == 8) {
				m_model.move.x = 0;
				m_model.move.z = 0;
			}
			// プレイヤーの方向に移動していた場合、取りやめて移動方向を逆向きにする（ランダム値に4を足すと移動方向が逆になるよう移動処理を設定してある）
			if(m_moveChange == ENEMY_MOVE_CHANGE && PEDistance > sqrt( ( (m_model.pos.x + m_model.move.x) - player.pos.x) * ( (m_model.pos.x + m_model.move.x) - player.pos.x) + ( (m_model.pos.z + m_model.move.z) - player.pos.z) * ( (m_model.pos.z + m_model.move.z) - player.pos.z) ) ) {
				m_rnd = (m_rnd + 4) % 8;
				m_model.move.x = 0.0f;
				m_model.move.z = 0.0f;
				m_newRot += 1.0f;
			}
		}

		if(m_jumpFlag && m_model.jump == false) {
			m_model.move.y += ENEMY_JUMP_SPEED;
			m_model.jump = true;
			m_jumpFlag = false;
			CSound::PlaySound(SOUND_LABEL_SE_JUMP);
		}


		// ジャンプ中であれば落下速度を加算
		if(m_model.jump) {
			m_model.move.y -= ENEMY_FALL_SPEED;
		}

		// 移動量反映
		m_model.pos.x += m_model.move.x;
		m_model.pos.y += m_model.move.y;
		m_model.pos.z += m_model.move.z;

		// 移動量減衰
		m_model.move.x *= ENEMY_MOVE_REDUCE;
		m_model.move.y *= ENEMY_FALL_REDUCE;
		m_model.move.z *= ENEMY_MOVE_REDUCE;

		// 世界の果て判定 しかっけいっていいよね
		if(m_model.pos.x < MESHFIELD_FIELD_X + 25.0f) {
			m_model.pos.x = MESHFIELD_FIELD_X + 25.0f;
		}
		else if(m_model.pos.x > MESHFIELD_FIELD_X + MESHFIELD_FIELD_WIDTH - 25.0f) {
			m_model.pos.x = MESHFIELD_FIELD_X + MESHFIELD_FIELD_WIDTH - 25.0f;
		}

		if(m_model.pos.z > MESHFIELD_FIELD_Z - 25.0f) {
			m_model.pos.z = MESHFIELD_FIELD_Z - 25.0f;
		}
		else if(m_model.pos.z < MESHFIELD_FIELD_Z - MESHFIELD_FIELD_HEIGHT + 25.0f) {
			m_model.pos.z = MESHFIELD_FIELD_Z - MESHFIELD_FIELD_HEIGHT + 25.0f;
		}

		// フィールド高さ取得
		field_height = CMeshfield::GetFieldHeight(m_model.pos);

		// 高さ処理
		if(m_model.jump == false && !m_model.onObject) {
			m_model.pos.y = field_height;
		}

		// フィールドめり込み処理
		if(m_model.pos.y < field_height ){
			m_model.pos.y = field_height;
			m_model.move.y = 0.0f;
			m_model.jump = false;
		}

		// オブジェクトとの当たり判定
		hitchk = CScene::HitCheckAll(m_model.pos, m_oldModel.pos);
		if(hitchk.type != 0) {
			if(hitchk.type == 1) {
				m_model.pos.x = m_oldModel.pos.x;
				m_model.pos.z = m_oldModel.pos.z;
				if(!m_model.jump) m_jumpFlag = true;
			}
			else{
				m_model.pos.y = hitchk.nor.y;
				m_model.move.y = 0;
				m_model.jump = false;
				m_model.onObject = true;
			}
		}
		else if(m_model.onObject) {
			m_model.onObject = false;
			m_model.jump = true;
		}

		// m_newRot調整
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

		if(m_respawn) {
			m_respawn--;
			if(m_respawn == 0) {
				CSound::PlaySound(SOUND_LABEL_SE_ATTACK);
			}
		}

		// 接触判定
		if( sqrt( (m_model.pos.x - player.pos.x) * (m_model.pos.x - player.pos.x) + (m_model.pos.z - player.pos.z) * (m_model.pos.z - player.pos.z) ) < 2.0f && m_respawn == 0) {
			CSound::PlaySound(SOUND_LABEL_SE_HIT);
//			CScore::AddScore(10);
			m_respawn += ENEMY_RESPAWN_TIME;
			m_use = false;
		}
	}
}


/*******************************************************************************
* 関数名：SetNewWay
* 引数：set : 設定する値
* 戻り値：なし
* 説明：移動量の設定
*******************************************************************************/

/*void CEnemy::SetNewWay(float set) {
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

void CEnemy::Draw(void) {
	if(m_use && m_respawn == 0) {
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
		D3DXMatrixScaling(&mtxScl, ENEMY_SCALE_X, ENEMY_SCALE_Y, ENEMY_SCALE_Z);
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
}


/*******************************************************************************
* 関数名：Create
* 引数：sceneX : アドレス
* 戻り値：なし
* 説明：オブジェクトの生成
*******************************************************************************/

CEnemy *CEnemy::Create(void) {
	CEnemy *sceneX;
	sceneX = new CEnemy;
	sceneX -> Init( );
	return sceneX;
}
