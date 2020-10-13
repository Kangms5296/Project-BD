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

	bool AddItem(FItemDataTable ItemData, int Count); // �κ��丮 �� Index�� �ش��ϴ� �������� Count ��ŭ �߰�. ����, ������ ���� �������̸� ���ο� Slot�� �������� �߰�.
	bool SubItem(FItemDataTable ItemData, int Count); // �κ��丮 �� Index�� �ش��ϴ� �������� Count ��ŭ ����. ����, ������ ���� �������̸� False ��ȯ.

	void ClearInventory();

	bool SetSlot(int Row, int Col, FItemDataTable ItemData, int Count); 
	void ResetSlot(int Row, int Col);

	bool GetEmptySlotIndex(int& EmptyRow, int& EmptyCol); // �κ��丮 �� �̻�� Slot Index ��ȯ

	int GetSlotIndex(int Row, int Col);

	void LoadDatasFromFile(FString SavedPath);
	void SaveDatasToFile(FString SavePath);
};