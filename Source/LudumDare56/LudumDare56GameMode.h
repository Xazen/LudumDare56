// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LudumDare56GameMode.generated.h"

UCLASS(minimalapi)
class ALudumDare56GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALudumDare56GameMode();

	UFUNCTION(BlueprintCallable)
	void HostLANGame();

	UFUNCTION(BlueprintCallable)
	void JoinLANGame();
};



