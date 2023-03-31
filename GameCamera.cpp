#include "GameCamera.h"
#include <windef.h>
#include "WinApp.h"
#include "MyMath.h"
#include"ImGuiManager.h"


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

	MaxCameraTime = 200;
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

	cameraPos = { 5,5,5 };
}

GameCamera::~GameCamera()
{
	delete easing_;
}

void GameCamera::Initialize() {



}

void GameCamera::Update(ViewProjection* viewProjection_) {

	if (input_->TriggerKey(DIK_F1)) {
		if (cameraMode == false) {
			cameraMode = true;
		}
		else {
			cameraMode = false;
		}
	}
	if (cameraMode == false) {
		/*if (input_->PushKey(DIK_LSHIFT)) {
			PlayerLockOnCamera(viewProjection_);
		}
		else {
			PlaySceneCamera(viewProjection_);
		}*/
		PlaySceneCamera(viewProjection_);
	}
	else {
		ImGui::Begin("camera");
		ImGui::SliderFloat("eye:x",&vTargetEye.x,-100.0f,100.0f);
		ImGui::SliderFloat("eye:y",&vTargetEye.y,-100.0f,700.0f);
		ImGui::SliderFloat("eye:z",&vTargetEye.z,-100.0f,100.0f);

		ImGui::SliderFloat("target:x", &target.x, -100.0f, 100.0f);
		ImGui::SliderFloat("target:y", &target.y, -100.0f, 100.0f);
		ImGui::SliderFloat("target:z", &target.z, -100.0f, 100.0f);

		ImGui::End();
	}
}

void GameCamera::PlaySceneCamera(ViewProjection* viewProjection_) {

	oldCameraPos = vTargetEye;

	if (spaceInput == true) {
		cameraTime = 0;
	}

	if (cameraTime < MaxCameraTime) {
		cameraTime++;
	}
	if (shakeTime > 0) {
		shakeTime--;
	}
	else {
		isShake = false;
	}

	//�J�����̉�]�x�N�g��
	Vector3 rotat = { 0, 0, 0 };
	//�J�����̈ړ��̑���
	const float cameraSpeed = 0.0005f;

	Vector2 windowWH = Vector2(winWidth / 2, winHeight / 2);
	POINT mousePosition;
	//�}�E�X���W(�X�N���[�����W)��擾����
	GetCursorPos(&mousePosition);

	//�N���C�A���g�G���A���W�ɕϊ�����
	HWND hwnd = WinApp::GetInstance()->Gethwnd();
	ScreenToClient(hwnd, &mousePosition);

	int xPos_absolute, yPos_absolute;

	int xPos = windowWH.x;  //�ړ��������������W�i�E�B���h�E��̑��΍��W�j
	int yPos = windowWH.y; //�ړ��������������W�i�E�B���h�E��̑��΍��W�j

	WINDOWINFO windowInfo;
	//�E�B���h�E�̈ʒu��擾
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//�}�E�X�̈ړ���̐�΍��W�i���j�^�[���ォ��̍��W�j
	xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//�Ȃ񂩂���Ă邩�璼��
	yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //�E�B���h�E�̃^�C�g���o�[�̕��i31px�j��v���X
	SetCursorPos(xPos_absolute, yPos_absolute);//�ړ�������

	//�}�E�X�̈ړ��ʂ�擾
	MouseMove = Vector2(0, 0);
	MouseMove = (Vector2(mousePosition.y, mousePosition.x) - Vector2(windowWH.y, windowWH.x));//���W���ŉ�]���Ă���֌W�ł����Ȃ�(X��Y������ւ�)

	if (input_->PushKey(DIK_LSHIFT) == 0) {
		mouseMoved += Vector2(MouseMove.x, MouseMove.y) / 500;
	}

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
	//cameraRot *= viewProjection_->matView;

	rot = rotation;
	CameraRot = cameraRot;




	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, playerCameraDistance);
	//���[���J�����̉�]�𔽉f
	forward = MyMath::MatVector(CameraRot, forward);

	target = easing_->InOutVec3(target, playerPos_, cameraTime, MaxCameraTime);

	//target = pos;
	vTargetEye = target + (forward * playerCameraDistance);

	if (input_->PushKey(DIK_LSHIFT)) {

		//if (input_->TriggerKey(DIK_F)) {	//�J�����̃��[�h�؂�ւ�
		//	if (cameraMode_ == 0) {
		//		cameraMode_ = 1;
		//	}
		//	else if (cameraMode_ == 1) {
		//		cameraMode_ = 0;
		//	}
		//	else {
		//		cameraMode_ = 0;
		//	}
		//}

		//�J�����̒����_�i���j
		target = EnemyPos_;

		//�J�����̈ʒu
		Vector3 eyeVec = playerPos_ - EnemyPos_;

		Vector3 eyePos = eyeVec;

		float mag = 1.0f;
		float eyeLen = std::sqrt(eyePos.x * eyePos.x + eyePos.y * eyePos.y + eyePos.z * eyePos.z);	//�x�N�g���̒���

		if (eyeLen > 1.0f) {	//��������̃x�N�g�����P�ʃx�N�g�����傫��������
			mag = 1.0f / eyeLen; //�x�N�g���̒�����1�ɂ���
		};

		eyePos.x *= mag;	//mag�������Ɛ��K�������
		eyePos.y *= mag;
		eyePos.z *= mag;


		if (cameraMode_ == 0) {
			if (cameraModeChangeCountTimer < MAX_CHANGE_TIMER) {
				cameraModeChangeCountTimer++;
			}
		}
		else if (cameraMode_ == 1) {
			if (cameraModeChangeCountTimer > 0) {
				cameraModeChangeCountTimer--;
			}
		}

		cameraDistance_ = easing_->InOut(MIN_CAMERA_DISTANCE, MAX_CAMERA_DISTANCE, cameraModeChangeCountTimer, MAX_CHANGE_TIMER);
		cameraHeight_ = easing_->InOut(3, 6, cameraModeChangeCountTimer, MAX_CHANGE_TIMER);

		Vector3 primalyCamera =
		{ playerPos_.x + eyePos.x * cameraDistance_,//���@����������ʒu�ɃJ������Z�b�g
		cameraHeight_,
		playerPos_.z + eyePos.z * cameraDistance_ };

		float eyeVecAngle = atan2f(primalyCamera.x - EnemyPos_.x, primalyCamera.z - EnemyPos_.z);//�J��������炷�ۂɎg����

		float shiftLen = 0.0f;	//���炷��
		Vector3 shiftVec = { primalyCamera.x + sinf(eyeVecAngle + PI / 2) * shiftLen,primalyCamera.y,primalyCamera.z + cosf(eyeVecAngle + PI / 2) * shiftLen };

		rot = MyMath::MatVector(viewProjection_->matView, rot);

		vTargetEye = shiftVec;
	}

	if (input_->PushKey(DIK_UP)) {
		cameraDis += 0.1f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		cameraDis -= 0.1f;
	}

	CameraAngle(vTargetEye.z - target.z, vTargetEye.x - target.x);


	//�x���J����
	//����
	Vector3 dVec = vTargetEye - cameraPos;
	dVec *= cameraDelay;
	cameraPos += dVec * cameraSpeed_;
	Vector3 player_camera = cameraPos - playerPos_;
	player_camera.normalize();
	cameraPos = playerPos_ + (player_camera * cameraDis);


	float distance = sqrt((vTargetEye.x - playerPos_.x) * (vTargetEye.x - playerPos_.x)
		+ (vTargetEye.y - playerPos_.y) * (vTargetEye.y - playerPos_.y)
		+ (vTargetEye.z - playerPos_.z) * (vTargetEye.z - playerPos_.z));

	float distance2 = sqrt((cameraPos.x - playerPos_.x) * (cameraPos.x - playerPos_.x)
		+ (cameraPos.y - playerPos_.y) * (cameraPos.y - playerPos_.y)
		+ (cameraPos.z - playerPos_.z) * (cameraPos.z - playerPos_.z));


	ImGui::Text("vTargetEye : %f", cameraDis);
	//ImGui::Text("vTargetEye : %f,%f,%f", vTargetEye.x, vTargetEye.y, vTargetEye.z);

	//if (isHit == true) {
	//	isHit = false;
	//	isShake = true;
	//	shakeTime = 10;

	//}

	if (isShake == true) {
		vTargetEye += Vector3(rand() % 4, rand() % 4, rand() % 4);
	}
}

