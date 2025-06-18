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
	// 캡슐 컴포넌트 크기 설정 (콜리전 범위)
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// 컨트롤러 회전값을 직접 캐릭터에 반영하지 않음 (카메라 회전에 영향을 주지 않음)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// === 카메라 붐(Spring Arm) 설정 ===
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent()); // 루트 컴포넌트(캡슐)에 붙임
	CameraBoom->TargetArmLength = 200.f;             // 캐릭터로부터의 거리 (3인칭 카메라 거리)
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f); // 카메라 위치 오프셋 (캐릭터 오른쪽 위로 약간 치우침)
	CameraBoom->bUsePawnControlRotation = true;      // 컨트롤러의 회전을 SpringArm에 적용 (카메라 회전 가능)

	// === 실제 카메라 설정 ===
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // SpringArm 끝에 붙임
	FollowCamera->bUsePawnControlRotation = false;   // 카메라는 자체 회전 X (SpringArm의 회전만 따름)

	// === 이동 관련 설정 ===
	GetCharacterMovement()->bOrientRotationToMovement = true; // 이동 방향으로 자동 회전
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); // 회전 속도
	GetCharacterMovement()->MaxWalkSpeed = 400.f;             // 걷기 속도
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; // 멈출 때 감속 정도
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// GAS 컨트롤러 할당이나 초기화 같은 것 여기서 가능
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
