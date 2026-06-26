// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeliveryRuleComponent.generated.h"

/**
 * 게임 룰 로직을 담당하는 컴포넌트
 * GameMode에 부착되며 점수 계산, 시간 관리, 배송 판정을 처리한다.
 *
 * 담당자: 한수현
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PARCEL_KNIGHT_API UDeliveryRuleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDeliveryRuleComponent();

	// [Server Only] 배송 판정은 서버에서만 — 결과는 GameState를 통해 클라이언트에 전파
	void EvaluateDelivery();

	// [Server Only] 택배 종류·콤보·감점을 반영한 최종 점수 반환
	int32 CalculateScore();

private:
	// 스테이지별로 DA_StageData에서 오버라이드 가능
	UPROPERTY()
	float TimeLimit;

	// 이 점수 달성 시 클리어 판정 — DA_StageData에서 설정
	UPROPERTY()
	int32 TargetScore;
};
