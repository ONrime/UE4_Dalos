// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "Dalos/Weapone/Public/WeaponeBase.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/PlayerUpperStateBase.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/PlayerDownStateBase.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/UnArmed_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Standing_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Crouch_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/Armed_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/ADS_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Splint_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Sliding_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerArm_AnimInstance.h"
#include "Dalos/Character/Public/Player/PlayerBody_AnimInstance.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameState.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerState.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameMode.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerController.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerStart.h"
#include "Dalos/Game/Public/GameInfo_Instance.h"
#include "Dalos/Widget/Public/MultiPlayer_HUD.h"
#include "Components/PostProcessComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/BlendableInterface.h"
#include "Materials/MaterialInstance.h"
#include "Net/UnrealNetwork.h"

AMultiPlayerBase::AMultiPlayerBase()
{
	SetReplicates(true);
	SetReplicateMovement(true);

	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;

	RootComponent = GetCapsuleComponent();
	GetCapsuleComponent()->InitCapsuleSize(50.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");

	GetMesh()->SetupAttachment(RootComponent);
	GetMesh()->SetIsReplicated(true);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>FULLBODY_SKELETALMESH(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (FULLBODY_SKELETALMESH.Succeeded()) { GetMesh()->SetSkeletalMesh(FULLBODY_SKELETALMESH.Object); }
	static ConstructorHelpers::FClassFinder<UAnimInstance>FULLBODY_ANIMBP(TEXT("AnimBlueprint'/Game/Player/Anim/Body/PlayerBodyAnimBP.PlayerBodyAnimBP_C'"));
	if (FULLBODY_ANIMBP.Succeeded()) { GetMesh()->SetAnimInstanceClass(FULLBODY_ANIMBP.Class); }
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(25.0f, 0.0f, 65.0f));
	SpringArm->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	SpringArm->bDoCollisionTest = true;
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	FollowCamera->bUsePawnControlRotation = false;

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->SetupAttachment(FollowCamera);
	ArmMesh->SetCollisionProfileName("CharacterMesh");
	ArmMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -155.712738));
	ArmMesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	ArmMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	ArmMesh->SetHiddenInGame(false);
	ArmMesh->SetCastShadow(false);
	ArmMesh->bCastHiddenShadow = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>ARM_SKELETALMESH(TEXT("SkeletalMesh'/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'"));
	if (ARM_SKELETALMESH.Succeeded()) { ArmMesh->SetSkeletalMesh(ARM_SKELETALMESH.Object); }
	static ConstructorHelpers::FClassFinder<UAnimInstance>ARM_ANIMBP(TEXT("AnimBlueprint'/Game/Player/Anim/Arm/PlayerArmAnimBP.PlayerArmAnimBP_C'"));
	if (ARM_ANIMBP.Succeeded()) { ArmMesh->SetAnimInstanceClass(ARM_ANIMBP.Class); }

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BODY"));
	BodyMesh->SetGenerateOverlapEvents(true);
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
	BodyMesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	BodyMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	BodyMesh->SetCollisionProfileName("EBodyMesh");
	//BodyMesh->OnComponentBeginOverlap.AddDynamic(this, &AMultiPlayerBase::OnOverlapBegin_Mesh);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>LEGBODY_SKELETALMESH(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (LEGBODY_SKELETALMESH.Succeeded()) { BodyMesh->SetSkeletalMesh(LEGBODY_SKELETALMESH.Object); }
	static ConstructorHelpers::FClassFinder<UAnimInstance>LEGBODY_ANIMBP(TEXT("AnimBlueprint'/Game/Player/Anim/Body/PlayerBodyAnimBP.PlayerBodyAnimBP_C'"));
	if (LEGBODY_ANIMBP.Succeeded()) { BodyMesh->SetAnimInstanceClass(LEGBODY_ANIMBP.Class); }

	GetCharacterMovement()->bOrientRotationToMovement = false; // 이동방향에 맞춰서 회전을 보간해준다.(회전은 내가 바꾸기 때문에 false)	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f); // 회전 속도 0.0으로 초기화 (왜 했지?)
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;  // 컨트롤러 방향으로 캐릭터 회전(무브먼트 기준)
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	
	static ConstructorHelpers::FObjectFinder<UObject>GRAY_MATERIAL(TEXT("Material'/Game/Player/Grey_Camera.Grey_Camera'"));
	if(GRAY_MATERIAL.Succeeded()) FollowCamera->AddOrUpdateBlendable(GRAY_MATERIAL.Object, 1.0f);
	FollowCamera->PostProcessBlendWeight = 0.0f;
	//FollowCamera->PostProcessSettings.WeightedBlendables.Array[0].Weight = 0.0f;

	
}

float AMultiPlayerBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage"));
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float currentHP = 0.0f;
	if (!IsDead) {
		ATwoVersus_PlayerState* playerstate = Cast<ATwoVersus_PlayerState>(GetPlayerState());
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) {
			const FPointDamageEvent* point = (FPointDamageEvent*)&DamageEvent;
			damage = point->Damage;
			currentHP = playerstate->GetPlyaerHP()- damage;
			playerstate->DamageToHP(damage);
			
			UE_LOG(LogTemp, Warning, TEXT("PlayerHP: %f"), currentHP);

			AMultiPlayer_HUD* hud = Cast<AMultiPlayer_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			if (GetWorld()->GetFirstPlayerController()->GetPawn() && hud) {
				ATwoVersus_PlayerController* Ctrl = Cast<ATwoVersus_PlayerController>(GetWorld()->GetFirstPlayerController());
				if (Ctrl) Ctrl->UpdateMatchHPCheck.Execute();
			}
			// 히트 표시를 할때 로컬 컨트롤을 쓰면 될 것 같다
			if (!IsLocallyControlled() && DamageCauser == GetWorld()->GetFirstPlayerController()->GetPawn() && hud) {
				hud->HitRedCheck.Execute(false);
			}
			// 상대방의 데미지가 깍일때 히트 표시를 하자
			if (IsLocallyControlled() && hud) {
				hud->PlyaerHitLocCheck.Execute(DamageCauser->GetActorLocation() + DamageCauser->GetActorForwardVector() * 50.0f);
				/*ATwoVersus_PlayerController* Ctrl = Cast<ATwoVersus_PlayerController>(GetController());
				Ctrl->UpdateMatchHPCheck.Execute();*/
			}
			if (HasAuthority()) {
				//playerstate->StopHeal();
				//GetWorldTimerManager().ClearTimer(healTimer);
				//GetWorldTimerManager().SetTimer(healTimer, this, &AMultiPlayerBase::StratAutoHeal, 4.0f, false);
				NetMulticast_SendPlayerHit(damage, DamageCauser->GetActorForwardVector(), point->HitInfo, DamageCauser);
			}
			CheckPlayerHP(currentHP);
		}
	}	return damage;
}

void AMultiPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	if(IsLocallyControlled()) Server_SetPlayerStartLoc();

	IsPlayerCameraTurn = false;
	IsMove = false;

	HUD = Cast<AMultiPlayer_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	GetMesh()->HideBoneByName(FName("spine_02"), PBO_None);
	GetMesh()->SetOnlyOwnerSee(true);
	GetMesh()->SetCastShadow(false);
	ArmMesh->SetOnlyOwnerSee(true);
	//IsPlayerCameraTurn = true;

	if(!IsLocallyControlled()) BodyMesh->SetCollisionProfileName("EBodyMesh");
	BodyMesh->SetOwnerNoSee(true);
	BodyMesh->SetCastShadow(true);
	BodyMesh->bCastHiddenShadow = true;

	upperState = NewObject<UUnArmed_PlayerUpper>(this, UUnArmed_PlayerUpper::StaticClass());
	downState = NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass());
	upperStateNClass = upperState->GetState();
	downStateNClass = downState->GetState();
	upperState->StateStart(this);
	downState->StateStart(this);

	characterHp = 100.0f;
	
	UE_LOG(LogTemp, Warning, TEXT("BeginePlay"));

}

void AMultiPlayerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	bodyAnim = Cast<UPlayerBody_AnimInstance>(BodyMesh->GetAnimInstance());
	armAnim = Cast<UPlayerArm_AnimInstance>(ArmMesh->GetAnimInstance());
	legAnim = Cast<UPlayerBody_AnimInstance>(GetMesh()->GetAnimInstance());

	legAnim->vaultDelegate.BindLambda([this]()->void {
		StopVault();
		});
	bodyAnim->vaultDelegate.BindLambda([this]()->void {
		StopVault();
		});
	legAnim->climbDelegate.BindLambda([this]()->void {
		StopClimb();
		});
	bodyAnim->climbDelegate.BindLambda([this]()->void {
		StopClimb();
		});
	CountDownEndCheck.BindLambda([this]()->void {
		Server_SendPlayerStart();
		//IsPlayerCameraTurn = true;
		//IsMove = true;
		});

	/*armAnim->playFire.BindLambda([this]()->void {
		
		});*/
}

void AMultiPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	downState->StateUpdate(this);
	upperState->StateUpdate(this);
	
	PlayerMove();
	if (IsLocallyControlled()) {
		CrossHairCheck();
		if (HUD && equipWeapone && !IsFire) {
			HUD->SetCrossHairSpread(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 330.0f), FVector2D(0.0f, equipWeapone->GetWalkSpreadSize()), GetVelocity().Size()), false, IsFire);
		}
	}
	RecoilCheck();

	ATwoVersus_PlayerState* playerstate = Cast<ATwoVersus_PlayerState>(GetPlayerState());
	if (IsLocallyControlled()&& playerstate && playerstate->GetPlyaerHP() > 40.0f) {
		FollowCamera->PostProcessBlendWeight = 0.0f;
	}

	/*if (WallForwardTracer() && WallHeightTracer(wallLoc, wallNomal) && !IsVault) {
		WallBackHeightTracer(wallLoc);
	}*/

	//if (WallForwardTracer() && WallHeightTracer(wallLoc, wallNomal)) WallBackHeightTracer(wallLoc);

	if (downStateNClass == USliding_PlayerDown::StaticClass() && GetVelocity().Size() < 80.0f) {
		DownPress(NewObject<UCrouch_PlayerDown>(this, UCrouch_PlayerDown::StaticClass()));
	}

	if (IsJumped && GetMovementComponent()->IsFalling() == false) IsJumped = false;
	if (GetVelocity().Size() < 0.3f && downStateNClass == USplint_PlayerDown::StaticClass()) {
		DownPress(NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass()));
	}

	if (IsFire && IsFireAuto) {
		FireBullet(equipWeaponeArm->BodyMesh->GetSocketLocation("Muzzle"), FollowCamera->GetComponentRotation(), bulletFireLoc);
		if (HUD && equipWeapone) {
			float spread = equipWeapone->GetFireStartSpreadSize();
			if (HUD->GetTargetSpread() <= spread) {
				HUD->SetCrossHairSpread(HUD->GetTargetSpread() + 80.0f, false, IsFire);
			}
			else {
				HUD->SetCrossHairSpread(spread + 30.0f, false, IsFire);
			}
			if (spreadSize > 10.0f) {
				spreadSize = spreadSize - 30.0f;
			}
		}
	}
	if (recoilReturn && IsFire == false) {
		float RecoilNowRotatorPitch = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 360.0f), FVector2D(-90.0f, 90.0f), (Controller->GetControlRotation()).Pitch);
		float RecoilRotatorPitch = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 360.0f), FVector2D(-90.0f, 90.0f), recoilRot.Pitch);

		if (RecoilNowRotatorPitch >= 85.0f && RecoilRotatorPitch <= -85.0f) {
			recoilReturn = false;
		}
		if (RecoilRotatorPitch >= 80.0f && RecoilNowRotatorPitch <= -80.0f) {
			if (RecoilRotatorPitch <= RecoilNowRotatorPitch) {
				recoilReturn = false;
				recoilRot = FRotator::ZeroRotator;
			}
		}
		else {
			if (RecoilRotatorPitch >= RecoilNowRotatorPitch) {
				recoilReturn = false;
				recoilRot = FRotator::ZeroRotator;
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("RecodddilNowRotatorPitch: %f"), RecoilNowRotatorPitch);
		//UE_LOG(LogTemp, Warning, TEXT("RecoilRotatorPitch: %f"), RecoilRotatorPitch);
		AddControllerPitchInput(0.05f);
	}
	
	if (HasAuthority()) { // 플레이어 전체 회전 (서버)
		if (IsLocallyControlled()) {
			InteractionCheck();
		}
		else {
		}
		controllerRot = GetControlRotation();
		SendControllerRot(controllerRot);
		//UE_LOG(LogTemp, Warning, TEXT("server_Updates: %s controllerRot %f"), *GetName(), controllerRot.Pitch);
	}
	else { // (클라이언트)
		if (IsLocallyControlled()) {
			controllerRot = GetControlRotation();
			InteractionCheck();
		}
		else {
		}
		//UE_LOG(LogTemp, Warning, TEXT("cla_Updates: %s controllerRot %f"), *GetName(), controllerRot.Pitch);
	}

	if (upperStateNClass == UArmed_PlayerUpper::StaticClass()) {
		//UE_LOG(LogTemp, Warning, TEXT("UArmed_PlayerUpper"));
	}
}

void AMultiPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AMultiPlayerBase::PlayerJump);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AMultiPlayerBase::PlayerCrouch);
	PlayerInputComponent->BindAction("Splint", EInputEvent::IE_Pressed, this, &AMultiPlayerBase::PlayerSplint);
	PlayerInputComponent->BindAction("FirstGun", EInputEvent::IE_Pressed, this, &AMultiPlayerBase::PlayerFirstGun);
	PlayerInputComponent->BindAction("SecondGun", EInputEvent::IE_Pressed, this, &AMultiPlayerBase::PlayerSecondGun);
	PlayerInputComponent->BindAction("UnArmed", EInputEvent::IE_Pressed, this, &AMultiPlayerBase::PlayerUnArmed);
	PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &AMultiPlayerBase::PlayerInteraction);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &AMultiPlayerBase::PlayerReload);
	PlayerInputComponent->BindAction("ADS", EInputEvent::IE_Pressed, this, &AMultiPlayerBase::PlayerADS);
	PlayerInputComponent->BindAction("ADS", EInputEvent::IE_Released, this, &AMultiPlayerBase::PlayerUnADS);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AMultiPlayerBase::PlayerFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AMultiPlayerBase::PlayerUnFire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMultiPlayerBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMultiPlayerBase::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMultiPlayerBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AMultiPlayerBase::LookUpAtRate);
}

void AMultiPlayerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMultiPlayerBase::OnOverlapBegin_Mesh(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("Hit"));
		ATwoVersus_GameState* gameState = Cast<ATwoVersus_GameState>(UGameplayStatics::GetGameState(GetWorld()));

		for (int i = 0; i < gameState->PlayerArray.Num(); i++) {
			if (gameState->PlayerArray[i] == GetPlayerState()) {

			}
		}
	}
}

