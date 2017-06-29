/*******************************************************************************
* タイトル フィールド描画処理
* ファイル名 meshwater.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/5/18
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "meshwater.h"
#include "manager.h"
#include "sceneX.h"
#include "player.h"
#include "debugProc.h"


LPDIRECT3DVERTEXBUFFER9 CMeshwater::m_pVtxBuffMeshwater = NULL;


/*******************************************************************************
* グローバル変数
*******************************************************************************/

float g_HeightWater[MESHWATER_X_BLOCK + 1][MESHWATER_Z_BLOCK + 1];


/*******************************************************************************
* 関数名：CMeshwater
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CMeshwater::CMeshwater(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* 関数名：~CMeshwater
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CMeshwater::~CMeshwater( ) {

}


/*******************************************************************************
* 関数名：InitMeshwater
* 引数：なし
* 戻り値：なし
* 説明：フィールド設定の初期化
*******************************************************************************/

void CMeshwater::Init(void) {
	// 変数宣言
	int i, j; // forループ用
	VERTEX_3D *pVtx;
	WORD *pIdx;
	D3DXVECTOR3 n0, v1, v2; // 法線計算用

	// 変数の取得
	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

	// メンバ変数の初期化
	m_pTextureMeshwater = NULL;
	m_pVtxBuffMeshwater = NULL;
	m_pIdxBuffMeshwater = NULL;
	m_time = 0;

	// 乱数設定
	srand( (unsigned) time(NULL) );

	for(int cntz = 0; cntz < MESHWATER_Z_BLOCK + 1; cntz++) {
		for(int cntx = 0; cntx < MESHWATER_X_BLOCK + 1; cntx++) {
			g_HeightWater[cntx][cntz] = MESHWATER_MIN_HEIGHT + ( (rand( ) % (10 * (MESHWATER_MAX_HEIGHT - MESHWATER_MIN_HEIGHT) ) ) / 10.0f);
		}
	}

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, MESHWATER_TEXTURE_NAME, &m_pTextureMeshwater);

	// 頂点バッファの生成
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * MESHWATER_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffMeshwater, NULL);

	// 頂点情報へのポインタを取得
	m_pVtxBuffMeshwater -> Lock(0, 0, (void**)&pVtx, 0);

	// 角度初期設定
	m_posMeshwater = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotMeshwater = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_sclMeshwater = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	for(i = 0; i < MESHWATER_VERTEX_NUM; i++) {
		// 座標設定
		pVtx[i].pos = D3DXVECTOR3( (float) (MESHWATER_WATER_X + (MESHWATER_WATER_WIDTH / (float)MESHWATER_X_BLOCK) * (i % (MESHWATER_X_BLOCK + 1) ) ), g_HeightWater[i / (MESHWATER_X_BLOCK + 1)][i % (MESHWATER_Z_BLOCK + 1)], (float) (MESHWATER_WATER_Z - (MESHWATER_WATER_HEIGHT / (float)MESHWATER_Z_BLOCK) * (i / (MESHWATER_X_BLOCK + 1) ) ) );

		// 頂点色設定
		pVtx[i].col = D3DCOLOR_RGBA(32, 32, 192, 255);

		// 法線設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// テクスチャUV座標設定
		pVtx[i].tex = D3DXVECTOR2( (1 / (float)MESHWATER_TEXTURE_X_BLOCK) * (i % (MESHWATER_X_BLOCK + 1) ), (1 / (float)MESHWATER_TEXTURE_Z_BLOCK) * (i / (MESHWATER_X_BLOCK + 1) ) );
	}

	for(i = 0; i < MESHWATER_Z_BLOCK + 1; i++) {
		for(j = 0; j < MESHWATER_X_BLOCK + 1; j++) {
			// 初期化
			n0 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 最初の行の処理
			if(i == 0) {
				// 最初の列の処理
				if(j == 0) {
					// 頂点に接する各ポリゴンの法線を求める
					// 1つ目のポリゴン
					v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					// 外積を求める
					D3DXVec3Cross(&v1, &v1, &v2);
					// 求めた法線を正規化して足し合わせる
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					// 2つ目のポリゴン v2は値が残っているので使い回し
					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					// 外積を求める　変数を使いまわしてるので外積を求める際のベクトルの順番に注意
					D3DXVec3Cross(&v2, &v2, &v1);
					// 求めた法線を正規化して足し合わせる
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					// 足し合わせた法線を正規化
					D3DXVec3Normalize(&n0, &n0);
					// 得られた頂点の法線を代入
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// 最後の列の処理
				else if(j == MESHWATER_X_BLOCK) {
					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// それ以外の列の処理
				else{
					v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
			}
			// 最後の行の処理
			else if(i == MESHWATER_Z_BLOCK) {
				// 最初の列の処理
				if(j == 0) {
					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// 最後の列の処理
				else if(j == MESHWATER_X_BLOCK) {
					v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// それ以外の列の処理
				else{
					v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
			}
			// それ以外の行の処理
			else{
				// 最初の列の処理
				if(j == 0) {
					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// 最後の列の処理
				else if(j == MESHWATER_X_BLOCK) {
					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// それ以外の列の処理
				else{
					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2,&v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
			}
		}
	}

	m_pVtxBuffMeshwater -> Unlock( );

	// インデックスバッファ確保
	device -> CreateIndexBuffer(sizeof(WORD) * MESHWATER_INDEX_NUM,
								 D3DUSAGE_WRITEONLY,
								 D3DFMT_INDEX16,
								 D3DPOOL_MANAGED,
								 &m_pIdxBuffMeshwater,
								 NULL);

	m_pIdxBuffMeshwater -> Lock(0, 0, (void**) &pIdx, 0);

	for(i = 0; i < MESHWATER_Z_BLOCK; i++) {
		if(i != 0) {
			pIdx[i * (MESHWATER_X_BLOCK + 2) * 2 - 1] = (i + 1) * (MESHWATER_X_BLOCK + 1);
		}

		for(j = 0; j < (MESHWATER_X_BLOCK + 1) * 2; j++) {
			pIdx[i * (MESHWATER_X_BLOCK + 2) * 2 + j] = (i + 1 - j % 2) * (MESHWATER_X_BLOCK + 1) + (j / 2);
		}

		j--;
		if(i < MESHWATER_Z_BLOCK - 1) {
			pIdx[i * (MESHWATER_X_BLOCK + 2) * 2 + j + 1] = (i + 1 - j % 2) * (MESHWATER_X_BLOCK + 1) + (j / 2);
		}
	}

	m_pIdxBuffMeshwater -> Unlock( );
}


/*******************************************************************************
* 関数名：UninitMeshwater
* 引数：なし
* 戻り値：なし
* 説明：フィールドの終了処理
*******************************************************************************/

void CMeshwater::Uninit(void) {
	if(m_pTextureMeshwater != NULL) {
		m_pTextureMeshwater -> Release( ); // テクスチャの開放
		m_pTextureMeshwater = NULL;
	}

	if(m_pVtxBuffMeshwater != NULL) {
		m_pVtxBuffMeshwater -> Release( );
		m_pVtxBuffMeshwater = NULL;
	}

	if(m_pIdxBuffMeshwater != NULL) {
		m_pIdxBuffMeshwater -> Release( );
		m_pIdxBuffMeshwater = NULL;
	}
}


/*******************************************************************************
* 関数名：UpdataMeshwater
* 引数：なし
* 戻り値：なし
* 説明：フィールドの情報更新
*******************************************************************************/

void CMeshwater::Update(void) {
	D3DXVECTOR2 model_pos = D3DXVECTOR2(0.0f, 0.0f);

	VERTEX_3D *pVtx;

	if(m_time % MESHWATER_MOVE_TIME == 0) {
		// 頂点情報へのポインタを取得
		m_pVtxBuffMeshwater -> Lock(0, 0, (void**)&pVtx, 0);

		for(int cntz = 0; cntz < MESHWATER_Z_BLOCK + 1; cntz++) {	// 330.15 330   3301.5 3300
			for(int cntx = 0; cntx < MESHWATER_X_BLOCK + 1; cntx++) {
				if(pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y * 10.0f - (int)(pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y * 10.0f) <= 0.1f) {
					pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y += 0.1f;
				}
				else{
					pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y -= 0.1f;
				}

				if(pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y > MESHWATER_MAX_HEIGHT) {
					pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y = MESHWATER_MAX_HEIGHT - 0.05f;
				}
				else if(pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y < MESHWATER_MIN_HEIGHT) {
					pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y = MESHWATER_MIN_HEIGHT;
				}
			}
		}

		m_pVtxBuffMeshwater -> Unlock( );
	}

	m_time++;
}


/*******************************************************************************
* 関数名：DrawMeshwater
* 引数：なし
* 戻り値：なし
* 説明：フィールドの描画処理
*******************************************************************************/

void CMeshwater::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // 計算用

	// アルファブレンドの方法を加算合成に
	device -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorldMeshwater);
	// ワールドマトリックスの作成
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorldMeshwater, &m_mtxWorldMeshwater, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f); // ※※※ y, x, z の順番に注意！！！ ※※※
	D3DXMatrixMultiply(&m_mtxWorldMeshwater, &m_mtxWorldMeshwater, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorldMeshwater, &m_mtxWorldMeshwater, &mtxTrans);

	// ワールドマトリックスの設定
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorldMeshwater);

	// インデックスバッファをバインド
	device -> SetIndices(m_pIdxBuffMeshwater);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuffMeshwater, 0, sizeof(VERTEX_3D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	device -> SetTexture(0, m_pTextureMeshwater);

	// ポリゴンの描画
/*	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		MESHWATER_TRIANGLE_NUM);*/

	device -> DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		MESHWATER_VERTEX_NUM,
		0,
		MESHWATER_TRIANGLE_NUM);

	// アルファブレンド設定を戻す
	device -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/*******************************************************************************
* 関数名：Create
* 引数：CMeshwater : アドレス
* 戻り値：なし
* 説明：オブジェクトの生成
*******************************************************************************/

CMeshwater *CMeshwater::Create(void) {
	CMeshwater *meshwater;
	meshwater = new CMeshwater;
	meshwater -> Init( );
	return meshwater;
}


/*******************************************************************************
* 関数名：GetWaterHeight
* 引数：pos : 現在の位置情報
* 戻り値：coord ： 座標
* 説明：現在地からその座標の高さを計算で求めて返す関数
*******************************************************************************/

float CMeshwater::GetWaterHeight(D3DXVECTOR3 pos) {
	// 変数の宣言
	D3DXVECTOR2 coord = D3DXVECTOR2(0.0f, 0.0f);

	int i, j;
	float height;
	VERTEX_3D *pVtx;
	D3DXVECTOR3 v1, v2;
	float n0, n1, n2;
	bool flag;

	float about_x, about_z;
	int start_x, end_x, start_z, end_z;

	about_x = (MESHWATER_WATER_WIDTH * 0.5f + pos.x) / MESHWATER_WATER_WIDTH * MESHWATER_X_BLOCK;
	about_z = (MESHWATER_WATER_HEIGHT * 0.5f - pos.z) / MESHWATER_WATER_HEIGHT * MESHWATER_Z_BLOCK;

	start_x = (int)about_x - 1;
	end_x = (int)about_x + 1;

	start_z = (int)about_z - 1;
	end_z = (int) about_z + 1;

	if(start_x < 0) {
		start_x = 0;
	}
	if(end_x > MESHWATER_X_BLOCK) {
		end_x = MESHWATER_X_BLOCK;
	}

	if(start_z < 0) {
		start_z = 0;
	}
	if(end_z > MESHWATER_Z_BLOCK) {
		end_z = MESHWATER_Z_BLOCK;
	}

	// 変数の初期化
	n0 = n1 = n2 = height = 0;
	v1 = v2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	flag = false;

	// 頂点情報へのポインタを取得
	m_pVtxBuffMeshwater -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標取得
	for(i = start_z; i < end_z; i++) {
		for(j = start_x; j < end_x; j++) {
			// 対象が右上ポリゴン上にいるか否かの判定
			v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			// 現在のポリゴン上に対象がいた場合
			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				// 法線を計算して正規化
				v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				// 高さ計算
				height = pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.y
						- (v1.x * (pos.x - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.x)
						   + v1.z * (pos.z - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.z) 
						   ) / v1.y;

				// ロックを解除してリターン
				m_pVtxBuffMeshwater -> Unlock( );
				return height;
			}

			// 対象が左下ポリゴン上にいるか否かの判定
			v1 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				v1 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				height = pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.y
						- (v1.x * (pos.x - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.x)
						   + v1.z * (pos.z - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.z) 
						   ) / v1.y;

				m_pVtxBuffMeshwater -> Unlock( );

				return height;
			}
		}
	}
	m_pVtxBuffMeshwater -> Unlock( );

	return height;
}