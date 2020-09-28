// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemTooltipBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UItemTooltipBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock* ItemName;

	UFUNCTION(BlueprintCallable)
	void SetItemName(FString NewName);
};