bool AMultiPlayerBase::InteractionCheck()
{
	FHitResult outHit;
	FVector startTracer = FollowCamera->GetComponentLocation();
	FVector endTracer = startTracer + FollowCamera->GetForwardVector() * 160.0f;
	TArray<AActor*> actorsToIgnore;
	bool hitis = UKismetSystemLibrary::SphereTraceSingle(this, startTracer, endTracer, 20.0f, ETraceTypeQuery::TraceTypeQuery3, false, actorsToIgnore
		, EDrawDebugTrace::None, outHit, true);

	if (hitis) {
		if (outHit.GetActor()->GetClass()->GetSuperClass() == AWeaponeBase::StaticClass()) {
			lookWeapone = Cast<AWeaponeBase>(outHit.Actor);
			if (lookWeapone->GetWeaponeState() == WEAPONSTATE::DROP && lookWeapone->GetIsEmpty() == false) {
				lookWeapone->Execute_InteractionStart(lookWeapone);
			}
		}
		else { return hitis; }
	}
	else {
		if (lookWeapone != nullptr) {
			lookWeapone->Execute_InteractionEnd(lookWeapone);
			lookWeapone = nullptr;
		}
	}

	return hitis;
}
bool AMultiPlayerBase::CrossHairCheck()
{
	FHitResult outHit;
	TArray<AActor*> actorsToIgnore;
	FVector startTracer = FollowCamera->GetComponentLocation();
	FVector endTracer = startTracer + FollowCamera->GetForwardVector() * 2500.0f;
	FVector muzzleLoc = FVector::ZeroVector;
	FVector bulletEndLoc = FVector::ZeroVector;
	float distance = 0.0f;
	FOutputDeviceNull aimRed;

	bool hitis = UKismetSystemLibrary::LineTraceSingle(GetWorld(), startTracer, endTracer, ETraceTypeQuery::TraceTypeQuery4, false
		, actorsToIgnore, EDrawDebugTrace::None, outHit, true);

	if (equipWeapone != nullptr) {
		muzzleLoc = equipWeapone->BodyMesh->GetSocketLocation("Muzzle");
	}
	else { muzzleLoc = FollowCamera->GetComponentLocation(); }

	bool redCheck = false;
	if (hitis && outHit.GetActor() != this) {
		crossHairEndLoc = outHit.Location;
		distance = outHit.Distance;
		if (outHit.GetComponent()->GetCollisionProfileName() == "BodyMesh") {
			redCheck = true;
		}
	}
	else {
		crossHairEndLoc = endTracer;
		distance = 2000.0f;
	}
	float radius = (FMath::Tan(FollowCamera->FieldOfView / 2.0f) * distance) / spreadSize; // 40을 바꾸기
	float bulletEndLocationZ = FMath::RandRange((crossHairEndLoc + (FollowCamera->GetUpVector() * radius)).Z,
		(crossHairEndLoc + (-(FollowCamera->GetUpVector()) * radius)).Z);
	//float bulletEndLocationTestZ = (crossHairEndLoc + ((FollowCamera->GetUpVector()) * radius)).Z;
	float bulletEndLocationY = FMath::RandRange((crossHairEndLoc + (FollowCamera->GetRightVector() * radius)).Y,
		(crossHairEndLoc + (-(FollowCamera->GetRightVector()) * radius)).Y);
	//float bulletEndLocationTestY = (crossHairEndLoc + (FollowCamera->GetRightVector() * radius)).Y;
	float bulletEndLocationX = crossHairEndLoc.X;

	bulletFireLoc = FVector(bulletEndLocationX, bulletEndLocationY, bulletEndLocationZ);
	/*bulletEndLoc = FVector(bulletEndLocationX, bulletEndLocationY, bulletEndLocationZ);
	if (upperState->GetState() == UADS_PlayerUpper::StaticClass()) {
		bulletRot = FollowCamera->GetForwardVector().Rotation();
	}
	else { bulletRot = UKismetMathLibrary::FindLookAtRotation(muzzleLoc, bulletEndLoc); }*/

	/*투영행렬을 이용하여 만들 수 있을것같다
	기존에 있는 것 보다 정확하게 수치에 따라 변하도록 업그레이드를 했다
	기존의 것은 거리에 따라 커지게는 만들었지만 일정하게 조절하지 못하게 만들었다.
	이번에는 fov 수치를 이용해서 좀더 정확한 수치가 나오도록 업그레이드를 했다.*/

	if (HUD && HUD->CrossHairRedCheck.IsBound()) HUD->CrossHairRedCheck.Execute(redCheck);

	return hitis;
}

bool AMultiPlayerBase::RecoilCheck()
{
	FHitResult outHit;
	TArray<AActor*> actorsToIgnore;
	FVector startTracer = FollowCamera->GetComponentLocation();
	FVector endTracer = startTracer + FollowCamera->GetForwardVector() * 2500.0f;
	FVector cameraEnd = startTracer + FollowCamera->GetForwardVector() * 2500.0f;
	FVector finalTracer = FVector::ZeroVector;
	if (recoilReturnLoc != FVector::ZeroVector && IsFire) {
		recoilReturn = true;
		//UE_LOG(LogTemp, Warning, TEXT("recoilReturn: %f"), recoilReturnDir.Z);
		/*if (((FollowCamera->GetForwardVector()).Z <= RecoilReturnDir.Z)) {
			RecoilReturnDir.Z = (FollowCamera->GetForwardVector()).Z;
			RecoilPitch = 0.0f;
			RecoilReturn = false;
		}*/
		FVector dirdir = FVector(FollowCamera->GetForwardVector().X, FollowCamera->GetForwardVector().Y, recoilReturnDir.Z);
		endTracer = startTracer + dirdir * 2500.0f;//RecoilReturnLocation;

	}
	bool hitis = UKismetSystemLibrary::LineTraceSingle(GetWorld(), startTracer, endTracer, ETraceTypeQuery::TraceTypeQuery4, false
		, actorsToIgnore, EDrawDebugTrace::None, outHit, true);

	return hitis;
}
void AMultiPlayerBase::WeaponCheck(AWeaponeBase* check)
{
	if (check != nullptr) {
		if (backWeapone1 != nullptr && backWeapone2 != nullptr) { // 무기가 두개 있을 때
			if (equipWeapone != nullptr) {
				FDetachmentTransformRules DetachmentRule = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
				equipWeapone->SetWeaponeState(WEAPONSTATE::DROP);
				equipWeapone->DetachFromActor(DetachmentRule);
				equipWeapone->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
				if (equipWeapone == backWeapone1) {
					backWeapone1 = check;
				}
				else backWeapone2 = check;
			}
		}
		else {
			if (equipWeapone != nullptr) { // 무기가 하나 있을 때
				if (equipWeapone->GetStaticClass() != check->GetStaticClass()) {
					
				}
				else { // 주우려는 무기가 같을 때는 탄약 수 더하기 
				}
				if (equipWeapone == backWeapone1) {
					equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("BackWeapone1"));
					backWeapone2 = check;
				}
				else {
					equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("BackWeapone2"));
					backWeapone1 = check;
				}
				equipWeapone->SetWeaponeState(WEAPONSTATE::BACKEQUIP);
			}
			else { // 무기가 없을 때
				backWeapone1 = check;
			}
		}
		ATwoVersus_PlayerState* state = Cast<ATwoVersus_PlayerState>(GetPlayerState());
		if (equipWeapone && equipWeapone->GetWeaponType() == WEAPONTYPE::RIFLE) {
			if (equipWeapone->GetLimitAmmo() - equipWeapone->GetBaseLoadedAmmo() >= state->rifleAmmo) {
				if (state->equipAmmo != 0) state->rifleAmmo += equipWeapone->GetBaseLoadedAmmo();
			}
			state->equipAmmo = state->rifleAmmo;
		}
		if (HUD) HUD->EquipAmmoCheck.Execute(state->equipAmmo);
		EquipGunOnHand(check);
	}
}

