// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "MainWidgetDD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UMainWidgetDD : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UUserWidget* WidgetToDrag;
	FVector2D MouseOffset;
};
