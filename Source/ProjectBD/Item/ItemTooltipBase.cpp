// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemTooltipBase.h"
#include "Components/TextBlock.h"

void UItemTooltipBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
}

void UItemTooltipBase::SetItemName(FString NewName)
{
	if (ItemName)
	{
		ItemName->SetText(FText::FromString(NewName));
	}
}

