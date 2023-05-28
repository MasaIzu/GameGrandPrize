#include "Player.h"
#include "WinApp.h"
#include"MyMath.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>
//#include"ImGuiManager.h"
#include <FbxLoader.h>
#include "Easing.h"

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
		AttackCollider[i] = new SphereCollider(Vector4(0, AttackRadius, 0, 0), AttackRadius);
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
	//worldTransform_.SetRot({ 0.0f,MyMath::GetAngle(-90.0f),0.0f });
	worldTransform_.alpha = 0.0;

	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();
	playerAttackTransform_.TransferMatrix();

	ParticleMan = std::make_unique<ParticleManager>();
	ParticleMan->Initialize();
	ParticleMan->SetTextureHandle(TextureManager::Load("effect4.png"));

	PlayerRot = { 0.0f,MyMath::GetAngle(-90.0f),0.0f };
	ALLROT = { 0.0f,MyMath::GetAngle(-90.0f),0.0f };
	AllRot = { 0.0f,MyMath::GetAngle(-90.0f),0.0f };
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

	fbxmodel.reset(FbxLoader::GetInstance()->LoadModelFromFile("3dKyaraFixUlt"));
	fbxmodel->Initialize();
	fbxmodel2 = fbxmodel.get();
	fbxmodel2->Initialize();
	fbxmodel->SetPolygonExplosion({ 1.0f,1.0f,9.42f,600.0f });
	modelAnim = std::make_unique<FbxAnimation>();
	modelAnim->Load("3dKyaraFixUlt");

	std::unique_ptr<FbxModel> fbxmodel;

	LBoneTrans.Initialize();
	RBoneTrans.Initialize();

	LBoneTrans.alpha = 0;
	RBoneTrans.alpha = 0;

	ULTKEN.Initialize();

	LSowrdModel.reset(Model::CreateFromOBJ("ken", true));
	RSowrdModel.reset(Model::CreateFromOBJ("ken", true));

	ModelKEN.reset(Model::CreateFromOBJ("kenn", true));

	{
		LightGroup light = LSowrdModel->GetLigit();

		light.SetDirLightActive(2, true);
		light.SetDirLightActive(1, true);
		light.SetDirLightActive(0, true);

		light.SetDirLightColor(0, { 5,5,0 });
		light.SetDirLightColor(1, { 5,5,0 });
		light.SetDirLightColor(2, { 5,5,0 });

		LSowrdModel->SetLight(light);
	}
	{
		LightGroup light = RSowrdModel->GetLigit();

		light.SetDirLightActive(2, true);
		light.SetDirLightActive(1, true);
		light.SetDirLightActive(0, true);

		light.SetDirLightColor(0, { 5,5,0 });
		light.SetDirLightColor(1, { 5,5,0 });
		light.SetDirLightColor(2, { 5,5,0 });

		RSowrdModel->SetLight(light);
	}

	BoneParent.Initialize();

	LSowrdModel->SetPolygonExplosion({ 0.0f,1.0f,6.28,600.0f });
	RSowrdModel->SetPolygonExplosion({ 0.0f,1.0f,6.28,600.0f });

	// SEの初期化
	playerAttackSE.SoundLoadWave("playerAttack.wav");

	playerAttackSE4.SoundLoadWave("playerAttack4.wav");

	playerDamegeSE.SoundLoadWave("playerDamage.wav");

	ModelTrans.Initialize();
	ModelTrans.translation_ = worldTransform_.translation_;
	ModelTrans.scale_ = worldTransform_.scale_;
	ModelTrans.alpha = worldTransform_.alpha;
	Matrix4 Mattttt;
	Mattttt *= MyMath::Rotation(PlayerRot, 2);
	ModelTrans.SetMatRot(Mattttt);
	ModelTrans.TransferMatrix();

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
		if (isPlayerDieMotion2 == false) {
			worldTransform_.alpha = a;
		}
		if (flame >= endflame)
		{
			isAdmission = false;
			//worldTransform_.alpha = 1;
			fbxmodel->SetPolygonExplosion({ Destruction,1.0f,polygon._RotationFactor,polygon._PositionFactor });
		}
	}

	if (isAdmission == false && HP > 0)
	{
		OldTrans = worldTransform_.translation_;

		if (isAwakening == false) {
			Move();

			BoneParent.translation_ = worldTransform_.translation_;
			BoneParent.scale_ = worldTransform_.scale_;

			Attack();

			if (isPlayerUlt == false) {
				UltStart();
			}

			KnockBackUpdate();
			UltUpdate();
		}
		if (playerNowMotion == PlayerMotion::soukenCombo1) {
			if (AttackWaitTime > 0) {
				AttackWaitTime--;
			}
			else {
				isAttckWaiting = true;
			}
			if (isAttckWaiting == true) {
				if (AttackWaitintTime > 0) {
					AttackWaitintTime--;
				}
				else {
					isAttckWaiting = false;
				}
			}
		}

		if (isAwakening == true) {
			if (UltKenGenerationTime < UltKenGenerationMaxTime) {
				UltKenGenerationTime++;
				AttackRotX = 0.0f;
				AttackRotY = 0.0f;
				AttackRotZ = 0.0f;
				SowrdDrowTime = 0;
			}
			else {
				isUltKenGeneration = true;

				if (UltKenGenerationTime < 125) {
					UltKenGenerationTime++;
				}
				else {

					if (attackMoveTimer < MaxAttackMoveTimer) {
						attackMoveTimer += 1.0;
						SowrdDrowTime++;
						NotSowrdDrowTime = 10;
					}
					if (SowrdDrowTime < MaxSowrdRotate) {
						if (SowrdDrowTime > 10 && SowrdDrowTime < 35) {
							AttackRotX += MyMath::GetAngle(100.0f) / 24.0f;
							AttackRotY += (-24.0f) / 24.0f;
							AttackRotZ += (25.0f + MyMath::GetAngle(90.0f)) / 24.0f;
						}
					}

					Matrix4 rooooootttt;
					rooooootttt *= MyMath::Rotation(Vector3(PlayerRot.x + MyMath::GetAngle(AttackRotX) + MyMath::GetAngle(AttackOnlyLeftRotX), PlayerRot.y, PlayerRot.z), 1);
					rooooootttt *= MyMath::Rotation(Vector3(0.0f, 0.0f, PlayerRot.z + MyMath::GetAngle(-AttackRotZ) + MyMath::GetAngle(-AttackOnlyLeftRotZ)), 3);
					rooooootttt *= MyMath::Rotation(Vector3(0.0f, PlayerRot.y + MyMath::GetAngle(AttackRotY) + MyMath::GetAngle(AttackOnlyLeftRotY), 0.0f), 2);

					ULTKEN.SetMatRot(rooooootttt);
					ULTKEN.SetLookMatRot(rooooootttt);
				}
			}
		}

		if (isAttckWaiting == false) {

			if (frem < MaxFrem) {
				if (FinULT == false) {
					frem += 0.013f;
					if (isPlayMotion) {
						frem += 0.015;
					}
					if (playerNowMotion == PlayerMotion::soukenCombo1) {
						frem += 0.015;
					}
					else if (playerNowMotion == PlayerMotion::soukenCombo2) {
						frem += 0.015;
					}
					if (playerNowMotion == PlayerMotion::Ult1) {
						frem -= 0.003f;
					}
					else if (playerNowMotion == PlayerMotion::Ult2) {
						frem -= 0.003f;
					}
				}
				else {
					frem -= 0.013;

					ULTKEN.alpha -= 0.01f;

					if (frem < 1.20f) {
						isUltKenGeneration = false;
					}
					if (frem < 0) {
						isPlayerUlt = false;
						FinULT = false;
						isAwakening = false;
						ULTKEN.alpha = 1.0f;
						playerNowMotion = PlayerMotion::taiki;
					}
				}

			}
			else {
				frem = MinimumFrem;
				isPlayMotion = false;
				if (isWalking == false) {
					if (isAttack == false) {
						if (isKnockBack == false) {
							if (isPlayerUlt == false) {
								playerNowMotion = PlayerMotion::taiki;
							}
						}
					}
				}
				if (isAwakening == true) {
					isAwakening = false;
					isPlayerUlt = true;
				}

				if (isPlayerUlt == true) {
					playerNowMotion = PlayerMotion::taikiBigSowrd;
				}
			}
			if (conboFlag == true) {
				receptionTime += 1;
			}

		}


		if (isAttackHit)
		{
			AttackCollision();
		}




	}
	if (HP <= 0.0f && isAlive)
	{
		//worldTransform_.alpha -= 0.05;
		if (isPlayerDieMotion == false) {
			isPlayerDieMotion = true;
			playerNowMotion = PlayerMotion::DeathMotion;
			frem = 0;

			//スペースキーを押していたら
			for (int i = 0; i < 10; i++)
			{
				//消えるまでの時間
				const float rnd_life = 10.0f;
				//最低限のライフ
				const float constlife = 10;
				float life = (float)rand() / RAND_MAX * rnd_life + constlife;

				//XYZの広がる距離
				const float rnd_pos = 30.0f;
				//Y方向には最低限の飛ぶ距離
				const float constPosY = 15;
				Vector3 pos{};
				pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
				pos.y = abs((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + 100;
				pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

				Vector3 startPos = MyMath::GetWorldTransform(worldTransform_.matWorld_);

				Vector3 endPos = MyMath::GetWorldTransform(worldTransform_.matWorld_) + pos;
				//追加
				ParticleMan->Add(ParticleManager::Type::Normal, life, false, startPos, { 0,0,0 }, endPos, 0.5f, 0.5f, { 0,0,0,1 }, { 0,0,0,1 });
			}



		}

		if (isPlayerDieMotion) {
			if (frem < 2.1f) {
				frem += 0.013f;
			}
			else {
				if (isPlayerDieMotion2 == false) {
					worldTransform_.alpha = 0.5f;
					isPlayerDieMotion2 = true;
				}
			}
		}

		if (isPlayerDieMotion2) {
			worldTransform_.alpha -= 0.01f;
			isAdmission = true;
		}

		if (worldTransform_.alpha <= 0.0f)
		{
			isAlive = false;
			isPlayerDieMotion2 = false;
			ParticleMan->AllDelete();
			LBoneTrans.alpha = 0;
			RBoneTrans.alpha = 0;
			LBoneTrans.TransferMatrix();
			RBoneTrans.TransferMatrix();
		}
	}
	ParticleMan->Update();

	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();
	playerAttackTransform_.TransferMatrix();
	BoneParent.TransferMatrix();

	ModelTrans.translation_ = worldTransform_.translation_;
	ModelTrans.scale_ = worldTransform_.scale_;
	ModelTrans.alpha = worldTransform_.alpha;
	ModelTrans.TransferMatrix();


	collider->AddPlayerMovement(PlayerMoveMent);
	collider->Update(worldTransform_.matWorld_);
	recovery->Update();

	fbxmodel->ModelAnimation(frem, modelAnim->GetAnimation(static_cast<int>(playerNowMotion)), BoneNum);
	matL = fbxmodel->GetLeftBonePos() * BoneParent.matWorld_;
	matR = fbxmodel->GetRightBonePos() * BoneParent.matWorld_;

	LBoneTrans.translation_ = MyMath::GetWorldTransform(matL);
	RBoneTrans.translation_ = MyMath::GetWorldTransform(matR);

	ULTKEN.translation_ = MyMath::GetWorldTransform(matL);

	LBoneTrans.scale_ = Vector3(5, 5, 5);
	RBoneTrans.scale_ = Vector3(5, 5, 5);
	ULTKEN.scale_ = Vector3(5, 5, 5);

	LBoneTrans.TransferMatrix();
	RBoneTrans.TransferMatrix();

	ULTKEN.TransferMatrix();

	isAttackHit = false;



	/*ImGui::Begin("player");


	ImGui::Text("UltGage:%d", UltGage);
	ImGui::Text("isPlayMotion:%d", isPlayMotion);
	ImGui::Text("isPlayMotion:%d", isCoolTimeRiset);
	ImGui::SliderInt("AttackWaitTime", &AttackWaitTime, 0, 60);
	ImGui::SliderInt("AttackWaitintTime", &AttackWaitintTime, 0, 60);

	ImGui::SliderInt("AttackWaitTime", &maxAttackWaitTime, 0, 60);
	ImGui::SliderInt("AttackWaitintTime", &maxAttackWaitintTime, 0, 60);



	ImGui::Text("frem:%f", frem);
	ImGui::Text("AttackCoolTime:%d", AttackCoolTimeMax);
	ImGui::Text("SowrdDrowTime:%d", SowrdDrowTime);
	ImGui::Text("MinimumFrem:%f", MinimumFrem);
	ImGui::Text("look:%f", worldTransform_.look.z);

	ImGui::Text("AttackMovememtX:%f", AttackMovememt.x);
	ImGui::Text("AttackMovememtY:%f", AttackMovememt.y);
	ImGui::Text("AttackMovememtZ:%f", AttackMovememt.z);

	ImGui::Text("PlayerContactPos:%f", PlayerContactPos.x);
	ImGui::Text("PlayerContactPos:%f", PlayerContactPos.y);
	ImGui::Text("PlayerContactPos:%f", PlayerContactPos.z);

	float endflame = 36;

	float Destruction = 1.0f * (SowrdDFlame / endflame);
	float a = 1.0f * (SowrdAFlame / endflame);
	ImGui::Text("a:%f", a);
	ImGui::Text("D:%f", Destruction);

	ImGui::SliderFloat("AttackRotX", &AttackRotX, -360.0f, 360.0f);
	ImGui::SliderFloat("AttackRotY", &AttackRotY, -360.0f, 360.0f);

	ImGui::SliderFloat("AttackRotZ", &AttackRotZ, -360.0f, 360.0f);
	ImGui::SliderFloat("AttackOnlyLeftRotX", &AttackOnlyLeftRotX, -360.0f, 360.0f);
	ImGui::SliderFloat("AttackOnlyLeftRotY", &AttackOnlyLeftRotY, -360.0f, 360.0f);
	ImGui::SliderFloat("AttackOnlyLeftRotZ", &AttackOnlyLeftRotZ, -360.0f, 360.0f);

	ImGui::SliderFloat("AttackOnlyRightRotX", &AttackOnlyRightRotX, -360.0f, 360.0f);
	ImGui::SliderFloat("AttackOnlyRightRotY", &AttackOnlyRightRotY, -360.0f, 360.0f);
	ImGui::SliderFloat("AttackOnlyRightRotZ", &AttackOnlyRightRotZ, -360.0f, 360.0f);

	ImGui::SliderFloat("BoneParentRotY", &BoneParentRotY, -360.0f, 360.0f);

	ImGui::End();*/
}

void Player::Move() {

	PlayerMoveMent = { 0,0,0 };
	PlayerMoveX = false;
	PlayerMoveZ = false;
	rot = { 0,0,0 };
	//ALLROT = { 0,0,0 };
	Avoidance = { 0,0,0 };
	isPushSenter = false;
	isPushLeft = false;
	isPushRight = false;
	isPushBack = false;
	isWalk = false;
	isSpace = false;
	isInput = false;
	isNotPush = false;

	if (isPlayerUlt == false) {
		CoolTime = 80;
	}
	else {
		CoolTime = 30;
	}

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

	root = (worldTransform_.look - worldTransform_.translation_);


	if (isPlayMotion == false) {
		if (input_->PushKey(DIK_W)) {
			PlayerMoveMent += cameraLook * playerSpeed;
			PlayerMoveZ = true;
			isPushSenter = true;
			isWalk = true;
			isInput = true;
			rot = Vector3(0, -MyMath::GetAngle(angle), 0);
			if (isPlayerUlt == false) {
				playerNowMotion = PlayerMotion::aruki;
			}
			else {
				ALLROT = Vector3(0, -MyMath::GetAngle(angle), MyMath::GetAngle(20));
				playerNowMotion = PlayerMotion::taikiBigSowrd;
			}
		}
		if (input_->PushKey(DIK_A)) {
			PlayerMoveMent += root.normalize() * playerSpeed;
			PlayerMoveX = true;
			isPushLeft = true;
			isWalk = true;
			isInput = true;
			rot = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(-90), 0);
			if (isPlayerUlt == false) {
				playerNowMotion = PlayerMotion::aruki;
			}
			else {
				ALLROT = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(-90), MyMath::GetAngle(20));
				playerNowMotion = PlayerMotion::taikiBigSowrd;
			}
		}
		if (input_->PushKey(DIK_S)) {
			PlayerMoveMent -= cameraLook * playerSpeed;
			PlayerMoveZ = true;
			isPushBack = true;
			isWalk = true;
			isInput = true;
			rot = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(180), 0);
			if (isPlayerUlt == false) {
				playerNowMotion = PlayerMotion::aruki;
			}
			else {
				ALLROT = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(180), MyMath::GetAngle(20));
				playerNowMotion = PlayerMotion::taikiBigSowrd;
			}
		}
		if (input_->PushKey(DIK_D)) {
			PlayerMoveMent += root.normalize() * playerSpeed;
			PlayerMoveX = true;
			isPushRight = true;
			isWalk = true;
			isInput = true;
			rot = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(90), 0);
			if (isPlayerUlt == false) {
				playerNowMotion = PlayerMotion::aruki;
			}
			else {
				ALLROT = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(90), MyMath::GetAngle(20));
				playerNowMotion = PlayerMotion::taikiBigSowrd;
			}
		}
		if (PlayerMoveZ == true && PlayerMoveX == true)
		{
			PlayerMoveMent /= 2;
		}
		if (isPushLeft == true && isPushRight == true) {
			PlayerMoveMent = Vector3(0, 0, 0);
			playerNowMotion = PlayerMotion::taiki;
		}

		if (isPushSenter == true && isPushBack == true) {
			playerNowMotion = PlayerMotion::taiki;
			if (isPushLeft == true || isPushRight == true) {
				playerNowMotion = PlayerMotion::aruki;
			}
		}

		if (isPlayerUlt == false) {
			if (isPushSenter == true && isPushLeft == true) {
				rot = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(-45.0f), 0.0f);
			}
			if (isPushSenter == true && isPushRight == true) {
				rot = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(45.0f), 0.0f);
			}

			if (isPushBack == true && isPushLeft == true) {
				rot = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(-135.0f), 0.0f);
			}
			if (isPushBack == true && isPushRight == true) {
				rot = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(135.0f), 0.0f);
			}
		}
		else {
			if (isPushSenter == true && isPushLeft == true) {
				rot = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(-45.0f), 0.0f);
				ALLROT = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(-45.0f), MyMath::GetAngle(20));
			}
			if (isPushSenter == true && isPushRight == true) {
				rot = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(45.0f), 0.0f);
				ALLROT = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(45.0f), MyMath::GetAngle(20));
			}

			if (isPushBack == true && isPushLeft == true) {
				rot = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(-135.0f), 0.0f);
				ALLROT = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(-135.0f), MyMath::GetAngle(20));
			}
			if (isPushBack == true && isPushRight == true) {
				rot = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(135.0f), 0.0f);
				ALLROT = Vector3(0, -MyMath::GetAngle(angle) + MyMath::GetAngle(135.0f), MyMath::GetAngle(20));
			}
		}

		if (isInput == true) {
			if (isPlayerUlt == false) {
				PlayerRot = rot;
				AllRot = rot;
			}
			else {
				PlayerRot = rot;
				AllRot = ALLROT;
			}
		}
		else {
			AllRot.z = 0.0f;
		}

	}
	else {



	}

	if (playerEvasionTimes > 0) {
		if (spaceInput == false) {
			if (input_->TriggerKey(DIK_SPACE)) {
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
				spaceInput = true;
				if (isPlayerUlt == false) {
					isPlayMotion = false;
					playerNowMotion = taiki;
				}
				else {
					isPlayMotion = false;
					playerNowMotion = PlayerMotion::taikiBigSowrd;
				}

				if (input_->PushKey(DIK_A)) {
					isPushLeft = true;
				}
				if (input_->PushKey(DIK_S)) {
					isPushBack = true;
				}
				if (input_->PushKey(DIK_D)) {
					isPushRight = true;
				}


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
					isNotPush = true;
				}
			}
		}
	}



	//worldTransform_.SetRot({ MyMath::GetAngle(rot.x),-MyMath::GetAngle(angle) + MyMath::GetAngle(rot.y), MyMath::GetAngle(rot.z) });
	Matrix4 roooooot;
	roooooot *= MyMath::Rotation(PlayerRot, 1);
	roooooot *= MyMath::Rotation(PlayerRot, 3);
	roooooot *= MyMath::Rotation(PlayerRot, 2);

	Matrix4 rooteee;
	rooteee *= MyMath::Rotation(AllRot, 1);
	rooteee *= MyMath::Rotation(AllRot, 3);
	rooteee *= MyMath::Rotation(AllRot, 2);

	worldTransform_.SetMatRot(roooooot);
	oldWorldTransform_.SetMatRot(roooooot);

	worldTransform_.SetLookMatRot(roooooot);
	worldTransform_.TransferMatrix();

	ModelTrans.SetMatRot(rooteee);
	ModelTrans.TransferMatrix();

	BoneParent.SetMatRot(roooooot);

	CameraRot = MyMath::Rotation(Vector3(Rot.x, Rot.y, Rot.z), 6);
	Avoidance = MyMath::MatVector(CameraRot, Avoidance);
	Avoidance.y = 0;
	Avoidance.normalize();
	Avoidance = Avoidance * playerAvoidance;

	if (isNotPush == true) {
		Avoidance = worldTransform_.look - worldTransform_.translation_;
		Avoidance.normalize();
		Avoidance *= playerAvoidance;
	}


	//worldTransform_.translation_ += playerMovement;
	//worldTransform_.translation_ += Avoidance;
	AvoidanceMove += Avoidance;

	Vector3 allMove = PlayerMoveMent + AvoidanceMove + AttackMovememt;

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
		if (isPlayerUlt == false) {
			MaxFrem = 1.3f;
			MinimumFrem = 0.45f;
		}
		if (isPlayerUlt == true) {
			MaxFrem = 2.0f;
			MinimumFrem = 0.0f;
		}
	}
	else {
		if (isPlayMotion == false) {
			if (isWalking == true) {
				if (playerNowMotion == PlayerMotion::aruki) {
					frem = 1.63f;
				}
				else if (playerNowMotion == PlayerMotion::taikiBigSowrd) {

				}
			}

			if (isPlayerUlt == false) {
				isWalking = false;
				MaxFrem = 1.8f;
				MinimumFrem = 1.8f;
				isSowrd = false;
			}
			else if (isPlayerUlt == true) {
				MaxFrem = 2.0f;
				MinimumFrem = 0.0f;
			}
		}

	}

	if (playerNowMotion == PlayerMotion::taiki) {
		MinimumFrem = 0.0f;
		MaxFrem = 2.0f;
		isSowrd = false;
	}

	worldTransform_.TransferMatrix();
}

