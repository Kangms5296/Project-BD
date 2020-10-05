// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_LookTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

void UBTService_LookTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));

	APawn* Zombie = OwnerComp.GetAIOwner()->GetPawn();
	if (Player && Zombie)
	{
		//FVector Dir = Player->GetActorLocation() - Zombie->GetActorLocation();
		FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(
			Zombie->GetActorLocation(),
			Player->GetActorLocation()
		);

		FRotator TargetLook = FMath::RInterpTo(Zombie->GetActorRotation(),
			LookRot,
			DeltaSeconds,
			15.0f);

		Zombie->SetActorRotation(TargetLook);
	}

}