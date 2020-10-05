// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NormalZombie.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AZombieAIController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBlackboardComponent* BBComponent;


	void SetCurrentState(EZombieState NewState);

	void SetTarget(FVector NewLocation);

	void SetPlayer(AActor* Player);
};
