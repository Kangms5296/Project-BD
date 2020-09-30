// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UInventorySlotWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	class UImage* I_Background;
	class UImage* I_ItemThumnail;
	class UTextBlock* T_ItemCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot")
	int RowIndex;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot")
	int ColIndex;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	bool IsUsing;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	int ItemIndex;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	int ItemCount;

	bool SlotSet(int NewIndex, int NewCount);
	bool SlotReset();
	bool SlotAdd(int AddCount);
	bool SlotSub(int SubCount);
};