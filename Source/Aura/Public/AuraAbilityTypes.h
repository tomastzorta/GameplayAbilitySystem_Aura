#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	//structs have a public constructor by default

	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsBlockedHit() const {return bIsBlockedHit;}

	void SetIsCriticalHit(bool bInIsCriticalHit) {bIsCriticalHit = bInIsCriticalHit;}
	void SetIsBlockedHit(bool bInIsBlockedHit) {bIsBlockedHit = bInIsBlockedHit;}

	/** Returns the actual struct used for serialization, sublcasses must override this */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			//does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, sublcasses must override this */
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
	
protected:

	UPROPERTY() 
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
	
	
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};