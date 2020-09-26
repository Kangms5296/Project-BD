// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "../../Weapon/WeaponComponent.h"
#include "../../Battle/BattleGM.h"
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

	Tags.Add(TEXT("Player"));
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;
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

}

float APlayerPawn::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	return 0.0f;
}

void APlayerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
}

void APlayerPawn::MoveForward(float AxisValue)
{
}

void APlayerPawn::MoveRight(float AxisValue)
{
}

void APlayerPawn::LookUp(float AxisValue)
{
}

void APlayerPawn::Turn(float AxisValue)
{
}

void APlayerPawn::C2S_SetSprint_Implementation(bool State)
{
}

void APlayerPawn::Sprint()
{
}

void APlayerPawn::StopSprint()
{
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
}

void APlayerPawn::C2S_SetReload_Implementation(bool newState)
{
}

void APlayerPawn::Reload()
{
}

void APlayerPawn::OnRep_CurrentHP()
{
}

void APlayerPawn::C2S_SetLeftLean_Implementation(bool State)
{
}

void APlayerPawn::StartLeftLean()
{
}

void APlayerPawn::StopLeftLean()
{
}

void APlayerPawn::C2S_SetRightLean_Implementation(bool State)
{
}

void APlayerPawn::StartRightLean()
{
}

void APlayerPawn::StopRightLean()
{
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
}

void APlayerPawn::S2A_DeadAction_Implementation(int Number)
{
}
