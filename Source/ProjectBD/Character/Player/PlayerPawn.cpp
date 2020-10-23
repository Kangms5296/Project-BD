// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "WeaponComponent.h"
#include "InventoryWidgetBase.h"
#include "MainWidgetBase.h"
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
	SpringArm->TargetArmLength = 150.0f;
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

	CurrentHP = MaxHP;
	OnRep_CurrentHP();

	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC && PC->IsLocalController())
	{
		Inventory = PC->GetMainWidgetObject()->InventoryWidgetObject;

		FString LoadPath = FPaths::ProjectContentDir() + "Data/Inventory/InventoryData.txt";
		UE_LOG(LogTemp, Warning, TEXT("%s"), *LoadPath);
		Inventory->LoadDatasFromFile(LoadPath);
	}
}

void APlayerPawn::UnPossessed()
{
	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC && PC->IsLocalController())
	{
		Inventory = PC->GetMainWidgetObject()->InventoryWidgetObject;
	}

	Super::UnPossessed();
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
	DOREPLIFETIME(APlayerPawn, bHaveWeapon);
	DOREPLIFETIME(APlayerPawn, bIsFire);
	//DOREPLIFETIME(APlayerPawn, bIsReload);
	DOREPLIFETIME(APlayerPawn, bIsIronsight);
}

float APlayerPawn::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0)
	{
		return 0.0f;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) //PointDamage 처리
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
			// 사망 처리
			S2A_DeadAction(FMath::RandRange(1, 3));

			ABattleGM* GM = Cast<ABattleGM>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GM)
			{
				GM->CountAlivePlayer();
			}
		}
		else
		{
			// 피격 처리
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
	bHaveWeapon = true;
}

void APlayerPawn::HaveWeapon()
{
	bHaveWeapon = true;

	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC)
	{
		PC->BattleWidgetObject->ShowCrosshair();
	}

	C2S_SetWeapon(true);
}

void APlayerPawn::DropWeapon()
{
	bHaveWeapon = false;

	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC)
	{
		PC->BattleWidgetObject->HideCrosshair();
	}

	C2S_SetWeapon(false);
}

void APlayerPawn::C2S_SetFire_Implementation(bool State)
{
	bIsFire = State;
}

void APlayerPawn::StartFire()
{
	if (!bHaveWeapon || GetCharacterMovement()->IsFalling())
		return;

	bIsFire = true;
	C2S_SetFire(true);

	OnFire();
}

void APlayerPawn::StopFire()
{
	bIsFire = false;
	C2S_SetFire(false);
}

void APlayerPawn::OnFire()
{
	if (!bIsFire)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		int32 ScreenSizeX;
		int32 ScreenSizeY;
		FVector CrosshairWorldPosition; //3D
		FVector CrosshairWorldDirection; //3D 

		FVector CameraLocation;
		FRotator CameraRotation;

		//사람 반동
		int RandX = FMath::RandRange(-20, 20);
		int RandY = FMath::RandRange(3, 20);

		PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
		PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2 + RandX, ScreenSizeY / 2 + RandY, CrosshairWorldPosition, CrosshairWorldDirection);

		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		//총구 들리기(Tick에서 총 쏜 후에 애니메이션)
		FRotator PlayerRotation = GetControlRotation();
		PlayerRotation.Pitch += FMath::FRandRange(0.2f, 1.0f);
		GetController()->SetControlRotation(PlayerRotation);

		FVector TraceStart = CameraLocation;
		FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 99999.f);

		C2S_ProcessFire(TraceStart, TraceEnd);
	}

	GetWorldTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&APlayerPawn::OnFire,
		0.12f,
		false
	);
}

void APlayerPawn::C2S_SetIronsight_Implementation(bool State)
{
	bIsIronsight = State;
}

void APlayerPawn::StartIronsight()
{
	if (GetCharacterMovement()->IsFalling())
		return;

	bIsIronsight = true;
	C2S_SetIronsight(true);
}

void APlayerPawn::StopIronsight()
{
	bIsIronsight = false;
	C2S_SetIronsight(false);
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
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

void APlayerPawn::C2S_ProcessFire_Implementation(FVector TraceStart, FVector TraceEnd)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

	Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(this);

	FHitResult OutHit;

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		TraceStart,
		TraceEnd,
		Objects,
		true,
		ActorToIgnore,
		EDrawDebugTrace::None,
		OutHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.0f
	);

	if (Result)
	{
		//all client spawn Hiteffect and Decal
		S2A_SpawnHitEffectAndDecal(OutHit);

		//Point Damage
		UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), //맞은놈
			1.0f,					//데미지
			-OutHit.ImpactNormal,	//데미지 방향
			OutHit,					//데미지 충돌 정보
			GetController(),		//때린 플레이어
			this,					//때린놈
			nullptr
		);

		MakeNoise(1.0f, this, OutHit.ImpactPoint);
	}

	//All Client Spawn Muzzleflash and Sound
	S2A_SpawnMuzzleFlashAndSound();
}

void APlayerPawn::S2A_SpawnMuzzleFlashAndSound_Implementation()
{
	//WeaponSound and MuzzleFlash
	if (WeaponSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
			WeaponSound,
			Weapon->GetComponentLocation()
		);
	}

	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(
			MuzzleFlash,
			Weapon,
			TEXT("Muzzle"));
	}
}

void APlayerPawn::S2A_SpawnHitEffectAndDecal_Implementation(FHitResult OutHit)
{
	//HitEffect(Blood) and Decal
	if (Cast<ACharacter>(OutHit.GetActor()))
	{
		//캐릭터
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			BloodHitEffect,
			OutHit.ImpactPoint + (OutHit.ImpactNormal * 10)
		);
	}
	else
	{
		//지형
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			HitEffect,
			OutHit.ImpactPoint + (OutHit.ImpactNormal * 10)
		);

		UDecalComponent* NewDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
			NormalDecal,
			FVector(5, 5, 5),
			OutHit.ImpactPoint,
			OutHit.ImpactNormal.Rotation(),
			10.0f
		);

		NewDecal->SetFadeScreenSize(0.005f);

	}
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
	Inventory->AddItem(ItemData, 1);
}

void APlayerPawn::UseItem(FItemDataTable ItemData)
{
	switch (ItemData.ItemType)
	{
	case EItemType::Consume:
	{
		C2S_RescueHP(ItemData.fValue1);
	}
	break;

	case EItemType::Equip:
	{
		switch (ItemData.EquipType)
		{
		case ESlotType::Weapon:
		{
			HaveWeapon();

			FStreamableManager Loader;
			USkeletalMesh* TempMesh = Loader.LoadSynchronous<USkeletalMesh>(ItemData.ItemSkeletalMesh);
			Weapon->SetSkeletalMesh(TempMesh);

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
		UGameplayStatics::SpawnEmitterAttached(
			RescueEffect,
			GetMesh(),
			TEXT("ParticleSocket"));
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
