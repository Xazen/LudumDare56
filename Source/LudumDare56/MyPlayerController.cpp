// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "LudumDare56GameMode.h"
#include "MultiplayerSessionSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyPawnClass = nullptr;
	bReplicates = true;
}

void AMyPlayerController::BeginPlay()
{
	ALudumDare56GameMode* GameMode = Cast<ALudumDare56GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		if (GetGameInstance()->GetSubsystem<UMultiplayerSessionSubsystem>()->IsHunter)
		{
			SetPlayerHunter();
		}
		else
		{
			SetPlayerPrey();
		}
	}

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

	ALudumDare56GameMode* GameMode = Cast<ALudumDare56GameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->RestartPlayer(this);
	}
}

// Replication
void AMyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerController, MyPawnClass);
}