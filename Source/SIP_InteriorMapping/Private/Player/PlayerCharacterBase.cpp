// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayerControllerBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	bLeftShift = false;
	bLMBDown = false;
	bRMBDown = false;
	bInputModeUIOnly = false;
}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	MainPlayerController = Cast<APlayerControllerBase>(GetController());
	MainPlayerController->bEnableClickEvents = true;
	MainPlayerController->bEnableMouseOverEvents = true;
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move player forward if both mouse buttons pressed
	if (bLMBDown && bRMBDown)
	{
		DoubleMouseMovePlayer();
	}
}

// Called to bind functionality to input
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
		 
	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacterBase::MoveRight);

	/* Left and Right mouse buttons */
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &APlayerCharacterBase::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &APlayerCharacterBase::LMBUp);
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &APlayerCharacterBase::RMBDown);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &APlayerCharacterBase::RMBUp);

	PlayerInputComponent->BindAction("LeftShift", IE_Pressed, this, &APlayerCharacterBase::LeftShiftDown);
	PlayerInputComponent->BindAction("LeftShift", IE_Released, this, &APlayerCharacterBase::LeftShiftUp);

	PlayerInputComponent->BindAction("Alt", IE_Pressed, this, &APlayerCharacterBase::AltPressed);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacterBase::LookUpAtRate);
}

void APlayerCharacterBase::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
		//Player is trying to walk so Set input mode to gameonly
		bInputModeUIOnly = false;
		MainPlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void APlayerCharacterBase::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
		//Player is trying to walk so Set input mode to gameonly
		bInputModeUIOnly = false;
		MainPlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void APlayerCharacterBase::LMBDown()
{
	bLMBDown = true;
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0); // Get player pawn from the World Context Object and the Player index
	APlayerCharacterBase* MainCharacter = Cast<APlayerCharacterBase>(Pawn); // Cast Pawn to AMainCharater to get char ref	 
}

void APlayerCharacterBase::LMBUp()
{
	bLMBDown = false;
	//MainPlayerController->bShowMouseCursor = true;
}

void APlayerCharacterBase::DoubleMouseMovePlayer()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0); // Get player pawn from the World Context Object and the Player index
	APlayerCharacterBase* MainCharacter = Cast<APlayerCharacterBase>(Pawn); // Cast Pawn to AMainCharater to get char ref
	FRotator Rotator = MainCharacter->GetControlRotation();
	Rotator.Pitch = 0.f;
	Rotator.Roll = 0.f;
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Rotator);
	MainCharacter->AddMovementInput(ForwardVector);

	//Player is trying to walk so Set input mode to gameonly
	bInputModeUIOnly = false;
	MainPlayerController->SetInputMode(FInputModeGameOnly());
}

void APlayerCharacterBase::RMBDown()
{
	UE_LOG(LogTemp, Warning, TEXT("RMB Down"));
	bRMBDown = true;	 
}

void APlayerCharacterBase::RMBUp()
{
	UE_LOG(LogTemp, Warning, TEXT("RMB UP"));
	bRMBDown = false;
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0); // Get player pawn from the World Context Object and the Player index
	APlayerCharacterBase* MainCharacter = Cast<APlayerCharacterBase>(Pawn); // Cast Pawn to AMainCharater to get char ref	 
}

void APlayerCharacterBase::LeftShiftDown()
{	
	bLeftShift = true;
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0); // Get player pawn from the World Context Object and the Player index
	APlayerCharacterBase* MainCharacter = Cast<APlayerCharacterBase>(Pawn); // Cast Pawn to AMainCharater to get char ref	 

	// Sprint speed
	CharMovement = this->GetCharacterMovement();
	// Save original walk speed
	OriginalWalkSpeed = CharMovement->MaxWalkSpeed;
	// Set walk speed to sprint levels
	CharMovement->MaxWalkSpeed = 1500.f;
}

void APlayerCharacterBase::AltPressed()
{	 
	bInputModeUIOnly = !bInputModeUIOnly;
	if (bInputModeUIOnly == true)
	{
		MainPlayerController->SetInputMode(FInputModeGameAndUI());
		MainPlayerController->bShowMouseCursor = true;
	}
	else
	{
		MainPlayerController->SetInputMode(FInputModeGameOnly());
		MainPlayerController->bShowMouseCursor = false;
	}	
}

void APlayerCharacterBase::LeftShiftUp()
{
	bLeftShift = false;

	// Sprint speed
	CharMovement = this->GetCharacterMovement();
	// Restore original (Walking) char movement
	CharMovement->MaxWalkSpeed = OriginalWalkSpeed;
}

void APlayerCharacterBase::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacterBase::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}