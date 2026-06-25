#include "Delivery/InteractionComponent.h"
#include "Delivery/Interactable.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "CollisionQueryParams.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceDistance = 300.f;
	CurrentFocusedActor = nullptr;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckTraceTarget();
}

void UInteractionComponent::CheckTraceTarget()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PC) return;

	FVector TraceStart;
	FRotator TraceRotation;
	PC->GetPlayerViewPoint(TraceStart, TraceRotation);

	FVector TraceEnd = TraceStart + (TraceRotation.Vector() * TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		AActor* HitActor = HitResult.GetActor();
		
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			if (CurrentFocusedActor != HitActor)
			{
				CurrentFocusedActor = HitActor;
			}
			return;
		}
	}

	CurrentFocusedActor = nullptr;
}

void UInteractionComponent::PrimaryInteract()
{
	if (!CurrentFocusedActor) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	IInteractable* Interactable = Cast<IInteractable>(CurrentFocusedActor);
	if (Interactable && Interactable->CanInteract(OwnerCharacter))
	{
		Interactable->Interact(OwnerCharacter);
	}
}