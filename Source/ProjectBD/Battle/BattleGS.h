// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BattleGS.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABattleGS : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing = "OnRep_AlivePlayerCount")
	int AlivePlayerCount = 0;

	UFUNCTION()
	void OnRep_AlivePlayerCount();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	int TotalPlayerCount = 0;
};
