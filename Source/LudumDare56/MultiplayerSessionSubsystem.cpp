// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"

#include "MyPlayerController.h"
#include "OnlineSubsystem.h"

#include "Engine/Engine.h"
#include "Online/OnlineSessionNames.h"

bool IsLan()
{
	return IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
}

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
	CreateSessionAfterDestroy = false;
	DestroyServerName = "";
	ServerNameToFind = "";
	MySessionName = FName("LD56 Session Name");
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
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionSubsystem::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionSubsystem::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionSubsystem::OnJoinSessionComplete);
			SessionInterface->OnSessionParticipantJoinedDelegates.AddUObject(this, &UMultiplayerSessionSubsystem::OnSessionParticipantJoined);
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

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);
	if (ExistingSession)
	{

		FString msg = FString::Printf(TEXT("Session %s already exists"), *MySessionName.ToString());
		PrintString(msg);
		CreateSessionAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}

	FOnlineSessionSettings SessionSettings;

	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bIsLANMatch = IsLan();
	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UMultiplayerSessionSubsystem::FindServer(FString ServerName)
{
	PrintString(ServerName);

	if (ServerName.IsEmpty())
	{
		PrintString("ServerName is empty");
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = IsLan();
	SessionSearch->MaxSearchResults = 999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	ServerNameToFind = ServerName;

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool WasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessionComplete: %d"), WasSuccessful));

	if (WasSuccessful)
	{
		FString Path = "/Game/ThirdPerson/Maps/Blockout?listen";
		if (!GameMapPath.IsEmpty())
		{
			Path = FString::Printf(TEXT("%s?listen"), *GameMapPath);
		}

		IsHunter = true;
		GetWorld()->ServerTravel(Path);
	}
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool WasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnDestroySessionComplete: %d"), WasSuccessful));
	if (CreateSessionAfterDestroy)
	{
		CreateSessionAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void UMultiplayerSessionSubsystem::OnFindSessionsComplete(bool WasSuccessful)
{
	if (!WasSuccessful || !SessionSearch.IsValid()) return;
	if (ServerNameToFind.IsEmpty()) return;

	TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
	if (SearchResults.Num() <= 0)
	{
		PrintString("No sessions found");
		return;
	}

	FString Msg = FString::Printf(TEXT("Found %d sessions"), SearchResults.Num());

	FString FoundServerName = "not-found";
	FOnlineSessionSearchResult* FoundServer = 0;
	PrintString(Msg);

	for (FOnlineSessionSearchResult SearchResult : SearchResults)
	{
		if (SearchResult.Session.SessionSettings.Get(FName("SERVER_NAME"), FoundServerName))
		{
			FString Msg2 = FString::Printf(TEXT("Found server: %s"), *FoundServerName);
			PrintString(Msg);

			if (ServerNameToFind == FoundServerName)
			{
				FString FoundSearchingServerMsg = FString::Printf(TEXT("Found server: %s"), *FoundServerName);
				FoundServer = &SearchResult;
				PrintString(FoundSearchingServerMsg);
				break;
			}
		}
	}

	if (FoundServer)
	{
		PrintString("Joining server");
		SessionInterface->JoinSession(0, MySessionName, *FoundServer);
	}
	else
	{
		PrintString("Could not find server");
		ServerNameToFind = "";
	}

}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	PrintString(FString::Printf(TEXT("OnJoinSessionComplete: %d"), Result));
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		FString Address = "";
		bool Success = SessionInterface->GetResolvedConnectString(MySessionName, Address);
		if (Success)
		{
			PrintString(FString::Printf(TEXT("Found resolved address: %s"), *Address));
			APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
			if (PC)
			{
				PrintString(FString::Printf(TEXT("Will join session")));
				IsHunter = false;
				PC->ClientTravel(Address, TRAVEL_Absolute);
			}
		}
		else
		{
			PrintString("Failed to get resolved connect string");
		}
	}
	else
	{
		PrintString("Failed to join session");
	}
}

void UMultiplayerSessionSubsystem::OnSessionParticipantJoined(FName SessionName, const FUniqueNetId& ParticipantId)
{
	PrintString(FString::Printf(TEXT("OnSessionParticipantJoined: %s"), *ParticipantId.ToString()));
}