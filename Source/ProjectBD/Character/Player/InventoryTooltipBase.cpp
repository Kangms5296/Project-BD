// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTooltipBase.h"
#include "Components/TextBlock.h"

void UInventoryTooltipBase::NativeConstruct()
{
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	ItemDesc = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemDesc")));
	ItemEffect = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemEffect")));
}

void UInventoryTooltipBase::SetTooltipInfo(FString NewName, FString NewDesc, FString NewEffect)
{
	this->ItemName->SetText(FText::FromString(NewName));
	this->ItemDesc->SetText(FText::FromString(NewDesc));
	this->ItemEffect->SetText(FText::FromString(NewEffect));
}

void UInventoryTooltipBase::SetTooltipPos(FVector2D NewPos)
{
	SetPositionInViewport(NewPos);
}
