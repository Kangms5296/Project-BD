// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePC.h"
#include "UI/BattleWidgetBase.h"
#include "../Item/ItemTooltipBase.h"
#include "../Character/Player/PlayerPCM.h"
#include "../Character/Player/InventoryWidgetBase.h"
#include "../Character/Player/InventoryTooltipBase.h"
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

		if (InventoryWidgetObject == nullptr)
		{
			InventoryWidgetObject = CreateWidget<UInventoryWidgetBase>(this, InventoryWidgetClass);
			if (InventoryWidgetObject)
			{
				InventoryWidgetObject->AddToViewport();
				InventoryWidgetObject->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		InventoryTooltipWidgetObject = CreateWidget<UInventoryTooltipBase>(this, InventoryTooltipWidgetClass);
		if (InventoryTooltipWidgetObject)
		{
			InventoryTooltipWidgetObject->AddToViewport();
			InventoryTooltipWidgetObject->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

UInventoryWidgetBase * ABattlePC::GetInventory()
{
	if (InventoryWidgetObject == nullptr)
	{
		InventoryWidgetObject = CreateWidget<UInventoryWidgetBase>(this, InventoryWidgetClass);
		if (InventoryWidgetObject)
		{
			InventoryWidgetObject->AddToViewport();
			InventoryWidgetObject->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	return InventoryWidgetObject;
}

void ABattlePC::ToggleInventory()
{
	if (IsShowInventory)
	{
		IsShowInventory = false;

		if (InventoryWidgetObject)
		{
			InventoryWidgetObject->SetVisibility(ESlateVisibility::Collapsed);
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}
	else
	{
		IsShowInventory = true;

		if (InventoryWidgetObject)
		{
			int ViewportX;
			int ViewportY;
			GetViewportSize(ViewportX, ViewportY);
			SetMouseLocation(ViewportX * 0.5f, ViewportY * 0.5f);

			InventoryWidgetObject->SetVisibility(ESlateVisibility::Visible);
			bShowMouseCursor = true;
			SetInputMode(FInputModeGameAndUI());
			/*
			FInputModeGameAndUI inputMode;
			inputMode.SetHideCursorDuringCapture(false);
			SetInputMode(inputMode);
			*/
		}
	}
}

void ABattlePC::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ABattlePC::ToggleInventory);
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

void ABattlePC::ShowInventoryTooltip(FString ItemName, FString ItemDesc, FString ItemEffect)
{
	if (InventoryTooltipWidgetObject)
	{
		FVector2D MousePos;
		GetMousePosition(MousePos.X, MousePos.Y);

		InventoryTooltipWidgetObject->SetTooltipInfo(ItemName, ItemDesc, ItemEffect);
		InventoryTooltipWidgetObject->SetTooltipPos(MousePos);
		InventoryTooltipWidgetObject->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void ABattlePC::HideInventoryTooltip()
{
	if (InventoryTooltipWidgetObject)
	{
		InventoryTooltipWidgetObject->SetVisibility(ESlateVisibility::Collapsed);
	}
}