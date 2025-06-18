// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PlayerCharacter.h"
#include "Components//CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "Components/Input/PlayerInputComponent.h"
#include "LOP_GameplayTags.h"

APlayerCharacter::APlayerCharacter()
{
	// ĸ�� ������Ʈ ũ�� ���� (�ݸ��� ����)
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// ��Ʈ�ѷ� ȸ������ ���� ĳ���Ϳ� �ݿ����� ���� (ī�޶� ȸ���� ������ ���� ����)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// === ī�޶� ��(Spring Arm) ���� ===
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent()); // ��Ʈ ������Ʈ(ĸ��)�� ����
	CameraBoom->TargetArmLength = 200.f;             // ĳ���ͷκ����� �Ÿ� (3��Ī ī�޶� �Ÿ�)
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f); // ī�޶� ��ġ ������ (ĳ���� ������ ���� �ణ ġ��ħ)
	CameraBoom->bUsePawnControlRotation = true;      // ��Ʈ�ѷ��� ȸ���� SpringArm�� ���� (ī�޶� ȸ�� ����)

	// === ���� ī�޶� ���� ===
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // SpringArm ���� ����
	FollowCamera->bUsePawnControlRotation = false;   // ī�޶�� ��ü ȸ�� X (SpringArm�� ȸ���� ����)

	// === �̵� ���� ���� ===
	GetCharacterMovement()->bOrientRotationToMovement = true; // �̵� �������� �ڵ� ȸ��
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); // ȸ�� �ӵ�
	GetCharacterMovement()->MaxWalkSpeed = 400.f;             // �ȱ� �ӵ�
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; // ���� �� ���� ����
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// GAS ��Ʈ�ѷ� �Ҵ��̳� �ʱ�ȭ ���� �� ���⼭ ����
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UPlayerInputComponent* MyPlayerInputComponent = CastChecked<UPlayerInputComponent>(PlayerInputComponent);

	MyPlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, LOP_GameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Move);
	MyPlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, LOP_GameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Look);

	MyPlayerInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag InputTag)
{
}

void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag InputTag)
{
}