void AMultiPlayerBase::EquipGunOnHand(AWeaponeBase* equip)
{
	equipWeapone = equip;
	equipWeapone->SetWeaponeState(WEAPONSTATE::EQUIP);
	equipWeapone->SetOwner(this); // 여기 문제 영상 참고
	equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponeLoc"));
	equipWeapone->BodyMesh->SetOwnerNoSee(true);
	equipWeaponeStaticClass = equipWeapone->GetStaticClass();

	if (IsLocallyControlled()) {
		if (equipWeaponeArm) {
			equipWeaponeArm->Destroy();
			equipWeaponeArm = nullptr;
		}
		equipWeaponeArm = equipWeapone->SpawnToHand(this, ArmMesh->GetSocketLocation(TEXT("WeaponeLoc")), ArmMesh->GetSocketRotation(TEXT("WeaponeLoc")));
		equipWeaponeArm->SetWeaponeState(WEAPONSTATE::EQUIP);
		equipWeaponeArm->BodyMesh->SetOnlyOwnerSee(true);
		equipWeaponeArm->BodyMesh->SetOwnerNoSee(false);
		equipWeaponeArm->SetOwner(this); // 여기 문제 영상 참고
		equipWeaponeArm->AttachToComponent(ArmMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponeLoc"));

		ATwoVersus_PlayerState* state = Cast<ATwoVersus_PlayerState>(GetPlayerState());
		if (equipWeapone->GetWeaponType() == WEAPONTYPE::RIFLE) {
			if (state->rifleAmmo == 0) state->rifleAmmo = equipWeapone->GetBaseKeepAmmo();
			state->equipAmmo = state->rifleAmmo;
			state->weaponName = equipWeapone->GetWeaponName();
		}
		state->loadedAmmo = equipWeapone->LoadedAmmo;
		if (HUD) {
			HUD->LoadedAmmoCheck.Execute(equipWeapone->LoadedAmmo);
			HUD->EquipAmmoCheck.Execute(state->equipAmmo);
			HUD->WeaponNameCheck.Execute(equipWeapone->GetWeaponName());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("EquipGunOnHand"));
	//EquipWeaponInAmmo = equip->InAmmo;
	//EquipWeaponType = equip->GetWeaponType();
}

void AMultiPlayerBase::PlayerCrouch()
{
	DownPress(nullptr);
	
	if (!HasAuthority()) {
		Server_SendDownPress(EPlayerPress::CROUCH);
	}
	else { NetMulticast_SendDownPress(EPlayerPress::CROUCH); }
}

void AMultiPlayerBase::PlayerJump()
{
	IsJumped = true;
	Server_SendIsJumped(IsJumped);
	auto jump = downState->PlayerJump(this);
	if (jump != nullptr) DownPress(jump);

	if (WallForwardTracer() && WallHeightTracer(wallLoc, wallNomal) && !IsVault) {
		WallBackHeightTracer(wallLoc);
		IsVault = true;
		PlayerVault();
		Server_SendValutCheck();
	}
}

void AMultiPlayerBase::PlayerSplint()
{
	DownPress(nullptr);
	if (!HasAuthority()) {
		Server_SendDownPress(EPlayerPress::SPLINT);
	}
	else {  NetMulticast_SendDownPress(EPlayerPress::SPLINT); }
}

void AMultiPlayerBase::PlayerFirstGun()
{
	if (backWeapone1) {
		if (equipWeapone == backWeapone2) {
			equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone2"));
			equipWeapone->SetWeaponeState(WEAPONSTATE::BACKEQUIP);
		}
		EquipGunOnHand(backWeapone1);
	}
	UpperPress(nullptr);
	if (!HasAuthority()) {
		Server_SendUpperPress(EPlayerPress::FIRSTGUN);
		Server_SendWeaponeChange(EPlayerPress::FIRSTGUN);
	}
	else { 
		NetMulticast_SendUpperPress(EPlayerPress::FIRSTGUN);
		NetMulticast_SendWeaponeChange(EPlayerPress::FIRSTGUN);
	}
}
void AMultiPlayerBase::PlayerSecondGun()
{
	if (backWeapone2) {
		if (equipWeapone == backWeapone1) {
			equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone1"));
			equipWeapone->SetWeaponeState(WEAPONSTATE::BACKEQUIP);
		}
		EquipGunOnHand(backWeapone2);
	}
	UpperPress(nullptr);
	if (!HasAuthority()) {
		Server_SendUpperPress(EPlayerPress::SECONDGUN);
		Server_SendWeaponeChange(EPlayerPress::SECONDGUN);
	}
	else { 
		NetMulticast_SendUpperPress(EPlayerPress::SECONDGUN); 
		NetMulticast_SendWeaponeChange(EPlayerPress::SECONDGUN);
	}
}
void AMultiPlayerBase::PlayerUnArmed()
{
	if (equipWeapone) {
		if (equipWeapone == backWeapone1) {
			equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone1"));
		}
		else equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone2"));
		equipWeapone->SetWeaponeState(WEAPONSTATE::BACKEQUIP);
	}
	UpperPress(nullptr);
	if (!HasAuthority()) {
		Server_SendUpperPress(EPlayerPress::UNARMED);
		Server_SendWeaponeChange(EPlayerPress::UNARMED);
	}
	else { 
		NetMulticast_SendUpperPress(EPlayerPress::UNARMED); 
		NetMulticast_SendWeaponeChange(EPlayerPress::UNARMED);
	}
}

void AMultiPlayerBase::PlayerInteraction()
{
	if (lookWeapone) {
		WeaponCheck(lookWeapone);
		UpperPress(nullptr);
		if (!HasAuthority()) {
			Server_SendWeaponeCheck(lookWeapone);
			Server_SendUpperPress(EPlayerPress::INTERACTION);
		}
		/*else { 
			NetMulticast_SendWeaponeCheck(lookWeapone);
			NetMulticast_SendUpperPress(EPlayerPress::INTERACTION); 
		}*/
	}
}

void AMultiPlayerBase::PlayerADS()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerADS"));
	armAnim->StopAllMontages(0.1f);
	UpperPress(nullptr);
}
void AMultiPlayerBase::PlayerUnADS()
{
	armAnim->StopAllMontages(0.1f);
	UpperPress(nullptr);
}

void AMultiPlayerBase::PlayerFire()
{
	IsFire = true;
	recoilPitch = 0.0f;
	recoilReturnDir = FVector::ZeroVector;
	recoilRot = FRotator::ZeroRotator;
	spreadSize = 70.0f;
	if (equipWeapone && equipWeaponeArm) {
		upperState->PlayerFire(this, equipWeapone, IsFireAuto, threeCount
			, equipWeaponeArm->BodyMesh->GetSocketLocation("Muzzle"), FollowCamera->GetComponentRotation(), bulletFireLoc);
	}
	//armAnim->PlayFireMontage();
}
void AMultiPlayerBase::PlayerUnFire()
{
	armAnim->StopFireMontage();
	IsFire = false;
	IsFireAuto = false;
	spreadSize = 70.0f;
	if (HUD && equipWeapone) HUD->SetCrossHairSpread(0.0f, false, false);
}

void AMultiPlayerBase::PlayerReload()
{
	if (equipWeapone) {
		armAnim->PlayReloadMontage();
		equipWeapone->PlayReloadMontage();
		equipWeaponeArm->PlayReloadMontage();
	}
	ATwoVersus_PlayerState* state = Cast<ATwoVersus_PlayerState>(GetPlayerState());
	if (state->rifleAmmo <= 0 || equipWeapone->LoadedAmmo >= equipWeapone->GetBaseLoadedAmmo()) { return; }
	if (state->rifleAmmo < (equipWeapone->GetBaseLoadedAmmo() - equipWeapone->LoadedAmmo)) {
		equipWeapone->LoadedAmmo = equipWeapone->LoadedAmmo + state->rifleAmmo;
		state->rifleAmmo = 0;
	}
	else {
		state->rifleAmmo = state->rifleAmmo - (equipWeapone->GetBaseLoadedAmmo() - equipWeapone->LoadedAmmo);
		equipWeapone->LoadedAmmo = equipWeapone->GetBaseLoadedAmmo();
	}
	state->loadedAmmo = equipWeapone->LoadedAmmo;
	state->equipAmmo = state->rifleAmmo;
	if (HUD) {
		HUD->LoadedAmmoCheck.Execute(equipWeapone->LoadedAmmo);
		HUD->EquipAmmoCheck.Execute(state->equipAmmo);
	}
	Server_SendReloadAmmo(state->loadedAmmo, state->equipAmmo);
}

void AMultiPlayerBase::PlayerVault()
{
	if (bodyAnim == nullptr || GetMesh() == nullptr) {
		return;
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;

	// WallNomal로 뽑아낸 벽의 방향을 Z축 으로 180도 회전하여 플레이어가 바라 보는 방향으로 바꿔서 WallNomalXRotator저장한다.
	FRotator wallNomalXRotator = FRotator(wallNomal.Rotation().Pitch, wallNomal.Rotation().Yaw - 180.0f, wallNomal.Rotation().Roll);;
	//UE_LOG(LogTemp, Warning, TEXT("WallNomalXRotator: %f, %f, %f "), WallNomalXRotator.Roll, WallNomalXRotator.Pitch, WallNomalXRotator.Yaw);
	//PlayerRotationYaw.Yaw = wallNomalXRotator.Yaw;
	SetActorRotation(FRotator(0.0f, wallNomalXRotator.Yaw, 0.0f));

	ArmMesh->SetOwnerNoSee(true);
	BodyMesh->SetOwnerNoSee(false);
	IsCameraLock = true;
	if (equipWeaponeArm) equipWeaponeArm->SetHidden(true);
	// 벽의 X,Y 좌표로 부터 플레이어가 정확하게 서야할(넘어가야할)위치를 찾는다.
	//벽의 좌표를 뽑아낼때 정확한 위치가 아닌 벽의 중앙 위치만 뽑아 낼것이다.
	//그래서 아래의 과정을 통해 플레이어 기준의 벽의 위치가 나온다.
	FVector wallLocationXY = wallLoc + (FRotationMatrix::MakeFromX(wallNomal).GetUnitAxis(EAxis::X) * (40.0f));
	// 플레이어가 넘어가야할 벽의 정확한 좌표
	FVector wallLocationTracer = FVector::ZeroVector;
	if (IsWallThick == false) {
		//UE_LOG(LogTemp, Warning, TEXT("ClimbMontage"));
		wallLocationTracer = FVector(wallLocationXY.X, wallLocationXY.Y, wallHeight.Z - 15.0f);
		bodyAnim->PlayClimbMontage();
		legAnim->PlayClimbMontage();
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("VaultMontage"));
		wallLocationTracer = FVector(wallLocationXY.X, wallLocationXY.Y, wallHeight.Z - 25.0f);
		bodyAnim->PlayVaultMontage();
		legAnim->PlayVaultMontage();

	}
	// 플레이어의 위치를 조정한다.
	SetActorLocation(wallLocationTracer);
}

void AMultiPlayerBase::PlayerDead()
{
	IsMove = false;
	GetWorldTimerManager().ClearTimer(healTimer);
	if (equipWeapone) {
		equipWeapone->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		equipWeapone->SetWeaponeState(WEAPONSTATE::DROP);
	}
	if (equipWeaponeArm) {
		equipWeaponeArm->Destroy();
	}
	if (backWeapone1) {
		backWeapone1->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		backWeapone1->SetWeaponeState(WEAPONSTATE::DROP);
	}
	if (backWeapone2) {
		backWeapone2->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		backWeapone2->SetWeaponeState(WEAPONSTATE::DROP);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeadColl"));
	ArmMesh->SetCollisionProfileName(TEXT("DeadNomal"));
	ArmMesh->HideBoneByName(FName("root"), PBO_None);
	GetMesh()->SetCollisionProfileName(TEXT("DeadNomal"));
	GetMesh()->HideBoneByName(FName("root"), PBO_None);
	BodyMesh->SetCollisionProfileName(TEXT("DeadMesh"));
	BodyMesh->SetOnlyOwnerSee(false);
	BodyMesh->SetOwnerNoSee(false);
	//GetMesh()->SetCollisionProfileName(TEXT("PhysicsActor"));
	BodyMesh->SetSimulatePhysics(true);

	ATwoVersus_PlayerController* Ctrl = Cast<ATwoVersus_PlayerController>(GetController());
	if(Ctrl && Ctrl->PlayerDeadCheck.IsBound()) Ctrl->PlayerDeadCheck.Execute();

}

void AMultiPlayerBase::MoveForward(float Value)
{
	if (IsMove) {
		inputDirForward = Value;
		if ((Controller != NULL) && (Value != 0.0f)) {
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			FVector Direction = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::X);
			Direction.Z = 0.0f;
			Direction.Normalize();
			moveDir += Direction * FMath::Clamp(Value, -1.0f, 1.0f);
			//UE_LOG(LogTemp, Warning, TEXT("InputDirForward: %f"), InputDirForward);
		}
	}
}
void AMultiPlayerBase::MoveRight(float Value)
{
	if (IsMove) {
		inputDirRight = Value;
		if ((Controller != NULL) && (Value != 0.0f)) {
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			FVector Direction = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::Y);
			Direction.Z = 0.0f;
			Direction.Normalize();
			moveDir += Direction * FMath::Clamp(Value, -1.0f, 1.0f);
		}
	}
}
void AMultiPlayerBase::PlayerMove()
{
	if (IsMove) {
		if (moveDir.IsZero()) {
			return;
		}
		moveDir.Normalize();
		if (FMath::Abs(inputDirForward) > FMath::Abs(inputDirRight)) {
			inputDir = FMath::Abs(inputDirForward);
		}
		else { inputDir = FMath::Abs(inputDirRight); }
		//UE_LOG(LogTemp, Warning, TEXT("Input: %f"), FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(0.0f, PlayerSpeed), InputDir));
		AddMovementInput(moveDir, FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(0.0f, PlayerSpeed), inputDir) * 0.008f);
		downState->PlayerMove(this, inputDir, inputDirRight);
	}
}

