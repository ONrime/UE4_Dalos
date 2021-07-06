// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Dalos/Character/Public/CharacterBase.h"
#include "MultiPlayerBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPlayerPress : uint8 {
	CROUCH UMETA(DisplayName = "Crouch"),
	JUMP UMETA(DisplayName = "Jump"),
	SPLINT UMETA(DisplayName = "Splint"),
	ADS UMETA(DisplayName = "ADS"),
	FIRSTGUN UMETA(DisplayName = "FirstGun"),
	SECONDGUN UMETA(DisplayName = "SecondGun"),
	UNARMED UMETA(DisplayName = "UnArmed"),
	INTERACTION UMETA(DisplayName = "Interaction")
	
};

UCLASS()
class DALOS_API AMultiPlayerBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AMultiPlayerBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AloowPrivateAccess = "true"))
	class USkeletalMeshComponent* ArmMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AloowPrivateAccess = "true"))
	class USkeletalMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerBase)
	float PlayerSpeed = 70.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate = 45.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate = 45.0f;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION() 
	void OnOverlapBegin_Mesh(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY()
	class UPlayerArm_AnimInstance* armAnim = nullptr;
	UPROPERTY()
	class UPlayerBody_AnimInstance* bodyAnim = nullptr;
	UPROPERTY()
	class UPlayerBody_AnimInstance* legAnim = nullptr;
	UPROPERTY()
	class AMultiPlayer_HUD* HUD = nullptr;
	class AWeaponeBase* equipWeapone = nullptr;
	class AWeaponeBase* equipWeaponeArm = nullptr;
	class AWeaponeBase* backWeapone1 = nullptr;
	class AWeaponeBase* backWeapone2 = nullptr;
	class AWeaponeBase* lookWeapone = nullptr;
	bool InteractionCheck();
	bool CrossHairCheck();
	bool RecoilCheck();
	void WeaponCheck(AWeaponeBase* check);  // 현제 들고 있는 무기들을 확인해서 장착할지 버릴지 결정한다.
	void EquipGunOnHand(AWeaponeBase* equip);
	UClass* equipWeaponeStaticClass = nullptr;
	float recoilReturnPitch = 0.0f;
	bool recoilReturn = false;
	float recoilPitch = 0.0f;
	FRotator recoilRot = FRotator::ZeroRotator;
	FVector crossHairEndLoc = FVector::ZeroVector; // 총알과 쉴드를 위해 밖으로빼어 만든 위치
	FVector recoilReturnLoc = FVector::ZeroVector;
	FVector recoilReturnDir = FVector::ZeroVector;
	FRotator bulletRot = FRotator::ZeroRotator;

	UPROPERTY(Replicated)
	float upperPitch = 0.0f;
	float upperYaw = 0.0f;
	float upperYaw2 = 0.0f; // 하체고정시 상체 회전 값 yaw2는 90도 정제 없이 180도 기준으로 출력
	//UPROPERTY(Replicated)
	FRotator controllerRot = FRotator::ZeroRotator;
	float aimDirRight = 0.0f, aimDirUp = 0.0f; // 컨트롤러로 부터 받은 시점 키값 (AnimBP에서 사용하기 위해)
	float inputDirForward = 0.0f;
	float inputDirRight = 0.0f; // 컨트롤러로 부터 받은 이동 키값 (AnimBP에서 사용하기 위해)
	FVector moveDir = FVector::ZeroVector;
	float inputDir = 0.0f;
	UPROPERTY(EditAnywhere)
	EPlayerPress playerPress;
	FVector cameraLoc = FVector::ZeroVector;
	bool IsCameraLock = false;

	bool IsFire = false;
	bool IsFireAuto = false;
	FTimerHandle fireTimer;
	float threeCount = 0.0f;

	FVector wallLoc = FVector::ZeroVector;
	FVector wallHeight = FVector::ZeroVector;
	FVector wallBackHeight = FVector::ZeroVector;
	FVector wallNomal = FVector::ZeroVector;

private:
	// 상태
	class UPlayerUpperStateBase* upperState = nullptr;
	class UPlayerDownStateBase* downState = nullptr;

	void PlayerCrouch();
	void PlayerJump();
	void PlayerSplint();
	void PlayerFirstGun();
	void PlayerSecondGun();
	void PlayerUnArmed();
	void PlayerInteraction();
	void PlayerADS();
	void PlayerUnADS();
	void PlayerFire();
	void PlayerUnFire();
	void PlayerReload();
	void PlayerVault();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void PlayerMove();
	void FireAutoOn();
	bool WallForwardTracer();
	bool WallHeightTracer(FVector loc, FVector nomal);
	bool WallBackHeightTracer(FVector loc);
	void StopVault();
	void StopClimb();

public:
	UPlayerUpperStateBase* GetUpperState() { return upperState; };
	UPlayerDownStateBase* GetDownState() { return downState; };
	UClass* upperStateBClass = nullptr; UClass* downStateBClass = nullptr;   //전 상태에 대한 Class값 (AnimBP에서 사용하기 위해)
	UClass* upperStateNClass = nullptr; UClass* downStateNClass = nullptr;   //전 상태에 대한 Class값 (AnimBP에서 사용하기 위해)
	void UpperPress(class UPlayerUpperStateBase* state);
	void DownPress(class UPlayerDownStateBase* state);
	float GetUpperPitch() { return upperPitch; }
	float GetUpperYaw() { return upperYaw; }
	void SetUpperYaw(float yaw) { upperYaw = yaw; }
	float GetUpperYaw2() { return upperYaw2; }
	FRotator GetControllerRot() { return controllerRot; }
	float GetAimDirRight() { return aimDirRight; } float GetAimDirUp() { return aimDirUp; }
	float GetInputDirForward() { return inputDirForward; } float GetInputDirRight() { return inputDirRight; }
	EPlayerPress GetPlayerPress() { return playerPress; }
	bool GetIsCameraLock() { return IsCameraLock; }
	class UPlayerBody_AnimInstance* GetBodyAnim() { return bodyAnim; }
	class UPlayerBody_AnimInstance* GetLegAnim() { return legAnim; }
	class UPlayerArm_AnimInstance* GetArmAnim() { return armAnim; }
	class AMultiPlayer_HUD* GetHUD() { return HUD; }

	void FireBullet(FVector muzzleLoc, FRotator muzzleRot, FRotator bulletRotation);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void SendControllerRot(FRotator rot);
	bool SendControllerRot_Validate(FRotator rot);
	void SendControllerRot_Implementation(FRotator rot);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendDownPress(EPlayerPress press);
	bool Server_SendDownPress_Validate(EPlayerPress press);
	void Server_SendDownPress_Implementation(EPlayerPress press);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendDownPress(EPlayerPress press);
	bool NetMulticast_SendDownPress_Validate(EPlayerPress press);
	void NetMulticast_SendDownPress_Implementation(EPlayerPress press);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendUpperPress(EPlayerPress press);
	bool Server_SendUpperPress_Validate(EPlayerPress press);
	void Server_SendUpperPress_Implementation(EPlayerPress press);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendUpperPress(EPlayerPress press);
	bool NetMulticast_SendUpperPress_Validate(EPlayerPress press);
	void NetMulticast_SendUpperPress_Implementation(EPlayerPress press);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendWeaponeCheck(AWeaponeBase* check);
	bool Server_SendWeaponeCheck_Validate(AWeaponeBase* check);
	void Server_SendWeaponeCheck_Implementation(AWeaponeBase* check);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendWeaponeCheck(AWeaponeBase* check);
	bool NetMulticast_SendWeaponeCheck_Validate(AWeaponeBase* check);
	void NetMulticast_SendWeaponeCheck_Implementation(AWeaponeBase* check);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendWeaponeChange(EPlayerPress press);
	bool Server_SendWeaponeChange_Validate(EPlayerPress press);
	void Server_SendWeaponeChange_Implementation(EPlayerPress press);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendWeaponeChange(EPlayerPress press);
	bool NetMulticast_SendWeaponeChange_Validate(EPlayerPress press);
	void NetMulticast_SendWeaponeChange_Implementation(EPlayerPress press);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendIsJumped(bool jumped);
	bool Server_SendIsJumped_Validate(bool jumped);
	void Server_SendIsJumped_Implementation(bool jumped);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendValutCheck();
	bool Server_SendValutCheck_Validate();
	void Server_SendValutCheck_Implementation();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendValutCheck();
	bool NetMulticast_SendValutCheck_Validate();
	void NetMulticast_SendValutCheck_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendFireBullet(FRotator rot);
	bool Server_SendFireBullet_Validate(FRotator rot);
	void Server_SendFireBullet_Implementation(FRotator rot);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendFireBullet(FRotator rot);
	bool NetMulticast_SendFireBullet_Validate(FRotator rot);
	void NetMulticast_SendFireBullet_Implementation(FRotator rot);

	bool IsMove = true;
	bool IsPlayerCameraTurn = true;
	bool IsPlayerRotationYawSpeedSlow = false;
	UPROPERTY(Replicated)
	bool IsJumped = false;
	bool IsWall = false;
	bool IsWallThick = false;
	bool IsVault = false;
	UPROPERTY(Replicated)
	bool IsHandUp = false;
	UPROPERTY(Replicated)
	bool IsCoverLeft = false;
	UPROPERTY(Replicated)
	bool IsCoverRight = false;
	UPROPERTY(Replicated)
	float coverAngle = 0.0f;

};
