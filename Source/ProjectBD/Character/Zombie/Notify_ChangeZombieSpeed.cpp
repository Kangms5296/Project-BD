// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify_ChangeZombieSpeed.h"
#include "NormalZombie.h"

void UNotify_ChangeZombieSpeed::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ANormalZombie* Zombie = Cast<ANormalZombie>(MeshComp->GetOwner());
	if (Zombie)
	{
		Zombie->SetSpeed(10);
	}
}

void UNotify_ChangeZombieSpeed::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ANormalZombie* Zombie = Cast<ANormalZombie>(MeshComp->GetOwner());
	if (Zombie)
	{
		Zombie->SetSpeed(150);
	}

}

