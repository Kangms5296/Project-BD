// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePS.h"
#include "Net/UnrealNetwork.h"

void ABattlePS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABattlePS, PlayerRank);
}