void AMultiPlayerBase::FireAutoOn()
{
	IsFireAuto = true;
}

bool AMultiPlayerBase::WallForwardTracer()
{
	TArray<AActor*> actorsToIgnore;
	FHitResult outHit;
	FVector startTracer = GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 80.0f);
	FVector endTracer = startTracer + GetMesh() ->GetRightVector() * 40.0f;
	bool IsHit = UKismetSystemLibrary::SphereTraceSingle(this, startTracer, endTracer, 20.0f, ETraceTypeQuery::TraceTypeQuery5, false
		, actorsToIgnore, EDrawDebugTrace::ForOneFrame, outHit, true);

	wallLoc = outHit.Location;
	wallNomal = outHit.Normal;
	if(IsHit) WallHeightTracer(outHit.Location, outHit.Normal);
	return IsHit;
}
bool AMultiPlayerBase::WallHeightTracer(FVector loc, FVector nomal)
{
	TArray<AActor*> actorsToIgnore;
	FHitResult outHit;
	FVector endTracer = loc + (FRotationMatrix::MakeFromX(nomal).GetUnitAxis(EAxis::X) * (-10.0f));
	FVector startTracer = endTracer + FVector(0.0f, 0.0f, 100.0f);
	bool IsHit = UKismetSystemLibrary::SphereTraceSingle(this, startTracer, endTracer, 20.0f, ETraceTypeQuery::TraceTypeQuery5, false
		, actorsToIgnore, EDrawDebugTrace::ForOneFrame, outHit, true);

	wallHeight = outHit.Location;
	if (wallHeight.Z >= startTracer.Z - 2.0f) {
		return false;
	}
	//UE_LOG(LogTemp, Warning, TEXT("WallHeight: %f, %f, %f"), WallHeight.X, WallHeight.Y, WallHeight.Z);
	//UE_LOG(LogTemp, Warning, TEXT("WallLocation: %f, %f, %f"), WallLocation.X, WallLocation.Y, WallLocation.Z);

	float minHeight = wallHeight.Z - loc.Z;
	if (minHeight > 60.0f) {
		IsWall = true;
	}
	else { IsWall = false; }
	return IsHit;
}
bool AMultiPlayerBase::WallBackHeightTracer(FVector loc)
{
	TArray<AActor*> actorsToIgnore;
	FHitResult outHit;
	FVector endTracer = loc + GetActorForwardVector() * 90.0f;
	FVector startTracer = endTracer + FVector(0.0f, 0.0f, 100.0f);
	bool IsHit = UKismetSystemLibrary::SphereTraceSingle(this, startTracer, endTracer, 15.0f, ETraceTypeQuery::TraceTypeQuery5, false
		, actorsToIgnore, EDrawDebugTrace::ForOneFrame, outHit, true);

	wallBackHeight = outHit.Location;
	if (!IsHit) {
		IsWallThick = true;
	}
	else { IsWallThick = false; }

	return IsHit;
}

