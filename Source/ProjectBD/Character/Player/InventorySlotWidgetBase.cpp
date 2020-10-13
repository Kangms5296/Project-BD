// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidgetBase.h"
#include "SlotWidgetDD.h"
#include "InventoryWidgetBase.h"
#include "../../Item/MasterItem.h"
#include "../../Battle/BattlePC.h"
#include "../../Character/Player/PlayerPawn.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventorySlotWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	I_ItemThumnail = Cast<UImage>(GetWidgetFromName(TEXT("I_ItemThumnail")));
	if (I_ItemThumnail)
	{
		I_ItemThumnail->SetVisibility(ESlateVisibility::Collapsed);

		if (DoChangeThumnail)
		{
			FStreamableManager Loader;
			I_ItemThumnail->SetBrushFromTexture(Loader.LoadSynchronous<UTexture2D>(CurrentItem.ItemThumnail));
			I_ItemThumnail->SetVisibility(ESlateVisibility::Visible);
		}
	}

	T_ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("T_ItemCount")));
	if (T_ItemCount)
	{
		T_ItemCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInventorySlotWidgetBase::NativeOnMouseEnter(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

}

FReply UInventorySlotWidgetBase::NativeOnMouseMove(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && IsUsing)
	{
		PC->ShowInventoryTooltip(CurrentItem.ItemName, CurrentItem.ItemDesc, CurrentItem.EffectDesc);
	}

	return Reply.NativeReply;
}

void UInventorySlotWidgetBase::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		// Nothinng To do
	}
	else
	{
		ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC && IsUsing)
		{
			PC->HideInventoryTooltip();
		}
	}
}

FReply UInventorySlotWidgetBase::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 우클릭 아이템 사용
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{	
		if (IsUsing)
		{
			ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (PC)
			{
				APlayerPawn* Pawn = Cast<APlayerPawn>(PC->GetPawn());
				if (Pawn)
				{
					Pawn->UseItem(CurrentItem);
					SubCount(1);

					// Ironsight 모드로의 진입을 막는다.
					Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton);
				}
			}

		}
	}
	// 좌클릭 드래그&드롭
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		if (IsUsing)
		{
			// Slot Swap
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
	}
		
	return Reply.NativeReply;
}

void UInventorySlotWidgetBase::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		USlotWidgetDD* SlotDD = Cast<USlotWidgetDD>(UWidgetBlueprintLibrary::CreateDragDropOperation(USlotWidgetDD::StaticClass()));
		if (SlotDD == nullptr)
		{
			return;
		}

		SlotDD->FromSlot = this;

		UInventorySlotWidgetBase* DragVisualSlot = CreateWidget<UInventorySlotWidgetBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0), GetClass());
		DragVisualSlot->DragSlotSet(CurrentItem);

		SlotDD->DefaultDragVisual = DragVisualSlot;
		SlotDD->Pivot = EDragPivot::CenterCenter;

		OutOperation = SlotDD;
	}
}

bool UInventorySlotWidgetBase::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USlotWidgetDD* SlotDD = Cast<USlotWidgetDD>(InOperation);
	if (SlotDD == nullptr)
	{
		return false;
	}

	UInventorySlotWidgetBase* FromSlot = Cast<UInventorySlotWidgetBase>(SlotDD->FromSlot);
	if (FromSlot)
	{
		if (IsUsing)
		{
			FItemDataTable TempData = CurrentItem;
			int TempCount = ItemCount;

			InventoryWidget->SetSlot(RowIndex, ColIndex, FromSlot->CurrentItem, FromSlot->ItemCount);
			InventoryWidget->SetSlot(FromSlot->RowIndex, FromSlot->ColIndex, TempData, TempCount);
		}
		else
		{
			InventoryWidget->SetSlot(RowIndex, ColIndex, FromSlot->CurrentItem, FromSlot->ItemCount);
			InventoryWidget->ResetSlot(FromSlot->RowIndex, FromSlot->ColIndex);
		}

		return true;
	}
	
	return false;
}

bool UInventorySlotWidgetBase::SetSlot(FItemDataTable ItemData, int NewCount)
{
	CurrentItem = ItemData;

	// Item Thumnail
	if (I_ItemThumnail)
	{
		FStreamableManager Loader;
		I_ItemThumnail->SetBrushFromTexture(Loader.LoadSynchronous<UTexture2D>(CurrentItem.ItemThumnail));
		I_ItemThumnail->SetVisibility(ESlateVisibility::Visible);
	}

		// Item Count Text
	if (T_ItemCount)
	{
		ItemCount = NewCount;
		FString CountStr = FString::FromInt(ItemCount);
		T_ItemCount->SetText(FText::FromString(CountStr));
		T_ItemCount->SetVisibility(ESlateVisibility::Visible);
	}

	IsUsing = true;

	return true;
}

bool UInventorySlotWidgetBase::ResetSlot()
{
	IsUsing = false;

	if (I_ItemThumnail)
	{
		I_ItemThumnail->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (T_ItemCount)
	{
		T_ItemCount->SetVisibility(ESlateVisibility::Collapsed);
	}

	return true;
}

bool UInventorySlotWidgetBase::AddCount(int AddCount)
{
	ItemCount += AddCount;
	FString CountStr = FString::FromInt(ItemCount);
	T_ItemCount->SetText(FText::FromString(CountStr));

	return true;
}

bool UInventorySlotWidgetBase::SubCount(int SubCount)
{
	ItemCount -= SubCount;
	if (ItemCount <= 0)
	{
		ResetSlot();

		ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC)
		{
			PC->HideInventoryTooltip();
		}
	}
	else
	{
		FString CountStr = FString::FromInt(ItemCount);
		T_ItemCount->SetText(FText::FromString(CountStr));
	}

	return true;
}

void UInventorySlotWidgetBase::SetOwner(UInventoryWidgetBase * NewInventoryWidget)
{
	InventoryWidget = NewInventoryWidget;
}

void UInventorySlotWidgetBase::DragSlotSet(FItemDataTable ItemData)
{
	if (I_ItemThumnail)
	{
		FStreamableManager Loader;
		I_ItemThumnail->SetBrushFromTexture(Loader.LoadSynchronous<UTexture2D>(ItemData.ItemThumnail));
		I_ItemThumnail->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CurrentItem = ItemData;
		DoChangeThumnail = true;
	}
}
