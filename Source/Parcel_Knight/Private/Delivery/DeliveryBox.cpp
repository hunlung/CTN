#include "Delivery/DeliveryBox.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ADeliveryBox::ADeliveryBox()
{
	PrimaryActorTick.bCanEverTick = false; // 호스트 PC 퍼포먼스를 위해 틱 비활성화
	bReplicates = true;
	SetReplicateMovement(true); // 리슨 서버 물리 동기화 활성화

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	RootComponent = BoxMesh;

	// 기본 무거운 물리 바디 콜리전 세팅
	BoxMesh->SetSimulatePhysics(true);
	BoxMesh->SetCollisionProfileName(TEXT("PhysicsBody"));
    
	BoxID = -1;
}

void ADeliveryBox::BeginPlay()
{
	Super::BeginPlay();
	
	//Todo : 물리적 충돌 이벤트
}

void ADeliveryBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeliveryBox, BoxID);
	DOREPLIFETIME(ADeliveryBox, BoxStateTags);
	DOREPLIFETIME(ADeliveryBox, BoxData);
	DOREPLIFETIME(ADeliveryBox, HolderPlayer);
}

void ADeliveryBox::InitializeBox(int32 InBoxID, const FBoxData& InBoxData)
{
	if (!HasAuthority()) return;

	BoxID = InBoxID;
	BoxData = InBoxData;
    
	// 서버에서 StaticMesh 즉시 적용
	if (BoxData.BoxMeshAsset)
	{
		BoxMesh->SetStaticMesh(BoxData.BoxMeshAsset);
		// 무게 적용 (밸런싱 수치 조절 (현재 1.0f))
		BoxMesh->SetMassOverrideInKg(NAME_None, BoxData.Weight * 1.0f, true);
	}

	// 초기 상태인 Spawned 태그 부여
	AddStateTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Spawned")));
}

void ADeliveryBox::OnRep_BoxData()
{
	// 클라이언트 측에서도 서버가 채워준 BoxData를 받으면 메시 외형을 동기화함
	if (BoxData.BoxMeshAsset)
	{
		BoxMesh->SetStaticMesh(BoxData.BoxMeshAsset);
	}
}

void ADeliveryBox::AddStateTag(FGameplayTag NewStateTag)
{
	if (!HasAuthority() || !NewStateTag.IsValid()) return;

	if (!BoxStateTags.HasTagExact(NewStateTag))
	{
		BoxStateTags.AddTag(NewStateTag);
        
		// 서버 측에서 상태 변경 시 즉각 처리할 물리 제어 규칙
		if (NewStateTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Held"))))
		{
			BoxMesh->SetSimulatePhysics(false);
			BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 들려있을 땐 환경 충돌 무시
		}
	}
}

void ADeliveryBox::RemoveStateTag(FGameplayTag StateTag)
{
	if (!HasAuthority() || !StateTag.IsValid()) return;

	if (BoxStateTags.HasTagExact(StateTag))
	{
		BoxStateTags.RemoveTag(StateTag);
        
		// 상태가 제거될 때의 예외 복구 로직
		if (StateTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Held"))))
		{
			BoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			BoxMesh->SetSimulatePhysics(true);
		}
	}
}

bool ADeliveryBox::HasStateTag(FGameplayTag StateTag) const
{
	return BoxStateTags.HasTag(StateTag);
}

void ADeliveryBox::OnRep_BoxStateTags()
{
	// 클라이언트 사이드 시각/청각 연출 분기점
	FGameplayTag HeldTag = FGameplayTag::RequestGameplayTag(TEXT("Box.State.Held"));
	FGameplayTag DamagedTag = FGameplayTag::RequestGameplayTag(TEXT("Box.State.Damaged"));

	if (BoxStateTags.HasTag(HeldTag))
	{
		BoxMesh->SetSimulatePhysics(false);
		BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		BoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BoxMesh->SetSimulatePhysics(true);
	}

	if (BoxStateTags.HasTag(DamagedTag))
	{
		// Todo : 찌그러진 메쉬로 교체하거나 내용물이 튀어나오는 이펙트/사운드 호출
	}
}

/* ==========================================================================
   IPKCarryable 인터페이스 실구현 부 Todo: Character와 연동 필요
   ========================================================================== */

bool ADeliveryBox::CanCarry(AActor* Carrier)
{
	// 월드에 생성되어 놓여있으면 집기 가능 허용 (여러 상태 이상일 때 못집게 할 수 있음)
	return BoxStateTags.HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Spawned")));
}

void ADeliveryBox::OnPickedUp(AActor* Carrier)
{
	if (!HasAuthority() || !Carrier) return;
	
	APawn* CarrierPawn = Cast<APawn>(Carrier);
	if (CarrierPawn)
	{
		HolderPlayer = Cast<APlayerController>(CarrierPawn->GetController());
		SetOwner(CarrierPawn); // 리슨 서버 소유권(Owner) 변경
	}

	RemoveStateTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Spawned")));
	AddStateTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Held")));
}

void ADeliveryBox::OnDropped()
{
	if (!HasAuthority()) return;

	HolderPlayer = nullptr;
	SetOwner(nullptr); // 소유권 월드로 반환

	RemoveStateTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Held")));
	AddStateTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Spawned")));
}

//Todo : OnPhysicsHit 물리적 충돌 구현