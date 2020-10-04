// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Item/ItemDataTable.h"
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
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	class UImage* I_ItemThumnail;
	class UTextBlock* T_ItemCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot")
	int RowIndex;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot")
	int ColIndex;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	bool IsUsing;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	FItemDataTable CurrentItem;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	int ItemCount;

	bool SlotSet(FItemDataTable ItemData, int NewCount);
	bool SlotReset();
	bool SlotAdd(int AddCount);
	bool SlotSub(int SubCount);
};