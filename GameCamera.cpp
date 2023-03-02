#include "GameCamera.h"
#include <windef.h>
#include "WinApp.h"
#include "MyMath.h"


GameCamera::GameCamera(int window_width, int window_height)
{

	input_ = Input::GetInstance();

	easing_ = new Easing();

	winWidth = window_width;
	winHeight = window_height;

	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX_ = 1.0f / (float)window_width;
	scaleY_ = 1.0f / (float)window_height;

	bool dirty = false;
	float angleX = 0;
	float angleY = 0;

	MaxCameraTime = 300;
	cameraTime = MaxCameraTime;
	oldMousePos = mousePos;
	mousePos = input_->GetMousePos();

	// �ǉ���]���̉�]�s��𐶐�
	Matrix4 matRotNew;
	matRotNew.rotateX(-angleX);
	matRotNew.rotateY(-angleY);

	MultiplyMatrix(matRotNew);

	worldTransform_.Initialize();
	EnemyWorld_.Initialize();
	EnemyWorld_.translation_ = Vector3(0, 0, 0);
	EnemyWorld_.TransferMatrix();
}

GameCamera::~GameCamera()
{
	delete easing_;
}

void GameCamera::Initialize() {



}

void GameCamera::Update(ViewProjection* viewProjection_) {

	PlaySceneCamera(viewProjection_);

}

void GameCamera::PlaySceneCamera(ViewProjection* viewProjection_) {

	if (spaceInput == true) {
		cameraTime = 0;
	}

	if (cameraTime < MaxCameraTime) {
		cameraTime++;
	}
	

	//�J�����̉�]�x�N�g��
	Vector3 rotat = { 0, 0, 0 };
	//�J�����̈ړ��̑���
	const float cameraSpeed = 0.0005f;

	Vector2 windowWH = Vector2(winWidth / 2, winHeight / 2);
	POINT mousePosition;
	//�}�E�X���W(�X�N���[�����W)���擾����
	GetCursorPos(&mousePosition);

	//�N���C�A���g�G���A���W�ɕϊ�����
	HWND hwnd = WinApp::GetInstance()->Gethwnd();
	ScreenToClient(hwnd, &mousePosition);

	int xPos_absolute, yPos_absolute;

	int xPos = windowWH.x;  //�ړ��������������W�i�E�B���h�E���̑��΍��W�j
	int yPos = windowWH.y; //�ړ��������������W�i�E�B���h�E���̑��΍��W�j

	WINDOWINFO windowInfo;
	//�E�B���h�E�̈ʒu���擾
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//�}�E�X�̈ړ���̐�΍��W�i���j�^�[���ォ��̍��W�j
	xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//�Ȃ񂩂���Ă邩�璼��
	yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //�E�B���h�E�̃^�C�g���o�[�̕��i31px�j���v���X
	SetCursorPos(xPos_absolute, yPos_absolute);//�ړ�������

	//�}�E�X�̈ړ��ʂ��擾
	MouseMove = Vector2(0, 0);
	MouseMove = (Vector2(mousePosition.y, mousePosition.x) - Vector2(windowWH.y, windowWH.x));//���W���ŉ�]���Ă���֌W�ł����Ȃ�(X��Y������ւ�)

	mouseMoved += Vector2(MouseMove.x, MouseMove.y) / 500;

	//�J��������
	if (mouseMoved.x < -0.80f) {
		mouseMoved.x = -0.80f;
	}
	else if (mouseMoved.x > 1.30f) {
		mouseMoved.x = 1.30f;
	}

	Vector3 rotation = Vector3(-mouseMoved.x, mouseMoved.y, 0);

	Matrix4 cameraRot;


	cameraRot = MyMath::Rotation(rotation, 6);

	rot = rotation;
	CameraRot = cameraRot;

	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, playerCameraDistance);
	//���[���J�����̉�]�𔽉f
	forward = MyMath::MatVector(cameraRot, forward);

	Vector3 pos = playerPos;

	target = easing_->InOutVec3(target, playerPos, cameraTime, MaxCameraTime);

	//target = pos;
	vTargetEye = target + (forward * playerCameraDistance);

}

void GameCamera::MultiplyMatrix(Matrix4& matrix) {
	// �ݐς̉�]�s�������
	matRot = matrix * matRot;

	// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
	vTargetEye = { 0.0f, 0.0f, -distance_ };
	vUp = { 0.0f, 1.0f, 0.0f };

	// �x�N�g������]
	vTargetEye = MyMath::MatVector(matRot, vTargetEye);

}
