// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterItem.h"
#include "ItemComponent.h"
#include "../Character/Player/PlayerPawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StreamableManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMasterItem::AMasterItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("Item"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetSphereRadius(150.0f);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (ItemComponent->ItemDataTable)
	{
		if (HasAuthority())
		{
			ItemIndex = FMath::RandRange(1, 5) * 10;
		}

		ItemData = ItemComponent->GetItemData(ItemIndex);
		if (ItemIndex != 0)
		{
			FStreamableManager Loader;
			StaticMesh->SetStaticMesh(Loader.LoadSynchronous<UStaticMesh>(ItemData.ItemMesh));
		}

	}

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMasterItem::ProcessBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMasterItem::ProcessEndOverlap);
}

// Called every frame
void AMasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMasterItem, ItemIndex);
}

void AMasterItem::ProcessBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		APlayerPawn* Pawn = Cast<APlayerPawn>(OtherActor);
		if (Pawn && Pawn->IsLocallyControlled())
		{
			Pawn->AddNearItem(this);
		}
	}
}

void AMasterItem::ProcessEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		APlayerPawn* Pawn = Cast<APlayerPawn>(OtherActor);
		if (Pawn && Pawn->IsLocallyControlled())
		{
			Pawn->SubNearItem(this);
		}
	}
}

