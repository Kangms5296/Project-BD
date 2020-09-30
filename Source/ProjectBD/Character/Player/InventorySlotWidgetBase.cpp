// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidgetBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventorySlotWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	I_Background = Cast<UImage>(GetWidgetFromName(TEXT("Background")));
	I_ItemThumnail = Cast<UImage>(GetWidgetFromName(TEXT("ItemThumnail")));
	if (I_ItemThumnail)
	{
		I_ItemThumnail->SetVisibility(ESlateVisibility::Collapsed);
	}
	T_ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
}

bool UInventorySlotWidgetBase::SlotSet(int NewIndex, int NewCount)
{
	IsUsing = true;

	// Item Thumnail
	ItemIndex = NewIndex;


	// Item Count Text
	ItemCount = NewCount;
	FString CountStr = FString::FromInt(ItemCount);
	T_ItemCount->SetText(FText::FromString(CountStr));

	return true;
}

bool UInventorySlotWidgetBase::SlotReset()
{
	IsUsing = false;

	ItemIndex = -1;

	ItemCount = 0;

	return true;
}

bool UInventorySlotWidgetBase::SlotAdd(int AddCount)
{
	ItemCount += AddCount;

	return true;
}

bool UInventorySlotWidgetBase::SlotSub(int SubCount)
{
	ItemCount -= SubCount;

	return true;
}