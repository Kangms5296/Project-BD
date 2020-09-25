// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "LobbyWidgetBase.h"

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		LobbyWidgetObject = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);
		if (LobbyWidgetObject)
		{
			LobbyWidgetObject->AddToViewport();
			if (HasAuthority()) // Only Host has a [Start Button]
			{
				LobbyWidgetObject->ShowStartGameButton();
			}
		}

		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}
}

//Server에서 실행
void ALobbyPC::C2S_SendMessage_Implementation(const FText& Message)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*Iter);
		if (PC)
		{
			//Server to Client
			PC->S2C_SendMessage(Message);
		}
	}
}

//Call Server, Execute Client
void ALobbyPC::S2C_SendMessage_Implementation(const FText& Message)
{
	if (LobbyWidgetObject)
	{
		LobbyWidgetObject->AddMessage(Message);
	}
}

