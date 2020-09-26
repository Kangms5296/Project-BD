// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGS.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "BattlePC.h"
#include "UI/BattleWidgetBase.h"

void ABattleGS::OnRep_AlivePlayerCount()
{
	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->BattleWidgetObject)
	{
		PC->BattleWidgetObject->SetAliveCount(AlivePlayerCount);
	}
}

void ABattleGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Generate net replicate code.
	DOREPLIFETIME(ABattleGS, AlivePlayerCount);
	DOREPLIFETIME(ABattleGS, TotalPlayerCount);
}
