/*******************************************************************************
* タイトル GM31 オブジェクト管理
* ファイル名 object.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/09/27
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
#include "object.h"
#include "meshfield.h"
#include "player.h"


typedef struct{
	float depth;
	int num;
}DEPTHCHK;


/*******************************************************************************
* 関数名：CObject
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CObject::CObject(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* 関数名：~CObject
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CObject::~CObject( ) {

}


/*******************************************************************************
* 関数名：Init
* 引数：なし
* 戻り値：なし
* 説明：初期化処理
*******************************************************************************/

void CObject::Init(void) {

}

void CObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth) {
	int i, j; // forループ用
	VERTEX_3D *pVtx;
	WORD *pIdx;

	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

	m_pTexture = NULL;
	m_playerOnFlag = false;
	m_alphaFlag = false;

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, "data/TEXTURE/field001.jpg", &m_pTexture);

	// 頂点バッファの生成
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * OBJECT_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * OBJECT_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxTopBuff, NULL);

	// 壁の初期設定
	m_object[0].pos = D3DXVECTOR3(pos.x, pos.y + height * 0.5f, pos.z);
	m_object[0].rot.x = 0.5f * D3DX_PI;
	m_object[0].rot.y = 0.0f;
	m_object[0].rot.z = 0.0f;

	m_object[1].pos = D3DXVECTOR3(pos.x, pos.y, pos.z + depth * 0.5f);
	m_object[1].rot.x = 0.0f;
	m_object[1].rot.y = D3DX_PI;
	m_object[1].rot.z = 0.0f;

	m_object[2].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y, pos.z);
	m_object[2].rot.x = 0.0f;
	m_object[2].rot.y = 0.5f * D3DX_PI;
	m_object[2].rot.z = 0.0f;

	m_object[3].pos = D3DXVECTOR3(pos.x + width * 0.5f, pos.y, pos.z);
	m_object[3].rot.x = 0.0f;
	m_object[3].rot.y = -0.5f * D3DX_PI;
	m_object[3].rot.z = 0.0f;

	m_object[4].pos = D3DXVECTOR3(pos.x, pos.y, pos.z - depth * 0.5f);
	m_object[4].rot.x = 0.0f;
	m_object[4].rot.y = 0.0f;
	m_object[4].rot.z = 0.0f;

	for(i = 0; i < 5; i++) {
		m_object[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		m_object[i].move = D3DXVECTOR3(0, 0, 0);
	}

	// 頂点情報へのポインタを取得
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0; i < OBJECT_VERTEX_NUM; i++) {
		// 座標設定
//		pVtx[i].pos = D3DXVECTOR3( (pos.x - width * 0.5f) + (width / (float)OBJECT_X_BLOCK) * (i % (OBJECT_X_BLOCK + 1) ),
//									(pos.y - height * 0.5f) - (height / (float)OBJECT_Y_BLOCK) * (i / (OBJECT_X_BLOCK + 1) ), 0);
		pVtx[i].pos = D3DXVECTOR3( (-width * 0.5f) + (width / (float)OBJECT_X_BLOCK) * (i % (OBJECT_X_BLOCK + 1) ),
									(height * 0.5f) - (height / (float)OBJECT_Y_BLOCK) * (i / (OBJECT_X_BLOCK + 1) ), 0);

		// 頂点色設定
		pVtx[i].col = D3DCOLOR_RGBA(130, 90, 20, 255);

		// 法線設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// テクスチャUV座標設定
		pVtx[i].tex = D3DXVECTOR2( (1 / (float)OBJECT_TEXTURE_X_BLOCK) * (i % (OBJECT_X_BLOCK + 1) ), (1 / (float)OBJECT_TEXTURE_Y_BLOCK) * (i / (OBJECT_X_BLOCK + 1) ) );
	}

	m_pVtxBuff -> Unlock( );

	m_pVtxTopBuff -> Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0; i < OBJECT_VERTEX_NUM; i++) {
		pVtx[i].pos = D3DXVECTOR3( (-width * 0.5f) + (width / (float)OBJECT_X_BLOCK) * (i % (OBJECT_X_BLOCK + 1) ),
									(depth * 0.5f) - (depth / (float)OBJECT_Y_BLOCK) * (i / (OBJECT_X_BLOCK + 1) ), 0);

		// 頂点色設定
		pVtx[i].col = D3DCOLOR_RGBA(64, 255, 64, 255);

		// 法線設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// テクスチャUV座標設定
		pVtx[i].tex = D3DXVECTOR2( (1 / (float)OBJECT_TEXTURE_X_BLOCK) * (i % (OBJECT_X_BLOCK + 1) ), (1 / (float)OBJECT_TEXTURE_Y_BLOCK) * (i / (OBJECT_X_BLOCK + 1) ) );
	}

	m_pVtxTopBuff -> Unlock( );

	// インデックスバッファ確保
	device -> CreateIndexBuffer(sizeof(WORD) * OBJECT_INDEX_NUM,
								 D3DUSAGE_WRITEONLY,
								 D3DFMT_INDEX16,
								 D3DPOOL_MANAGED,
								 &m_pIdxBuff,
								 NULL);

	m_pIdxBuff -> Lock(0, 0, (void**) &pIdx, 0);

	for(i = 0; i < OBJECT_Y_BLOCK; i++) {
		if(i != 0) {
			pIdx[i * (OBJECT_X_BLOCK + 2) * 2 - 1] = (i + 1) * (OBJECT_X_BLOCK + 1);
		}

		for(j = 0; j < (OBJECT_X_BLOCK + 1) * 2; j++) {
			pIdx[i * (OBJECT_X_BLOCK + 2) * 2 + j] = (i + 1 - j % 2) * (OBJECT_X_BLOCK + 1) + (j / 2);
		}

		if(i < OBJECT_Y_BLOCK - 1) {
			pIdx[i * (OBJECT_X_BLOCK + 2) * 2 + j] = (i + 1 - (j - 1) % 2) * (OBJECT_X_BLOCK + 1) + ( (j - 1) / 2);
		}
	}

	m_pIdxBuff -> Unlock( );
}


