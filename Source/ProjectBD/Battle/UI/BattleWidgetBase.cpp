// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UBattleWidgetBase::NativeConstruct()
{
	AliveCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("AliveCount")));
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
}

void UBattleWidgetBase::SetAliveCount(int NewCount)
{
	if (AliveCount)
	{
		FString Temp = FString::FromInt(NewCount);
		AliveCount->SetText(FText::FromString(Temp));
	}
}

void UBattleWidgetBase::UpdateHPBar(float Percent)
{
	if (HPBar)
	{
		HPBar->SetPercent(Percent);
	}
}