void AMultiPlayerBase::StopVault()
{
	ArmMesh->SetOwnerNoSee(false);
	BodyMesh->SetOwnerNoSee(true);
	bodyAnim->StopMontage();
	legAnim->StopMontage();
	IsCameraLock = false;
	if (equipWeaponeArm) equipWeaponeArm->SetHidden(false);
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	IsVault = false;
	//UE_LOG(LogTemp, Warning, TEXT("Vault"));
}
void AMultiPlayerBase::StopClimb()
{
	ArmMesh->SetOwnerNoSee(false);
	BodyMesh->SetOwnerNoSee(true);
	bodyAnim->StopMontage();
	legAnim->StopMontage();
	IsCameraLock = false;
	if(equipWeaponeArm) equipWeaponeArm->SetHidden(false);
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	IsVault = false;
}

void AMultiPlayerBase::StratAutoHeal()
{
	UE_LOG(LogTemp, Warning, TEXT("StratAutoHeal"));
	FollowCamera->PostProcessBlendWeight = 0.0f;
	/*ATwoVersus_PlayerState* state = Cast<ATwoVersus_PlayerState>(GetPlayerState());
	if (state) {
		state->StartHeal();
	}*/
}

void AMultiPlayerBase::TurnAtRate(float Rate)
{
	aimDirRight = Rate;
	if (downState) {
		downState->TurnAtRate(this, aimDirRight);
	}
	
}
void AMultiPlayerBase::LookUpAtRate(float Rate)
{
	aimDirUp = Rate;
	if (downState) {
		downState->LookUpAtRate(this, Rate);
	}
}

void AMultiPlayerBase::UpperPress(UPlayerUpperStateBase* state)
{
	UPlayerUpperStateBase* temp = nullptr;
	if (!state) {
		temp = upperState->HandleInput(this);
		if (temp == nullptr) { return; }
	}
	else { temp = state; }
	upperState->StateEnd(this);
	upperStateBClass = upperState->GetState();
	if (temp != nullptr) {
		upperState->DestroyComponent();
		upperState = temp;
		upperStateNClass = upperState->GetState();
		upperState->StateStart(this);
	}
}
void AMultiPlayerBase::DownPress(UPlayerDownStateBase* state)
{
	UPlayerDownStateBase* temp = nullptr;
	if (!state) {
		temp = downState->HandleInput(this);
		if (temp == nullptr) { return; }
	}
	else { temp = state; }
	downState->StateEnd(this);
	downStateBClass = downState->GetState();
	if (temp != nullptr) {
		downState->DestroyComponent();
		downState = temp;
		downStateNClass = downState->GetState();
		downState->StateStart(this);
	}
}

void AMultiPlayerBase::FireBullet(FVector muzzleLoc, FRotator muzzleRot, FVector bulletLoc)
{
	if (equipWeapone) {
		armAnim->PlayFireMontage();
		equipWeapone->PlayFireMontage();
		equipWeaponeArm->PlayFireMontage();

		ATwoVersus_PlayerState* state = Cast<ATwoVersus_PlayerState>(GetPlayerState());
		if (equipWeapone->LoadedAmmo > 1) {
			equipWeapone->LoadedAmmo -= 1;
			state->loadedAmmo -= 1;
		}else { ; } // empty 동작
		if (HUD) HUD->LoadedAmmoCheck.Execute(equipWeapone->LoadedAmmo);
		/*if (EquipWeaponInAmmo > 1.0f) {
			EquipWeaponInAmmo -= 1.0f;
		}
		else { equipWeapone->PlayEmptyMontage(); }*/

		IsFireAuto = false;
		recoilReturnLoc = FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 2500.0f;
		if (recoilRot == FRotator::ZeroRotator) {
			recoilReturnDir = FollowCamera->GetForwardVector();
			recoilRot = GetControlRotation();
		}

		float AddPitch = equipWeapone->GetFireRecoilPitch();
		float AddYaw = equipWeapone->GetFireRecoilYaw();
		recoilPitch += FMath::Abs(AddPitch);
		AddControllerPitchInput(AddPitch);
		AddControllerYawInput(AddYaw);

		FRotator bulletFireRot = FRotator::ZeroRotator;
		if (upperState->GetState() == UADS_PlayerUpper::StaticClass()) {
			bulletFireRot = FollowCamera->GetForwardVector().Rotation();
		}
		else { bulletFireRot = UKismetMathLibrary::FindLookAtRotation(muzzleLoc, bulletLoc); }

		equipWeaponeArm->ProjectileFire(muzzleLoc, muzzleRot, bulletFireRot);
		if (equipWeapone->GetWeaponeLever() == WEAPONLEVER::FULLAUTO) {
			GetWorldTimerManager().SetTimer(fireTimer, this, &AMultiPlayerBase::FireAutoOn, 0.15f, false);
		}
		if (HasAuthority()) {
			NetMulticast_SendFireBullet(bulletLoc);
		}
		else { Server_SendFireBullet(bulletLoc); }
		
	}
}

