// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UMainWidgetBase : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UInventoryWidgetBase* InventoryWidgetObject;

	UPROPERTY()
	class UItemTooltipBase* ItemTooltipWidgetObject;

	UPROPERTY()
	class UInventoryTooltipBase* InventoryTooltipWidgetObject;

public:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	class UInventoryWidgetBase* GetInventoryWidget();
	void ShowInventory();
	void HideInventory();

	class UItemTooltipBase* GetItemTooltip();
	void ShowItemTooltip(FString ItemName);
	void HideItemTooltip();

	class UInventoryTooltipBase* GetInventoryTooltip();
	void ShowInventoryTooltip(FString ItemName, FString ItemDesc, FString ItemEffect, FVector2D MousePos, FIntPoint ViewportSize);
	void HideInventoryTooltip();


};