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
#include "Dalos/Character/Public/Player/PlayerArm_AnimInstance.h"
#include "Dalos/Character/Public/Player/PlayerBody_AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
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
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
	BodyMesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	BodyMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	BodyMesh->SetCollisionProfileName("CharacterMesh");
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


}

void AMultiPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(FName("spine_02"), PBO_None);
	GetMesh()->SetOnlyOwnerSee(true);
	GetMesh()->SetCastShadow(false);
	ArmMesh->SetOnlyOwnerSee(true);
	IsPlayerCameraTurn = true;
	//auto controller = Cast<APlayerController>(GetController());
	//controller->SetInputMode(FInputModeGameAndUI());
	BodyMesh->SetOwnerNoSee(true);
	BodyMesh->SetCastShadow(true);
	BodyMesh->bCastHiddenShadow = true;

	upperState = NewObject<UUnArmed_PlayerUpper>(this, UUnArmed_PlayerUpper::StaticClass());
	downState = NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass());
	upperStateNClass = upperState->GetState();
	downStateNClass = downState->GetState();
	upperState->StateStart(this);
	downState->StateStart(this);

}

void AMultiPlayerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	bodyAnim = Cast<UPlayerBody_AnimInstance>(BodyMesh->GetAnimInstance());
	armAnim = Cast<UPlayerArm_AnimInstance>(ArmMesh->GetAnimInstance());

	/*armAnim->playFire.BindLambda([this]()->void {
		
		});*/
}

void AMultiPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	downState->StateUpdate(this);
	upperState->StateUpdate(this);
	
	PlayerMove();
	CrossHairCheck();
	RecoilCheck();

	if (IsFire && IsFireAuto) {
		FireBullet();
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

	if (hitis) {
		crossHairEndLoc = outHit.Location;
		distance = outHit.Distance;
	}
	else {
		crossHairEndLoc = endTracer;
		distance = 2000.0f;
	}
	float radius = (FMath::Tan(FollowCamera->FieldOfView / 2.0f) * distance) / 30.0f; // 40을 바꾸기

	float bulletEndLocationZ = FMath::RandRange((crossHairEndLoc + (FollowCamera->GetUpVector() * radius)).Z,
		(crossHairEndLoc + (-(FollowCamera->GetUpVector()) * radius)).Z);
	float bulletEndLocationY = FMath::RandRange((crossHairEndLoc + (FollowCamera->GetRightVector() * radius)).Y,
		(crossHairEndLoc + (-(FollowCamera->GetRightVector()) * radius)).Y);
	float bulletEndLocationX = crossHairEndLoc.X;

	bulletEndLoc = FVector(bulletEndLocationX, bulletEndLocationY, bulletEndLocationZ);
	if (upperState->GetState() == UADS_PlayerUpper::StaticClass()) {
		bulletRot = FollowCamera->GetForwardVector().Rotation();
	}
	else { bulletRot = UKismetMathLibrary::FindLookAtRotation(muzzleLoc, bulletEndLoc); }

	/*투영행렬을 이용하여 만들 수 있을것같다
	기존에 있는 것 보다 정확하게 수치에 따라 변하도록 업그레이드를 했다
	기존의 것은 거리에 따라 커지게는 만들었지만 일정하게 조절하지 못하게 만들었다.
	이번에는 fov 수치를 이용해서 좀더 정확한 수치가 나오도록 업그레이드를 했다.*/

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
		EquipGunOnHand(check);
	}
}

void AMultiPlayerBase::EquipGunOnHand(AWeaponeBase* equip)
{
	equipWeapone = equip;
	equipWeapone->SetWeaponeState(WEAPONSTATE::EQUIP);
	equipWeapone->SetOwner(this); // 여기 문제 영상 참고
	equipWeapone->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponeLoc"));
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

}

void AMultiPlayerBase::PlayerSplint()
{
	DownPress(nullptr);
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
	else { NetMulticast_SendUpperPress(EPlayerPress::FIRSTGUN); }
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
	else { NetMulticast_SendUpperPress(EPlayerPress::SECONDGUN); }
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
	else { NetMulticast_SendUpperPress(EPlayerPress::UNARMED); }
}

void AMultiPlayerBase::PlayerInteraction()
{
	if (lookWeapone) {
		WeaponCheck(lookWeapone);
		if (!HasAuthority()) Server_SendWeaponeCheck(lookWeapone);
		UpperPress(nullptr);
		if (!HasAuthority()) {
			Server_SendUpperPress(EPlayerPress::INTERACTION);
		}
		else { NetMulticast_SendUpperPress(EPlayerPress::INTERACTION); }
	}
}

void AMultiPlayerBase::PlayerADS()
{
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
	upperState->PlayerFire(this, equipWeapone, IsFireAuto, threeCount);
	//armAnim->PlayFireMontage();
}
void AMultiPlayerBase::PlayerUnFire()
{
	armAnim->StopFireMontage();
	IsFire = false;
	IsFireAuto = false;
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
		//UE_LOG(LogTemp, Warning, TEXT("PlayerSpeed: %f"), PlayerSpeed);
		moveDir.Normalize();

		//float InputDir = 0.0f;
		if (FMath::Abs(inputDirForward) > FMath::Abs(inputDirRight)) {
			inputDir = FMath::Abs(inputDirForward);
		}
		else { inputDir = FMath::Abs(inputDirRight); }
		//UE_LOG(LogTemp, Warning, TEXT("Input: %f"), FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(0.0f, PlayerSpeed), InputDir));
		AddMovementInput(moveDir, FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(0.0f, PlayerSpeed), inputDir) * 0.008f);
		//MoveDir.Set(0.0f, 0.0f, 0.0f);
	}
}

void AMultiPlayerBase::FireAutoOn()
{
	IsFireAuto = true;
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

void AMultiPlayerBase::FireBullet()
{
	if (equipWeapone) {
		armAnim->PlayFireMontage();
		//equipWeapone->PlayFireMontage();
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

		FVector muzzleLoc= equipWeaponeArm->BodyMesh->GetSocketLocation("Muzzle");
		FRotator muzzleRot = FollowCamera->GetComponentRotation();
		//UE_LOG(LogTemp, Warning, TEXT("MuzzleLocation: %f, %f, %f"), MuzzleLocation.X, MuzzleLocation.Y, MuzzleLocation.Z);

		equipWeaponeArm->ProjectileFire(muzzleLoc, muzzleRot, bulletRot);
		if (equipWeapone->GetWeaponeLever() == WEAPONLEVER::FULLAUTO) {
			GetWorldTimerManager().SetTimer(fireTimer, this, &AMultiPlayerBase::FireAutoOn, 0.15f, false);
		}
		//EquipWeapon->SetIsFire(false);
		//EnemyHearing->ReportNoiseEvent(this, GetActorLocation(), 1.0f, this, 10000.0f, FName("FireNoise"));
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
		//if (HasAuthority()) NetMulticast_SendWeaponeCheck(check);
	}
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
	//if(HasAuthority()) NetMulticast_SendWeaponeChange(press);
}

void AMultiPlayerBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMultiPlayerBase, upperPitch, COND_SkipOwner);
	//DOREPLIFETIME_CONDITION(AMultiPlayerBase, controllerRot, COND_SkipOwner);
}