void AMultiPlayerBase::CheckPlayerHP(float hp)
{
	if (hp <= 40.0f && hp > 0 && FollowCamera->PostProcessBlendWeight != 1.0f) {
		UE_LOG(LogTemp, Warning, TEXT("Gray"));
		FollowCamera->PostProcessBlendWeight = 1.0f;
		GetWorldTimerManager().ClearTimer(healTimer);
		GetWorldTimerManager().SetTimer(healTimer, this, &AMultiPlayerBase::StratAutoHeal, 4.0f, false);
	}
	else if (hp <= 0.0f) {
		UE_LOG(LogTemp, Warning, TEXT("Dead"));
		FollowCamera->PostProcessBlendWeight = 1.0f;
		IsDead = true;
		PlayerDead();
	}
}

bool AMultiPlayerBase::SendControllerRot_Validate(FRotator rot)
{
	return true;
}
void AMultiPlayerBase::SendControllerRot_Implementation(FRotator rot)
{
	controllerRot = rot;
}

bool AMultiPlayerBase::Server_SendDownPress_Validate(EPlayerPress press)
{
	return true;
}
void AMultiPlayerBase::Server_SendDownPress_Implementation(EPlayerPress press)
{
	NetMulticast_SendDownPress(press);
	//UE_LOG(LogTemp, Warning, TEXT("Server_SendDownState"));
}
bool AMultiPlayerBase::NetMulticast_SendDownPress_Validate(EPlayerPress press)
{
	return true;
}
void AMultiPlayerBase::NetMulticast_SendDownPress_Implementation(EPlayerPress press)
{
	if (!IsLocallyControlled()) {
		UPlayerDownStateBase* temp = downState->SendHandleInput(press);
		if (temp != nullptr) {
			downState->StateEnd(this);
			downStateBClass = downState->GetState();
			downState->DestroyComponent();
			downState = temp;
			downStateNClass = downState->GetState();
			downState->StateStart(this);
		}
	}
}

bool AMultiPlayerBase::Server_SendUpperPress_Validate(EPlayerPress press)
{
	return true;
}
void AMultiPlayerBase::Server_SendUpperPress_Implementation(EPlayerPress press)
{
	NetMulticast_SendUpperPress(press);
}
bool AMultiPlayerBase::NetMulticast_SendUpperPress_Validate(EPlayerPress press)
{
	return true;
}
void AMultiPlayerBase::NetMulticast_SendUpperPress_Implementation(EPlayerPress press)
{
	if (!IsLocallyControlled()) {
		UPlayerUpperStateBase* temp = upperState->SendHandleInput(press);
		if (temp != nullptr) {
			upperState->StateEnd(this);
			upperStateBClass = upperState->GetState();
			upperState->DestroyComponent();
			upperState = temp;
			upperStateNClass = upperState->GetState();
			upperState->StateStart(this);
		}
	}
}

bool AMultiPlayerBase::Server_SendWeaponeCheck_Validate(AWeaponeBase* check)
{
	return true;
}
void AMultiPlayerBase::Server_SendWeaponeCheck_Implementation(AWeaponeBase* check)
{
	if (check) {
		WeaponCheck(check);
		NetMulticast_SendWeaponeCheck(check);
	}
}
bool AMultiPlayerBase::NetMulticast_SendWeaponeCheck_Validate(AWeaponeBase* check)
{
	return true;
}
void AMultiPlayerBase::NetMulticast_SendWeaponeCheck_Implementation(AWeaponeBase* check)
{
	if (!HasAuthority() && !IsLocallyControlled()) WeaponCheck(check);
}
bool AMultiPlayerBase::Server_SendWeaponeChange_Validate(EPlayerPress press)
{
	return true;
}
void AMultiPlayerBase::Server_SendWeaponeChange_Implementation(EPlayerPress press)
{
	switch (press) {
	case EPlayerPress::FIRSTGUN :
		if (backWeapone1) {
			if (equipWeapone == backWeapone2) {
				equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone2"));
				equipWeapone->SetWeaponeState(WEAPONSTATE::BACKEQUIP);
			}
			EquipGunOnHand(backWeapone1);
		}
		break;
	case EPlayerPress::SECONDGUN:
		if (backWeapone2) {
			if (equipWeapone == backWeapone1) {
				equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone1"));
				equipWeapone->SetWeaponeState(WEAPONSTATE::BACKEQUIP);
			}
			EquipGunOnHand(backWeapone2);
		}
		break;
	case EPlayerPress::UNARMED:
		if (equipWeapone) {
			if (equipWeapone == backWeapone1) {
				equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone1"));
			}
			else equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone2"));
			equipWeapone->SetWeaponeState(WEAPONSTATE::BACKEQUIP);
		}
		break;
	}
	NetMulticast_SendWeaponeChange(press);
}
bool AMultiPlayerBase::NetMulticast_SendWeaponeChange_Validate(EPlayerPress press)
{
	return true;
}
void AMultiPlayerBase::NetMulticast_SendWeaponeChange_Implementation(EPlayerPress press)
{
	if (!HasAuthority()) {
		switch (press) {
		case EPlayerPress::FIRSTGUN:
			if (backWeapone1) {
				if (equipWeapone == backWeapone2) {
					equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone2"));
					equipWeapone->SetWeaponeState(WEAPONSTATE::BACKEQUIP);
				}
				EquipGunOnHand(backWeapone1);
			}
			break;
		case EPlayerPress::SECONDGUN:
			if (backWeapone2) {
				if (equipWeapone == backWeapone1) {
					equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone1"));
					equipWeapone->SetWeaponeState(WEAPONSTATE::BACKEQUIP);
				}
				EquipGunOnHand(backWeapone2);
			}
			break;
		case EPlayerPress::UNARMED:
			if (equipWeapone) {
				if (equipWeapone == backWeapone1) {
					equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone1"));
				}
				else equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BackWeapone2"));
				equipWeapone->SetWeaponeState(WEAPONSTATE::BACKEQUIP);
			}
			break;
		}
	}
}

bool AMultiPlayerBase::Server_SendIsJumped_Validate(bool jumped)
{
	return  true;
}
void AMultiPlayerBase::Server_SendIsJumped_Implementation(bool jumped)
{
	IsJumped = jumped;
}

bool AMultiPlayerBase::Server_SendValutCheck_Validate()
{
	return true;
}
void AMultiPlayerBase::Server_SendValutCheck_Implementation()
{
	if (WallForwardTracer() && WallHeightTracer(wallLoc, wallNomal) && !IsVault) {
		WallBackHeightTracer(wallLoc);
		IsVault = true;
		PlayerVault();
	}
	NetMulticast_SendValutCheck();
}
bool AMultiPlayerBase::NetMulticast_SendValutCheck_Validate()
{
	return true;
}
void AMultiPlayerBase::NetMulticast_SendValutCheck_Implementation()
{
	if (WallForwardTracer() && WallHeightTracer(wallLoc, wallNomal) && !IsVault) {
		WallBackHeightTracer(wallLoc);
		IsVault = true;
		PlayerVault();
	}
}

