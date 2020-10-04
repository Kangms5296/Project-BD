// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidgetBase.h"
#include "../../Item/MasterItem.h"
#include "../../Battle/BattlePC.h"
#include "../../Character/Player/PlayerPawn.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UInventorySlotWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	I_ItemThumnail = Cast<UImage>(GetWidgetFromName(TEXT("I_ItemThumnail")));
	if (I_ItemThumnail)
	{
		I_ItemThumnail->SetVisibility(ESlateVisibility::Collapsed);
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

	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && IsUsing)
	{
		PC->ShowInventoryTooltip(CurrentItem.ItemName, CurrentItem.ItemDesc, CurrentItem.EffectDesc);
	}
}

void UInventorySlotWidgetBase::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && IsUsing)
	{
		PC->HideInventoryTooltip();
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
					SlotSub(1);
				}
			}

		}
	}
	// 좌클릭 드래그&드롭
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{

	}
		
	return Reply.NativeReply;
}

bool UInventorySlotWidgetBase::SlotSet(FItemDataTable ItemData, int NewCount)
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

bool UInventorySlotWidgetBase::SlotReset()
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

bool UInventorySlotWidgetBase::SlotAdd(int AddCount)
{
	ItemCount += AddCount;
	FString CountStr = FString::FromInt(ItemCount);
	T_ItemCount->SetText(FText::FromString(CountStr));

	return true;
}

bool UInventorySlotWidgetBase::SlotSub(int SubCount)
{
	ItemCount -= SubCount;
	if (ItemCount <= 0)
	{
		SlotReset();

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