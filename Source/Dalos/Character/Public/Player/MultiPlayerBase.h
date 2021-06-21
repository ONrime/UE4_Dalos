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

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	class UPlayerArm_AnimInstance* armAnim;
	UPROPERTY()
	class UPlayerBody_AnimInstance* bodyAnim;
	UPROPERTY()
	class UPlayerBody_AnimInstance* legAnim;
	class AWeaponeBase* equipWeapone;
	class AWeaponeBase* equipWeaponeArm;
	class AWeaponeBase* backWeapone1;
	class AWeaponeBase* backWeapone2;
	class AWeaponeBase* lookWeapone;
	bool InteractionCheck();
	bool CrossHairCheck();
	bool RecoilCheck();
	void WeaponCheck(AWeaponeBase* check);  // ���� ��� �ִ� ������� Ȯ���ؼ� �������� ������ �����Ѵ�.
	void EquipGunOnHand(AWeaponeBase* equip);
	UClass* equipWeaponeStaticClass = nullptr;
	float recoilReturnPitch = 0.0f;
	bool recoilReturn = false;
	float recoilPitch = 0.0f;
	FRotator recoilRot = FRotator::ZeroRotator;
	FVector crossHairEndLoc = FVector::ZeroVector; // �Ѿ˰� ���带 ���� �����λ��� ���� ��ġ
	FVector recoilReturnLoc = FVector::ZeroVector;
	FVector recoilReturnDir = FVector::ZeroVector;
	FRotator bulletRot = FRotator::ZeroRotator;

	UPROPERTY(Replicated)
	float upperPitch = 0.0f;
	float upperYaw = 0.0f;
	float upperYaw2 = 0.0f; // ��ü������ ��ü ȸ�� �� yaw2�� 90�� ���� ���� 180�� �������� ���
	//UPROPERTY(Replicated)
	FRotator controllerRot = FRotator::ZeroRotator;
	float aimDirRight = 0.0f, aimDirUp = 0.0f; // ��Ʈ�ѷ��� ���� ���� ���� Ű�� (AnimBP���� ����ϱ� ����)
	float inputDirForward = 0.0f;
	float inputDirRight = 0.0f; // ��Ʈ�ѷ��� ���� ���� �̵� Ű�� (AnimBP���� ����ϱ� ����)
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
	// ����
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
	UClass* upperStateBClass = nullptr; UClass* downStateBClass = nullptr;   //�� ���¿� ���� Class�� (AnimBP���� ����ϱ� ����)
	UClass* upperStateNClass = nullptr; UClass* downStateNClass = nullptr;   //�� ���¿� ���� Class�� (AnimBP���� ����ϱ� ����)
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
	UPlayerArm_AnimInstance* GetArmAnim() { return armAnim; }
	bool GetIsCameraLock() { return IsCameraLock; }

	void FireBullet();

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
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendWeaponeChange(EPlayerPress press);
	bool Server_SendWeaponeChange_Validate(EPlayerPress press);
	void Server_SendWeaponeChange_Implementation(EPlayerPress press);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendIsJumped(bool jumped);
	bool Server_SendIsJumped_Validate(bool jumped);
	void Server_SendIsJumped_Implementation(bool jumped);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendValutCheck(bool check, FVector loc, FVector nomal, FVector heightLoc);
	bool Server_SendValutCheck_Validate(bool check, FVector loc, FVector nomal, FVector heightLoc);
	void Server_SendValutCheck_Implementation(bool check, FVector loc, FVector nomal, FVector heightLoc);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendValutCheck(bool check, FVector loc, FVector nomal, FVector heightLoc);
	bool NetMulticast_SendValutCheck_Validate(bool check, FVector loc, FVector nomal, FVector heightLoc);
	void NetMulticast_SendValutCheck_Implementation(bool check, FVector loc, FVector nomal, FVector heightLoc);

	bool IsMove = true;
	bool IsPlayerCameraTurn = true;
	bool IsPlayerRotationYawSpeedSlow = false;
	UPROPERTY(Replicated)
	bool IsJumped = false;
	bool IsWall = false;
	bool IsWallThick = false;
	bool IsVault = false;

};