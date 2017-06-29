/*******************************************************************************
* �^�C�g�� GM31 2D�|���S���`��
* �t�@�C���� main.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/13
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "debugProc.h"


/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define ID_TIMER (121) // �^�C�}�[��ID


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);


/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/

int FPS = 0;


/*******************************************************************************
* �֐����Fmain
* �����F�Ȃ�
* �߂�l�F(int)msg.wParam
* �����F���b�Z�[�W�{�b�N�X�\��
*******************************************************************************/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) (COLOR_WINDOW + 1),
		NULL,
		"2D",
		NULL
	};

	HWND hWnd;
	MSG msg;

	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	RegisterClassEx(&wcex);

	hWnd = CreateWindowEx(
		0,
		"2D",
		"�@",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hInstance,
		NULL);
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	CManager::Init(hInstance, hWnd, TRUE);

	while(1) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
			if(msg.message == WM_QUIT) {
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
#ifdef _DEBUG
			// �e�o�r�\���p�ϐ�
			static int nFPSCount = 0; // �t���[�����̃J�E���g
			static int nFPSDrawWait = 0; // ����̂e�o�r�\���X�V�܂ł̎���
			char szFPS[256];

			// �e�o�r����p
			static int nLastTime = 0; // ����̍X�V����
			int nNewTime = timeGetTime(); // ���݂̎���

			// �X�V
			if(nLastTime <= nNewTime)
			{
				nLastTime = nNewTime + 16; // �P�U�����Ɉ�x�X�V����i��U�O�e�o�r�j
				nFPSCount++; // �t���[�����J�E���g�ǉ�
				// �e�o�r�\���X�V
				if(nFPSDrawWait <= nNewTime)
				{
					FPS = (int)(nFPSCount * 1000) / (nNewTime - nFPSDrawWait + 1000);
					nFPSDrawWait = nNewTime + 1000;	//����e�o�r�\���X�V�͂P�O�O�O�����i�P�b�j��
					nFPSCount = 0;	//�t���[�����J�E���g���Z�b�g
				}
			}
#endif

			CManager::Update( );
			CManager::Draw( );
		}
	}

	CManager::Uninit( );

	UnregisterClass("2D", wcex.hInstance);
	return (int)msg.wParam;
}


/*******************************************************************************
* �֐����FWndProc
* �����FhWnd, uMsg, wParam, lParam
* �߂�l�FDefWindowProc(hWnd, uMsg, wParam, lParam)
* �����F�E�C���h�E�̐ݒ�
*******************************************************************************/

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int nID; // ���b�Z�[�W�{�b�N�X����̖߂�l�ۑ��p

	switch(uMsg) {
	case WM_DESTROY: // �E�C���h�E���j�����ꂽ
		PostQuitMessage(0);
		break;

	case WM_CLOSE: // �o�c�{�^���������ꂽ���̏���
			nID = MessageBox(hWnd, "�I�����Ă�낵���ł����H", "�v���O�����̏I��", MB_YESNO);
			if(nID == IDNO) {
				return 0;
			}
		break;

	case WM_CREATE: // �E�C���h�E���������ꂽ

		SetTimer(hWnd, ID_TIMER, 1000/60, NULL);

		break;

	case WM_KEYDOWN: // �L�[�������ꂽ
		switch(wParam){
		case VK_ESCAPE: // Esc�L�[
			nID = MessageBox(hWnd, "�I�����Ă�낵���ł����H", "�v���O�����̏I��", MB_YESNO);
			if(nID == IDYES) {
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_LBUTTONDOWN: // �}�E�X���N���b�N
		SetFocus(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


/*******************************************************************************
* �֐����FInit
* �����FhInstance, hWnd, bWindow
* �߂�l�FE_FAIL(�������s��) or S_OK(����������)
* �����F����������
*******************************************************************************/

void Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow) {

}

/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I������
*******************************************************************************/

void Uninit(void) {

}

/*******************************************************************************
* �֐����FUpdate
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�V����
*******************************************************************************/

void Update(void) {

}


/*******************************************************************************
* �֐����FDraw
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�`�揈��
*******************************************************************************/

void Draw(void) {

}


/*******************************************************************************
* �֐����FDrawFPS
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����FFPS��\��
*******************************************************************************/

void DrawFPS(void) {
//	char aStr[1024];
	// �����ݒ�
//	sprintf(&aStr[0], "[�J�����̎��_       : (%3.2f : %3.2f : %3.2f)]\n[�J�����̒����_     : (%3.2f : %3.2f : %3.2f)]\n[�J�����̌���       : (%3.2f)]\n\n*** �J�������� ***\n���_�㏸�@�@ �F [ �x ]\n���_���~�@�@ �F [ �m ]\n�����_�㏸�@ �F [ �s ]\n�����_���~�@ �F [ �a ]\n�Y�[���C���@ �F [ �t ]\n�Y�[���A�E�g �F [ �l ]\n\n*** ��s�@���� ***\n[��s�@�̈ʒu : (%3.2f : %3.2f : %3.2f)]\n�O�ړ� �F [ �v ]\n��ړ� �F [ �r ]\n���ړ� �F [ �` ]\n�E�ړ� �F [ �c ]\n�㏸�@ �F [ �h ]\n���~�@ �F [ �j ]\n\n[��s�@�̌��� : (%3.2f)]\n������ : [ LSHIFT ]\n�E���� : [ RSHIFT ]\n\n�e���� : [ SPACE ]\n", camera.posV.x, camera.posV.y, camera.posV.z, camera.posR.x, camera.posR.y, camera.posR.z, camera.rot.y * D3DX_PI, model.pos.x, model.pos.y, model.pos.z, model.rot.y);
	// �\��
	PrintDebugProc("FPS:%d\n", FPS);
}