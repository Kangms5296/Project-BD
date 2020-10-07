// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryTooltipBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UInventoryTooltipBase : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UTextBlock* ItemName;
	class UTextBlock* ItemDesc;
	class UTextBlock* ItemEffect;

public:
	virtual void NativeConstruct() override;

	void SetTooltipInfo(FString NewName, FString NewDesc, FString NewEffect);
	void SetTooltipPos(FVector2D NewPos);
	void SetTooltipOffset(FVector2D OffsetPos);
};
