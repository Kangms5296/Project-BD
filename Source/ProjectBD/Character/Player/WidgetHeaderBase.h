// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetHeaderBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UWidgetHeaderBase : public UUserWidget
{
	GENERATED_BODY()

private:
	class UUserWidget* OwnerWidget;

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	void SetOwnerWidget(UUserWidget* NewOwnerWidget);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UBorder* HeaderBackground;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UTextBlock* HeaderText;
};