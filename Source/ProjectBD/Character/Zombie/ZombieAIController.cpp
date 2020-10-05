// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AZombieAIController::AZombieAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
}

void AZombieAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//AIController가 Zombie를 잡으면 그때 BT정보를 초기화 해서 사용
	ANormalZombie* Zombie = Cast<ANormalZombie>(InPawn);
	if (Zombie)
	{
		if (Zombie->ZombieBT && Zombie->ZombieBT->BlackboardAsset)
		{
			BBComponent->InitializeBlackboard(*(Zombie->ZombieBT->BlackboardAsset));
			BTComponent->StartTree(*(Zombie->ZombieBT));
		}
	}
}

void AZombieAIController::OnUnPossess()
{
	UE_LOG(LogClass, Warning, TEXT("OnUnPossess"));
	BTComponent->StopTree();

	Super::OnUnPossess();
}

void AZombieAIController::SetCurrentState(EZombieState NewState)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsEnum(TEXT("CurrentState"), (uint8)NewState);
	}
}

void AZombieAIController::SetTarget(FVector NewLocation)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsVector(TEXT("Target"), NewLocation);
	}
}

void AZombieAIController::SetPlayer(AActor* Player)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsObject(TEXT("Player"), Player);
	}
}