/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：終了処理
*******************************************************************************/

void CObject::Uninit(void) {
	if(m_pTexture != NULL) {
		m_pTexture -> Release( ); // テクスチャの開放
		m_pTexture = NULL;
	}

	if(m_pVtxBuff != NULL) {
		m_pVtxBuff -> Release( );
		m_pVtxBuff = NULL;
	}

	if(m_pIdxBuff != NULL) {
		m_pIdxBuff -> Release( );
		m_pIdxBuff = NULL;
	}
}


/*******************************************************************************
* 関数名：Update
* 引数：なし
* 戻り値：なし
* 説明：更新処理
*******************************************************************************/

void CObject::Update(void) {

}


/*******************************************************************************
* 関数名：Draw
* 引数：なし
* 戻り値：なし
* 説明：描画処理
*******************************************************************************/

void CObject::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得
	CCamera *getCamera;
	getCamera = CManager::GetCamera( );
	CAMERA camera = getCamera -> GetCameraData( ); // カメラ情報取得
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // 計算用
	D3DXVECTOR3 cameraFrontWay, cameraToWall;

	// 透過設定がONなら透過された裏面を描画してから表面を描画する（
	if(m_alphaFlag) {
		// アルファブレンド
		device -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// 裏面描画
		device -> SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		for(int i = 0; i < 5; i++) {
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&m_mtxWorld);
			// ワールドマトリックスの作成
			// スケールを反映
			D3DXMatrixScaling(&mtxScl, m_object[i].scl.x, m_object[i].scl.y, m_object[i].scl.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_object[i].rot.y, m_object[i].rot.x, m_object[i].rot.z); // ※※※ y, x, z の順番に注意！！！ ※※※
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans, m_object[i].pos.x, m_object[i].pos.y, m_object[i].pos.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

			// インデックスバッファをバインド
			device -> SetIndices(m_pIdxBuff);

			// 頂点バッファをデータストリームにバインド
			if(i == 0) {
				device -> SetStreamSource(0, m_pVtxTopBuff, 0, sizeof(VERTEX_3D) );
			}
			else {
				device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D) );
			}

			// 頂点フォーマットの設定
			device -> SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			device -> SetTexture(0, m_pTexture);

			// ポリゴンの描画
			device -> DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				OBJECT_VERTEX_NUM,
				0,
				OBJECT_TRIANGLE_NUM);
		}

		// 表面描画に戻す
		device -> SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		m_alphaFlag = false;
	}

	// 側面の描画
	for(int i = 0; i < 5; i++) {

/*		// 透過処理確認用の側面色替え
		// 1赤　2緑　3青　4白
		m_pVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

		for(int j = 0; j < OBJECT_VERTEX_NUM; j++) {
			switch(getDepth[i].num){
			case 1:
				pVtx[j].col = D3DCOLOR_RGBA(255, 20, 20, 255);
				break;
			case 2:
				pVtx[j].col = D3DCOLOR_RGBA(20, 255, 20, 255);
				break;
			case 3:
				pVtx[j].col = D3DCOLOR_RGBA(20, 20, 255, 255);
				break;
			case 4:
				pVtx[j].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				break;
			default:
				break;
			}
		}

		m_pVtxBuff -> Unlock( );*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);
		// ワールドマトリックスの作成
		// スケールを反映
		D3DXMatrixScaling(&mtxScl, m_object[i].scl.x, m_object[i].scl.y, m_object[i].scl.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_object[i].rot.y, m_object[i].rot.x, m_object[i].rot.z); // ※※※ y, x, z の順番に注意！！！ ※※※
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTrans, m_object[i].pos.x, m_object[i].pos.y, m_object[i].pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// インデックスバッファをバインド
		device -> SetIndices(m_pIdxBuff);

		// 頂点バッファをデータストリームにバインド
		if(i == 0) {
			device -> SetStreamSource(0, m_pVtxTopBuff, 0, sizeof(VERTEX_3D) );
		}
		else {
			device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D) );
		}

		// 頂点フォーマットの設定
		device -> SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		device -> SetTexture(0, m_pTexture);

		device -> DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			OBJECT_VERTEX_NUM,
			0,
			OBJECT_TRIANGLE_NUM);
	}

	// ブレンド設定を戻す（透過してない時は無駄な処理）
	device -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/*******************************************************************************
* 関数名：Create
* 引数：pos : 座標
*		rot : 回転量
*		height : 高さ
*		width : 幅
*		depth : 奥行き
* 戻り値：object : 新しく生成したクラスのアドレス
* 説明：オブジェクトの生成
*******************************************************************************/

CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth) {
	CObject *object;
	object = new CObject;
	object -> Init(pos, rot, height, width, depth);
	return object;
}


