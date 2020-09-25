// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ALobbyGM : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	FTimerHandle LobbyTimer;

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;

	void CountConnectPlayer();

	UFUNCTION()
	void DecreaseTime();

	void StartGame();
};
