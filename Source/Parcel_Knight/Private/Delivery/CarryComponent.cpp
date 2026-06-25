#include "Delivery/CarryComponent.h"
#include "Components/ActorComponent.h" 
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCarryComponent::UCarryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentCarryingBox = nullptr;
}

void UCarryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCarryComponent::PickUpBox(AActor* InBox)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter || !InBox) return;

	CurrentCarryingBox = InBox;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	InBox->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentRules, TEXT("HandSocket"));

	if (UCharacterMovementComponent* Movement = OwnerCharacter->GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = 600.f * 0.6f;
	}
}

void UCarryComponent::DropBox()
{
	if (!CurrentCarryingBox) return;

	CurrentCarryingBox->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		if (UCharacterMovementComponent* Movement = OwnerCharacter->GetCharacterMovement())
		{
			Movement->MaxWalkSpeed = 600.f; 
		}
	}

	CurrentCarryingBox = nullptr;
}