void Player::Attack() {

	Vector3 moveRot = cameraLook;
	playerAttackMovement = 0.0f;
	AttackMovememt = { 0, 0, 0 };
	isCoolTimeRiset = false;
	if (isPlayerUlt == false) {

		if (spaceInput == false) {
			if (isEnemyDamage == false) {
				if (input_->MouseInputTrigger(0)) {
					//実行前にカウント値を取得
					//計測開始時間の初期化
					startCount = 0;
					timeRate = 0;
					startIndex = 1;

					isPorigonFlag = false;

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
						MinimumFrem = 2.0f;
						MaxFrem = 2.0f;
						frem = 0.0f;
						receptionTime = 0;
						conboFlag = true;

					}
					if (attackConbo == 1) {
						if (receptionTime > 19 && receptionTime < 50) {
							attackConbo = 2;
							playerNowMotion = PlayerMotion::soukenCombo2;

							isPlayMotion = true;
							MinimumFrem = 2.0f;
							MaxFrem = 2.0f;
							frem = 0.0f;
							//AttackWaitTime = 10;
							//AttackWaitingTime = 10;
							receptionTime = 0;

							conboFlag = true;


							SowrdDFlame = 36;
							SowrdAFlame = 0;

						}
					}
					else if (attackConbo == 2) {
						if (receptionTime > 19 && receptionTime < 50) {
							attackConbo = 3;
							playerNowMotion = PlayerMotion::soukenCombo3;
							isPlayMotion = true;
							MinimumFrem = 1.86f;
							MaxFrem = 1.88f;
							frem = 0.0f;
							receptionTime = 0;
						}
					}
					else if (attackConbo == 3) {
						if (receptionTime > 30 && receptionTime < 60) {
							attackConbo = 4;
							playerNowMotion = PlayerMotion::soukenCombo4;
							isPlayMotion = true;
							MinimumFrem = 1.86f;
							MaxFrem = 1.88f;
							frem = 0.0f;
							receptionTime = 0;
						}
					}
					else if (attackConbo == 4) {
						if (receptionTime > 27 && receptionTime < 70) {
							attackConbo = 5;
							playerNowMotion = PlayerMotion::soukenCombo5;
							isPlayMotion = true;
							MinimumFrem = 1.86f;
							MaxFrem = 1.88f;
							frem = 0.0f;
							receptionTime = 0;

						}
					}

				}
			}
		}


		if (isSowrd)
		{
			if (SowrdAFlame < 18 && SowrdDFlame>0)
			{
				SowrdAFlame++;

				float endflame = 18;

				float a = 1.0f * (SowrdAFlame / endflame);
				{
					LBoneTrans.alpha = a;
				}
				{
					RBoneTrans.alpha = a;
				}
			}
		}
		else
		{
			if (SowrdAFlame > 0 && SowrdDFlame < 36)
			{
				SowrdAFlame--;
				SowrdDFlame++;

				float endflame = 36;

				float Destruction = 1.0f * (SowrdDFlame / endflame);
				float a = 1.0f * (SowrdAFlame / endflame);
				{
					Model::ConstBufferPolygonExplosion polygon = LSowrdModel->GetPolygonExplosion();
					LSowrdModel->SetPolygonExplosion({ Destruction,polygon._ScaleFactor,polygon._PositionFactor,polygon._PositionFactor });
					LBoneTrans.alpha = a;
				}
				{
					Model::ConstBufferPolygonExplosion polygon = RSowrdModel->GetPolygonExplosion();
					RSowrdModel->SetPolygonExplosion({ Destruction,polygon._ScaleFactor,polygon._PositionFactor,polygon._PositionFactor });
					RBoneTrans.alpha = a;
				}
			}
		}



		if (isEnemyDamage == false) {
			if (playerNowMotion == PlayerMotion::soukenCombo1) {
				if (IsCombo == false) {
					playerAttackMovement = 20.0f;
					LookingMove = worldTransform_.look - GetWorldPosition();

					if (isPlayerEnemycontact == true) {
						LookingMove = LookingMove * playerAttackMovement;
					}
					else {
						LookingMove = LookingMove * playerAttackMovement;
					}
					attackMoveTimer = 0;

					// 音を鳴らす
					playerAttackSE.SoundPlayWave(false, 0.5f);

					AttackNowPos = worldTransform_.translation_;
					IsCombo = true;
					IsCombo2 = false;
					IsCombo3 = false;
					IsCombo4 = false;
					IsCombo5 = false;

					BoneParentRotY = 0.0f;

					SowrdDrowTime = 0;
					MaxSowrdRotate = 35;
					OldAttackRotX = 0.0f;
					OldAttackRotY = 0.0f;
					OldAttackRotZ = 0.0f;
					AttackRotX = 0.0f;
					AttackRotY = 0.0f;
					AttackRotZ = 2.0f;
					AttackOnlyLeftRotX = 0.0f;
					AttackOnlyLeftRotY = 0.0f;
					AttackOnlyLeftRotZ = 0.0f;
					AttackOnlyRightRotX = 0.0f;
					AttackOnlyRightRotY = 0.0f;
					AttackOnlyRightRotZ = 0.0f;
					AttackWaitintTime = maxAttackWaitintTime;
					AttackWaitTime = maxAttackWaitTime;
					isSowrd = true;
					Model::ConstBufferPolygonExplosion polygon = LSowrdModel->GetPolygonExplosion();
					LSowrdModel->SetPolygonExplosion({ 0,polygon._ScaleFactor,polygon._RotationFactor,polygon._PositionFactor });
					RSowrdModel->SetPolygonExplosion({ 0,polygon._ScaleFactor,polygon._RotationFactor,polygon._PositionFactor });

					AttackCoolTimeMax = 360;

					isCoolTimeRiset = true;
				}
				if (attackMoveTimer < MaxAttackMoveTimer) {
					attackMoveTimer += 1.0;
					SowrdDrowTime++;
					NotSowrdDrowTime = 10;
				}
				if (SowrdDrowTime < MaxSowrdRotate) {
					AttackRotX += 6.0f;
					AttackRotZ += 1.1f;
				}

				AttackNowPos += PlayerContactPos;

				AttackMovememt = Easing::InOutVec3(AttackNowPos, AttackNowPos + LookingMove, attackMoveTimer, MaxAttackMoveTimer) - worldTransform_.translation_;

			}
			else if (playerNowMotion == PlayerMotion::soukenCombo2) {
				if (IsCombo2 == false) {
					playerAttackMovement = 10.0f;
					LookingMove = worldTransform_.look - GetWorldPosition();

					if (isPlayerEnemycontact == true) {
						LookingMove = LookingMove * playerAttackMovement;
					}
					else {
						LookingMove = LookingMove * playerAttackMovement;
					}
					attackMoveTimer = 0;
					// 音を鳴らす
					playerAttackSE.SoundPlayWave(false, 0.5f);

					AttackNowPos = worldTransform_.translation_;
					IsCombo = false;
					IsCombo2 = true;
					IsCombo3 = false;
					IsCombo4 = false;
					IsCombo5 = false;

					//前回の回転を残す
					OldAttackRotX = AttackRotX;
					OldAttackRotZ = AttackRotZ;

					SowrdDrowTime = 0;
					MaxSowrdRotate = 35;
					AttackRotX = -212.0f;
					AttackRotY = 0.0f;
					AttackRotZ = 0.0f;
					AttackOnlyLeftRotX = 0.0f;
					AttackOnlyLeftRotY = 0.0f;
					AttackOnlyLeftRotZ = 0.0f;

					AttackOnlyRightRotX = 0.0f;
					AttackOnlyRightRotY = 0.0f;
					AttackOnlyRightRotZ = 0.0f;

					BoneParentRotY = 0.0f;

					saveRotX = AttackRotX;

					AttackCoolTimeMax = 360;

					isCoolTimeRiset = true;

					/*Matrix4 rooooootttt;
					rooooootttt *= MyMath::Rotation(Vector3(MyMath::GetAngle(100.0f) + PlayerRot.x + MyMath::GetAngle(AttackRotX), PlayerRot.y, PlayerRot.z), 1);
					rooooootttt *= MyMath::Rotation(Vector3(0.0f, 0.0f, PlayerRot.z + MyMath::GetAngle(-AttackRotZ)), 3);
					rooooootttt *= MyMath::Rotation(Vector3(0.0f, MyMath::GetAngle(90.0f) + PlayerRot.y + MyMath::GetAngle(AttackRotY), 0.0f), 2);

					LBoneTrans.SetMatRot(rooooootttt);

					rooooootttt = MyMath::Initialize();
					rooooootttt *= MyMath::Rotation(Vector3(MyMath::GetAngle(100.0f) + PlayerRot.x + MyMath::GetAngle(AttackRotX), PlayerRot.y, PlayerRot.z), 1);
					rooooootttt *= MyMath::Rotation(Vector3(0.0f, 0.0f, PlayerRot.z + MyMath::GetAngle(AttackRotZ)), 3);
					rooooootttt *= MyMath::Rotation(Vector3(0.0f, MyMath::GetAngle(90.0f) + PlayerRot.y + MyMath::GetAngle(AttackRotY), 0.0f), 2);

					RBoneTrans.SetMatRot(rooooootttt);*/

				}
				if (attackMoveTimer < MaxAttackMoveTimer) {
					attackMoveTimer += 1.0;
					SowrdDrowTime++;
					NotSowrdDrowTime = 10;
				}
				if (SowrdDrowTime < MaxSowrdRotate) {
					if (SowrdDrowTime < 10) {
						AttackRotZ += (-110.0f - OldAttackRotZ) / 10;
					}
					AttackRotX += (-38.0f - saveRotX) / MaxSowrdRotate;

				}

				AttackNowPos += PlayerContactPos;
				AttackMovememt = Easing::InOutVec3(AttackNowPos, AttackNowPos + LookingMove, attackMoveTimer, MaxAttackMoveTimer) - worldTransform_.translation_;
			}
			else if (playerNowMotion == PlayerMotion::soukenCombo3) {
				if (IsCombo3 == false) {
					playerAttackMovement = 15.0f;
					LookingMove = worldTransform_.look - GetWorldPosition();

					if (isPlayerEnemycontact == true) {
						LookingMove = LookingMove * playerAttackMovement;
					}
					else {
						LookingMove = LookingMove * playerAttackMovement;
					}
					attackMoveTimer = 0;
					// 音を鳴らす
					playerAttackSE.SoundPlayWave(false, 0.5f);

					AttackNowPos = worldTransform_.translation_;
					IsCombo = false;
					IsCombo2 = false;
					IsCombo3 = true;
					IsCombo4 = false;
					IsCombo5 = false;

					//前回の回転を残す
					OldAttackRotX = AttackRotX;
					OldAttackRotZ = AttackRotZ;

					SowrdDrowTime = 0;
					MaxSowrdRotate = 35;
					AttackRotX = 0.0f;
					AttackRotY = 0.0f;
					AttackRotZ = 0.0f;

					AttackOnlyLeftRotX = 0.0f;
					AttackOnlyLeftRotY = 0.0f;
					AttackOnlyLeftRotZ = 0.0f;

					AttackOnlyRightRotX = 0.0f;
					AttackOnlyRightRotY = 0.0f;
					AttackOnlyRightRotZ = 0.0f;

					BoneParentRotY = 0.0f;

					AttackCoolTimeMax = 360;

					isCoolTimeRiset = true;
				}
				if (attackMoveTimer < MaxAttackMoveTimer) {
					attackMoveTimer += 1.0;
					SowrdDrowTime++;
					NotSowrdDrowTime = 10;
				}
				if (SowrdDrowTime < MaxSowrdRotate) {
					if (SowrdDrowTime < 10) {
						AttackOnlyLeftRotY += 190.0f / 10.0f;
						AttackOnlyLeftRotZ += (-85.0f) / 10;
					}

				}
				AttackNowPos += PlayerContactPos;
				AttackMovememt = Easing::InOutVec3(AttackNowPos, AttackNowPos + LookingMove, attackMoveTimer, MaxAttackMoveTimer) - worldTransform_.translation_;
			}
			else if (playerNowMotion == PlayerMotion::soukenCombo4) {
				if (IsCombo4 == false) {
					playerAttackMovement = 35.0f;
					LookingMove = worldTransform_.look - GetWorldPosition();

					if (isPlayerEnemycontact == true) {
						LookingMove = LookingMove * playerAttackMovement;
					}
					else {
						LookingMove = LookingMove * playerAttackMovement;
					}
					attackMoveTimer = 0;
					// 音を鳴らす
					playerAttackSE4.SoundPlayWave(false, 0.5f);

					AttackNowPos = worldTransform_.translation_;
					IsCombo = false;
					IsCombo2 = false;
					IsCombo3 = false;
					IsCombo4 = true;
					IsCombo5 = false;

					//前回の回転を残す
					OldAttackRotX = AttackRotX;
					OldAttackRotZ = AttackRotZ;

					SowrdDrowTime = 0;
					MaxSowrdRotate = 35;
					AttackRotX = 0.0f;
					AttackRotY = 0.0f;
					AttackRotZ = 0.0f;

					AttackOnlyLeftRotX = 0.0f;
					AttackOnlyLeftRotY = 0.0f;
					AttackOnlyLeftRotZ = 0.0f;

					AttackOnlyRightRotX = 0.0f;
					AttackOnlyRightRotY = 0.0f;
					AttackOnlyRightRotZ = 0.0f;

					BoneParentRotY = 0.0f;

					AttackCoolTimeMax = 15;

					isCoolTimeRiset = true;
				}
				if (attackMoveTimer < MaxAttackMoveTimer) {
					attackMoveTimer += 1.0;
					SowrdDrowTime++;
					NotSowrdDrowTime = 10;
				}
				if (SowrdDrowTime < MaxSowrdRotate) {
					if (SowrdDrowTime < 11) {
						AttackOnlyLeftRotY += 155.0f / 10.0f;
						AttackOnlyLeftRotZ += 86.0f / 10.0f;

						AttackOnlyRightRotY += 333.0f / 10.0f;
						AttackOnlyRightRotZ += 108.0f / 10.0f;
					}

					BoneParentRotY += (-360.0f * 3.0f) / (MaxSowrdRotate - 1);
				}
				AttackNowPos += PlayerContactPos;
				AttackMovememt = Easing::InOutVec3(AttackNowPos, AttackNowPos + LookingMove, attackMoveTimer, MaxAttackMoveTimer) - worldTransform_.translation_;
			}
			else if (playerNowMotion == PlayerMotion::soukenCombo5) {
				if (IsCombo5 == false) {
					playerAttackMovement = 20.0f;
					LookingMove = worldTransform_.look - GetWorldPosition();

					if (isPlayerEnemycontact == true) {
						LookingMove = LookingMove * playerAttackMovement;
					}
					else {
						LookingMove = LookingMove * playerAttackMovement;
					}
					attackMoveTimer = 0;
					// 音を鳴らす
					playerAttackSE.SoundPlayWave(false, 0.5f);

					AttackNowPos = worldTransform_.translation_;
					IsCombo = false;
					IsCombo2 = false;
					IsCombo3 = false;
					IsCombo4 = false;
					IsCombo5 = true;

					SowrdDrowTime = 0;
					MaxSowrdRotate = 35;
					AttackRotX = 0.0f;
					AttackRotY = 0.0f;
					AttackRotZ = 0.0f;

					AttackOnlyLeftRotX = 0.0f;
					AttackOnlyLeftRotY = 0.0f;
					AttackOnlyLeftRotZ = 0.0f;

					AttackOnlyRightRotX = 0.0f;
					AttackOnlyRightRotY = 0.0f;
					AttackOnlyRightRotZ = 0.0f;

					BoneParentRotY = 0.0f;

					AttackCoolTimeMax = 360;
					isCoolTimeRiset = true;

				}
				if (attackMoveTimer < MaxAttackMoveTimer) {
					attackMoveTimer += 1.0;
					SowrdDrowTime++;
					NotSowrdDrowTime = 10;
				}
				if (SowrdDrowTime < MaxSowrdRotate) {
					if (SowrdDrowTime < 5) {
						AttackRotX += -60.0f / 4.0f;
						AttackRotY += -182.0f / 4.0f;
						AttackRotZ += 39.0f / 4.0f;
					}
					if (SowrdDrowTime > 5 && SowrdDrowTime < 20) {
						AttackRotX += 14.0f;
					}
				}
				AttackCoolTimeMax = 120;

				AttackNowPos += PlayerContactPos;
				AttackMovememt = Easing::InOutVec3(AttackNowPos, AttackNowPos + LookingMove, attackMoveTimer, MaxAttackMoveTimer) - worldTransform_.translation_;
			}
			else {
				attackMoveTimer = 0;

				AttackRotX = 0.0f;
				AttackRotY = 0.0f;
				AttackRotZ = 0.0f;

				AttackOnlyLeftRotX = 0.0f;
				AttackOnlyLeftRotY = 0.0f;
				AttackOnlyLeftRotZ = 0.0f;

				AttackOnlyRightRotX = 0.0f;
				AttackOnlyRightRotY = 0.0f;
				AttackOnlyRightRotZ = 0.0f;

				IsCombo = false;
				IsCombo2 = false;
				IsCombo3 = false;
				IsCombo4 = false;
				IsCombo5 = false;
				AttackCoolTime = 0;
			}

		}



		Matrix4 rooooootttt;
		rooooootttt *= MyMath::Rotation(Vector3(MyMath::GetAngle(100.0f) + PlayerRot.x + MyMath::GetAngle(AttackRotX) + MyMath::GetAngle(AttackOnlyLeftRotX), PlayerRot.y, PlayerRot.z), 1);
		rooooootttt *= MyMath::Rotation(Vector3(0.0f, 0.0f, PlayerRot.z + MyMath::GetAngle(-AttackRotZ) + MyMath::GetAngle(-AttackOnlyLeftRotZ)), 3);
		rooooootttt *= MyMath::Rotation(Vector3(0.0f, MyMath::GetAngle(90.0f) + PlayerRot.y + MyMath::GetAngle(AttackRotY) + MyMath::GetAngle(AttackOnlyLeftRotY), 0.0f), 2);

		LBoneTrans.SetMatRot(rooooootttt);
		LBoneTrans.SetLookMatRot(rooooootttt);

		rooooootttt = MyMath::Initialize();
		rooooootttt *= MyMath::Rotation(Vector3(MyMath::GetAngle(100.0f) + PlayerRot.x + MyMath::GetAngle(AttackRotX) + MyMath::GetAngle(AttackOnlyRightRotX), PlayerRot.y, PlayerRot.z), 1);
		rooooootttt *= MyMath::Rotation(Vector3(0.0f, 0.0f, PlayerRot.z + MyMath::GetAngle(AttackRotZ) + MyMath::GetAngle(-AttackOnlyRightRotZ)), 3);
		rooooootttt *= MyMath::Rotation(Vector3(0.0f, MyMath::GetAngle(90.0f) + PlayerRot.y + MyMath::GetAngle(AttackRotY) + MyMath::GetAngle(AttackOnlyRightRotY), 0.0f), 2);

		RBoneTrans.SetMatRot(rooooootttt);
		RBoneTrans.SetLookMatRot(rooooootttt);

		Matrix4 roooooottttee;
		roooooottttee *= MyMath::Rotation(Vector3(PlayerRot.x, 0.0f, 0.0f), 1);
		roooooottttee *= MyMath::Rotation(Vector3(0.0f, 0.0f, PlayerRot.z), 3);
		roooooottttee *= MyMath::Rotation(Vector3(0.0f, PlayerRot.y + MyMath::GetAngle(BoneParentRotY), 0.0f), 2);

		BoneParent.SetMatRot(roooooottttee);

		Vector3 LBoneLook = LBoneTrans.lookUp - LBoneTrans.translation_;
		LBoneLook.normalize();

		Vector3 RBoneLook = RBoneTrans.lookDown - RBoneTrans.translation_;
		RBoneLook.normalize();

		for (int i = 0; i < SphereCount; i++) {
			if (i < SphereCount / 2) {
				if (i < SphereCount / 4 + 1) {
					playerAttackTransformaaaa_[i].translation_ = MyMath::GetWorldTransform(matL) + (LBoneLook * i) * AttackCollisionDistance;
				}
				else {
					playerAttackTransformaaaa_[i].translation_ = MyMath::GetWorldTransform(matL) - (LBoneLook * (i - (SphereCount / 4))) * AttackCollisionDistance;
				}
			}
			else {
				if (i < SphereCount / 2 + SphereCount / 4 + 1) {
					playerAttackTransformaaaa_[i].translation_ = MyMath::GetWorldTransform(matR) + (RBoneLook * (i - (SphereCount / 2 + 1))) * AttackCollisionDistance;
				}
				else {
					playerAttackTransformaaaa_[i].translation_ = MyMath::GetWorldTransform(matR) - (RBoneLook * (i - (SphereCount / 2 + SphereCount / 4 - 1))) * AttackCollisionDistance;
				}
			}
			playerAttackTransformaaaa_[i].TransferMatrix();
		}
		if (isPlayMotion == true) {
			for (int i = 0; i < SphereCount; i++) {
				AttackCollider[i]->Update(playerAttackTransformaaaa_[i].matWorld_, AttackCoolTimeMax, isCoolTimeRiset);
				AttackCollider[i]->SetAttribute(COLLISION_ATTR_ATTACK);
			}
		}
		else {
			for (int i = 0; i < SphereCount; i++) {
				AttackCollider[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
			}
			if (isPorigonFlag == false) {
				isPorigonFlag = true;
				isSowrd = false;
				SowrdDFlame = 0;
				SowrdAFlame = 36;
			}
		}
	}
	else if (isPlayerUlt == true) {

		if (spaceInput == false) {
			if (isEnemyDamage == false) {
				if (input_->MouseInputTrigger(0)) {
					//実行前にカウント値を取得
					//計測開始時間の初期化
					startCount = 0;
					nowCount = 0;
					timeRate = 0;
					startIndex = 1;
					//IsCombo = false;
					if (isPlayMotion == false) {

						attackConbo = 1;
						playerNowMotion = PlayerMotion::Ult1;
						isPlayMotion = true;
						MinimumFrem = 1.6f;
						MaxFrem = 1.6f;
						frem = 0.0f;
						receptionTime = 0;
						conboFlag = true;
						isAttckWaiting = false;
					}
					if (attackConbo == 1) {
						if (receptionTime > 32 && receptionTime < 60) {
							attackConbo = 2;
							playerNowMotion = PlayerMotion::Ult2;

							isPlayMotion = true;
							MinimumFrem = 1.6f;
							MaxFrem = 1.6f;
							frem = 0.0f;
							//AttackWaitTime = 10;
							//AttackWaitingTime = 10;
							receptionTime = 0;

							conboFlag = true;
							isAttckWaiting = false;

						}
					}
				}
			}
		}


		if (isEnemyDamage == false) {
			if (playerNowMotion == PlayerMotion::Ult1) {
				if (IsCombo == false) {
					playerAttackMovement = 20.0f;
					LookingMove = worldTransform_.look - GetWorldPosition();

					if (isPlayerEnemycontact == true) {
						LookingMove = LookingMove * playerAttackMovement;
					}
					else {
						LookingMove = LookingMove * playerAttackMovement;
					}
					attackMoveTimer = 0;

					AttackNowPos = worldTransform_.translation_;
					IsCombo = true;
					IsCombo2 = false;
					IsCombo3 = false;
					IsCombo4 = false;
					IsCombo5 = false;

					BoneParentRotY = 0.0f;

					SowrdDrowTime = 0;
					MaxSowrdRotate = 35;
					OldAttackRotX = 0.0f;
					OldAttackRotY = -24.0f;
					OldAttackRotZ = 16.0f;
					AttackRotX = 0.0f;
					AttackRotY = -24.0f;
					AttackRotZ = 16.0f;
					AttackOnlyLeftRotX = 0.0f;
					AttackOnlyLeftRotY = 0.0f;
					AttackOnlyLeftRotZ = 0.0f;
					AttackOnlyRightRotX = 0.0f;
					AttackOnlyRightRotY = 0.0f;
					AttackOnlyRightRotZ = 0.0f;
					AttackWaitintTime = maxAttackWaitintTime;
					AttackWaitTime = maxAttackWaitTime;
					isSowrd = true;
					Model::ConstBufferPolygonExplosion polygon = ModelKEN->GetPolygonExplosion();
					ModelKEN->SetPolygonExplosion({ 0,polygon._ScaleFactor,polygon._RotationFactor,polygon._PositionFactor });

					AttackCoolTimeMax = 360;
					isCoolTimeRiset = true;

				}
				if (attackMoveTimer < MaxAttackMoveTimer) {
					attackMoveTimer += 1.0;
					SowrdDrowTime++;
					NotSowrdDrowTime = 10;
				}
				if (SowrdDrowTime < MaxSowrdRotate) {
					if (SowrdDrowTime < 34) {
						AttackRotX += (30.0f - OldAttackRotX) / 33.0f;
						AttackRotY += (-280.0f - OldAttackRotY) / 33.0f;
						AttackRotZ += (137.0f - OldAttackRotZ) / 33.0f;
					}
				}

				AttackNowPos += PlayerContactPos;

				AttackMovememt = Easing::InOutVec3(AttackNowPos, AttackNowPos + LookingMove, attackMoveTimer, MaxAttackMoveTimer) - worldTransform_.translation_;

			}
			else if (playerNowMotion == PlayerMotion::Ult2) {//ここやる
				if (IsCombo2 == false) {
					playerAttackMovement = 5.0f;
					LookingMove = worldTransform_.look - GetWorldPosition();

					if (isPlayerEnemycontact == true) {
						LookingMove = LookingMove * playerAttackMovement;
					}
					else {
						LookingMove = LookingMove * playerAttackMovement;
					}
					attackMoveTimer = 0;

					AttackNowPos = worldTransform_.translation_;
					IsCombo = false;
					IsCombo2 = true;
					IsCombo3 = false;
					IsCombo4 = false;
					IsCombo5 = false;

					BoneParentRotY = 0.0f;

					SowrdDrowTime = 0;
					MaxSowrdRotate = 35;
					OldAttackRotX = 30.0f;
					OldAttackRotY = -280.0f;
					OldAttackRotZ = 137.0f;
					AttackRotX = 30.0f;
					AttackRotY = -280.0f;
					AttackRotZ = 137.0f;
					AttackOnlyLeftRotX = 0.0f;
					AttackOnlyLeftRotY = 0.0f;
					AttackOnlyLeftRotZ = 0.0f;
					AttackOnlyRightRotX = 0.0f;
					AttackOnlyRightRotY = 0.0f;
					AttackOnlyRightRotZ = 0.0f;
					AttackWaitintTime = maxAttackWaitintTime;
					AttackWaitTime = maxAttackWaitTime;
					isSowrd = true;
					Model::ConstBufferPolygonExplosion polygon = ModelKEN->GetPolygonExplosion();
					ModelKEN->SetPolygonExplosion({ 0,polygon._ScaleFactor,polygon._RotationFactor,polygon._PositionFactor });

					AttackCoolTimeMax = 360;
					isCoolTimeRiset = true;

				}
				if (attackMoveTimer < MaxAttackMoveTimer) {
					attackMoveTimer += 1.0;
					SowrdDrowTime++;
					NotSowrdDrowTime = 10;
				}
				if (SowrdDrowTime < MaxSowrdRotate) {
					if (SowrdDrowTime < 34) {
						AttackRotX += (0.0f - OldAttackRotX) / 33.0f;
						AttackRotY += (-24.0f - OldAttackRotY) / 33.0f;
						AttackRotZ += (16.0f - OldAttackRotZ) / 33.0f;
					}
				}

				AttackNowPos += PlayerContactPos;

				AttackMovememt = Easing::InOutVec3(AttackNowPos, AttackNowPos + LookingMove, attackMoveTimer, MaxAttackMoveTimer) - worldTransform_.translation_;

			}
			else {
				attackMoveTimer = 0;
				IsCombo = false;
				IsCombo2 = false;
				IsCombo3 = false;
				IsCombo4 = false;
				IsCombo5 = false;

				AttackRotX = 0.0f;
				AttackRotY = -24.0f;
				AttackRotZ = 16.0f;
			}
		}



		Matrix4 rooooootttt;
		rooooootttt *= MyMath::Rotation(Vector3(MyMath::GetAngle(100.0f) + PlayerRot.x + MyMath::GetAngle(AttackRotX), PlayerRot.y, PlayerRot.z), 1);
		rooooootttt *= MyMath::Rotation(Vector3(0.0f, 0.0f, PlayerRot.z + MyMath::GetAngle(-AttackRotZ)), 3);
		rooooootttt *= MyMath::Rotation(Vector3(0.0f, MyMath::GetAngle(90.0f) + PlayerRot.y + MyMath::GetAngle(AttackRotY), 0.0f), 2);

		ULTKEN.SetMatRot(rooooootttt);
		ULTKEN.SetLookMatRot(rooooootttt);

		Vector3 LBoneLook = ULTKEN.lookUp - ULTKEN.translation_;
		LBoneLook.normalize();
		AttackCollisionDistance = 3;
		for (int i = 0; i < SphereCount; i++) {

			playerAttackTransformaaaa_[i].translation_ = MyMath::GetWorldTransform(matL) + (LBoneLook * i) * AttackCollisionDistance;
			playerAttackTransformaaaa_[i].scale_ = { AttackRadius,AttackRadius,AttackRadius };
			playerAttackTransformaaaa_[i].TransferMatrix();
		}

		if (isPlayMotion == true) {
			for (int i = 0; i < SphereCount; i++) {
				AttackCollider[i]->Update(playerAttackTransformaaaa_[i].matWorld_, AttackCoolTimeMax, isCoolTimeRiset);
				AttackCollider[i]->SetAttribute(COLLISION_ATTR_ATTACK);
			}
		}
		else {
			for (int i = 0; i < SphereCount; i++) {
				AttackCollider[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
			}
			if (isPorigonFlag == false) {
				isPorigonFlag = true;
				SowrdDFlame = 0;
				SowrdAFlame = 36;
			}
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

	NotMoveTime = 0;

	isKnockBack = true;

	isEnemyDamage = true;
}

void Player::KnockBackUpdate()
{
	if (moveTime < MaxMoveTime) {
		moveTime++;
		KnockBack += PlayerMoveMent / 2;
		KnockBack += Avoidance;
		KnockBack += AttackMovememt / 2;
		Vector3 KnockBackMove = easing_->InOutVec3(MyMath::GetWorldTransform(worldTransform_.matWorld_), KnockBack, moveTime, MaxMoveTime);

		float AR;
		float BR;

		damageFlashFlame++;

		AR = pow((KnockBackMove.x) - 0, 2) + pow((KnockBackMove.z) - 0, 2);
		BR = pow((satgeSize - worldTransform_.scale_.x * 2), 2);

		if (AR <= BR)
		{
			worldTransform_.translation_ = KnockBackMove;
		}
		worldTransform_.TransferMatrix();
		if (isEnemyDamage == true) {

			if (NotMoveTime < MaxNotMoveTime) {
				NotMoveTime++;
				playerNowMotion = PlayerMotion::DamegeAction;

				attackMoveTimer = 0;
				isPlayMotion = false;

				attackConbo = 0;

				frem = 30.0f;
				receptionTime = 0;

				SowrdDrowTime = 0;
				MaxSowrdRotate = 35;
				AttackRotX = 0.0f;
				AttackRotY = 0.0f;
				AttackRotZ = 0.0f;

				AttackOnlyLeftRotX = 0.0f;
				AttackOnlyLeftRotY = 0.0f;
				AttackOnlyLeftRotZ = 0.0f;

				AttackOnlyRightRotX = 0.0f;
				AttackOnlyRightRotY = 0.0f;
				AttackOnlyRightRotZ = 0.0f;

				BoneParentRotY = 0.0f;

				IsCombo = false;
				IsCombo2 = false;
				IsCombo3 = false;
				IsCombo4 = false;
				IsCombo5 = false;

				LookingMove = { 0,0,0 };
				AttackNowPos = worldTransform_.translation_;
			}
			else {
				isEnemyDamage = false;
			}
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

	//for (int i = 0; i < SphereCount; i++) {
	//	playerModel_->Draw(playerAttackTransformaaaa_[i], viewProjection_);
	//}


	startPointModel->Draw(startPointTrans, viewProjection_);

	recovery->Draw(viewProjection_);

	/*if (SowrdDrowTime > NotSowrdDrowTime) {*/

	LSowrdModel->Draw(LBoneTrans, viewProjection_);
	RSowrdModel->Draw(RBoneTrans, viewProjection_);

	if (isUltKenGeneration == true) {
		ModelKEN->Draw(ULTKEN, viewProjection_);
	}
	//}

}

void Player::PlayerFbxDraw(ViewProjection viewProjection_) {
	if (timer > 0) {
		oldWorldTransform_.alpha = alpha;
		fbxmodel2->Draw(oldWorldTransform_, viewProjection_);
	}
	if (spaceInput == false) {
		if (isKnockBack == false || damageFlashFlame % 6 == 0)
		{
			fbxmodel->Draw(ModelTrans, viewProjection_);
		}
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
		playerDamegeSE.SoundPlayWave(false, 0.6f);
		SetKnockBackCount();
		HP -= damage;
		damageFlashFlame = 0;
		if (HP < 0)
		{
			HP = 0;
		}

		int ParticleNumber = 10;
		if (HP <= 0)
		{
			ParticleNumber = 0;
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
			ParticleMan->Add(ParticleManager::Type::Out, life, true, startPos, controlPos, endPos, 1.0f, 1.0f, { 0.5,0,0,1 }, { 0.5,0,0,1 });
		}
		IsHpAlfa = true;
		hpAlfaSize = hpSize;
	}
}

void Player::AttackCollision()
{
	//スペースキーを押していたら
	for (int i = 0; i < 1; i++)
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
	worldTransform_.alpha = 0;
	playerAvoidance = 15.0f;
	moveTime = 300;

	LBoneTrans.translation_ = MyMath::GetWorldTransform(matL);
	RBoneTrans.translation_ = MyMath::GetWorldTransform(matR);

	LBoneTrans.scale_ = Vector3(5, 5, 5);
	RBoneTrans.scale_ = Vector3(5, 5, 5);

	LBoneTrans.alpha = 0;
	RBoneTrans.alpha = 0;;

	LBoneTrans.TransferMatrix();
	RBoneTrans.TransferMatrix();


	//worldTransform_.translation_ = { 0,0,-100 };

	//ワールド変換の初期化
	worldTransform_.translation_ = { 0.0f,0.0f,150.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };

	worldTransform_.alpha = 0.0;

	attackConbo = 0;

	frem = 0.0f;
	receptionTime = 0;

	SowrdDrowTime = 0;
	MaxSowrdRotate = 35;
	AttackRotX = 0.0f;
	AttackRotY = 0.0f;
	AttackRotZ = 0.0f;

	AttackOnlyLeftRotX = 0.0f;
	AttackOnlyLeftRotY = 0.0f;
	AttackOnlyLeftRotZ = 0.0f;

	AttackOnlyRightRotX = 0.0f;
	AttackOnlyRightRotY = 0.0f;
	AttackOnlyRightRotZ = 0.0f;

	BoneParentRotY = 0.0f;


	isPlayMotion = false;
	playerNowMotion = PlayerMotion::taiki;
	isEnemyDamage = false;

	worldTransform_.TransferMatrix();
	collider->Update(worldTransform_.matWorld_);

	ModelTrans.alpha = 0;
	ModelTrans.TransferMatrix();
	AllRot = { 0.0f,MyMath::GetAngle(-90.0f),0.0f };
	oldWorldTransform_.translation_ = { 0,0,0 };
	oldWorldTransform_.rotation_ = { 0,0,0 };
	oldWorldTransform_.scale_ = { 0,0,0 };
	oldWorldTransform_.TransferMatrix();

	playerAttackTransform_.translation_ = { 0,0,0 };
	playerAttackTransform_.rotation_ = { 0,0,0 };
	playerAttackTransform_.scale_ = { 0,0,0 };
	playerAttackTransform_.TransferMatrix();
	isPlayerDieMotion = false;

	for (int i = 0; i < SphereCount; i++) {
		playerAttackTransformaaaa_[i].translation_ = { 0,0,0 };
		playerAttackTransformaaaa_[i].rotation_ = { 0,0,0 };
		playerAttackTransformaaaa_[i].scale_ = { 1,1,1 };
		playerAttackTransformaaaa_[i].TransferMatrix();
	}

	playerEvasionTimes == playerEvasionMaxTimes;

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
	MaxMoveTime = 40;

	isPlayerUlt = false;

	spaceInput = false;
	isEnemyDamage = false;

	x = 0;
	radius = 5.0f;//当たり判定半径
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

	receptionTime = 0;
	conboFlag = false;

	isSpace = false;

	isAdmission = true;

	isAlive = true;

	playerEvasionTimes = 3;
	playerEvasionCoolTime = 0;
	playerEvasionMaxTimes = 3;
	CoolTime = 80;
	spriteAlpha1 = 1.0f;
	spriteAlpha2 = 1.0f;
	spriteAlpha3 = 1.0f;

	flame = 0;

	PlayerRot = Vector3(0, MyMath::GetAngle(-90.0f), 0);

	isInput = false;

	IsCombo = false;
	IsCombo2 = false;
	IsCombo3 = false;
	IsCombo4 = false;
	IsCombo5 = false;

}

void Player::AddUltCount(int count)
{
	if (UltGage < UltMaxGage) {
		UltGage += count;
	}
}

void Player::UltStart()
{
	if (isPlayMotion == false) {
		if (input_->TriggerKey(DIK_Q)) {
			if (isPlayerUlt == false) {
				if (UltGage >= UltMaxGage) {
					isAwakening = true;
					playerNowMotion = PlayerMotion::AwakeningMotion;
					frem = 0.0f;
					MinimumFrem = 2.6f;
					MaxFrem = 2.6f;
					UltKenGenerationTime = 0;
					isUltKenGeneration = false;
				}
			}
		}
	}

}

void Player::UltUpdate()
{

	if (isPlayerUlt == true) {
		if (UltGage > 0) {
			UltGage--;
		}
		else {
			playerNowMotion = PlayerMotion::AwakeningMotion;
			FinULT = true;
			isAwakening = true;
			frem = 2.59f;
			MinimumFrem = 2.6f;
			MaxFrem = 2.6f;

			AttackRotX = 0.0f;
			AttackRotY = 0.0f;
			AttackRotZ = 0.0f;

			AllRot.z = 0.0f;

			Matrix4 rooteee;
			rooteee *= MyMath::Rotation(AllRot, 1);
			rooteee *= MyMath::Rotation(AllRot, 3);
			rooteee *= MyMath::Rotation(AllRot, 2);

			ModelTrans.SetMatRot(rooteee);
			ModelTrans.TransferMatrix();

		}
	}

}

void Player::EnemyNotAttackCollision(bool IsPlayerEnemycontact, Vector3 Pos)
{
	isPlayerEnemycontact = IsPlayerEnemycontact;
	PlayerContactPos = Pos;
	if (isPlayerEnemycontact == true) {
		worldTransform_.translation_ += PlayerContactPos;
	}
	worldTransform_.TransferMatrix();
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
