// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePC.h"
#include "UI/BattleWidgetBase.h"
#include "Components/InputComponent.h"

void ABattlePC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		BattleWidgetObject = CreateWidget<UBattleWidgetBase>(this, BattleWidgetClass);
		if (BattleWidgetObject)
		{
			BattleWidgetObject->AddToViewport();
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}
}