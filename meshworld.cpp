/*******************************************************************************
* タイトル 外郭描画処理
* ファイル名 meshworld.cpp
* 作成者 AT-12C-245 秋間雄太
* 作成日 2015/12/10
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "meshfield.h"
#include "meshworld.h"
#include "debugProc.h"


/*******************************************************************************
* 構造体定義
*******************************************************************************/


/*******************************************************************************
* グローバル変数
*******************************************************************************/



/*******************************************************************************
* 関数名：CMeshworld
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CMeshworld::CMeshworld(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* 関数名：~CMeshworld
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CMeshworld::~CMeshworld( ) {

}

/*******************************************************************************
* 関数名：InitMeshworld
* 引数：なし
* 戻り値：なし
* 説明：外郭設定の初期化
*******************************************************************************/

void CMeshworld::Init(void) {
	// 変数宣言
	int i, j; // forループ用
	float angle_xz, angle_y;
	VERTEX_3D *pVtx;
	WORD *pIdx;

	// 変数の取得
	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

	// メンバ変数の初期化
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pVtxBuffLid = NULL;
	m_pIdxBuff = NULL;

	// 角度計算
	angle_xz = 2 * D3DX_PI / MESHWORLD_X_BLOCK;
	angle_y = 0.5f * D3DX_PI / (MESHWORLD_Y_BLOCK + 1);

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, MESHWORLD_TEXTURE_NAME, &m_pTexture);

	// 頂点バッファの生成
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * MESHWORLD_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);


	// 頂点情報へのポインタを取得
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	// 外郭の初期設定
	m_meshworld.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_meshworld.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_meshworld.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	for(i = 0; i < MESHWORLD_VERTEX_NUM; i++) {
		// 座標設定
		pVtx[i].pos = D3DXVECTOR3(
			cosf(angle_xz * (MESHWORLD_X_BLOCK - (i % (MESHWORLD_X_BLOCK + 1) ) ) ) * cosf(angle_y * (MESHWORLD_Y_BLOCK - (i / (MESHWORLD_X_BLOCK + 1) ) ) ) * MESHWORLD_DISTANCE,	// X座標
			sinf(angle_y * (MESHWORLD_Y_BLOCK - (i / (MESHWORLD_X_BLOCK + 1) ) ) ) * MESHWORLD_DISTANCE,																			// Y座標
			sinf(angle_xz * (MESHWORLD_X_BLOCK - (i % (MESHWORLD_X_BLOCK + 1) ) ) ) * cosf(angle_y * (MESHWORLD_Y_BLOCK - (i / (MESHWORLD_X_BLOCK + 1) ) ) ) * MESHWORLD_DISTANCE	// Z座標
		);

		// 頂点色設定
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// 法線設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// テクスチャUV座標設定
		if(i / (MESHWORLD_X_BLOCK + 1) >= MESHWORLD_TEXTURE_START_Y) {
			pVtx[i].tex = D3DXVECTOR2( (1 / (float)MESHWORLD_TEXTURE_X_BLOCK) * (i % (MESHWORLD_X_BLOCK + 1) ), (1 / (float)MESHWORLD_TEXTURE_Y_BLOCK) * (i / (MESHWORLD_X_BLOCK + 1) ) + MESHWORLD_TEXTURE_ADJUST);
		}
		else {
			pVtx[i].tex = D3DXVECTOR2( (1 / (float)MESHWORLD_TEXTURE_X_BLOCK) * (i % (MESHWORLD_X_BLOCK + 1) ), (1 / (float)MESHWORLD_TEXTURE_Y_BLOCK) * MESHWORLD_TEXTURE_START_Y + MESHWORLD_TEXTURE_ADJUST);
		}
	}

	m_pVtxBuff -> Unlock( );

	// インデックスバッファ確保
	device -> CreateIndexBuffer(sizeof(WORD) * MESHWORLD_INDEX_NUM,
								 D3DUSAGE_WRITEONLY,
								 D3DFMT_INDEX16,
								 D3DPOOL_MANAGED,
								 &m_pIdxBuff,
								 NULL);

	m_pIdxBuff -> Lock(0, 0, (void**) &pIdx, 0);

	for(i = 0; i < MESHWORLD_Y_BLOCK; i++) {
		if(i != 0) {
			pIdx[i * (MESHWORLD_X_BLOCK + 2) * 2 - 1] = (i + 1) * (MESHWORLD_X_BLOCK + 1);
		}

		for(j = 0; j < (MESHWORLD_X_BLOCK + 1) * 2; j++) {
			pIdx[i * (MESHWORLD_X_BLOCK + 2) * 2 + j] = (i + 1 - j % 2) * (MESHWORLD_X_BLOCK + 1) + (j / 2);
		}

		if(i < MESHWORLD_Y_BLOCK - 1) {
			pIdx[i * (MESHWORLD_X_BLOCK + 2) * 2 + j] = (i + 1 - (j - 1) % 2) * (MESHWORLD_X_BLOCK + 1) + ( (j - 1) / 2);
		}
	}

	m_pIdxBuff -> Unlock( );

	// 頂点バッファの生成(蓋)
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * MESHWORLD_VERTEX_FAN_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffLid, NULL);


	// 頂点情報へのポインタを取得
	m_pVtxBuffLid -> Lock(0, 0, (void**)&pVtx, 0);

		// 原点設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, MESHWORLD_DISTANCE, 0.0f);
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[0].tex = D3DXVECTOR2(0.0f, MESHWORLD_TEXTURE_ADJUST);

		for(i = 0; i < MESHWORLD_VERTEX_FAN_NUM - 1; i++) {
			// 位置設定
			pVtx[i + 1].pos = D3DXVECTOR3(
				cosf(angle_xz * i) * cosf(angle_y * MESHWORLD_Y_BLOCK) * MESHWORLD_DISTANCE, // X座標
				sinf(angle_y * MESHWORLD_Y_BLOCK) * MESHWORLD_DISTANCE,						 // Y座標
				sinf(angle_xz * i) * cosf(angle_y * MESHWORLD_Y_BLOCK) * MESHWORLD_DISTANCE  // Z座標
			);

			// 頂点色設定
			pVtx[i + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// 法線設定
			pVtx[i + 1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// テクスチャ設定
			pVtx[i + 1].tex = D3DXVECTOR2( (1 / (float)MESHWORLD_TEXTURE_X_BLOCK) * (i % (MESHWORLD_X_BLOCK + 1) ), MESHWORLD_TEXTURE_ADJUST);
		}

	m_pVtxBuff -> Unlock( );
}


/*******************************************************************************
* 関数名：UninitMeshworld
* 引数：なし
* 戻り値：なし
* 説明：外郭の終了処理
*******************************************************************************/

void CMeshworld::Uninit(void) {
	if(m_pTexture != NULL) {
		m_pTexture -> Release( ); // テクスチャの開放
		m_pTexture = NULL;
	}

	if(m_pVtxBuff != NULL) {
		m_pVtxBuff -> Release( );
		m_pVtxBuff = NULL;
	}

	if(m_pVtxBuffLid != NULL) {
		m_pVtxBuffLid -> Release( );
		m_pVtxBuffLid = NULL;
	}

	if(m_pIdxBuff != NULL) {
		m_pIdxBuff -> Release( );
		m_pIdxBuff = NULL;
	}
}


/*******************************************************************************
* 関数名：UpdataMeshworld
* 引数：なし
* 戻り値：なし
* 説明：外郭の情報更新
*******************************************************************************/

void CMeshworld::Update(void) {

}


/*******************************************************************************
* 関数名：DrawMeshworld
* 引数：なし
* 戻り値：なし
* 説明：外郭の描画処理
*******************************************************************************/

void CMeshworld::Draw(void) {
	// 変数の取得
	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // 計算用

	// ライト設定オフ
	device -> SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// ワールドマトリックスの作成
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_meshworld.scl.x, m_meshworld.scl.y, m_meshworld.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshworld.rot.y, m_meshworld.rot.x, m_meshworld.rot.z); // ※※※ y, x, z の順番に注意！！！ ※※※
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_meshworld.pos.x, m_meshworld.pos.y, m_meshworld.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// インデックスバッファをバインド
	device -> SetIndices(m_pIdxBuff);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	device -> SetTexture(0, m_pTexture);

	device -> DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		MESHWORLD_VERTEX_NUM,
		0,
		MESHWORLD_TRIANGLE_NUM);

	// 蓋の描画
	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_3D);

	device->DrawPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		MESHWORLD_TRIANGLE_FAN_NUM);

	// ライト設定オン
	device -> SetRenderState(D3DRS_LIGHTING, TRUE);
}

/*******************************************************************************
* 関数名：Create
* 引数：CMeshworld : アドレス
* 戻り値：なし
* 説明：オブジェクトの生成
*******************************************************************************/

CMeshworld *CMeshworld::Create(void) {
	CMeshworld *meshworld;
	meshworld = new CMeshworld;
	meshworld -> Init( );
	return meshworld;
}