#include "Delivery/DeliveryZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Delivery/DeliveryBox.h"
#include "Delivery/PhysicsJudgeManager.h"
#include "Delivery/DeliverySubsystem.h"

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
	if (!Box || !HasAuthority()) return;

	FBoxData Data = Box->GetBoxData();
	bool bIsCorrectZone = false;

	// 매칭해둔 상자의 GameplayTag와 이 구역의 ZoneType을 비교 검사합니다.
	FGameplayTag BoxType = Data.BoxTypeTag;
	
	if (ZoneType == EDestinationType::ZoneA && BoxType.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Box.Type.ZoneA")))) bIsCorrectZone = true;
	else if (ZoneType == EDestinationType::ZoneB && BoxType.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Box.Type.ZoneB")))) bIsCorrectZone = true;
	else if (ZoneType == EDestinationType::ZoneC && BoxType.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Box.Type.ZoneC")))) bIsCorrectZone = true;
	else if (ZoneType == EDestinationType::Emergency && BoxType.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Box.Type.Emergency")))) bIsCorrectZone = true;

	if (bIsCorrectZone)
	{
		UE_LOG(LogDelivery, Log, TEXT("[Server] Success: Delivered to the correct zone! Score +%d"), Data.BaseScore);
		Box->AddStateTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Delivered")));
		
		// Todo: 게임모드 팀원에게 점수 반영 함수 호출 연동
		// GetAuthGameMode()->AddTeamScore(Data.BaseScore);
	}
	else
	{
		UE_LOG(LogDelivery, Warning, TEXT("[Server] Fail: Delivered to the wrong zone! Penalty %d"), Data.DamagePenalty);
		Box->AddStateTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Failed")));
		
		// Todo: 감점 처리 연동
		// GetAuthGameMode()->AddTeamScore(Data.DamagePenalty);
	}

	// 사용 완료된 상자는 디스폰 서브시스템을 통해 깔끔하게 삭제
	if (UDeliverySubsystem* DeliverySubsystem = GetWorld()->GetSubsystem<UDeliverySubsystem>())
	{
		DeliverySubsystem->DespawnBox(Box);
	}
}