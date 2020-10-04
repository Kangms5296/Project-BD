// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "WeaponComponent.h"
#include "InventoryWidgetBase.h"
#include "../../Battle/BattleGM.h"
#include "../../Battle/BattlePC.h"
#include "../../Battle/UI/BattleWidgetBase.h"
#include "../../Item/MasterItem.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameframeWork/CharacterMovementComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/StreamableManager.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->SocketOffset = FVector(0, 40.0f, 88.f);
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	NormalSpringArmPosition = SpringArm->GetRelativeLocation();
	CrouchedSpringArmPosition = NormalSpringArmPosition + FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 2);

	LeftLeanSpringArmPosition = FVector(0, -30, 0);
	RightLeanSpringArmPosition = FVector(0, 30, 0);

	Tags.Add(TEXT("Player"));
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC)
	{
		Inventory = Cast<ABattlePC>(GetController())->InventoryWidgetObject;
	}

	CurrentHP = 10;
	OnRep_CurrentHP();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerPawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerPawn::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerPawn::LookUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerPawn::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &APlayerPawn::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APlayerPawn::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APlayerPawn::StopSprint);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerPawn::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &APlayerPawn::StopFire);

	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Pressed, this, &APlayerPawn::StartIronsight);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Released, this, &APlayerPawn::StopIronsight);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerPawn::StartCrouch);
	
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &APlayerPawn::Reload);

	PlayerInputComponent->BindAction(TEXT("LeftLean"), IE_Pressed, this, &APlayerPawn::StartLeftLean);
	PlayerInputComponent->BindAction(TEXT("LeftLean"), IE_Released, this, &APlayerPawn::StopLeftLean);

	PlayerInputComponent->BindAction(TEXT("RightLean"), IE_Pressed, this, &APlayerPawn::StartRightLean);
	PlayerInputComponent->BindAction(TEXT("RightLean"), IE_Released, this, &APlayerPawn::StopRightLean);

	PlayerInputComponent->BindAction(TEXT("Pickup"), IE_Pressed, this, &APlayerPawn::Pickup);
}

void APlayerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerPawn, bIsSprint);
	DOREPLIFETIME(APlayerPawn, CurrentHP);
	DOREPLIFETIME(APlayerPawn, MaxHP);
	DOREPLIFETIME(APlayerPawn, bLeftLean);
	DOREPLIFETIME(APlayerPawn, bRightLean);
	//DOREPLIFETIME(APlayerPawn, bHaveWeapon);
	//DOREPLIFETIME(APlayerPawn, bIsFire);
	//DOREPLIFETIME(APlayerPawn, bIsReload);
	//DOREPLIFETIME(APlayerPawn, bIsIronsight);
}

float APlayerPawn::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0)
	{
		return 0.0f;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) //PointDamage 贸府
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
		}
		else
		{
			CurrentHP -= DamageAmount;
			CurrentHP = FMath::Clamp(CurrentHP, 0.0f, MaxHP);
		}
		OnRep_CurrentHP();

		if (CurrentHP <= 0)
		{
			// 荤噶 贸府
			S2A_DeadAction(FMath::RandRange(1, 3));

			ABattleGM* GM = Cast<ABattleGM>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GM)
			{
				GM->CountAlivePlayer();
			}
		}
		else
		{
			// 乔拜 贸府
			S2A_HitAction(FMath::RandRange(1, 4));
		}
	}

	return CurrentHP;
}

void APlayerPawn::MoveForward(float AxisValue)
{
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FRotator YawBaseRotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector CameraForward = UKismetMathLibrary::GetForwardVector(YawBaseRotation);
	AddMovementInput(CameraForward, AxisValue);
}

void APlayerPawn::MoveRight(float AxisValue)
{
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FRotator YawBaseRotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector CameraRight = UKismetMathLibrary::GetRightVector(YawBaseRotation);
	AddMovementInput(CameraRight, AxisValue);
}

void APlayerPawn::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void APlayerPawn::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void APlayerPawn::C2S_SetSprint_Implementation(bool State)
{
	bIsSprint = State;
	GetCharacterMovement()->MaxWalkSpeed = State ? SprintSpeed : WalkSpeed;
}

void APlayerPawn::Sprint()
{
	//Client
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	//Server
	C2S_SetSprint(true);
}

void APlayerPawn::StopSprint()
{
	//Client
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//Server
	C2S_SetSprint(false);
}

void APlayerPawn::C2S_SetWeapon_Implementation(bool State)
{

}

void APlayerPawn::HaveWeapon()
{
	bHaveWeapon = true;

	C2S_SetWeapon(true);
}

void APlayerPawn::DropWeapon()
{
	bHaveWeapon = false;

	C2S_SetWeapon(false);
}

void APlayerPawn::C2S_SetFire_Implementation(bool State)
{

}

void APlayerPawn::StartFire()
{

}

void APlayerPawn::StopFire()
{

}

void APlayerPawn::OnFire()
{

}

void APlayerPawn::C2S_SetIronsight_Implementation(bool State)
{

}

void APlayerPawn::StartIronsight()
{

}

void APlayerPawn::StopIronsight()
{

}