void GameCamera::PlayerLockOnCamera(ViewProjection* viewProjection_)
{
	Vector3 PlayerAndEnemy = (playerPos_ + EnemyPos_) / 2;

	target = EnemyPos_;

}

void GameCamera::MultiplyMatrix(Matrix4& matrix) {
	// �ݐς̉�]�s������
	matRot = matrix * matRot;

	// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
	vTargetEye = { 0.0f, 0.0f, -distance_ };
	vUp = { 0.0f, 1.0f, 0.0f };

	// �x�N�g�����]
	vTargetEye = MyMath::MatVector(matRot, vTargetEye);

}

void GameCamera::Collision()
{
	isShake = true;
	shakeTime = 10;
}

// �J�����̈ʒu��v�Z����֐�
Vector3 GameCamera::calculateCameraPosition(ViewProjection* viewProjection_, float distance, float angle) {
	/*float horizontalDistance = distance * cos(angle);
	float verticalDistance = distance * sin(angle);
	Vector3 playerPos = playerPos_;
	Vector3 cameraPos = playerPos_;
	cameraPos.y += verticalDistance;
	float pitch = getPitch(viewProjection_);
	float yaw = getYaw();
	cameraPos.x += -horizontalDistance * sin(yaw);
	cameraPos.z += -horizontalDistance * cos(yaw);*/
	return Vector3(0, 0, 0);
}
Vector3 GameCamera::calculateLookAtPosition(Vector3 target, Vector3 camera) {
	Vector3 direction = target - camera;
	direction.norm();
	return camera + direction;
}

void GameCamera::CameraAngle(float x, float z)
{
	angle = atan2(x, z);

	if (angle < 0) {
		angle = angle + 2 * MyMath::PI;
	}

	angle = floor(angle * 360 / (2 * MyMath::PI));

}

Vector3 GameCamera::GetEye() {

	if (cameraTime < MaxCameraTime - 170) {
		return vTargetEye;
	}
	else {
		return cameraPos;
	}
}