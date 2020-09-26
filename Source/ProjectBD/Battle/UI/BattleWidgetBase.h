// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UBattleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock* AliveCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar* HPBar;

	UFUNCTION(BlueprintCallable)
	void SetAliveCount(int NewCount);

	UFUNCTION(BlueprintCallable)
	void UpdateHPBar(float Percent);
};
