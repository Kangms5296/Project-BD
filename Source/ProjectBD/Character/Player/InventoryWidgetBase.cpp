// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"
#include "InventorySlotWidgetBase.h"
#include "WidgetHeaderBase.h"
#include "../../Item/MasterItem.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	// Slot ����
	ItemSlots = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("ItemSlots")));
	if (ItemSlots)
	{
		for (int Row = 0; Row < Rows; ++Row)
		{
			for (int Col = 0; Col < Cols; ++Col)
			{
				// Slot ����.
				UInventorySlotWidgetBase* CurSlot = CreateWidget<UInventorySlotWidgetBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0), InventorySlotWidgetClass);
				CurSlot->RowIndex = Row;
				CurSlot->ColIndex = Col;
				CurSlot->SetOwner(this);
				Slots.Add(CurSlot);

				// Slot ȭ�鿡 ǥ��.
				UUniformGridSlot* GridSlot = ItemSlots->AddChildToUniformGrid(CurSlot, Row, Col);
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
		}
	}

	Gold = Cast<UTextBlock>(GetWidgetFromName(TEXT("Gold")));
	if (Gold)
	{
		FString Temp = "0";
		Gold->SetText(FText::FromString(Temp));
	}

	InventoryHeader = Cast<UWidgetHeaderBase>(GetWidgetFromName(TEXT("InventoryHeader")));
	{
		InventoryHeader->SetOwnerWidget(this);
	}
}

bool UInventoryWidgetBase::AddItem(FItemDataTable ItemData, int Count)
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->IsUsing && Slots[i]->CurrentItem.ItemIndex == ItemData.ItemIndex)
		{
			bool Result = Slots[i]->AddCount(Count);
			if (Result)
			{
				// ���� ���Կ� ���������� ������ ����ŭ �߰�
				return true;
			}
		}
	}

	int EmptyRow;
	int EmptyCol;
	if (GetEmptySlotIndex(EmptyRow, EmptyCol))
	{
		bool Result = SetSlot(EmptyRow, EmptyCol, ItemData, Count);
		if (Result)
		{
			// ���ο� ���Կ� ���������� ������ ����ŭ �߰�
			return true;
		}
	}

	// ���� ����, �� ���Կ� �߰��� �� ����
	return false;
}

bool UInventoryWidgetBase::SubItem(FItemDataTable ItemData, int Count)
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->IsUsing && Slots[i]->CurrentItem.ItemIndex == ItemData.ItemIndex)
		{
			bool Result = Slots[i]->SubCount(Count);
			if (Result)
			{
				// ���� ���Կ� ���������� ������ ����ŭ ����
				return true;
			}
		}
	}

	// ������ �������� ���� ���
	return false;
}

void UInventoryWidgetBase::ClearInventory()
{
	for (UInventorySlotWidgetBase* MySlot : Slots)
	{
		MySlot->ResetSlot();
	}
}

bool UInventoryWidgetBase::SetSlot(int Row, int Col, FItemDataTable ItemData, int Count)
{
	bool Result = Slots[GetSlotIndex(Row, Col)]->SetSlot(ItemData, Count);

	return Result;
}

void UInventoryWidgetBase::ResetSlot(int Row, int Col)
{
	Slots[GetSlotIndex(Row, Col)]->ResetSlot();
}

bool UInventoryWidgetBase::GetEmptySlotIndex(int& EmptyRow, int& EmptyCol)
{
	for (UInventorySlotWidgetBase* MySlot : Slots)
	{
		if (false == MySlot->IsUsing)
		{
			EmptyRow = MySlot->RowIndex;
			EmptyCol = MySlot->ColIndex;
			return true;
		}
	}

	return false;
}

int UInventoryWidgetBase::GetSlotIndex(int Row, int Col)
{
	return Row * Cols + Col;
}
