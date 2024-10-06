// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "GameFramework/GameModeBase.h"
#include "Net/UnrealNetwork.h"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HunterPawn = nullptr;
	PreyPawn = nullptr;

	bReplicates = true;
}

void AMyPlayerController::BeginPlay()
{
}


void AMyPlayerController::SetPlayerHunter()
{
	DeterminePawnClass(HunterPawn);
}

void AMyPlayerController::SetPlayerPrey()
{
	DeterminePawnClass(PreyPawn);
}

void AMyPlayerController::DeterminePawnClass_Implementation(TSubclassOf<APawn> InPawnClass)
{
	if (IsLocalController())
	{
		ServerSetPawn(InPawnClass);
	}
}

void AMyPlayerController::ServerSetPawn_Implementation(TSubclassOf<APawn> InPawnClass)
{
	MyPawnClass = InPawnClass;
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

// Replication
void AMyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerController, MyPawnClass);
}