// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"

#include "Engine/Engine.h"

class FString;

void PrintString(const FString& Str)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Str);
	}
}

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem()
{
	PrintString("Constructor");
}

void UMultiplayerSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	PrintString("Initialize");
}

void UMultiplayerSessionSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("Deinitialize"));
}