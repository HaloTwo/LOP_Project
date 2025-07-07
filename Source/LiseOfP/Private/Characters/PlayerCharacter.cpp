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
	// === ���� �ٵ� (�ִϸ��̼� ���) ===
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// GetMesh()�� �ٵ� ���̷�Ż �޽ÿ� �ִϸ��̼� BP ����

	// === �߰� ������ (���븸 ����) ===
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetMesh());
	HeadMesh->SetMasterPoseComponent(GetMesh());

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(GetMesh());
	HairMesh->SetMasterPoseComponent(GetMesh());

	ArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"));
	ArmsMesh->SetupAttachment(GetMesh());
	ArmsMesh->SetMasterPoseComponent(GetMesh());


	// ĸ�� ������Ʈ ũ�� ���� (�ݸ��� ����)
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ĳ���Ͱ� ������ �� ī�޶� ��������� ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	// ī�޶� ĳ������ ȸ���� ���󰡵��� ����
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// ĳ���� �̵� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent && AttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Both AbilitySystemComponent and AttributeSet are valid."));
	}
	else
	{
		if (!AbilitySystemComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT(" AbilitySystemComponent is NULL"));
		}
		if (!AttributeSet)
		{
			UE_LOG(LogTemp, Warning, TEXT(" AttributeSet is NULL"));
		}
	}


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

	// �⺻ �׼� ���ε�
	MyPlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, LOP_GameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Move);
	MyPlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, LOP_GameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Look);
	
	// ����/�ٱ� ��� �׼� ���ε�
	MyPlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, LOP_GameplayTags::InputTag_Walk, ETriggerEvent::Started, this, &APlayerCharacter::Input_Walk);

	// �ɷ� �Է� �׼� ���ε�
	MyPlayerInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
}

void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	// TEST ������ ���� �ӵ� ����
	const float MovementScale = bWalk ? 0.375f : 1.0f;

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y * MovementScale);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X * MovementScale);
	}
}

void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	// X��: �¿� ȸ�� (Yaw)
	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	// Y��: ���� ȸ�� (Pitch)
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Input_Walk()
{
	bWalk = !bWalk;
}

void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag InputTag)
{
}

void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag InputTag)
{
}
