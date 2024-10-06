// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE56_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	AMyPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	FORCEINLINE UClass* GetPlayerPawnClass()
	{
		return MyPawnClass;
	}

	void SetPlayerHunter();
	void SetPlayerPrey();

protected:

	UFUNCTION(Reliable, Client)
	void DeterminePawnClass(TSubclassOf<APawn> InPawnClass);
	virtual void DeterminePawnClass_Implementation(TSubclassOf<APawn> InPawnClass);

	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerSetPawn(TSubclassOf<APawn> InPawnClass);
	virtual void ServerSetPawn_Implementation(TSubclassOf<APawn> InPawnClass);
	virtual bool ServerSetPawn_Validate(TSubclassOf<APawn> InPawnClass) { return true; }

	/* The actual pawn class to use */
	UPROPERTY(Replicated)
	TSubclassOf<APawn> MyPawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="My Controller")
	TSubclassOf<APawn> HunterPawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="My Controller")
	TSubclassOf<APawn> PreyPawn;
};
