// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
	float Speed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
	float Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsFire : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsSprint : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsIronsight : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	uint64 bIsCrouched : 1;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
	float CurrentLeanAngle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
	float AimYaw;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
	float AimPitch;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
	float bIsReload;

	UFUNCTION()
	void AnimNotify_ReloadEnd(UAnimNotify* AnimNotify);
};
