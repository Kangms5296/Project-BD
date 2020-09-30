// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"
#include "InventorySlotWidgetBase.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	// Slot 생성
	ItemSlots = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("ItemSlots")));
	if (ItemSlots)
	{
		for (int Row = 0; Row < Rows; ++Row)
		{
			for (int Col = 0; Col < Cols; ++Col)
			{
				// Slot 생성.
				UInventorySlotWidgetBase* CurSlot = CreateWidget<UInventorySlotWidgetBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0), InventorySlotWidgetClass);
				CurSlot->RowIndex = Row;
				CurSlot->ColIndex = Col;
				Slots.Add(CurSlot);

				// Slot 화면에 표시.
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

}

/*
void UInventoryWidgetBase::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (OutOperation == nullptr)
	{
		UMyDragDropOperation* oper = NewObject<UMyDragDropOperation>();
		OutOperation = oper;
		oper->DefaultDragVisual = this;
	}
}
FReply UInventoryWidgetBase::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}
	return reply.NativeReply;
}
*/

bool UInventoryWidgetBase::AddItem(int ItemIndex, int Count)
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->IsUsing && Slots[i]->ItemIndex == ItemIndex)
		{
			bool Result = Slots[i]->SlotAdd(Count);
			if (Result)
			{
				// 기존 슬롯에 성공적으로 아이템 수만큼 추가
				return true;
			}
		}
	}

	int EmptyRow;
	int EmptyCol;
	if (GetEmptySlotIndex(EmptyRow, EmptyCol))
	{
		bool Result = AddItemAtSlot(EmptyRow, EmptyCol, Count);
		if (Result)
		{
			// 새로운 슬롯에 성공적으로 아이템 수만큼 추가
			return true;
		}
	}

	// 기존 슬롯, 새 슬롯에 추가할 수 없음
	return false;
}

bool UInventoryWidgetBase::SubItem(int ItemIndex, int Count)
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->IsUsing && Slots[i]->ItemIndex == ItemIndex)
		{
			bool Result = Slots[i]->SlotSub(Count);
			if (Result)
			{
				// 기존 슬롯에 성공적으로 아이템 수만큼 제거
				return true;
			}
		}
	}

	// 기존에 아이템이 없는 경우
	return false;
}

bool UInventoryWidgetBase::AddItemAtSlot(int Row, int Col, int Count)
{
	bool Result = Slots[GetSlotIndex(Row, Col)]->SlotAdd(Count);

	return Result;
}

bool UInventoryWidgetBase::SubItemAtSlot(int Row, int Col, int Count)
{
	bool Result = Slots[GetSlotIndex(Row, Col)]->SlotSub(Count);

	return Result;
}

void UInventoryWidgetBase::ClearInventory()
{
	for (UInventorySlotWidgetBase* MySlot : Slots)
	{
		MySlot->SlotReset();
	}
}

bool UInventoryWidgetBase::SetSlot(int Row, int Col, int ItemIndex, int Count)
{
	bool Result = Slots[GetSlotIndex(Row, Col)]->SlotSet(ItemIndex, Count);

	return Result;
}

void UInventoryWidgetBase::ResetSlot(int Row, int Col)
{
	Slots[GetSlotIndex(Row, Col)]->SlotReset();
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
