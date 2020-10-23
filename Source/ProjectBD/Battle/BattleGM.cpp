// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGM.h"
#include "BattleGS.h"
#include "BattlePC.h"
#include "BattlePS.h"
#include "../Character/Player/PlayerPawn.h"
#include "../Item/ItemSpawnPoint.h"
#include "../Item/MasterItem.h"
#include "../BDGameInstance.h"
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

	// 모든 유저의 BeginPlay 함수가 호출된 뒤, 레벨 내 모든 아이템을 화면에 표시
	FTimerHandle WaitHandle;
	float WaitTime = 3.0f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		TArray<AActor*> ItemPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawnPoint::StaticClass(), ItemPoints);

		for (int i = 0; i < ItemPoints.Num(); ++i)
		{
			AMasterItem* NewItem = GetWorld()->SpawnActor<AMasterItem>(SpawnItemClass, ItemPoints[i]->GetActorTransform());
			NewItem->Init(Cast<UBDGameInstance>(GetGameInstance())->GetItemData(FMath::RandRange(1, 4)));
		}
	}), WaitTime, false);
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

		//죽은 플레이어 랭킹 지정
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