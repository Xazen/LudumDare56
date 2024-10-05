// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/Engine.h"

void PrintString(const FString& Str)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Str);
	}
}

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem()
{
	PrintString("Constructor");
}

void UMultiplayerSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		FString Subsystemname = OnlineSubsystem->GetSubsystemName().ToString();
		PrintString("OnlineSubsystem: " + Subsystemname);

		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionSubsystem::OnCreateSessionComplete);
		}
	}
}

void UMultiplayerSessionSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("Deinitialize"));
}

void UMultiplayerSessionSubsystem::CreateServer(FString ServerName)
{
	PrintString(ServerName);

	if (ServerName.IsEmpty())
	{
		PrintString("ServerName is empty");
		return;
	}

	FName MySessionName = FName("LD56 Session Name");

	FOnlineSessionSettings SessionSettings;

	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;

	bool IsLan = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		IsLan = true;
	}
	SessionSettings.bIsLANMatch = IsLan;

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UMultiplayerSessionSubsystem::FindServer(FString ServerName)
{
	PrintString(ServerName);
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool WasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessionComplete: %d"), WasSuccessful));

	if (WasSuccessful)
	{
		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
}