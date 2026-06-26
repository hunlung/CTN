// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/DeliveryRuleComponent.h"

UDeliveryRuleComponent::UDeliveryRuleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDeliveryRuleComponent::EvaluateDelivery()
{
}

int32 UDeliveryRuleComponent::CalculateScore()
{
	return 0;
}
