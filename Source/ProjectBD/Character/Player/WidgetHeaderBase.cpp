// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetHeaderBase.h"
#include "MainWidgetDD.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UWidgetHeaderBase::NativeConstruct()
{
	Super::NativeConstruct();

	HeaderBackground = Cast<UBorder>(GetWidgetFromName(TEXT("HeaderBackground")));
	HeaderText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HeaderText")));
}

FReply UWidgetHeaderBase::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	return reply.NativeReply;
}


void UWidgetHeaderBase::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UMainWidgetDD* WidgetDD = Cast<UMainWidgetDD>(UWidgetBlueprintLibrary::CreateDragDropOperation(UMainWidgetDD::StaticClass()));
	if (WidgetDD == nullptr)
	{
		return;
	}
	WidgetDD->WidgetToDrag = OwnerWidget;
	WidgetDD->MouseOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	WidgetDD->DefaultDragVisual = OwnerWidget;
	WidgetDD->Pivot = EDragPivot::MouseDown;

	OutOperation = WidgetDD;

	OwnerWidget->RemoveFromParent();
}

void UWidgetHeaderBase::SetOwnerWidget(UUserWidget * NewOwnerWidget)
{
	OwnerWidget = NewOwnerWidget;
}
