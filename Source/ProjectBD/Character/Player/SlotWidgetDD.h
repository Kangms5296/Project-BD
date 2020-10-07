// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SlotWidgetDD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API USlotWidgetDD : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UUserWidget* FromSlot;
};
