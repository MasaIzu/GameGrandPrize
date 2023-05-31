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

	// 画面サイズに対する相対的なスケールに調整
	scaleX_ = 1.0f / (float)window_width;
	scaleY_ = 1.0f / (float)window_height;

	dirty = false;
	float angleX = 0;
	float angleY = 0;

	MaxCameraTime = 400;
	cameraTime = MaxCameraTime;
	oldMousePos = mousePos;
	mousePos = input_->GetMousePos();

	// 追加回転分の回転行列を生成
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

	//mouseMoved = Vector2(0.15f, 0);

}

void GameCamera::InitializeCameraPosition()
{
	Vector2 windowWH = Vector2(winWidth / 2, winHeight / 2);

	mouseMoved = Vector2(0, 0);
	CameraRot = MyMath::MakeIdentity();


	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->Gethwnd();

	int xPos = windowWH.x;  //移動させたいｘ座標（ウィンドウ内の相対座標）
	int yPos = windowWH.y; //移動させたいｙ座標（ウィンドウ内の相対座標）

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	int xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	int yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	SetCursorPos(xPos_absolute, yPos_absolute);//移動させる

	target = playerPos_ + Vector3(0, 14, 0);

	//ワールド前方ベクトル
	Vector3 forward(0, 0, playerCameraDistance);
	//レールカメラの回転を反映
	forward = MyMath::MatVector(CameraRot, forward);

	forward.normalize();

	//target = pos;
	vTargetEye = target + (forward * cameraDis);

	cameraPos = vTargetEye;

	//距離
	//cameraPos += PlayerMoveMent;
	Vector3 dVec = vTargetEye - cameraPos;
	dVec *= cameraDelay;
	cameraPos += dVec * cameraSpeed_;
	Vector3 player_camera = cameraPos - target;
	player_camera.normalize();
	cameraPos = target + (player_camera * cameraDis);

}

