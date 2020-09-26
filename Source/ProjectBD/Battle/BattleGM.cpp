// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGM.h"
#include "BattleGS.h"
#include "BattlePC.h"
#include "BattlePS.h"
#include "../Character/Player/PlayerPawn.h"
#include "Kismet/GameplayStatics.h"

void ABattleGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ABattleGS* GS = GetGameState<ABattleGS>();
	if (GS)
	{
		GS->TotalPlayerCount = GetNumPlayers();
	}

	CountAlivePlayer();
}

void ABattleGM::Logout(AController* Exiting)
{
	CountAlivePlayer();

	Super::Logout(Exiting);
}

void ABattleGM::BeginPlay()
{
	Super::BeginPlay();

}

void ABattleGM::CountAlivePlayer()
{
	ABattleGS* GS = GetGameState<ABattleGS>();
	if (GS)
	{
		//STL Iterator, 
		//for(FConstPlayerControllerIterator Iter = GetWorld()->GetPlayerControllerIterator() )
		GS->AlivePlayerCount = 0;
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			ABattlePC* PC = Cast<ABattlePC>(*Iter);
			if (PC)
			{
				APlayerPawn* Pawn = Cast<APlayerPawn>(PC->GetPawn());
				if (Pawn)
				{
					if (Pawn->CurrentHP > 0)
					{
						GS->AlivePlayerCount++;
					}
				}
			}
		}

		//Á×Àº ÇÃ·¹ÀÌ¾î ·©Å· ÁöÁ¤
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			ABattlePC* PC = Cast<ABattlePC>(*Iter);
			if (PC)
			{
				APlayerPawn* Pawn = Cast<APlayerPawn>(PC->GetPawn());
				if (Pawn)
				{
					if (Pawn->CurrentHP <= 0)
					{
						ABattlePS* PS = Pawn->GetPlayerState<ABattlePS>();
						if (PS)
						{
							if (PS->PlayerRank == -1)
							{
								PS->PlayerRank = GS->AlivePlayerCount + 1;
							}
						}
					}
				}
			}
		}

		if (GS->AlivePlayerCount == 1 && GetNumPlayers() > 1)
		{
			for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
			{
				ABattlePC* PC = Cast<ABattlePC>(*Iter);
				if (PC)
				{
					APlayerPawn* Pawn = Cast<APlayerPawn>(PC->GetPawn());
					if (Pawn)
					{
						if (Pawn->CurrentHP > 0)
						{
							ABattlePS* PS = Pawn->GetPlayerState<ABattlePS>();
							PS->PlayerRank = 1;
						}
					}
				}
			}

			//GameOver
			//Network Result UI Show
			//Lobby Timer
			FTimerHandle EndingTimer;
			GetWorldTimerManager().SetTimer(
				EndingTimer,
				this,
				&ABattleGM::GoLobby,
				3.0f
			);
		}

		//Host only, UI Update
		GS->OnRep_AlivePlayerCount();
	}
}

void ABattleGM::GoLobby()
{
	GetWorld()->ServerTravel(TEXT("Lobby"));
}