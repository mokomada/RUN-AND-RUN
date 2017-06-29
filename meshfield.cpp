/*******************************************************************************
* タイトル フィールド描画処理
* ファイル名 meshfield.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/5/18
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "meshfield.h"
#include "manager.h"
#include "sceneX.h"
#include "player.h"
#include "debugProc.h"


LPDIRECT3DVERTEXBUFFER9 CMeshfield::m_pVtxBuffMeshfield = NULL;


/*******************************************************************************
* グローバル変数
*******************************************************************************/



/*******************************************************************************
* 関数名：CMeshfield
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CMeshfield::CMeshfield(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* 関数名：~CMeshfield
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CMeshfield::~CMeshfield( ) {

}


/*******************************************************************************
* 関数名：InitMeshfield
* 引数：なし
* 戻り値：なし
* 説明：フィールド設定の初期化
*******************************************************************************/

void CMeshfield::Init(void) {
	// 変数宣言
	int i, j, cntx, cntz; // forループ用
	VERTEX_3D *pVtx;
	WORD *pIdx;
	D3DXVECTOR3 n0, v1, v2; // 法線計算用

	// 変数の取得
	CRenderer *renderer; // デバイス取得用変数
	renderer = CManager::GetRenderer( ); // レンダラのアドレス取得
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得

	float heightMap[MESHFIELD_X_BLOCK + 1][MESHFIELD_Z_BLOCK + 1] = {
//		  1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41
		{300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300}, // 1
		{300, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 300}, // 2
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 3
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 4
		{300, 350, 360, 360, 350, 350, 350, 350, 360, 350, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 5
		{300, 350, 360, 360, 350, 300, 300, 300, 350, 340, 340, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 6
		{300, 350, 360, 360, 350, 300, 300, 300, 350, 340, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 7
		{300, 350, 360, 360, 350, 300, 300, 300, 340, 330, 340, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 8
		{300, 350, 360, 360, 360, 350, 350, 350, 350, 340, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 9
		{300, 350, 360, 370, 370, 360, 360, 360, 360, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 10
		{300, 360, 370, 380, 380, 370, 360, 370, 370, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 11
		{300, 350, 360, 370, 370, 360, 360, 360, 370, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 12
		{300, 350, 360, 360, 360, 350, 350, 360, 360, 360, 360, 360, 370, 368, 371, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 13
		{300, 350, 360, 350, 350, 340, 340, 350, 360, 360, 360, 360, 372, 380, 382, 373, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 14
		{300, 350, 360, 350, 340, 330, 340, 350, 360, 360, 360, 360, 367, 377, 380, 370, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 15
		{300, 350, 360, 350, 340, 330, 340, 350, 360, 360, 360, 360, 360, 373, 369, 371, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 16
		{300, 350, 360, 360, 350, 340, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 17
		{300, 350, 360, 360, 360, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 18
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 19
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 20
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 360, 360, 360, 350, 300}, // 21
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 360, 350, 300}, // 22
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 330, 330, 330, 330, 330, 330, 330, 330, 360, 360, 340, 360, 360, 350, 300}, // 23
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 370, 370, 370, 370, 370, 370, 330, 330, 360, 340, 360, 360, 350, 300}, // 24
		{300, 350, 352, 345, 345, 343, 345, 342, 346, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 320, 320, 320, 320, 370, 370, 330, 360, 340, 360, 360, 350, 300}, // 25
		{300, 350, 350, 345, 330, 330, 328, 331, 327, 330, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 380, 380, 320, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 26
		{300, 347, 354, 345, 330, 322, 320, 318, 320, 320, 330, 343, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 420, 380, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 27
		{300, 343, 350, 342, 332, 320, 321, 320, 315, 320, 330, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 420, 420, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 28
		{300, 346, 343, 345, 334, 321, 323, 325, 320, 321, 332, 340, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 420, 420, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 29
		{300, 345, 348, 348, 331, 320, 320, 320, 318, 320, 330, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 420, 420, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 30
		{300, 348, 350, 345, 328, 317, 315, 316, 320, 322, 333, 343, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 420, 420, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 31
		{300, 350, 360, 342, 330, 320, 318, 320, 320, 320, 330, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 320, 380, 380, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 32
		{300, 350, 360, 360, 345, 330, 328, 330, 330, 330, 330, 348, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 370, 320, 320, 320, 320, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 33
		{300, 350, 360, 360, 360, 347, 345, 348, 345, 341, 344, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 330, 370, 370, 370, 370, 370, 370, 370, 330, 360, 340, 360, 360, 350, 300}, // 34
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 360, 330, 330, 330, 330, 330, 330, 330, 330, 330, 360, 340, 360, 360, 350, 300}, // 35
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 360, 350, 300}, // 36
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 360, 360, 350, 300}, // 37
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 38
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 39
		{300, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 300}, // 40
		{300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300}, // 41
	}; // 高さ初期値設定用

	// メンバ変数の初期化
	m_pTextureMeshfield = NULL;
	m_pVtxBuffMeshfield = NULL;
	m_pIdxBuffMeshfield = NULL;



	// 乱数設定
	srand( (unsigned) time(NULL) );

	// 高さ初期値設定

	for(cntz = 0; cntz < MESHFIELD_Z_BLOCK + 1; cntz++) {
		for(cntx = 0; cntx < MESHFIELD_X_BLOCK + 1; cntx++) {
			m_HeightMap[cntx][cntz] = 300.0f + rand( ) % 80;
			if(cntx != 0) {
				if(m_HeightMap[cntx][cntz] > m_HeightMap[cntx - 1][cntz] + MESHFIELD_DIFFERENCE_MAX) {
					m_HeightMap[cntx][cntz] = m_HeightMap[cntx - 1][cntz] + MESHFIELD_DIFFERENCE_MAX;
				}
				else if(m_HeightMap[cntx][cntz] < m_HeightMap[cntx - 1][cntz] - MESHFIELD_DIFFERENCE_MAX) {
					m_HeightMap[cntx][cntz] = m_HeightMap[cntx - 1][cntz] - MESHFIELD_DIFFERENCE_MAX;
				}
			}
			if(cntz != 0) {
				if(m_HeightMap[cntx][cntz] > m_HeightMap[cntx][cntz - 1] + MESHFIELD_DIFFERENCE_MAX) {
					m_HeightMap[cntx][cntz] = m_HeightMap[cntx][cntz - 1] + MESHFIELD_DIFFERENCE_MAX;
				}
				else if(m_HeightMap[cntx][cntz] < m_HeightMap[cntx][cntz - 1] - MESHFIELD_DIFFERENCE_MAX) {
					m_HeightMap[cntx][cntz] = m_HeightMap[cntx][cntz - 1] - MESHFIELD_DIFFERENCE_MAX;
				}
			}
			m_HeightMap[cntx][cntz] = heightMap[cntx][cntz];
		}
	}

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, MESHFIELD_TEXTURE_NAME, &m_pTextureMeshfield);

	// 頂点バッファの生成
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffMeshfield, NULL);

	// 頂点情報へのポインタを取得
	m_pVtxBuffMeshfield -> Lock(0, 0, (void**)&pVtx, 0);

	// 角度初期設定
	m_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_sclMeshfield = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	for(i = 0; i < MESHFIELD_VERTEX_NUM; i++) {
		// 座標設定
		pVtx[i].pos = D3DXVECTOR3( 
			(float) (MESHFIELD_FIELD_X + (MESHFIELD_FIELD_WIDTH / (float)MESHFIELD_X_BLOCK) * (i % (MESHFIELD_X_BLOCK + 1) ) ), 
			m_HeightMap[i / (MESHFIELD_X_BLOCK + 1)][i % (MESHFIELD_Z_BLOCK + 1)], 
			(float) (MESHFIELD_FIELD_Z - (MESHFIELD_FIELD_HEIGHT / (float)MESHFIELD_Z_BLOCK) * (i / (MESHFIELD_X_BLOCK + 1) ) ) );

		// 頂点色設定
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// 法線設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// テクスチャUV座標設定
		pVtx[i].tex = D3DXVECTOR2( (1 / (float)MESHFIELD_TEXTURE_X_BLOCK) * (i % (MESHFIELD_X_BLOCK + 1) ), (1 / (float)MESHFIELD_TEXTURE_Z_BLOCK) * (i / (MESHFIELD_X_BLOCK + 1) ) );
	}

	for(i = 0; i < MESHFIELD_Z_BLOCK + 1; i++) {
		for(j = 0; j < MESHFIELD_X_BLOCK + 1; j++) {
			// 初期化
			n0 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 最初の行の処理
			if(i == 0) {
				// 最初の列の処理
				if(j == 0) {
					// 頂点に接する各ポリゴンの法線を求める
					// 1つ目のポリゴン
					v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					// 外積を求める
					D3DXVec3Cross(&v1, &v1, &v2);
					// 求めた法線を正規化して足し合わせる
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					// 2つ目のポリゴン v2は値が残っているので使い回し
					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					// 外積を求める　変数を使いまわしてるので外積を求める際のベクトルの順番に注意
					D3DXVec3Cross(&v2, &v2, &v1);
					// 求めた法線を正規化して足し合わせる
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					// 足し合わせた法線を正規化
					D3DXVec3Normalize(&n0, &n0);
					// 得られた頂点の法線を代入
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// 最後の列の処理
				else if(j == MESHFIELD_X_BLOCK) {
					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// それ以外の列の処理
				else{
					v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
			}
			// 最後の行の処理
			else if(i == MESHFIELD_Z_BLOCK) {
				// 最初の列の処理
				if(j == 0) {
					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// 最後の列の処理
				else if(j == MESHFIELD_X_BLOCK) {
					v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// それ以外の列の処理
				else{
					v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
			}
			// それ以外の行の処理
			else{
				// 最初の列の処理
				if(j == 0) {
					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// 最後の列の処理
				else if(j == MESHFIELD_X_BLOCK) {
					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// それ以外の列の処理
				else{
					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2,&v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
			}
		}
	}

	// 高さによって色変え
	for(i = 0; i < MESHFIELD_Z_BLOCK + 1; i++) {
		for(j = 0; j < MESHFIELD_X_BLOCK + 1; j++) {
			if(pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.y > 340) {
				pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].col = D3DCOLOR_RGBA(64, 255, 64, 255);
			}
			else if(pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.y > 290) {
				pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].col = D3DCOLOR_RGBA(130, 90, 20, 255);
			}
			else{
				pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].col = D3DCOLOR_RGBA(64, 64, 255, 255);
			}
		}
	}

	m_pVtxBuffMeshfield -> Unlock( );

	// インデックスバッファ確保
	device -> CreateIndexBuffer(sizeof(WORD) * MESHFIELD_INDEX_NUM,
								 D3DUSAGE_WRITEONLY,
								 D3DFMT_INDEX16,
								 D3DPOOL_MANAGED,
								 &m_pIdxBuffMeshfield,
								 NULL);

	m_pIdxBuffMeshfield -> Lock(0, 0, (void**) &pIdx, 0);

	for(i = 0; i < MESHFIELD_Z_BLOCK; i++) {
		if(i != 0) {
			pIdx[i * (MESHFIELD_X_BLOCK + 2) * 2 - 1] = (i + 1) * (MESHFIELD_X_BLOCK + 1);
		}

		for(j = 0; j < (MESHFIELD_X_BLOCK + 1) * 2; j++) {
			pIdx[i * (MESHFIELD_X_BLOCK + 2) * 2 + j] = (i + 1 - j % 2) * (MESHFIELD_X_BLOCK + 1) + (j / 2);
		}

		j--;
		if(i < MESHFIELD_Z_BLOCK - 1) {
			pIdx[i * (MESHFIELD_X_BLOCK + 2) * 2 + j + 1] = (i + 1 - j % 2) * (MESHFIELD_X_BLOCK + 1) + (j / 2);
		}
	}

	m_pIdxBuffMeshfield -> Unlock( );
}


/*******************************************************************************
* 関数名：UninitMeshfield
* 引数：なし
* 戻り値：なし
* 説明：フィールドの終了処理
*******************************************************************************/

void CMeshfield::Uninit(void) {
	if(m_pTextureMeshfield != NULL) {
		m_pTextureMeshfield -> Release( ); // テクスチャの開放
		m_pTextureMeshfield = NULL;
	}

	if(m_pVtxBuffMeshfield != NULL) {
		m_pVtxBuffMeshfield -> Release( );
		m_pVtxBuffMeshfield = NULL;
	}

	if(m_pIdxBuffMeshfield != NULL) {
		m_pIdxBuffMeshfield -> Release( );
		m_pIdxBuffMeshfield = NULL;
	}
}


/*******************************************************************************
* 関数名：UpdataMeshfield
* 引数：なし
* 戻り値：なし
* 説明：フィールドの情報更新
*******************************************************************************/

void CMeshfield::Update(void) {
/*	MODEL model;
	model = CPlayer::GetPlayer( );

	D3DXVECTOR2 model_pos = D3DXVECTOR2(0.0f, 0.0f);

	int i, j;
	VERTEX_3D *pVtx;
	D3DXVECTOR3 v1, v2;
	float n0, n1, n2;
	float about_x, about_z;
	int start_x, end_x, start_z, end_z;

	// プレイヤーの座標を割り出し、その前後の整数を取得してフィールドとの当たり判定に用いる
	about_x = (MESHFIELD_FIELD_WIDTH * 0.5f + model.pos.x) / MESHFIELD_FIELD_WIDTH * MESHFIELD_X_BLOCK;
	about_z = (MESHFIELD_FIELD_HEIGHT * 0.5f - model.pos.z) / MESHFIELD_FIELD_HEIGHT * MESHFIELD_Z_BLOCK;

	start_x = (int)about_x - 1;
	end_x = (int)about_x + 1;

	start_z = (int)about_z - 1;
	end_z = (int) about_z + 1;

	if(start_x < 0) {
		start_x = 0;
	}
	if(end_x > MESHFIELD_X_BLOCK) {
		end_x = MESHFIELD_X_BLOCK;
	}

	if(start_z < 0) {
		start_z = 0;
	}
	if(end_z > MESHFIELD_Z_BLOCK) {
		end_z = MESHFIELD_Z_BLOCK;
	}
	
	n0 = n1 = n2 = 0;
	v1 = v2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点情報へのポインタを取得
	m_pVtxBuffMeshfield -> Lock(0, 0, (void**)&pVtx, 0);

	for(i = start_z; i < end_z; i++) {
		for(j = start_x; j < end_x; j++) {
			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = model.pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = model.pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = model.pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				model_pos = D3DXVECTOR2( (float)j, (float)i);
				break;
			}

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = model.pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = model.pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = model.pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				model_pos = D3DXVECTOR2( (float)j, (float)i);
				break;
			}
		}
		if(model_pos.x != 0 && model_pos.y !=0) {
			break;
		}
	}

	m_pVtxBuffMeshfield -> Unlock( );

	PrintDebugProc("現在地座標(%f, %f)\n", model_pos.x, model_pos.y);*/
}


/*******************************************************************************
* 関数名：DrawMeshfield
* 引数：なし
* 戻り値：なし
* 説明：フィールドの描画処理
*******************************************************************************/

void CMeshfield::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // デバイス取得
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // 計算用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorldMeshfield);
	// ワールドマトリックスの作成
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f); // ※※※ y, x, z の順番に注意！！！ ※※※
	D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxTrans);

	// ワールドマトリックスの設定
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorldMeshfield);

	// インデックスバッファをバインド
	device -> SetIndices(m_pIdxBuffMeshfield);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	device -> SetTexture(0, m_pTextureMeshfield);

	// ポリゴンの描画
/*	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		MESHFIELD_TRIANGLE_NUM);*/

	device -> DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		MESHFIELD_VERTEX_NUM,
		0,
		MESHFIELD_TRIANGLE_NUM);
}


/*******************************************************************************
* 関数名：Create
* 引数：CMeshfield : アドレス
* 戻り値：なし
* 説明：オブジェクトの生成
*******************************************************************************/

CMeshfield *CMeshfield::Create(void) {
	CMeshfield *meshfield;
	meshfield = new CMeshfield;
	meshfield -> Init( );
	return meshfield;
}


/*******************************************************************************
* 関数名：GetFieldHeight
* 引数：pos : 現在の位置情報
* 戻り値：height ： 高さ
* 説明：現在地からその座標の高さを計算で求めて返す関数
*******************************************************************************/

float CMeshfield::GetFieldHeight(D3DXVECTOR3 pos) {
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

	about_x = (MESHFIELD_FIELD_WIDTH * 0.5f + pos.x) / MESHFIELD_FIELD_WIDTH * MESHFIELD_X_BLOCK;
	about_z = (MESHFIELD_FIELD_HEIGHT * 0.5f - pos.z) / MESHFIELD_FIELD_HEIGHT * MESHFIELD_Z_BLOCK;

	start_x = (int)about_x - 1;
	end_x = (int)about_x + 1;

	start_z = (int)about_z - 1;
	end_z = (int) about_z + 1;

	if(start_x < 0) {
		start_x = 0;
	}
	if(end_x > MESHFIELD_X_BLOCK) {
		end_x = MESHFIELD_X_BLOCK;
	}

	if(start_z < 0) {
		start_z = 0;
	}
	if(end_z > MESHFIELD_Z_BLOCK) {
		end_z = MESHFIELD_Z_BLOCK;
	}

	// 変数の初期化
	n0 = n1 = n2 = height = 0;
	v1 = v2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	flag = false;

	// 頂点情報へのポインタを取得
	m_pVtxBuffMeshfield -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標取得
	for(i = start_z; i < end_z; i++) {
		for(j = start_x; j < end_x; j++) {
			// 対象が右上ポリゴン上にいるか否かの判定
			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			// 現在のポリゴン上に対象がいた場合
			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				// 法線を計算して正規化
				v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				// 高さ計算
				height = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.y
						- (v1.x * (pos.x - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.x)
						   + v1.z * (pos.z - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.z) 
						   ) / v1.y;

				// ロックを解除してリターン
				m_pVtxBuffMeshfield -> Unlock( );
				return height;
			}

			// 対象が左下ポリゴン上にいるか否かの判定
			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				height = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.y
						- (v1.x * (pos.x - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.x)
						   + v1.z * (pos.z - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.z) 
						   ) / v1.y;

				m_pVtxBuffMeshfield -> Unlock( );

				return height;
			}
		}
	}
	m_pVtxBuffMeshfield -> Unlock( );

	return height;
}


/*******************************************************************************
* 関数名：GetFieldNorm
* 引数：pos : 現在の位置情報
* 戻り値：v1 ： 法線
* 説明：現在地からその座標の法線を計算で求めて返す関数
*******************************************************************************/

D3DXVECTOR3 CMeshfield::GetFieldNorm(D3DXVECTOR3 pos) {
	// 変数の宣言
	D3DXVECTOR2 coord = D3DXVECTOR2(0.0f, 0.0f);

	int i, j;
	VERTEX_3D *pVtx;
	D3DXVECTOR3 v1, v2;
	float n0, n1, n2;
	bool flag;

	float about_x, about_z;
	int start_x, end_x, start_z, end_z;

	about_x = (MESHFIELD_FIELD_WIDTH * 0.5f + pos.x) / MESHFIELD_FIELD_WIDTH * MESHFIELD_X_BLOCK;
	about_z = (MESHFIELD_FIELD_HEIGHT * 0.5f - pos.z) / MESHFIELD_FIELD_HEIGHT * MESHFIELD_Z_BLOCK;

	start_x = (int)about_x - 1;
	end_x = (int)about_x + 1;

	start_z = (int)about_z - 1;
	end_z = (int) about_z + 1;

	if(start_x < 0) {
		start_x = 0;
	}
	if(end_x > MESHFIELD_X_BLOCK) {
		end_x = MESHFIELD_X_BLOCK;
	}

	if(start_z < 0) {
		start_z = 0;
	}
	if(end_z > MESHFIELD_Z_BLOCK) {
		end_z = MESHFIELD_Z_BLOCK;
	}

	// 変数の初期化
	n0 = n1 = n2 = 0;
	v1 = v2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	flag = false;

	// 頂点情報へのポインタを取得
	m_pVtxBuffMeshfield -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標取得
	for(i = start_z; i < end_z; i++) {
		for(j = start_x; j < end_x; j++) {
			// 対象が右上ポリゴン上にいるか否かの判定
			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			// 現在のポリゴン上に対象がいた場合
			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				// 法線を計算して正規化
				v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				return v1;
			}

			// 対象が左下ポリゴン上にいるか否かの判定
			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				return v1;
			}
		}
	}
	m_pVtxBuffMeshfield -> Unlock( );

	return v1;
}