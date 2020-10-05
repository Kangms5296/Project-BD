// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ANormalZombie* Zombie = Cast<ANormalZombie>(TryGetPawnOwner());
	if (Zombie && Zombie->IsValidLowLevel())
	{
		CurrentState = Zombie->CurrentState;
		Speed = Zombie->GetCharacterMovement()->Velocity.Size();
	}
}