void GameCamera::Update(ViewProjection* viewProjection_) {
	if (cameraMode == false) {
		/*if (input_->PushKey(DIK_LSHIFT)) {
			PlayerLockOnCamera(viewProjection_);
		}
		else {
			PlaySceneCamera(viewProjection_);
		}*/
		PlaySceneCamera(viewProjection_);

		ImGui::Text("isShake : %d", isShake);
	}
	else {
		ImGui::Begin("camera");
		ImGui::SliderFloat("eye:x", &vTargetEye.x, -100.0f, 100.0f);
		ImGui::SliderFloat("eye:y", &vTargetEye.y, -100.0f, 700.0f);
		ImGui::SliderFloat("eye:z", &vTargetEye.z, -100.0f, 100.0f);

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

	//カメラの回転ベクトル
	Vector3 rotat = { 0, 0, 0 };
	//カメラの移動の速さ
	const float cameraSpeed = 0.0005f;

	Vector2 windowWH = Vector2(winWidth / 2, winHeight / 2);
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->Gethwnd();
	ScreenToClient(hwnd, &mousePosition);

	int xPos_absolute, yPos_absolute;

	int xPos = windowWH.x;  //移動させたいｘ座標（ウィンドウ内の相対座標）
	int yPos = windowWH.y; //移動させたいｙ座標（ウィンドウ内の相対座標）

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	SetCursorPos(xPos_absolute, yPos_absolute);//移動させる

	//マウスの移動量を取得
	MouseMove = Vector2(0, 0);
	MouseMove = (Vector2(mousePosition.y, mousePosition.x) - Vector2(windowWH.y, windowWH.x));//座標軸で回転している関係でこうなる(XとYが入れ替え)

	MovementMous = Vector2(MouseMove.x, MouseMove.y) / 500;
	mouseMoved += Vector2(MouseMove.x, MouseMove.y) / 500;

	if (MovementMous.x > 0) {
		cameraUp = true;
		cameraDown = false;
	}
	else if (MovementMous.x < 0) {
		cameraUp = false;
		cameraDown = true;
	}
	else {
		cameraUp = false;
		cameraDown = false;
	}

	//if (input_->PushKey(DIK_8) == 0) {
	//	cameraDis += 0.1f;
	//}
	//if (input_->PushKey(DIK_9) == 0) {
	//	cameraDis += -0.1f;
	//}
	//if (input_->PushKey(DIK_6) == 0) {
	//	Fov += 0.1f;
	//}
	//if (input_->PushKey(DIK_7) == 0) {
	//	Fov += -0.1f;
	//}

	ImGui::Begin("camera");
	ImGui::Text("mouseMovedX : %f", mouseMoved.x);
	ImGui::Text("CameraMouseMoved : %f", CameraMouseMoved);
	ImGui::Text("cameraDis : %f", cameraDis);
	ImGui::Text("CameraDistanceMinus : %f", CameraDistanceMinus);

	ImGui::Text("MovementMous : %f,%f", MovementMous.x, MovementMous.y);
	ImGui::Text("cameraDown : %d", cameraDown);
	ImGui::Text("cameraDown : %d", cameraUp);
	ImGui::End();

	//カメラ制限

	if (cameraDown == true) {
		if (mouseMoved.x < -0.10f + CameraMouseMoved) {
			mouseMoved.x = -0.10f + CameraMouseMoved;
			if (cameraDis - 10.0f > CameraDistanceMinus) {
				CameraDistanceMinus -= MovementMous.x * 40;
				if (CameraMouseMoved > -0.2f) {
					CameraMouseMoved += MovementMous.x;
				}
				if (cameraDis - 10.0f < CameraDistanceMinus) {
					CameraDistanceMinus = cameraDis - 10.0f;
					CameraMouseMoved = -0.2f;
				}

			}
			else {
				CameraMouseMoved = -0.2f;
			}
		}
		else {
		}
	}


	if (cameraUp == true) {

		if (mouseMoved.x < -0.10f) {
			mouseMoved.x = -0.10f + CameraMouseMoved;
		}

		if (CameraDistanceMinus > 0) {
			CameraDistanceMinus -= MovementMous.x * 60;
			if (CameraMouseMoved < 0.0f) {
				CameraMouseMoved += MovementMous.x / 2;
			}
			if (CameraDistanceMinus < 0) {
				CameraDistanceMinus = 0;
			}
		}
		else {
			CameraMouseMoved = 0;
		}

		if (mouseMoved.x > 1.30f) {
			mouseMoved.x = 1.30f;
		}
	}


	Vector3 rotation = Vector3(-mouseMoved.x, mouseMoved.y, 0);

	Matrix4 cameraRot;

	cameraRot = MyMath::Rotation(rotation, 6);
	//cameraRot *= viewProjection_->matView;

	rot = rotation;
	CameraRot = cameraRot;

	target = easing_->InOutVec3(target, playerPos_ + Vector3(0, 14, 0), cameraTime, MaxCameraTime);

	//ワールド前方ベクトル
	Vector3 forward(0, 0, playerCameraDistance);
	//レールカメラの回転を反映
	forward = MyMath::MatVector(CameraRot, forward);

	forward.normalize();

	//target = pos;
	vTargetEye = target + (forward * cameraDis);

	CameraAngle(vTargetEye.z - target.z, vTargetEye.x - target.x);


	//遅延カメラ
	//距離
	cameraPos += PlayerMoveMent;
	Vector3 dVec = vTargetEye - cameraPos;
	dVec *= cameraDelay;
	cameraPos += dVec * cameraSpeed_;
	Vector3 player_camera = cameraPos - target;
	player_camera.normalize();

	float DISTANCE = cameraDis - CameraDistanceMinus;

	player_camera.x = player_camera.x * DISTANCE;
	player_camera.y = player_camera.y * (cameraDis - CameraDistanceMinus / 2);
	player_camera.z = player_camera.z * DISTANCE;

	cameraPos = target + (player_camera);

	if (isShake == true) {
		vTargetEye += Vector3(rand() % 4, rand() % 4, rand() % 4);
	}
}

void GameCamera::Collision()
{
	isShake = true;
	shakeTime = 10;
}

void GameCamera::Reset()
{
	// カメラ注視点までの距離
	distance_ = 10;
	mousePos = { 0.0f,0.0f };
	oldMousePos = { 0.0f,0.0f };

	// 回転行列
	fTheta = 4.57f;//カメラ横方向角度
	fDelta = 0.43f;//カメラ縦方向角度

	mousepoint_a;//マウス位置
	mousepoint_b;//マウス位置
	dirty = false;

	spaceInput = false;

	// スケーリング
	scaleX_ = 1.0f;
	scaleY_ = 1.0f;

	vTargetEye = { 0,0,0 };
	vUp = { 0,0,0 };
	playerPos_ = { 0,0,0 };
	target = { 0,0,0 };

	cameraMode = false;

	cameraType = 0;

	winWidth = 0;
	winHeight = 0;
	MouseMove = { 0,0 };
	mouseMoved = { 0,0 };
	EnemyPos_ = { 0,0,0 };

	angleAroundPlayer = 0; // プレイヤーの周りを回転する角度


	float playerCameraDistance = 5.5f;

	int cameraTime = 0;
	int MaxCameraTime = 0;


	//カメラモード(tekito-)
	int cameraMode_ = 0;
	//カメラ距離関係
	cameraDistance_ = 20.0f;
	cameraModeChangeCountTimer = 30;
	cameraHeight_ = 6;

	isShake = false;
	shakeTime = 0;

	angle = 0.0f;

	// カメラの速度
	cameraSpeed_ = 3;

	// カメラが追跡する際の遅延量
	cameraDelay = 0.1f;

	cameraDis = 45.0f;

	LatePlayerPos = { 0,0,0 };
	playerCameraDelay = 0.1f;
	playerCameraSpeed_ = 3;

	TargetCameraDelay = 0.05f;
	TargetCameraSpeed_ = 1.0;

	higth = { 0,10,0 };

	Fov = 45.0f;
	float angleX = 0;
	float angleY = 0;

	MaxCameraTime = 400;
	cameraTime = MaxCameraTime;
	oldMousePos = mousePos;
	mousePos = input_->GetMousePos();

	// 追加回転分の回転行列を生成
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

void GameCamera::MousePositionReset()
{

	Vector2 windowWH = Vector2(winWidth / 2, winHeight / 2);
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->Gethwnd();
	ScreenToClient(hwnd, &mousePosition);

	int xPos_absolute, yPos_absolute;

	int xPos = windowWH.x;  //移動させたいｘ座標（ウィンドウ内の相対座標）
	int yPos = windowWH.y; //移動させたいｙ座標（ウィンドウ内の相対座標）

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	SetCursorPos(xPos_absolute, yPos_absolute);//移動させる

}

void GameCamera::PlayerLockOnCamera(ViewProjection* viewProjection_)
{
	Vector3 PlayerAndEnemy = (playerPos_ + EnemyPos_) / 2;

	target = EnemyPos_;

}

void GameCamera::MultiplyMatrix(Matrix4& matrix) {
	// 累積の回転行列を合成
	matRot = matrix * matRot;

	// 注視点から視点へのベクトルと、上方向ベクトル
	vTargetEye = { 0.0f, 0.0f, -distance_ };
	vUp = { 0.0f, 1.0f, 0.0f };

	// ベクトルを回転
	vTargetEye = MyMath::MatVector(matRot, vTargetEye);

}

// カメラの位置を計算する関数
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

	return cameraPos;
}