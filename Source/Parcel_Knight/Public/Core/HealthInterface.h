#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UHealthInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 체력 시스템 인터페이스
 *
 * 플레이어·적·함정 등 체력을 가진 모든 액터에 적용한다.
 * 체력 관련 로직을 클래스 종류와 무관하게 일관되게 호출하기 위해 인터페이스로 분리.
 * 
 * 담당자: 한수현
 */
class PARCEL_KNIGHT_API IHealthInterface
{
	GENERATED_BODY()

public:
	virtual float GetHP() const = 0;
	virtual float GetMaxHP() const = 0;
	
	/**
* @param Amount 회복량 —
* NOTE: 최대 체력을 초과하지 않도록 구현 측에서 클램프 필요
* [Server Only] 체력 변경은 서버에서만 — 클라이언트에서 호출 시 무시됨
* NOTE: 구현 측에서 HasAuthority() 체크 필수
*/
	virtual void AddHP(float Amount) = 0;
	/**
* @param Amount 감소량 —
* NOTE: 최소 체력 이하로 떨어지지 않도록 구현 측에서 클램프 필요
* [Server Only] 데미지 처리는 서버에서만 — 결과는 Replicated HP로 클라이언트에 전파
*/
	virtual void TakeDamage(float Amount) = 0;
	
	// [Server Only] 체력이 0이 됐을 때 서버에서 호출 — 구현 측에서 Multicast RPC로 연출 전파
	virtual bool IsDead() const = 0;
};
