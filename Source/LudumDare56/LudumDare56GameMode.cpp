// Copyright Epic Games, Inc. All Rights Reserved.

#include "LudumDare56GameMode.h"
#include "MyPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ALudumDare56GameMode::ALudumDare56GameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AMyPlayerController> MyPlayerController(TEXT("/Game/PlayerController/BP_PlayerController"));
	if (MyPlayerController.Class != NULL)
	{
		PlayerControllerClass = MyPlayerController.Class;
	}
}

UClass* ALudumDare56GameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(InController);
	if (PlayerController && PlayerController->GetPlayerPawnClass() != NULL)
	{
		return PlayerController->GetPlayerPawnClass();
	}

	return DefaultPawnClass;
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