bool AMultiPlayerBase::Server_SendFireBullet_Validate(FVector loc)
{
	return true;
}
void AMultiPlayerBase::Server_SendFireBullet_Implementation(FVector loc)
{
	//FireBullet(equipWeapone->BodyMesh->GetSocketLocation("Muzzle"), FollowCamera->GetComponentRotation(), rot);
	if (equipWeapone && !IsLocallyControlled()) {
		//armAnim->PlayFireMontage();
		equipWeapone->PlayFireMontage();
		/*if (EquipWeaponInAmmo > 1.0f) {
			EquipWeaponInAmmo -= 1.0f;
		}
		else { equipWeapone->PlayEmptyMontage(); }*/
		
		IsFireAuto = false;
		FVector muzzleLoc = equipWeapone->BodyMesh->GetSocketLocation("Muzzle");
		FRotator bulletFireRot = FRotator::ZeroRotator;
		if (upperState->GetState() == UADS_PlayerUpper::StaticClass()) {
			bulletFireRot = FollowCamera->GetForwardVector().Rotation();
		}
		else { bulletFireRot = UKismetMathLibrary::FindLookAtRotation(muzzleLoc, loc); }

		equipWeapone->ProjectileFire(equipWeapone->BodyMesh->GetSocketLocation("Muzzle")
			, FollowCamera->GetComponentRotation(), bulletFireRot);
		if (equipWeapone->GetWeaponeLever() == WEAPONLEVER::FULLAUTO) {
			GetWorldTimerManager().SetTimer(fireTimer, this, &AMultiPlayerBase::FireAutoOn, 0.15f, false);
		}
		//EquipWeapon->SetIsFire(false);
		//EnemyHearing->ReportNoiseEvent(this, GetActorLocation(), 1.0f, this, 10000.0f, FName("FireNoise"));
		NetMulticast_SendFireBullet(loc);
	}
}
bool AMultiPlayerBase::NetMulticast_SendFireBullet_Validate(FVector loc)
{
	return true;
}
void AMultiPlayerBase::NetMulticast_SendFireBullet_Implementation(FVector loc)
{
	//FireBullet(equipWeapone->BodyMesh->GetSocketLocation("Muzzle"), FollowCamera->GetComponentRotation(), rot);
	if (equipWeapone && !IsLocallyControlled() && !HasAuthority()) {
		//armAnim->PlayFireMontage();
		equipWeapone->PlayFireMontage();
		/*if (EquipWeaponInAmmo > 1.0f) {
			EquipWeaponInAmmo -= 1.0f;
		}
		else { equipWeapone->PlayEmptyMontage(); }*/

		IsFireAuto = false;
		FVector muzzleLoc = equipWeapone->BodyMesh->GetSocketLocation("Muzzle");
		FRotator bulletFireRot = FRotator::ZeroRotator;
		if (upperState->GetState() == UADS_PlayerUpper::StaticClass()) {
			bulletFireRot = FollowCamera->GetForwardVector().Rotation();
		}
		else { bulletFireRot = UKismetMathLibrary::FindLookAtRotation(muzzleLoc, loc); }

		equipWeapone->ProjectileFire(equipWeapone->BodyMesh->GetSocketLocation("Muzzle")
			, bulletFireRot, bulletFireRot);
		if (equipWeapone->GetWeaponeLever() == WEAPONLEVER::FULLAUTO) {
			GetWorldTimerManager().SetTimer(fireTimer, this, &AMultiPlayerBase::FireAutoOn, 0.15f, false);
		}
		//EquipWeapon->SetIsFire(false);
		//EnemyHearing->ReportNoiseEvent(this, GetActorLocation(), 1.0f, this, 10000.0f, FName("FireNoise"));
	}
}

bool AMultiPlayerBase::NetMulticast_SendPlayerHit_Validate(float Damage, FVector Dir, FHitResult Hit, AActor* DamageCauser)
{
	return true;
}
void AMultiPlayerBase::NetMulticast_SendPlayerHit_Implementation(float Damage, FVector Dir, FHitResult Hit, AActor* DamageCauser)
{
	if (IsLocallyControlled()) {
		
	}
	if (!HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("NetMulticast_SendPlayerHit: %f"), Damage);
		float cd = Damage;
		UGameplayStatics::ApplyPointDamage(this, cd, Dir, Hit, nullptr, DamageCauser, nullptr);
	}
	else {
		ATwoVersus_GameState* State = Cast<ATwoVersus_GameState>(UGameplayStatics::GetGameState(this));
		if (State) State->ChangeTeamHPCheck.Execute();
	}
}

bool AMultiPlayerBase::Server_SendReloadAmmo_Validate(int loadAmmo, int equipAmmo)
{
	return true;
}
void AMultiPlayerBase::Server_SendReloadAmmo_Implementation(int loadAmmo, int equipAmmo)
{
	ATwoVersus_PlayerState* state = Cast<ATwoVersus_PlayerState>(GetPlayerState());
	state->loadedAmmo = loadAmmo;
	state->equipAmmo = equipAmmo;
}

bool AMultiPlayerBase::Server_SendPlayerStart_Validate()
{
	return true;
}
void AMultiPlayerBase::Server_SendPlayerStart_Implementation()
{
	NetMulticast_SendPlayerStart();
}
bool AMultiPlayerBase::NetMulticast_SendPlayerStart_Validate()
{
	return true;
}
void AMultiPlayerBase::NetMulticast_SendPlayerStart_Implementation()
{
	IsPlayerCameraTurn = true;
	IsMove = true;
}

bool AMultiPlayerBase::Server_SetPlayerStartLoc_Validate()
{
	return true;
}
void AMultiPlayerBase::Server_SetPlayerStartLoc_Implementation()
{
	ATwoVersus_GameState* State = Cast<ATwoVersus_GameState>(UGameplayStatics::GetGameState(this));
	ATwoVersus_GameMode* GameMode = Cast<ATwoVersus_GameMode>(UGameplayStatics::GetGameMode(this));
	ATwoVersus_PlayerController* Ctrl = Cast<ATwoVersus_PlayerController>(GetController());
	ATwoVersus_PlayerState* PState = Cast<ATwoVersus_PlayerState>(GetPlayerState());
	PState->TeamName = Ctrl->GetTeamName();
	for (int i = 0; i < State->AllPlayerStart.Num(); i++) {
		ATwoVersus_PlayerStart* PlayerStart = Cast<ATwoVersus_PlayerStart>(State->AllPlayerStart[i]);
		UE_LOG(LogTemp, Warning, TEXT("TeamName_P: %s"), *(PlayerStart->TeamName));
		UE_LOG(LogTemp, Warning, TEXT("TeamName_C: %s"), *(Ctrl->GetTeamName()));
		if (Ctrl->GetTeamName() == PlayerStart->TeamName && !(PlayerStart->IsUse)) {
			UE_LOG(LogTemp, Warning, TEXT("index: %d"), i);
			PlayerStart->IsUse = true;
			SetActorLocation(PlayerStart->GetActorLocation());
			break;
		}
		/*if (Ctrl->GetTeamName() == "Red") {
			GameMode->SetRedTeamCount(GameMode->GetRedTeamCount() + 1);
		}
		else {
			GameMode->SetBlueTeamCount(GameMode->GetBlueTeamCount() + 1);
		}*/
	}

}

void AMultiPlayerBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMultiPlayerBase, upperPitch, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AMultiPlayerBase, IsJumped, COND_SkipOwner);
	DOREPLIFETIME(AMultiPlayerBase, IsHandUp);
	DOREPLIFETIME(AMultiPlayerBase, IsCoverLeft);
	DOREPLIFETIME(AMultiPlayerBase, IsCoverRight);
	DOREPLIFETIME(AMultiPlayerBase, coverAngle);
	//DOREPLIFETIME_CONDITION(AMultiPlayerBase, PlayerSpeed, COND_SkipOwner);
	//DOREPLIFETIME_CONDITION(AMultiPlayerBase, controllerRot, COND_SkipOwner);
}
