#include "Player.h"
#include "WinApp.h"
#include"MyMath.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>
#include"ImGuiManager.h"
#include <FbxLoader.h>


float easeOutQuin(float x)
{
	return sin((x * PI) / 2);
}

Player::Player()
{
}

Player::~Player()
{
	delete easing_;
}

void Player::Initialize(Model* model, float WindowWidth, float WindowHeight) {
	//NULLポインタチェック
	assert(model);
	playerModel_ = model;
	oldPlayerModel_.reset(Model::CreateFromOBJ("UFO", true));

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();

	Window_Width = WindowWidth;
	Window_Height = WindowHeight;

	easing_ = new Easing();

	// コリジョンマネージャに追加
	collider = new SphereCollider(Vector4(0, radius, 0, 0), radius);
	CollisionManager::GetInstance()->AddCollider(collider);

	playerAvoidance = 15.0f;
	moveTime = 300;

	for (int i = 0; i < SphereCount; i++) {
		// コリジョンマネージャに追加
		AttackCollider[i] = new SphereCollider(Vector4(0, radius, 0, 0), radius);
		CollisionManager::GetInstance()->AddCollider(AttackCollider[i]);
		AttackCollider[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
	}

	//worldTransform_.translation_ = { 0,0,-100 };

	//ワールド変換の初期化
	worldTransform_.Initialize();
	oldWorldTransform_.Initialize();
	playerAttackTransform_.Initialize();

	collider->Update(worldTransform_.matWorld_);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	for (int i = 0; i < SphereCount; i++) {
		playerAttackTransformaaaa_[i].Initialize();
		playerAttackTransformaaaa_[i].TransferMatrix();
	}
	worldTransform_.translation_ = { 0.0f,0.0f,150.0f };
	worldTransform_.scale_ = { 0.03f,0.03f,0.03f };

	worldTransform_.alpha = 0.0;

	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();
	playerAttackTransform_.TransferMatrix();

	ParticleMan = std::make_unique<ParticleManager>();
	ParticleMan->Initialize();
	ParticleMan->SetTextureHandle(TextureManager::Load("effect4.png"));

	playerEvasionTimes = 3;

	recovery = std::make_unique<Recovery>();
	recovery->Initialize();

	startModel.reset(Model::CreateFromOBJ("warp", true));

	startPointModel.reset(Model::CreateFromOBJ("warpPoint", true));

	startTrans.Initialize();

	startPointTrans.Initialize();

	startPointTrans.translation_ = { 0.0f,0.0f,150.0f };

	startPointTrans.scale_ = { 3,3,3 };

	startPointTrans.TransferMatrix();

	// スプライトの初期化処理
	SpriteInitialize();

	fbxmodel.reset(FbxLoader::GetInstance()->LoadModelFromFile("3dKyaraFix2"));
	fbxmodel->Initialize();
	fbxmodel2 = fbxmodel.get();
	fbxmodel2->Initialize();
	fbxmodel->SetPolygonExplosion({ 1.0f,1.0f,9.42f,600.0f });
	modelAnim = std::make_unique<FbxAnimation>();
	modelAnim->Load("3dKyaraFix2");

	std::unique_ptr<FbxModel> fbxmodel;

	LBoneTrans.Initialize();
	RBoneTrans.Initialize();

	LSowrdModel.reset(Model::CreateFromOBJ("ken", true));
	RSowrdModel.reset(Model::CreateFromOBJ("ken", true));

	LSowrdModel->SetPolygonExplosion({ 1.0f,1.0f,6.28,600.0f});
	RSowrdModel->SetPolygonExplosion({ 1.0f,1.0f,6.28,600.0f });
}


void Player::Update(const ViewProjection& viewProjection) {
	if (isAdmission == true)
	{

		flame++;

		float endflame = 120;

		float Destruction = (0.0f - 1.0f) * easeOutQuin(flame / endflame);
		Destruction++;
		float a = (1.0f - 0.0f) * easeOutQuin(flame / endflame);

		float scale = (0.2f - 0.0f) * easeOutQuin(flame / endflame);

		FbxModel::ConstBufferPolygonExplosion polygon = fbxmodel->GetPolygonExplosion();
		fbxmodel->SetPolygonExplosion({ Destruction,scale,polygon._RotationFactor,polygon._PositionFactor });
		worldTransform_.alpha = a;
		if (flame >= endflame)
		{
			isAdmission = false;
			//worldTransform_.alpha = 1;
			fbxmodel->SetPolygonExplosion({ Destruction,1.0f,polygon._RotationFactor,polygon._PositionFactor });
		}
	}

	if (isAdmission == false && HP > 0)
	{
		Move();
		Attack();
		KnockBackUpdate();

		if (frem < MaxFrem) {
			frem += 0.013f;
			if (isPlayMotion) {
				frem += 0.007;
			}

		}
		else {
			frem = MinimumFrem;
			isPlayMotion = false;
			if (isWalking == false) {
				if (isAttack == false) {
					playerNowMotion = PlayerMotion::taiki;
				}
			}
		}
		if (conboFlag == true) {
			receptionTime += 0.02f;
		}


		if (isAttackHit)
		{
			AttackCollision();
		}
	}

	if (HP <= 0.0f && isAlive)
	{
		worldTransform_.alpha -= 0.05;
		if (worldTransform_.alpha <= 0.0f)
		{
			isAlive = false;
		}
	}
	ParticleMan->Update();

	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();
	playerAttackTransform_.TransferMatrix();

	collider->Update(worldTransform_.matWorld_);
	recovery->Update();
	if (input_->PushKey(DIK_P)) {
		frem = fremX;
		playerNowMotion = PlayerMotion::soukenCombo1;
	}
	if (input_->PushKey(DIK_J)) {
		fremX += 0.1;
	}
	if (input_->PushKey(DIK_K)) {
		fremX += 0.01;
	}
	if (input_->PushKey(DIK_N)) {
		fremX += -0.1;
	}
	if (input_->PushKey(DIK_M)) {
		fremX += -0.01;
	}

	fbxmodel->ModelAnimation(frem, modelAnim->GetAnimation(static_cast<int>(playerNowMotion)));
	matL = fbxmodel->GetLeftBonePos() * MyMath::Scale(Vector3(0, worldTransform_.scale_.y, 0));
	matR = fbxmodel->GetRightBonePos() * worldTransform_.matWorld_;

	LBoneTrans.translation_ = MyMath::GetWorldTransform(matL) + MyMath::GetWorldTransform(worldTransform_.matWorld_);
	RBoneTrans.translation_ = MyMath::GetWorldTransform(matR);



	LBoneTrans.TransferMatrix();
	RBoneTrans.TransferMatrix();

	isAttackHit = false;



	ImGui::Begin("player");

	ImGui::SliderFloat("posx", &avoidGaugeUnderPos.x, 0.0f, 1280.0f);
	ImGui::SliderFloat("posy", &avoidGaugeUnderPos.y, 0.0f, 720.0f);

	ImGui::SliderFloat("sizex", &avoidGaugeUnderSize.x, 0.0f, 512.0f);
	ImGui::SliderFloat("sizey", &avoidGaugeUnderSize.y, 0.0f, 512.0f);

	ImGui::Text("flame:%f", flame);
	ImGui::Text("a:%f", worldTransform_.alpha);
	ImGui::Text("fra:%d", isAdmission);
	//ImGui::Text("frem:%f", frem);

	//ImGui::Text("root:%f,%f,%f", root.x, root.y, root.z);

	//ImGui::Text("MaxFrem:%f", MaxFrem);
	//ImGui::Text("MiniFrem:%f", MinimumFrem);
	//ImGui::Text("attackConbo:%d", attackConbo);

	//ImGui::Text("isPlayMotion:%d", isPlayMotion);



	ImGui::Text("rotX:%f", worldTransform_.translation_.x);
	ImGui::Text("rotY:%f", worldTransform_.translation_.y);
	ImGui::Text("rotZ:%f", worldTransform_.translation_.z);

	ImGui::Text("playerEvasionTimes:%d", playerEvasionTimes);
	ImGui::Text("playerEvasionCoolTime:%d", playerEvasionCoolTime);

	ImGui::End();
}

void Player::Move() {

	PlayerMoveMent = { 0,0,0 };
	rot = { 0,0,0 };
	Avoidance = { 0,0,0 };
	isPushSenter = false;
	isPushLeft = false;
	isPushRight = false;
	isPushBack = false;
	isWalk = false;
	isSpace = false;
	isInput = false;

	if (timer > 0) {
		timer--;
		alpha -= 0.02f;
	}
	else {
		spaceInput = false;
		collider->SetAttribute(COLLISION_ATTR_ALLIES);
	}

	if (playerEvasionTimes < playerEvasionMaxTimes) {
		playerEvasionCoolTime--;
	}
	if (playerEvasionCoolTime < 0 && playerEvasionTimes < playerEvasionMaxTimes) {
		playerEvasionTimes++;
		playerEvasionCoolTime = CoolTime;
	}

	if (playerEvasionTimes == 0) {
		spriteAlpha1 = 1.0f - playerEvasionCoolTime / CoolTime;
		spriteAlpha2 = 0.0f;
		spriteAlpha3 = 0.0f;
	}
	else if (playerEvasionTimes == 1) {
		spriteAlpha1 = 1.0f;
		spriteAlpha2 = 1.0f - playerEvasionCoolTime / CoolTime;
		spriteAlpha3 = 0.0f;
	}
	else if (playerEvasionTimes == 2) {
		spriteAlpha1 = 1.0f;
		spriteAlpha2 = 1.0f;
		spriteAlpha3 = 1.0f - playerEvasionCoolTime / CoolTime;
	}
	else if (playerEvasionTimes == 3) {
		spriteAlpha1 = 1.0f;
		spriteAlpha2 = 1.0f;
		spriteAlpha3 = 1.0f;
	}

	cameraLookmat = MyMath::Rotation(Vector3(0, 90, 0), 2);

	Vector3 moveRot = cameraLook;
	cameraLook.normalize();

	root = (worldTransform_.lookLeft - worldTransform_.translation_);


	if (spaceInput == false) {

		if (isPlayMotion == false) {
			if (input_->PushKey(DIK_W)) {
				PlayerMoveMent += cameraLook * playerSpeed;
				isPushSenter = true;
				isWalk = true;
				isInput = true;
				rot += Vector3(0, 0, 0.2f);
				playerNowMotion = PlayerMotion::aruki;
			}
			if (input_->PushKey(DIK_A)) {
				PlayerMoveMent += root.normalize() * playerSpeed;
				isPushLeft = true;
				isWalk = true;
				isInput = true;
				rot += Vector3(-0.1f, MyMath::GetAngle(-90), 0);
				playerNowMotion = PlayerMotion::aruki;
			}
			if (input_->PushKey(DIK_S)) {
				PlayerMoveMent -= cameraLook * playerSpeed;
				isPushBack = true;
				isWalk = true;
				isInput = true;
				rot += Vector3(0, MyMath::GetAngle(180), -0.2f);
				playerNowMotion = PlayerMotion::aruki;
			}
			if (input_->PushKey(DIK_D)) {
				PlayerMoveMent -= root.normalize() * playerSpeed;
				isPushRight = true;
				isWalk = true;
				isInput = true;
				rot += Vector3(0.1f, MyMath::GetAngle(90), 0);
				playerNowMotion = PlayerMotion::aruki;
			}

			if (isPushSenter == true && isPushLeft == true) {
				rot = Vector3(-0.05f, MyMath::GetAngle(-45.0f), 0.0f);
			}
			if (isPushSenter == true && isPushRight == true) {
				rot = Vector3(0.05f, MyMath::GetAngle(45.0f), 0.0f);
			}

			if (isPushBack == true && isPushLeft == true) {
				rot = Vector3(-0.05f, MyMath::GetAngle(-135.0f), 0.0f);
			}
			if (isPushBack == true && isPushRight == true) {
				rot = Vector3(0.05f, MyMath::GetAngle(135.0f), 0.0f);
			}

			if (isInput == true) {
				PlayerRot = rot;
			}

		}

		if (playerEvasionTimes > 0) {
			if (input_->TriggerKey(DIK_SPACE)) {
				spaceInput = true;
				isSpace = true;
				if (playerEvasionTimes == playerEvasionMaxTimes) {
					playerEvasionCoolTime = CoolTime;
				}
				playerEvasionTimes--;
				timer = 20;
				alpha = 0.3f;
				collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
				oldWorldTransform_.scale_ = worldTransform_.scale_;
				oldWorldTransform_.translation_ = worldTransform_.translation_;

				if (isPushLeft == true) {
					Avoidance.x = playerAvoidance;
				}
				else if (isPushRight == true) {
					Avoidance.x = -playerAvoidance;
				}
				else if (isPushBack == true) {
					Avoidance.z = playerAvoidance;
				}
				else {
					Avoidance.z = -playerAvoidance;
				}
			}
		}

	}

	//worldTransform_.SetRot({ MyMath::GetAngle(rot.x),-MyMath::GetAngle(angle) + MyMath::GetAngle(rot.y), MyMath::GetAngle(rot.z) });
	Matrix4 roooooot;
	roooooot *= MyMath::Rotation(Vector3(PlayerRot.x, -MyMath::GetAngle(angle) + PlayerRot.y, PlayerRot.z), 1);
	roooooot *= MyMath::Rotation(Vector3(PlayerRot.x, -MyMath::GetAngle(angle) + PlayerRot.y, PlayerRot.z), 3);
	roooooot *= MyMath::Rotation(Vector3(PlayerRot.x, -MyMath::GetAngle(angle) + PlayerRot.y, PlayerRot.z), 2);
	worldTransform_.SetMatRot(roooooot);
	oldWorldTransform_.SetMatRot(roooooot);

	worldTransform_.SetLookRot({ 0,-MyMath::GetAngle(angle),0 });

	CameraRot = MyMath::Rotation(Vector3(Rot.x, Rot.y, Rot.z), 6);
	Avoidance = MyMath::MatVector(CameraRot, Avoidance);
	Avoidance.y = 0;
	Avoidance.normalize();
	Avoidance = Avoidance * playerAvoidance;

	//worldTransform_.translation_ += playerMovement;
	//worldTransform_.translation_ += Avoidance;
	AvoidanceMove += Avoidance;

	Vector3 allMove = PlayerMoveMent + AvoidanceMove;

	AvoidanceMove = { 0,0,0 };
	//ステージからでないようにする処理
	{
		float AR;
		float BR;

		AR = pow((worldTransform_.translation_.x + allMove.x) - 0, 2) + pow((0 + worldTransform_.translation_.z + allMove.z) - 0, 2);
		BR = pow((satgeSize - worldTransform_.scale_.x * 2), 2);

		if (AR <= BR)
		{
			worldTransform_.translation_ += allMove;
		}
	}
	//回復と当たっているかの処理
	{
		float AR;
		float BR;

		AR = pow((worldTransform_.translation_.x + allMove.x) - recovery->GetWorldPosition().x, 2) + pow((0 + worldTransform_.translation_.z + allMove.z) - recovery->GetWorldPosition().z, 2);
		BR = pow((worldTransform_.scale_.x - recovery->GetScale().x), 2);

		if (AR <= BR && recovery->GetActive())
		{
			HP += 50;
			if (HP > maxHP)
			{
				HP = maxHP;
			}
			recovery->Collision();
		}
	}

	if (isWalk == true) {
		if (isWalking == false) {
			isWalking = true;
			frem = 0;
		}
		MaxFrem = 1.3f;
		MinimumFrem = 0.45f;
	}
	else {
		if (isPlayMotion == false) {
			if (isWalking == true) {
				frem = 1.63f;
			}

			isWalking = false;

			MaxFrem = 1.8f;
			MinimumFrem = 1.8f;
		}

	}

	if (playerNowMotion == PlayerMotion::taiki) {
		MinimumFrem = 0.0f;
		MaxFrem = 2.0f;
	}

}

void Player::Attack() {

	Vector3 moveRot = cameraLook;

	if (spaceInput == false) {
		if (input_->MouseInputTrigger(0)) {
			//実行前にカウント値を取得
			//計測開始時間の初期化
			isAttack = true;
			startCount = 0;
			nowCount = 0;
			timeRate = 0;
			startIndex = 1;

			/*if (attackConbo < 3) {
				attackConbo++;
			}
			else {
				attackConbo = 0;
			}*/


			if (isPlayMotion == false) {

				attackConbo = 1;
				playerNowMotion = PlayerMotion::soukenCombo1;
				isPlayMotion = true;
				MinimumFrem = 0.0f;
				MaxFrem = 2.0f;
				frem = 0.0f;

				receptionTime = 0.0f;
				conboFlag = true;

			}
			if (attackConbo == 1) {
				if (receptionTime > 0.8f && receptionTime < 1.36f) {
					attackConbo = 2;
					playerNowMotion = PlayerMotion::soukenCombo2;
					isPlayMotion = true;
					MinimumFrem = 1.86f;
					MaxFrem = 1.88f;
					frem = 0.0f;
					receptionTime = 0.0f;
				}
			}
			else if (attackConbo == 2) {
				if (receptionTime > 0.8f && receptionTime < 1.36f) {
					attackConbo = 3;
					playerNowMotion = PlayerMotion::soukenCombo3;
					isPlayMotion = true;
					MinimumFrem = 1.86f;
					MaxFrem = 1.88f;
					frem = 0.0f;
					receptionTime = 0.0f;
				}
			}
			else if (attackConbo == 3) {
				if (receptionTime > 0.8f && receptionTime < 1.36f) {
					attackConbo = 4;
					playerNowMotion = PlayerMotion::soukenCombo4;
					isPlayMotion = true;
					MinimumFrem = 1.86f;
					MaxFrem = 1.88f;
					frem = 0.0f;
					receptionTime = 0.0f;
				}
			}
			else if (attackConbo == 4) {
				if (receptionTime > 0.8f && receptionTime < 1.36f) {
					attackConbo = 5;
					playerNowMotion = PlayerMotion::soukenCombo5;
					isPlayMotion = true;
					MinimumFrem = 1.86f;
					MaxFrem = 1.88f;
					frem = 0.0f;
					receptionTime = 0.0f;
				}
			}
		}
	}



	if (nowCount < maxTime * 4) {
		nowCount++;
	}
	else {
		isAttack = false;
		for (int i = 0; i < SphereCount; i++) {
			AttackCollider[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
		}
	}

	if (isAttack == true) {

		//補間で使うデータ
		//start → end を知らん秒で完了させる
		Vector3 p0(worldTransform_.lookRight);									//スタート地点
		Vector3 p1((worldTransform_.look + worldTransform_.lookRight) / 2);		//制御点その1
		Vector3 p2(worldTransform_.look);										//制御点その2
		Vector3 p3((worldTransform_.look + worldTransform_.lookLeft) / 2);		//制御点その3
		Vector3 p4(worldTransform_.lookLeft);									//ゴール地点

		p0 = p0 + ((p0 - GetWorldPosition()) * attackDistanceX);
		p1 = p1 + ((p1 - GetWorldPosition()) * attackDistanceZ);
		p2 = p2 + ((p2 - GetWorldPosition()) * attackDistanceZ);
		p3 = p3 + ((p3 - GetWorldPosition()) * attackDistanceZ);
		p4 = p4 + ((p4 - GetWorldPosition()) * attackDistanceX);


		points = { p0,p0,p1,p2,p3,p4,p4 };

		// 落下
		// スタート地点        ：start
		// エンド地点        　：end
		// 経過時間            ：elapsedTime [s]
		// 移動完了の率(経過時間/全体時間) : timeRate (%)
		elapsedTime = nowCount - startCount;
		timeRate = elapsedTime / maxTime;

		if (timeRate >= 1.0f)
		{
			if (startIndex < points.size() - 3)
			{
				startIndex += 1.0f;
				timeRate -= 1.0f;
				startCount = nowCount;
			}
			else
			{
				timeRate = 1.0f;
			}
		}

		position = splinePosition(points, startIndex, timeRate);

		playerAttackTransform_.translation_ = position;

		float sphereX = position.x - GetWorldPosition().x;
		float sphereY = position.y - GetWorldPosition().y;
		float sphereZ = position.z - GetWorldPosition().z;

		Vector3 sphere(sphereX / SphereCount, sphereY / SphereCount, sphereZ / SphereCount);

		for (int i = 0; i < SphereCount; i++) {
			colliderPos[i] = GetWorldPosition() + sphere * i;
			worldSpherePos[i] = MyMath::Translation(colliderPos[i]);
			AttackCollider[i]->Update(worldSpherePos[i]);
			AttackCollider[i]->SetAttribute(COLLISION_ATTR_ATTACK);
			playerAttackTransformaaaa_[i].translation_ = colliderPos[i];
			playerAttackTransformaaaa_[i].TransferMatrix();
		}

	}

}

void Player::SetKnockBackCount()
{
	KnockBack = MyMath::GetWorldTransform(worldTransform_.matWorld_) - MyMath::GetWorldTransform(EnemyPos);
	KnockBack.y = 0;
	KnockBack.normalize();
	KnockBack = KnockBack * KnockBackDistance;

	moveTime = 0;
	KnockBack = MyMath::GetWorldTransform(worldTransform_.matWorld_) + KnockBack;

	isKnockBack = true;
}

void Player::KnockBackUpdate()
{
	if (moveTime < MaxMoveTime) {
		moveTime++;
		KnockBack += PlayerMoveMent;
		Vector3 KnockBackMove = easing_->InOutVec3(MyMath::GetWorldTransform(worldTransform_.matWorld_), KnockBack, moveTime, MaxMoveTime);

		float AR;
		float BR;

		AR = pow((KnockBackMove.x) - 0, 2) + pow((KnockBackMove.z) - 0, 2);
		BR = pow((satgeSize - worldTransform_.scale_.x * 2), 2);

		if (AR <= BR)
		{
			worldTransform_.translation_ = KnockBackMove;
		}
	}
	else
	{
		isKnockBack = false;
	}

}

void Player::Draw(ViewProjection viewProjection_) {

	//if (timer == 0) {
	//	//playerModel_->Draw(worldTransform_, viewProjection_);
	//}
	//if (isAttack) {
	//	for (int i = 0; i < SphereCount; i++) {
	//		playerModel_->Draw(playerAttackTransformaaaa_[i], viewProjection_);
	//	}
	//}

	startPointModel->Draw(startPointTrans, viewProjection_);

	recovery->Draw(viewProjection_);

	//LSowrdModel->Draw(LBoneTrans, viewProjection_);
	//RSowrdModel->Draw(RBoneTrans, viewProjection_);
}

void Player::PlayerFbxDraw(ViewProjection viewProjection_) {
	if (timer > 0) {
		oldWorldTransform_.alpha = alpha;
		fbxmodel2->Draw(oldWorldTransform_, viewProjection_);
	}
	if (spaceInput == false) {
		fbxmodel->Draw(worldTransform_, viewProjection_);
	}
}


void Player::DrawHealth() {

	// HPのセット
	float nowHp = HP / maxHP;
	hpSize = { 553.0f * nowHp,25.0f };
	healthSprite->SetSize(hpSize);

	// Hpの下の部分を減らす処理


	if (IsHpAlfa) {
		// 攻撃を受けてから 30 フレーム下のHpは動かない
		if (hpAlfaTimer < 30) {
			hpAlfaTimer++;
		}
		else {
			// 赤ゲージよりサイズが大きいなら減らす
			if (hpSize.x < hpAlfaSize.x) {
				hpAlfaSize.x -= 2.0f;
				healthAlfaSprite->SetSize(hpAlfaSize);
			}
			// 赤ゲージよりサイズが小さくなったら減らすのをやめ、赤ゲージのサイズに合わせる
			// 下のゲージのフラグをオフにする
			else if (hpSize.x >= hpAlfaSize.x) {
				hpAlfaTimer = 0;
				healthAlfaSprite->SetSize(hpSize);
				IsHpAlfa = false;
			}
		}
	}



	Vector2 pos = { 54.5f,35.0f };

	Vector2 AttackFontpos = { 150,380 };

	Vector2 MoveFontpos = { 130,450 };

	Vector2 W_Fontpos = { 270,430 };

	Vector2 A_Fontpos = { 240,460 };

	Vector2 S_Fontpos = { 270,460 };

	Vector2 D_Fontpos = { 300,460 };

	Vector2 AvoidFontpos = { 175,520 };

	Vector2 HP_barPos = { 330,50 };

	//Vector2 avoidGauge1Pos = { 175,520 };

	//Vector2 avoidGauge2Pos = { 175,520 };

	//Vector2 avoidGauge3Pos = { 175,520 };





	// スプライト描画
	healthAlfaSprite->Draw(pos, { 1,1,1,1 });

	healthSprite->Draw(pos, { 1,1,1,1 });

	MoveFontSprite->Draw(MoveFontpos, { 1,1,1,1 });

	HP_barSprite->Draw(HP_barPos, { 1,1,1,1 });

	avoidGauge_under->Draw(avoidGaugeUnderPos, { 1,1,1,1 });
	avoidGauge1->Draw(avoidGaugeUnderPos, { 1,1,1,spriteAlpha1 });
	avoidGauge2->Draw(avoidGaugeUnderPos, { 1,1,1,spriteAlpha2 });
	avoidGauge3->Draw(avoidGaugeUnderPos, { 1,1,1,spriteAlpha3 });


	for (int i = 0; i < 2; i++) {
		if (input_->PushKey(DIK_W)) {
			W_FontSprite[1]->Draw(W_Fontpos, { 1,1,1,1 });
		}
		else {
			W_FontSprite[0]->Draw(W_Fontpos, { 1,1,1,1 });
		}
		if (input_->PushKey(DIK_A)) {
			A_FontSprite[1]->Draw(A_Fontpos, { 1,1,1,1 });
		}
		else {
			A_FontSprite[0]->Draw(A_Fontpos, { 1,1,1,1 });
		}
		if (input_->PushKey(DIK_S)) {
			S_FontSprite[1]->Draw(S_Fontpos, { 1,1,1,1 });
		}
		else {
			S_FontSprite[0]->Draw(S_Fontpos, { 1,1,1,1 });
		}
		if (input_->PushKey(DIK_D)) {
			D_FontSprite[1]->Draw(D_Fontpos, { 1,1,1,1 });
		}
		else {
			D_FontSprite[0]->Draw(D_Fontpos, { 1,1,1,1 });
		}
		if (spaceInput) {
			AvoidFontSprite[1]->Draw(AvoidFontpos, { 1,1,1,1 });
		}
		else {
			AvoidFontSprite[0]->Draw(AvoidFontpos, { 1,1,1,1 });
		}

		if (isAttack) {
			AttackFontSprite[0]->Draw(AttackFontpos, { 1,1,1,1 });
		}
		else if (isAttack == false) {
			AttackFontSprite[1]->Draw(AttackFontpos, { 1,1,1,1 });
		}


	}

}

void Player::ParticleDraw(ViewProjection view)
{
	ParticleMan->Draw(view);
}


Vector3 Player::bVelocity(Vector3 velocity, WorldTransform& worldTransform) {

	Vector3 result = { 0, 0, 0 };


	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];

	return result;
}

void Player::Collision(int damage)
{
	if (isKnockBack == false)
	{
		SetKnockBackCount();
		HP -= damage;
		if (HP < 0)
		{
			HP = 0;
		}

		int ParticleNumber = 10;
		if (HP <= 0)
		{
			ParticleNumber = 100;
		}
		//スペースキーを押していたら
		for (int i = 0; i < ParticleNumber; i++)
		{
			//消えるまでの時間
			const float rnd_life = 70.0f;
			//最低限のライフ
			const float constlife = 10;
			float life = (float)rand() / RAND_MAX * rnd_life + constlife;

			//XYZの広がる距離
			const float rnd_pos = 30.0f;
			//Y方向には最低限の飛ぶ距離
			const float constPosY = 15;
			Vector3 pos{};
			pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.y = abs((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + 2;
			pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

			Vector3 startPos = MyMath::GetWorldTransform(worldTransform_.matWorld_);

			Vector3 controlPos = { MyMath::GetWorldTransform(worldTransform_.matWorld_).x,MyMath::GetWorldTransform(worldTransform_.matWorld_).y + pos.y,MyMath::GetWorldTransform(worldTransform_.matWorld_).z };

			Vector3 endPos = MyMath::GetWorldTransform(worldTransform_.matWorld_) + pos;

			startPos.y += 10;

			controlPos.y += 10;

			endPos.y += 10;
			//追加
			ParticleMan->Add(ParticleManager::Type::Out, life, true, startPos, controlPos, endPos, 0.5f, 0.5f, { 0,0,0,1 }, { 0,0,0,1 });
		}
		IsHpAlfa = true;
		hpAlfaSize = hpSize;
	}
}

void Player::AttackCollision()
{
	//スペースキーを押していたら
	for (int i = 0; i < 10; i++)
	{
		//消えるまでの時間
		const float rnd_life = 70.0f;
		//最低限のライフ
		const float constlife = 10;
		float life = (float)rand() / RAND_MAX * rnd_life + constlife;

		//XYZの広がる距離
		const float rnd_pos = 30.0f;
		//Y方向には最低限の飛ぶ距離
		const float constPosY = 15;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = abs((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + 10;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		Vector3 startPos = MyMath::GetWorldTransform(CollisionManager::GetInstance()->GetAttackHitWorldPos());

		Vector3 endPos = MyMath::GetWorldTransform(CollisionManager::GetInstance()->GetAttackHitWorldPos()) + pos;

		Vector3 controlPos = { startPos.x,endPos.y,startPos.z };

		//startPos.y += 10;

		//controlPos.y += 10;

		//endPos.y += 10;
		//追加
		ParticleMan->Add(ParticleManager::Type::Out, life, true, startPos, controlPos, endPos, 2.0f, 2.0f, { 0,0,0,1 }, { 0,0,0,1 });
	}
}

Vector3 Player::GetWorldPosition() {

	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetPosition(Vector3 pos)
{
	worldTransform_.translation_ = pos;
}

Vector3 Player::splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t)
{
	// 補完すべき点の数
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n]; // Pnの値を返す
	if (startIndex < 1)return points[1]; // P1の値を返す

	// p0～p3の制御点を取得する　※　p1～p2を補完する
	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	// Catmull - Romの式による補間
	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t
		+ (2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t
		+ (-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);

	return position;
}

void Player::Reset()
{

	playerAvoidance = 15.0f;
	moveTime = 300;


	//worldTransform_.translation_ = { 0,0,-100 };

	//ワールド変換の初期化
	worldTransform_.translation_ = { 0.0f,0.0f,150.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };

	worldTransform_.alpha = 0.0;

	worldTransform_.TransferMatrix();
	collider->Update(worldTransform_.matWorld_);

	oldWorldTransform_.translation_ = { 0,0,0 };
	oldWorldTransform_.rotation_ = { 0,0,0 };
	oldWorldTransform_.scale_ = { 0,0,0 };
	oldWorldTransform_.TransferMatrix();

	playerAttackTransform_.translation_ = { 0,0,0 };
	playerAttackTransform_.rotation_ = { 0,0,0 };
	playerAttackTransform_.scale_ = { 0,0,0 };
	playerAttackTransform_.TransferMatrix();

	for (int i = 0; i < SphereCount; i++) {
		playerAttackTransformaaaa_[i].translation_ = { 0,0,0 };
		playerAttackTransformaaaa_[i].rotation_ = { 0,0,0 };
		playerAttackTransformaaaa_[i].scale_ = { 0,0,0 };
		playerAttackTransformaaaa_[i].TransferMatrix();
	}

	playerEvasionTimes = 3;

	recovery->Reset();

	fbxmodel->SetPolygonExplosion({ 1.0f,1.0f,9.42f,600.0f });

	playerNowMotion = PlayerMotion::taiki;
	MaxFrem = 2.0f;
	MinimumFrem = 0.5f;
	isWalk = false;
	isWalking = false;

	isEnemyHit = false;
	isAttackHit = false;
	makeColliders = false;

	timer = 0;
	alpha = 0.0f;
	MaxMoveTime = 60;

	x = 0;
	radius = 4.0f;//当たり判定半径
	playerSpeed = 0.5f;
	playerAvoidance = 20.0f;

	isPushSenter = false;
	isPushLeft = false;
	isPushRight = false;
	isPushBack = false;
	spaceInput = false;

	angle = 0.0f;
	KnockBackDistance = 20.0f;
	isKnockBack = false;
	///攻撃に使う変数

	//時間計算に必要なデータ
	startCount = 0;
	nowCount = 0;
	elapsedCount_ = 0;

	elapsedTime = 0;
	size_t startIndex = 1;

	isAttack = false;

	attackDistanceX = 8.0f;
	attackDistanceZ = 20.0f;

	HP = maxHP;

	hpAlfaSize = { 553.0f,25.0f };
	IsHpAlfa = false;
	hpAlfaTimer = 0;
	frem = 0;

	fremX = 1.0f;

	taikiFlag = false;

	attackConbo = 0;
	isPlayMotion = false;

	receptionTime = 0.0f;
	conboFlag = false;

	isSpace = false;

	isAdmission = true;

	isAlive = true;

	playerEvasionTimes = 0;
	playerEvasionCoolTime = 0;
	playerEvasionMaxTimes = 3;
	CoolTime = 180;

	flame = 0;

	spriteAlpha1 = 0.0f;
	spriteAlpha2 = 0.0f;
	spriteAlpha3 = 0.0f;
	isInput = false;
}

void Player::SpriteInitialize()
{


#pragma region 画像の読み込み
	// 画像の読み込み

	//体力の画像読み込み
	healthSprite = Sprite::Create(TextureManager::Load("Hp_inside.png"));
	healthSprite->SetAnchorPoint({ 0,0 });

	healthAlfaSprite = Sprite::Create(TextureManager::Load("Hp_insideAlfa.png"));
	healthAlfaSprite->SetAnchorPoint({ 0,0 });

	HP_barSprite = Sprite::Create(TextureManager::Load("bossBar.png"));
	HP_barSprite->SetAnchorPoint({ 0.5,0.5 });

	// Attackフォント
	AttackFontSprite[0] = Sprite::Create(TextureManager::Load("Attack_off.png"));
	AttackFontSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	AttackFontSprite[1] = Sprite::Create(TextureManager::Load("Attack_on.png"));
	AttackFontSprite[1]->SetAnchorPoint({ 0.5f,0.5f });
	// MOVEフォント
	MoveFontSprite = Sprite::Create(TextureManager::Load("Move_Font.png"));
	MoveFontSprite->SetAnchorPoint({ 0.5f,0.5f });
	// Wフォント
	W_FontSprite[0] = Sprite::Create(TextureManager::Load("160x144_W_Font.png"));
	W_FontSprite[0]->SetTextureRect({ 0,0 }, { 160,144 });
	W_FontSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	W_FontSprite[1] = Sprite::Create(TextureManager::Load("160x144_W_Font.png"));
	W_FontSprite[1]->SetTextureRect({ 160,0 }, { 160,144 });
	W_FontSprite[1]->SetAnchorPoint({ 0.5f,0.5f });
	// Aフォント
	A_FontSprite[0] = Sprite::Create(TextureManager::Load("160x144_A_Font.png"));
	A_FontSprite[0]->SetTextureRect({ 0,0 }, { 160,144 });
	A_FontSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	A_FontSprite[1] = Sprite::Create(TextureManager::Load("160x144_A_Font.png"));
	A_FontSprite[1]->SetTextureRect({ 160,0 }, { 160,144 });
	A_FontSprite[1]->SetAnchorPoint({ 0.5f,0.5f });
	// Sフォント
	S_FontSprite[0] = Sprite::Create(TextureManager::Load("160x144_S_Font.png"));
	S_FontSprite[0]->SetTextureRect({ 0,0 }, { 160,144 });
	S_FontSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	S_FontSprite[1] = Sprite::Create(TextureManager::Load("160x144_S_Font.png"));
	S_FontSprite[1]->SetTextureRect({ 160,0 }, { 160,144 });
	S_FontSprite[1]->SetAnchorPoint({ 0.5f,0.5f });
	// Dフォント
	D_FontSprite[0] = Sprite::Create(TextureManager::Load("160x144_D_Font.png"));
	D_FontSprite[0]->SetTextureRect({ 0,0 }, { 160,144 });
	D_FontSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	D_FontSprite[1] = Sprite::Create(TextureManager::Load("160x144_D_Font.png"));
	D_FontSprite[1]->SetTextureRect({ 160,0 }, { 160,144 });
	D_FontSprite[1]->SetAnchorPoint({ 0.5f,0.5f });
	// Avoidフォント
	AvoidFontSprite[0] = Sprite::Create(TextureManager::Load("Avoid_OnOff_Sheet.png"));
	AvoidFontSprite[0]->SetTextureRect({ 0,0 }, { 576,96 });
	AvoidFontSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	AvoidFontSprite[1] = Sprite::Create(TextureManager::Load("Avoid_OnOff_Sheet.png"));
	AvoidFontSprite[1]->SetTextureRect(Vector2{ 576,0 }, Vector2{ 576,96 });
	AvoidFontSprite[1]->SetAnchorPoint({ 0.5f,0.5f });

	avoidGauge1 = Sprite::Create(TextureManager::Load("avoidGauge_01.png"));
	avoidGauge1->SetAnchorPoint({ 0,0 });

	avoidGauge2 = Sprite::Create(TextureManager::Load("avoidGauge_02.png"));
	avoidGauge2->SetAnchorPoint({ 0,0 });

	avoidGauge3 = Sprite::Create(TextureManager::Load("avoidGauge_03.png"));
	avoidGauge3->SetAnchorPoint({ 0,0 });

	avoidGauge_under = Sprite::Create(TextureManager::Load("avoidGauge_under.png"));
	avoidGauge_under->SetAnchorPoint({ 0,0 });
#pragma endregion



	// サイズ指定の変数

	Vector2 AttackFontsize = { 188.0f,54.0f };
	Vector2 MoveFontsize = { 163.0f,43.0f };
	Vector2 W_Fontsize = { 32.0f ,28.0f };
	Vector2 A_Fontsize = { 32.0f ,28.0f };
	Vector2 S_Fontsize = { 32.0f ,28.0f };
	Vector2 D_Fontsize = { 32.0f ,28.0f };
	Vector2 AvoidFontsize = { 259.0f ,43.0f };
	Vector2 HP_barSize = { 576.0f ,45.0f };


	avoidGauge1->SetSize(avoidGaugeUnderSize);
	avoidGauge2->SetSize(avoidGaugeUnderSize);
	avoidGauge3->SetSize(avoidGaugeUnderSize);
	avoidGauge_under->SetSize(avoidGaugeUnderSize);


	// サイズをセットする
	healthAlfaSprite->SetSize(hpAlfaSize);
	MoveFontSprite->SetSize(MoveFontsize);
	HP_barSprite->SetSize(HP_barSize);

	for (int i = 0; i < 2; i++) {
		AttackFontSprite[i]->SetSize(AttackFontsize);
		W_FontSprite[i]->SetSize(W_Fontsize);
		A_FontSprite[i]->SetSize(A_Fontsize);
		S_FontSprite[i]->SetSize(S_Fontsize);
		D_FontSprite[i]->SetSize(D_Fontsize);
		AvoidFontSprite[i]->SetSize(AvoidFontsize);
	}
}
