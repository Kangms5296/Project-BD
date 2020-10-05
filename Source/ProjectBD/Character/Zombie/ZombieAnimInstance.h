// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NormalZombie.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	EZombieState CurrentState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float Speed;
};
