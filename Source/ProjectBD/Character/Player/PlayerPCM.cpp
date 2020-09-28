// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPCM.h"
#include "PlayerPawn.h"
#include "GameFramework/SpringArmComponent.h"

void APlayerPCM::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	APlayerPawn* Pawn = GetOwningPlayerController()->GetPawn<APlayerPawn>();

	if (Pawn)
	{
		if (Pawn->bHaveWeapon)
		{
			float TargetFOV = Pawn->bIsIronsight ? IronsightFOV : NormalFOV;
			float ResultFOV = FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, 20.0f);

			SetFOV(ResultFOV);
		}

		// Crouch
		FVector TargetPosition = Pawn->bIsCrouched ? Pawn->CrouchedSpringArmPosition : Pawn->NormalSpringArmPosition;
		FVector ResultSpringArmPosition = FMath::VInterpTo(Pawn->SpringArm->GetRelativeLocation(), TargetPosition, DeltaTime, 15.0f);
		Pawn->SpringArm->SetRelativeLocation(ResultSpringArmPosition);
	}
}