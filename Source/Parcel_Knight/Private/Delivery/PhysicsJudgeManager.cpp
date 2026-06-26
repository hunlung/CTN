#include "Delivery/PhysicsJudgeManager.h"
#include "Delivery/DeliveryBox.h"

void UPhysicsJudgeManager::EvaluateImpact(ADeliveryBox* Box, float ImpactForce)
{
	if (!Box) return;
	if (GetWorld() && GetWorld()->GetNetMode() == NM_Client) return;

	float DamageThreshold = Box->GetDamageThreshold(); 

	if (ImpactForce >= DamageThreshold)
	{
		Box->AddStateTag(FGameplayTag::RequestGameplayTag(TEXT("Box.State.Damaged")));
		
		UE_LOG(LogTemp, Warning, TEXT("[Server] 서버 판정: 택배 ID %d번이 %f의 충격(임계값: %f)으로 파손되었습니다!"), 
			Box->GetBoxID(), ImpactForce, DamageThreshold);
	}
}

void UPhysicsJudgeManager::EvaluateTrapImpact(ADeliveryBox* Box, float TrapImpactForce)
{
	if (!Box) return;
	if (GetWorld() && GetWorld()->GetNetMode() == NM_Client) return;

	UE_LOG(LogTemp, Warning, TEXT("서버 판정: 함정 직접 충격 감지! (대미지: %f)"), TrapImpactForce);
}