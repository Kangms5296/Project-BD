// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Item/ItemDataTable.h"
#include "InventoryWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	class UUniformGridPanel* ItemSlots;
	class UTextBlock* Gold;
	class UWidgetHeaderBase* InventoryHeader;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot")
	TSubclassOf<class UInventorySlotWidgetBase> InventorySlotWidgetClass;
	TArray <class UInventorySlotWidgetBase*> Slots;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot")
	int Rows;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot")
	int Cols;

	bool AddItem(FItemDataTable ItemData, int Count); // 인벤토리 내 Index에 해당하는 아이템을 Count 만큼 추가. 만약, 기존에 없는 아이템이면 새로운 Slot에 아이템을 추가.
	bool SubItem(FItemDataTable ItemData, int Count); // 인벤토리 내 Index에 해당하는 아이템을 Count 만큼 제거. 만약, 기존에 없는 아이템이면 False 반환.

	void ClearInventory();

	bool SetSlot(int Row, int Col, FItemDataTable ItemData, int Count); 
	void ResetSlot(int Row, int Col);

	bool GetEmptySlotIndex(int& EmptyRow, int& EmptyCol); // 인벤토리 내 미사용 Slot Index 반환

	int GetSlotIndex(int Row, int Col);

	void LoadDatasFromFile(FString SavedPath);
	void SaveDatasToFile(FString SavePath);
};