/*******************************************************************************
* 関数名：HitCheck
* 引数：pos : 座標
*		rot : 回転量
*		height : 高さ
*		width : 幅
*		depth : 奥行き
* 戻り値：object : 新しく生成したクラスのアドレス
* 説明：オブジェクトの生成
*******************************************************************************/

OBJHITCHK CObject::HitCheck(D3DXVECTOR3 pos, D3DXVECTOR3 oldPos) {
	OBJHITCHK hitchk;

	hitchk.type = 0;
	hitchk.nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	if(m_object[1].pos.z > pos.z && m_object[2].pos.x < pos.x && m_object[3].pos.x > pos.x && m_object[4].pos.z < pos.z && m_object[0].pos.y > pos.y) {
		if(m_object[0].pos.y <= oldPos.y) {
			hitchk.type = 2;
			hitchk.nor.y = m_object[0].pos.y; // 上面と衝突
		}
		else if(m_object[1].pos.z <= oldPos.z) {
			hitchk.type = 1;
			hitchk.nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		}
		else if(m_object[2].pos.x >= oldPos.x) {
			hitchk.type = 1;
			hitchk.nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		}
		else if(m_object[3].pos.x <= oldPos.x) {
			hitchk.type = 1;
			hitchk.nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		}
		else if(m_object[4].pos.z >= oldPos.z) {
			hitchk.type = 1;
			hitchk.nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}

		return hitchk;
	}

	return hitchk;
}


/*******************************************************************************
* 関数名：SetAlphaFlag
* 引数：なし
* 戻り値：なし
* 説明：オブジェクト透過のONOFF切り替え
*******************************************************************************/

void CObject::SetAlphaFlag(bool onoff) {
	m_alphaFlag = onoff;
}