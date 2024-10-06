// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LudumDare56GameMode.generated.h"

UCLASS(minimalapi)
class ALudumDare56GameMode : public AGameModeBase
{
	GENERATED_BODY()
	ALudumDare56GameMode(const FObjectInitializer& ObjectInitializer);
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

public:

	UFUNCTION(BlueprintCallable)
	void HostLANGame();

	UFUNCTION(BlueprintCallable)
	void JoinLANGame();
};



