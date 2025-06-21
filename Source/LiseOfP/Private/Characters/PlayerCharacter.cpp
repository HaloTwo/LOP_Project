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

	// ��Ʈ�ѷ� ȸ������ ���� ĳ���Ϳ� �ݿ����� ���� (ī�޶� ȸ���� ������ ���� ����)
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// SpringArm ���� (���� �˵���)
	CameraBoom->SetupAttachment(GetRootComponent());  // ĳ���� �߽ɿ� ���̱�
	CameraBoom->TargetArmLength = 300.f;              // �Ÿ�
	CameraBoom->bUsePawnControlRotation = false;      // ��Ʈ�ѷ� ����
	CameraBoom->bDoCollisionTest = true;              // �� �浹 ����

	// === ȸ�� ���� ===
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// === ���� ī�޶� ���� ===
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // ī�޶�� ��ü ȸ�� �� ��

	// === �̵� ���� ===
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;  // �ڵ� ȸ�� ����
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f); // ���� ȸ��
	GetCharacterMovement()->MaxWalkSpeed = 300.f;             // �ȱ� �ӵ�
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

	// ������Ʈ ���� ���� Ȯ��
	UE_LOG(LogTemp, Warning, TEXT("CameraBoom exists: %s"), CameraBoom ? TEXT("YES") : TEXT("NO"));
	UE_LOG(LogTemp, Warning, TEXT("FollowCamera exists: %s"), FollowCamera ? TEXT("YES") : TEXT("NO"));
	UE_LOG(LogTemp, Warning, TEXT("Camera parent: %s"), FollowCamera->GetAttachParent() ? *FollowCamera->GetAttachParent()->GetName() : TEXT("NONE"));

	// ViewTarget Ȯ��
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		AActor* ViewTarget = PC->GetViewTarget();
		UE_LOG(LogTemp, Warning, TEXT("Current ViewTarget: %s"), ViewTarget ? *ViewTarget->GetName() : TEXT("NONE"));
	}
}

// Input_Move - ī�޶� ���� �̵�
void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	float WalkSpeedRatio = 100.0f / 300.0f;

	if (MovementVector.Y != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y * WalkSpeedRatio);
	}
	if (MovementVector.X != 0.f)
	{
		AddMovementInput(GetActorRightVector(), MovementVector.X * WalkSpeedRatio);

		// ĳ���͸� 90�� ȸ��
		FRotator NewRot = GetActorRotation();
		NewRot.Yaw += (MovementVector.X > 0) ? 90.0f : -90.0f;
		SetActorRotation(NewRot);
	}
}

// Input_Look - ��Ʈ�ѷ�(ī�޶�) ȸ��
void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f || LookAxisVector.Y != 0.f)
	{
		// SpringArm ��ü�� ȸ�����Ѽ� ���� �˵� �����
		FRotator CurrentRotation = CameraBoom->GetRelativeRotation();

		// ���� ȸ�� (�¿�� ����)
		CurrentRotation.Yaw += LookAxisVector.X;

		// ���� ȸ�� (���Ʒ�)
		CurrentRotation.Pitch += LookAxisVector.Y;
		CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch, -80.0f, 80.0f);

		CameraBoom->SetRelativeRotation(CurrentRotation);

		UE_LOG(LogTemp, Warning, TEXT("SpringArm Rotation: %s"), *CurrentRotation.ToString());
	}
}

void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag InputTag)
{
}

void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag InputTag)
{
}
