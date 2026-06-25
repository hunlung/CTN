#include "Delivery/PhysicsJudgeManager.h"
// #include "DeliveryBox.h" // 박스 주석

void UPhysicsJudgeManager::EvaluateImpact(ADeliveryBox* Box, float ImpactForce)
{
	if (!Box) return;

	if (GetWorld() && GetWorld()->GetNetMode() == NM_Client) return;

	float DamageThreshold = 500.f; 

	if (ImpactForce >= DamageThreshold)
	{
		// 택배박스 호출
		// Box->SetBoxState(EBoxState::Damaged);
		
		UE_LOG(LogTemp, Warning, TEXT("서버 판정: 택배 ID %d번이 %f의 충격으로 파손되었습니다!"), 1001, ImpactForce);
	}
}

void UPhysicsJudgeManager::EvaluateTrapImpact(ADeliveryBox* Box, float TrapImpactForce)
{
	if (!Box) return;
	if (GetWorld() && GetWorld()->GetNetMode() == NM_Client) return;

	UE_LOG(LogTemp, Warning, TEXT("서버 판정: 함정 직접 충격 감지! (대미지: %f)"), TrapImpactForce);
}