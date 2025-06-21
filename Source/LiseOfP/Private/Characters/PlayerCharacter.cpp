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
	// === 메인 바디 (애니메이션 담당) ===
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// GetMesh()에 바디 스켈레탈 메시와 애니메이션 BP 설정

	// === 추가 부위들 (뼈대만 공유) ===
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetMesh()); 
	HeadMesh->SetMasterPoseComponent(GetMesh()); 

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(GetMesh());
	HairMesh->SetMasterPoseComponent(GetMesh()); 

	ArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"));
	ArmsMesh->SetupAttachment(GetMesh());
	ArmsMesh->SetMasterPoseComponent(GetMesh()); 


	// 캡슐 컴포넌트 크기 설정 (콜리전 범위)
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// 컨트롤러 회전값을 직접 캐릭터에 반영하지 않음 (카메라 회전에 영향을 주지 않음)
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// SpringArm 설정 (원형 궤도용)
	CameraBoom->SetupAttachment(GetRootComponent());  // 캐릭터 중심에 붙이기
	CameraBoom->TargetArmLength = 300.f;              // 거리
	CameraBoom->bUsePawnControlRotation = false;      // 컨트롤러 무시
	CameraBoom->bDoCollisionTest = true;              // 벽 충돌 감지

	// === 회전 제어 ===
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// === 실제 카메라 설정 ===
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // 카메라는 자체 회전 안 함

	// === 이동 관련 ===
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;  // 자동 회전 끄기
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f); // 빠른 회전
	GetCharacterMovement()->MaxWalkSpeed = 300.f;             // 걷기 속도
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

	// 컴포넌트 연결 상태 확인
	UE_LOG(LogTemp, Warning, TEXT("CameraBoom exists: %s"), CameraBoom ? TEXT("YES") : TEXT("NO"));
	UE_LOG(LogTemp, Warning, TEXT("FollowCamera exists: %s"), FollowCamera ? TEXT("YES") : TEXT("NO"));
	UE_LOG(LogTemp, Warning, TEXT("Camera parent: %s"), FollowCamera->GetAttachParent() ? *FollowCamera->GetAttachParent()->GetName() : TEXT("NONE"));

	// ViewTarget 확인
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		AActor* ViewTarget = PC->GetViewTarget();
		UE_LOG(LogTemp, Warning, TEXT("Current ViewTarget: %s"), ViewTarget ? *ViewTarget->GetName() : TEXT("NONE"));
	}
}

// Input_Move - 카메라 기준 이동
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

		// 캐릭터만 90도 회전
		FRotator NewRot = GetActorRotation();
		NewRot.Yaw += (MovementVector.X > 0) ? 90.0f : -90.0f;
		SetActorRotation(NewRot);
	}
}

// Input_Look - 컨트롤러(카메라) 회전
void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f || LookAxisVector.Y != 0.f)
	{
		// SpringArm 자체를 회전시켜서 원형 궤도 만들기
		FRotator CurrentRotation = CameraBoom->GetRelativeRotation();

		// 수평 회전 (좌우로 원형)
		CurrentRotation.Yaw += LookAxisVector.X;

		// 수직 회전 (위아래)
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
