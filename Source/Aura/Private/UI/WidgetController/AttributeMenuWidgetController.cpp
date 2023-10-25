// CopyrightTrojanEngine


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Tag : AttributeInfo.Get()->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Tag.AttributeGetter).AddLambda([this, Tag](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Tag.AttributeTag);
		});
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	//the DataAsset
	check(AttributeInfo);

	for (auto& Tag : AttributeInfo.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(Tag.AttributeTag);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag) const
{
	//getting info from DataAsset based on the gameplay tag match.
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	//Set the hidden attribute value in the dataset.
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);
	//broadcast for those who subscribe. IE the attribute menu widget in blueprint
	AttributeInfoDelegate.Broadcast(Info);
	
}