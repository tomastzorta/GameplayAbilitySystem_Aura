// CopyrightTrojanEngine

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h" //since its not a pointer or reference, we can include it here
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

USTRUCT(BlueprintType) 
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly) //edit defaults only means that we can only edit this in the editor, not in the blueprint
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const; //this function will return a pointer to the input action that matches the input tag

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) //blueprint readonly means that we can only read this in the blueprint, not edit it
	TArray<FAuraInputAction> AbilityInputActions;
};
