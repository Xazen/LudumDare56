// Copyright Epic Games, Inc. All Rights Reserved.

#include "LudumDare56GameMode.h"
#include "LudumDare56Character.h"
#include "UObject/ConstructorHelpers.h"

ALudumDare56GameMode::ALudumDare56GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ALudumDare56GameMode::HostLANGame()
{
	GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonExampleMap?listen");
}

void ALudumDare56GameMode::JoinLANGame()
{
	APlayerController *PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel("192.168.178.171", TRAVEL_Absolute);
	}
}
