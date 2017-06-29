/*******************************************************************************
* タイトル GM31 基底クラス
* ファイル名 scene.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/19
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "sceneX.h"
#include "object.h"
#include "enemy.h"

/*******************************************************************************
* staticメンバ変数の初期化
*******************************************************************************/

CScene *CScene::m_top[3] = {NULL};
CScene *CScene::m_cur[3] = {NULL};


/*******************************************************************************
* 関数名：CScene
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CScene::CScene(int priority, OBJTYPE objType) {
	if(m_top[priority] == NULL) {
		m_top[priority] = this;
		m_cur[priority] = this;
		m_prev = NULL;
		m_next = NULL;
	}
	else {
		m_cur[priority] -> m_next = this;
		m_prev = m_cur[priority];
		m_cur[priority] = this;
		m_next = NULL;
	}

	m_objType = objType;
}


/*******************************************************************************
* 関数名：~CScene
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CScene::~CScene( ) {

}

/*******************************************************************************
* 関数名：SetPosition
* 引数：pos : 座標情報
* 戻り値：なし
* 説明：座標をセットする
*******************************************************************************/

void CScene::SetPosition(D3DXVECTOR3 pos) {
	m_position = pos;
}


/*******************************************************************************
* 関数名：GetPosition
* 引数：なし
* 戻り値：m_position : 座標情報
* 説明：座標情報を返す
*******************************************************************************/

D3DXVECTOR3 CScene::GetPosition(void) {
	return m_position;
}


/*******************************************************************************
* 関数名：SetRotation
* 引数：rot : 回転情報
* 戻り値：なし
* 説明：回転角をセットする
*******************************************************************************/

void CScene::SetRotation(D3DXVECTOR3 rot) {
	m_rotation = rot;
}


/*******************************************************************************
* 関数名：GetRotation
* 引数：なし
* 戻り値：m_rotation : 回転情報
* 説明：回転情報を返す
*******************************************************************************/

D3DXVECTOR3 CScene::GetRotation(void) {
	return m_rotation;
}


/*******************************************************************************
* 関数名：UpdateAll
* 引数：なし
* 戻り値：なし
* 説明：リストの全てのUpdateを実行する
*******************************************************************************/

void CScene::UpdateAll(void) {
	for(int i = 0; i < SCENE_PRIORITY_NUM; i++) {
		CScene *scene = m_top[i];
		CScene *next;

		while(scene != NULL) {
			next = scene -> m_next;
			scene -> Update( );
			scene = next;
		}
	}
}


/*******************************************************************************
* 関数名：DrawAll
* 引数：なし
* 戻り値：なし
* 説明：リストの全てのDrawを実行する
*******************************************************************************/

void CScene::DrawAll(void) {
	CScene *keepScene[64];
	int cnt = 0;
	for(int i = 0; i < SCENE_PRIORITY_NUM; i++) {
		CScene *scene = m_top[i];

		while(scene != NULL) {
			if(scene -> m_objType == OBJTYPE_SOLID && ( (CObject*)(scene) )->GetAlphaFlag( ) ) {
				keepScene[cnt] = scene;
				cnt++;
			}
			else {
				scene -> Draw( );
			}
			scene = scene -> m_next;
		}
	}

	for(int i = 0; i < cnt; i++) {
		keepScene[i] -> Draw( );
	}
}


/*******************************************************************************
* 関数名：UninitAll
* 引数：なし
* 戻り値：なし
* 説明：リストの全てのUninitを実行する
*******************************************************************************/

void CScene::UninitAll(void) {
	CScene *next;
	for(int i = 0; i < SCENE_PRIORITY_NUM; i++) {
		CScene *scene = m_top[i];

		while(scene != NULL) {
			next = scene -> m_next;
			scene -> Uninit( );
			delete scene;
			scene = next;
		}
		m_top[i] = NULL;
		m_cur[i] = NULL;
	}
}


/*******************************************************************************
* 関数名：Release
* 引数：なし
* 戻り値：なし
* 説明：オブジェクトをリストから削除する
*******************************************************************************/

void CScene::Release(int priority) {
	if(m_prev != NULL) {
		m_prev -> m_next = m_next;
	}
	else {
		m_top[priority] = m_next;
	}

	if(m_next != NULL) {
		m_next -> m_prev = m_prev;
	}
	else {
		m_cur[priority] = m_prev;
	}

	delete this;
}


/*******************************************************************************
* 関数名：GetList
* 引数：priority : 入手したいリストの優先値
* 戻り値：m_top[priority] : 該当のリストの先頭アドレス
* 説明：指定された優先値のリストの先頭アドレスを返す
*******************************************************************************/

CScene *CScene::GetList(int priority) {
	return m_top[priority];
}


/*******************************************************************************
* 関数名：GetNext
* 引数：なし
* 戻り値：m_next : 次のリストの場所
* 説明：リストの次の場所へのアドレスを取得する
*******************************************************************************/

CScene *CScene::GetNext(void) {
	return m_next;
}


/*******************************************************************************
* 関数名：GetObjType
* 引数：なし
* 戻り値：m_objType : オブジェクトタイプ
* 説明：自分のオブジェクトタイプを返す
*******************************************************************************/

OBJTYPE CScene::GetObjType(void) {
	return m_objType;
}


/*******************************************************************************
* 関数名：HitCheckAll
* 引数：pos : 当たり判定を行う対象の現在地
* 戻り値：int : 当たり判定の結果
* 説明：各オブジェクトとの当たり判定を行い、その結果を返す
*******************************************************************************/

OBJHITCHK CScene::HitCheckAll(D3DXVECTOR3 pos, D3DXVECTOR3 oldPos) {
	OBJHITCHK chk;
	CObject *object;
	for(int i = 0; i < SCENE_PRIORITY_NUM; i++) {
		CScene *scene = m_top[i];

		while(scene != NULL) {
			if(scene -> m_objType == OBJTYPE_SOLID) {
				object = (CObject*)(scene);
				chk = object -> HitCheck(pos, oldPos);
				if(chk.type != 0) {
					if( (int)oldPos.z == -12345) object -> SetAlphaFlag(true);
					return chk;
				}
			}
			scene = scene -> m_next;
		}
	}
	return chk;
}


/*******************************************************************************
* 関数名：ChkEnemyAll
* 引数：なし
* 戻り値：int : 当たり判定の結果
* 説明：各オブジェクトとの当たり判定を行い、その結果を返す
*******************************************************************************/

bool CScene::ChkEnemyAll(void) {
	bool chk;
	CEnemy *enemy;

	for(int i = 0; i < ENEMY_MAX; i++) {
		CScene *scene = m_top[i];

		while(scene != NULL) {
			if(scene -> m_objType == OBJTYPE_ENEMY) {
				enemy = (CEnemy*)(scene);
				chk = enemy ->ChkEnemy( );
				if(chk) return true;
			}
			scene = scene -> m_next;
		}
	}
	return false;
}