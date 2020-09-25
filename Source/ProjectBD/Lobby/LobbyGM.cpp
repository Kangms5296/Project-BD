// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"
#include "LobbyGS.h"

void ALobbyGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	CountConnectPlayer();
}

void ALobbyGM::Logout(AController* Exiting)
{
	CountConnectPlayer();

	Super::Logout(Exiting);
}

void ALobbyGM::CountConnectPlayer()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->ConnectCount = 0;
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			GS->ConnectCount++;
			GS->OnRep_ConnectCount(); //Host Only
		}
	}
}

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		LobbyTimer,
		this,
		&ALobbyGM::DecreaseTime,
		1.0f,
		true,
		1.0f
	);
}

void ALobbyGM::DecreaseTime()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->LeftTime--;
		GS->OnRep_LeftTime();

		if (GS->LeftTime < 0)
		{
			//게임 시작
			StartGame();
		}
	}
}

void ALobbyGM::StartGame()
{
	GetWorld()->ServerTravel(TEXT("Battle"));
}

