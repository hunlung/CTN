// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/ParcelGameMode.h"

AParcelGameMode::AParcelGameMode()
{
}

void AParcelGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AParcelGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void AParcelGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	// TODO: StartRound() 호출 예정
}

void AParcelGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	// TODO: EndRound() 호출 예정
}

void AParcelGameMode::StartRound()
{
}

void AParcelGameMode::EndRound()
{
}
