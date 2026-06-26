// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"

/**
 * 개인 점수, 콤보, 성공·실패 횟수를 관리하는 컴포넌트
 * PlayerState에 부착된다.
 *
 * 담당자: 한수현
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PARCEL_KNIGHT_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerStatComponent();

	/**
	 * [Server Only] 배송 성공·감점 결과를 개인 점수에 반영
	 * @param Amount 양수면 획득, 음수면 감점
	 */
	void AddScore(int32 Amount);

private:
	UPROPERTY()
	int32 PersonalScore;  // 개인 누적 점수

	UPROPERTY()
	int32 SuccessCount;   // 연속 성공 콤보 계산 기준

	UPROPERTY()
	int32 ComboCount;     // 콤보 배율 적용에 사용
};
