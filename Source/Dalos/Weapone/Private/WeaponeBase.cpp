// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Weapone/Public/WeaponeBase.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h"

// Sets default values
AWeaponeBase::AWeaponeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	SetReplicateMovement(true);

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BODY"));
	RootComponent = BodyMesh;
	BodyMesh->SetIsReplicated(true);

	Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("AUDIO"));
	Sound->SetupAttachment(RootComponent);

	InteractionUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("INTERACTION_UI"));
	InteractionUI->SetupAttachment(RootComponent);
	InteractionUI->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	InteractionUI->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionUI->SetDrawAtDesiredSize(true);
	InteractionUI->SetHiddenInGame(true);
	InteractionUI->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FClassFinder<UUserWidget>INTERACTION_HUD(TEXT("WidgetBlueprint'/Game/UI/Item/Item_Interaction.Item_Interaction_C'"));
	if (INTERACTION_HUD.Succeeded()) { InteractionUI->SetWidgetClass(INTERACTION_HUD.Class); }

	WeaponeLever = WEAPONLEVER::SINGLEFIRE;
}

// Called when the game starts or when spawned
void AWeaponeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponeBase::StateStart(WEAPONSTATE state)
{
	switch (state)
	{
	case WEAPONSTATE::EQUIP:
		UE_LOG(LogTemp, Warning, TEXT("WEAPONSTATE::EQUIP"));
		BodyMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		BodyMesh->SetSimulatePhysics(false);
		BodyMesh->SetCollisionProfileName("NoCollision");
		break;
	case WEAPONSTATE::BACKEQUIP:
		UE_LOG(LogTemp, Warning, TEXT("WEAPONSTATE::BACKEQUIP"));
		BodyMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		BodyMesh->SetSimulatePhysics(false);
		BodyMesh->SetCollisionProfileName("NoCollision");
		break;
	case WEAPONSTATE::DROP:
		UE_LOG(LogTemp, Warning, TEXT("WEAPONSTATE::DROP"));
		BodyMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		BodyMesh->SetSimulatePhysics(true);
		BodyMesh->SetCollisionProfileName("ItemObject");
		break;
	case WEAPONSTATE::THROW:
		BodyMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		BodyMesh->SetSimulatePhysics(true);
		BodyMesh->SetCollisionProfileName("ItemObject");
		break;
	default:
		break;
	}
}

void AWeaponeBase::InteractionStart_Implementation()
{
	InteractionUI->SetHiddenInGame(false);
}

void AWeaponeBase::InteractionUpdate_Implementation()
{
}

void AWeaponeBase::InteractionEnd_Implementation()
{
	InteractionUI->SetHiddenInGame(true);
}

UClass* AWeaponeBase::GetStaticClass()
{
	return nullptr;
}

void AWeaponeBase::SetWeaponeState(WEAPONSTATE set) {
	WeaponeState = set;
	StateStart(WeaponeState);
}

