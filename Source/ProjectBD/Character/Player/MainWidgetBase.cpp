// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidgetBase.h"
#include "InventoryWidgetBase.h"
#include "InventorySlotWidgetBase.h"
#include "InventoryTooltipBase.h"
#include "../../Item/ItemTooltipBase.h"
#include "MainWidgetDD.h"

void UMainWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryWidgetObject == nullptr)
	{
		InventoryWidgetObject = Cast<UInventoryWidgetBase>(GetWidgetFromName(TEXT("InventoryWidget")));
		if (InventoryWidgetObject)
		{
			HideInventory();
		}
	}

	if (ItemTooltipWidgetObject == nullptr)
	{
		ItemTooltipWidgetObject = Cast<UItemTooltipBase>(GetWidgetFromName(TEXT("ItemTooltipWidget")));
		if (ItemTooltipWidgetObject)
		{
			HideItemTooltip();
		}
	}

	if (InventoryTooltipWidgetObject == nullptr)
	{
		InventoryTooltipWidgetObject = Cast<UInventoryTooltipBase>(GetWidgetFromName(TEXT("InventoryTooltipWidget")));
		if (InventoryTooltipWidgetObject)
		{
			HideInventoryTooltip();
		}
	}
}

bool UMainWidgetBase::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UMainWidgetDD* WidgetDD = Cast<UMainWidgetDD>(InOperation);
	if (WidgetDD == nullptr)
	{
		return false;
	}

	WidgetDD->WidgetToDrag->AddToViewport();
	FVector2D NewPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition()) - WidgetDD->MouseOffset;
	WidgetDD->WidgetToDrag->SetPositionInViewport(NewPosition, false);

	return true;
}

void UMainWidgetBase::ShowInventory()
{
	if (InventoryWidgetObject)
	{
		InventoryWidgetObject->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainWidgetBase::HideInventory()
{
	if (InventoryWidgetObject)
	{
		InventoryWidgetObject->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UItemTooltipBase * UMainWidgetBase::GetItemTooltip()
{
	if (ItemTooltipWidgetObject)
	{
		return ItemTooltipWidgetObject;
	}
	else
	{
		ItemTooltipWidgetObject = Cast<UItemTooltipBase>(GetWidgetFromName(TEXT("ItemTooltipWidget")));
		return ItemTooltipWidgetObject;
	}
}

void UMainWidgetBase::ShowItemTooltip(FString ItemName)
{
	if (ItemTooltipWidgetObject)
	{
		ItemTooltipWidgetObject->SetItemName(ItemName);
		ItemTooltipWidgetObject->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainWidgetBase::HideItemTooltip()
{
	if (ItemTooltipWidgetObject)
	{
		ItemTooltipWidgetObject->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UInventoryTooltipBase * UMainWidgetBase::GetInventoryTooltip()
{
	if (InventoryTooltipWidgetObject)
	{
		return InventoryTooltipWidgetObject;
	}
	else
	{
		InventoryTooltipWidgetObject = Cast<UInventoryTooltipBase>(GetWidgetFromName(TEXT("InventoryTooltipWidget")));
		return InventoryTooltipWidgetObject;
	}
}

void UMainWidgetBase::ShowInventoryTooltip(FString ItemName, FString ItemDesc, FString ItemEffect, FVector2D MousePos, FIntPoint ViewportSize)
{
	if (InventoryTooltipWidgetObject)
	{
		InventoryTooltipWidgetObject->SetTooltipInfo(ItemName, ItemDesc, ItemEffect);

		FVector2D OffsetPos;
		OffsetPos.X = MousePos.X / ViewportSize.X;
		OffsetPos.Y = MousePos.Y / ViewportSize.Y;
		InventoryTooltipWidgetObject->SetTooltipOffset(OffsetPos);

		InventoryTooltipWidgetObject->SetTooltipPos(MousePos);


		InventoryTooltipWidgetObject->AddToViewport();
	}
}

void UMainWidgetBase::HideInventoryTooltip()
{
	if (InventoryTooltipWidgetObject)
	{
		InventoryTooltipWidgetObject->RemoveFromParent();
	}
}