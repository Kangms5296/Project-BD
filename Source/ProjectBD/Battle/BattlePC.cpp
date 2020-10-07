// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePC.h"
#include "UI/BattleWidgetBase.h"
#include "../Item/ItemTooltipBase.h"
#include "../Character/Player/PlayerPCM.h"
#include "../Character/Player/MainWidgetBase.h"
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

		MainWidgetObject = CreateWidget<UMainWidgetBase>(this, MainWidgetClass);
		if (MainWidgetObject)
		{
			MainWidgetObject->AddToViewport();
		}
	}
}

void ABattlePC::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ABattlePC::ToggleInventory);
}

void ABattlePC::ToggleInventory()
{
	if (IsShowInventory)
	{
		IsShowInventory = false;

		if (MainWidgetObject)
		{
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
			MainWidgetObject->HideInventory();
		}
	}
	else
	{
		IsShowInventory = true;

		if (MainWidgetObject)
		{
			int ViewportX;
			int ViewportY;
			GetViewportSize(ViewportX, ViewportY);
			SetMouseLocation(ViewportX * 0.5f, ViewportY * 0.5f);

			bShowMouseCursor = true;
			SetInputMode(FInputModeGameAndUI());
			MainWidgetObject->ShowInventory();
		}
	}
}

void ABattlePC::ShowItemTooltip(FString ItemName)
{
	if (MainWidgetObject)
	{
		MainWidgetObject->ShowItemTooltip(ItemName);
	}
}

void ABattlePC::HideItemTooltip()
{
	if (MainWidgetObject)
	{
		MainWidgetObject->HideItemTooltip();
	}
}

void ABattlePC::ShowInventoryTooltip(FString ItemName, FString ItemDesc, FString ItemEffect)
{
	if (IsShowInventoryTooltip)
		return;

	if (MainWidgetObject)
	{
		FVector2D MousePos;
		GetMousePosition(MousePos.X, MousePos.Y);

		FIntPoint ViewportSize;
		GetViewportSize(ViewportSize.X, ViewportSize.Y);

		MainWidgetObject->ShowInventoryTooltip(ItemName, ItemDesc, ItemEffect, MousePos, ViewportSize);

		IsShowInventoryTooltip = true;
	}
}

void ABattlePC::HideInventoryTooltip()
{
	if (!IsShowInventoryTooltip)
		return;

	if (MainWidgetObject)
	{
		MainWidgetObject->HideInventoryTooltip();

		IsShowInventoryTooltip = false;
	}
}