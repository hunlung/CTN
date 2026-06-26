// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/PlayerStatComponent.h"

UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStatComponent::AddScore(int32 Amount)
{
}
