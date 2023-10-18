// CopyrightTrojanEngine


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	* Line trace from cursor. There are several scenarios:
	* 1. LastActor is null && CurrentActor is null
	*	- Do nothing.
	* 2. LastActor is null && CurrentActor is valid
	*	- Highlight CurrentActor.
	* 3. LastActor is valid && CurrentActor is null
	*	- Unhighlight LastActor.
	* 4. Both actors are valid, but LastActor != CurrentActor
	*	- Unhighlight LastActor and highlight CurrentActor.
	* 5. Both actors are valid, but LastActor == CurrentActor
	*  - Do nothing.
	*/
	if (LastActor == nullptr)
	{
		if (CurrentActor !=	nullptr)
		{
			//Case 2
			CurrentActor->HighlightActor();
		}
		else
		{
			//Case 1 - both are null, do nothing
			
		}
	}
	else //last actor is valid
	{
		if (CurrentActor == nullptr)
		{
			//Case 3
			LastActor->UnhighlightActor();
		}
		else //both actors are valid
		{
			if (LastActor != CurrentActor)
			{
				//Case 4
				LastActor->UnhighlightActor();
				CurrentActor->HighlightActor();
			}
			else
			{
				//Case 5 - do nothing
			}
		}
		
	}
	
	
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext); // This will crash the game if AuraContext is null

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D& InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

