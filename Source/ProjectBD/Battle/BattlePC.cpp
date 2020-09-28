// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePC.h"
#include "UI/BattleWidgetBase.h"
#include "../Item/ItemTooltipBase.h"
#include "../Character/Player/PlayerPCM.h"
#include "Components/InputComponent.h"

ABattlePC::ABattlePC()
{
	PlayerCameraManagerClass = APlayerPCM::StaticClass();
}

void ABattlePC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		BattleWidgetObject = CreateWidget<UBattleWidgetBase>(this, BattleWidgetClass);
		if (BattleWidgetObject)
		{
			BattleWidgetObject->AddToViewport();
		}

		ItemTooltipObject = CreateWidget<UItemTooltipBase>(this, ItemTooltipClass);
		if (BattleWidgetObject)
		{
			ItemTooltipObject->AddToViewport();
			ItemTooltipObject->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ABattlePC::ShowItemTooltip(FString ItemName)
{
	if (ItemTooltipObject)
	{
		ItemTooltipObject->SetItemName(ItemName);
		ItemTooltipObject->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABattlePC::HideItemTooltip()
{
	if (ItemTooltipObject)
	{
		ItemTooltipObject->SetVisibility(ESlateVisibility::Collapsed);
	}
}