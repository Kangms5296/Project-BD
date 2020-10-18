// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerPawn.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Speed = Pawn->GetCharacterMovement()->Velocity.Size();
		Direction = CalculateDirection(Pawn->GetCharacterMovement()->Velocity, Pawn->GetActorRotation());

		bIsCrouched = Pawn->bIsCrouched;
		bIsSprint = Pawn->bIsSprint;
		bHaveWeapon = Pawn->bHaveWeapon;
		bIsFire = Pawn->bIsFire;
		bIsIronsight = Pawn->bIsIronsight;
		bIsInAir = Pawn->GetCharacterMovement()->IsFalling();

		// 본을 직접 회전시켜 캐릭터의 기울기를 구현.
		float TargetLeanAngle;
		if (Pawn->bLeftLean && !Pawn->bRightLean)
		{
			TargetLeanAngle = -30.0f;
		}
		else if (Pawn->bRightLean && !Pawn->bLeftLean)
		{
			TargetLeanAngle = 30.0f;
		}
		else
		{
			TargetLeanAngle = 0;
		}
		CurrentLeanAngle = FMath::FInterpTo(CurrentLeanAngle, TargetLeanAngle, DeltaSeconds, 8.0f);

		FRotator AimRotation = Pawn->GetAimOffset();
		AimYaw = AimRotation.Yaw;
		AimPitch = AimRotation.Pitch;

		bIsReload = Pawn->bIsReload;

		if (Pawn->ReloadMontage && Pawn->bIsReload)
		{
			if (!Montage_IsPlaying(Pawn->ReloadMontage))
			{
				Pawn->PlayAnimMontage(Pawn->ReloadMontage);
			}
		}
	}
}

void UPlayerAnimInstance::AnimNotify_ReloadEnd(UAnimNotify* AnimNotify)
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		bIsReload = false;
		Pawn->bIsReload = false;
		Pawn->C2S_SetReload(false);
	}
}