void APlayerPawn::StartCrouch()
{
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void APlayerPawn::C2S_SetReload_Implementation(bool newState)
{

}

void APlayerPawn::Reload()
{

}

void APlayerPawn::OnRep_CurrentHP()
{
	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC && PC->IsLocalController())
	{
		if (PC->BattleWidgetObject)
		{
			PC->BattleWidgetObject->UpdateHPBar(CurrentHP / MaxHP);
		}
	}
}

void APlayerPawn::C2S_SetLeftLean_Implementation(bool State)
{
	bLeftLean = State;
}

void APlayerPawn::StartLeftLean()
{
	bLeftLean = true;
	C2S_SetLeftLean(true);
}

void APlayerPawn::StopLeftLean()
{
	bLeftLean = false;
	C2S_SetLeftLean(false);
}

void APlayerPawn::C2S_SetRightLean_Implementation(bool State)
{
	bRightLean = State;
}

void APlayerPawn::StartRightLean()
{
	bRightLean = true;
	C2S_SetRightLean(true);
}

void APlayerPawn::StopRightLean()
{
	bRightLean = false;
	C2S_SetRightLean(false);
}

FRotator APlayerPawn::GetAimOffset() const
{
	return FRotator();
}

void APlayerPawn::C2S_ProcessFire_Implementation(FVector TraceStart, FVector TraceEnd)
{
}

void APlayerPawn::S2A_SpawnMuzzleFlashAndSound_Implementation()
{
}

void APlayerPawn::S2A_SpawnHitEffectAndDecal_Implementation(FHitResult OutHit)
{
}

void APlayerPawn::S2A_HitAction_Implementation(int Number)
{
	if (HitActionMontage)
	{
		FString SectionName = FString::Printf(TEXT("Hit%d"), Number);
		PlayAnimMontage(HitActionMontage, 1.0f, FName(SectionName));
	}
}

void APlayerPawn::S2A_DeadAction_Implementation(int Number)
{
	if (DeadMontage)
	{
		FString SectionName = FString::Printf(TEXT("Death_%d"), Number);
		PlayAnimMontage(DeadMontage, 1.0f, FName(SectionName));
	}

	DisableInput(Cast<APlayerController>(GetController()));
}

void APlayerPawn::AddNearItem(AMasterItem * AddItem)
{
	NearItemList.Add(AddItem);

	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC)
	{
		PC->ShowItemTooltip(AddItem->ItemData.ItemName);
	}
}

void APlayerPawn::SubNearItem(AMasterItem * SubItem)
{
	NearItemList.Remove(SubItem);

	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC)
	{
		if (NearItemList.Num() > 0)
		{
			PC->ShowItemTooltip(NearItemList[NearItemList.Num() - 1]->ItemData.ItemName);
		}
		else
		{
			PC->HideItemTooltip();
		}
	}
}

void APlayerPawn::Pickup()
{
	if (NearItemList.Num() > 0)
	{
		C2S_CheckPickupItem(NearItemList[NearItemList.Num() - 1]);
	}
}

void APlayerPawn::C2S_CheckPickupItem_Implementation(AMasterItem * NearItem)
{
	if (NearItem && !NearItem->IsPendingKill())
	{
		S2C_InsertItem(NearItem->ItemData);
		NearItem->Destroy();
	}
}

void APlayerPawn::S2C_InsertItem_Implementation(FItemDataTable ItemData)
{
	Inventory->AddItem(ItemData, ItemData.ItemCount);
}

void APlayerPawn::UseItem(FItemDataTable ItemData)
{
	switch (ItemData.ItemType)
	{
	case EItemType::Consume:
	{
		if (ItemData.EffectValue1 == "HP")
		{
			C2S_RescueHP(ItemData.EffectValue2);
		}
	}
	break;

	case EItemType::Equip:
	{
		switch (ItemData.EquipType)
		{
		case ESlotType::Weapon:
		{
			FStreamableManager Loader;
			USkeletalMesh* TempMesh = Loader.LoadSynchronous<USkeletalMesh>(ItemData.ItemSkeletalMesh);

			C2S_ArmWeapon(TempMesh);
		}
		break;
		case ESlotType::Head:
		{

		}
		break;
		case ESlotType::Body:
		{

		}
		break;
		}
	}
	break;
	}
}

void APlayerPawn::C2S_RescueHP_Implementation(int RescueValue)
{
	CurrentHP += RescueValue;
	CurrentHP = FMath::Clamp(CurrentHP, 0.0f, MaxHP);
	OnRep_CurrentHP();

	S2A_SpawnRescueEffect();
}

void APlayerPawn::S2A_SpawnRescueEffect_Implementation()
{
	if (RescueEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			RescueEffect,
			GetActorLocation()
			);
	};
}

void APlayerPawn::C2S_ArmWeapon_Implementation(USkeletalMesh* WeaponMesh)
{
	S2A_ArmWeapon(WeaponMesh);
}

void APlayerPawn::S2A_ArmWeapon_Implementation(USkeletalMesh* WeaponMesh)
{
	Weapon->SetSkeletalMesh(WeaponMesh);
}
