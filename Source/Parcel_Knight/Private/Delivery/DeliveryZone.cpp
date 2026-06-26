#include "Delivery/DeliveryZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Delivery/DeliveryBox.h"
#include "Delivery/PhysicsJudgeManager.h"

ADeliveryZone::ADeliveryZone()
{
	PrimaryActorTick.bCanEverTick = false;

	ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneMesh"));
	RootComponent = ZoneMesh;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	OverlapVolume->SetupAttachment(RootComponent);
	
	OverlapVolume->SetCollisionProfileName(TEXT("Trigger"));
	
	ZoneType = EDestinationType::None;
}

void ADeliveryZone::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &ADeliveryZone::OnZoneOverlap);
	}
}

void ADeliveryZone::OnZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
								  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
								  bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	if (ADeliveryBox* Box = Cast<ADeliveryBox>(OtherActor))
	{
		if (Box->HasStateTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Delivered")))) return;

		ProcessDelivery(Box);
	}
}

void ADeliveryZone::ProcessDelivery(AActor* InBox)
{
	ADeliveryBox* Box = Cast<ADeliveryBox>(InBox);
	if (!Box) return;

	FBoxData Data = Box->GetBoxData();
	
	UE_LOG(LogTemp, Log, TEXT("서버 배송처: 구역[%d]에 택배 ID[%d]가 진입함."), static_cast<uint8>(ZoneType), Box->GetBoxID());
	
	Box->AddStateTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Delivered")));
	
	if (UPhysicsJudgeManager* JudgeManager = GetWorld()->GetSubsystem<UPhysicsJudgeManager>())
	{

	}
}