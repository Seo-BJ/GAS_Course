


#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIOwner = OwnerComp.GetAIOwner();
	AActor* ActorOwner = OwnerComp.GetOwner();
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, *AIOwner->GetName());
	GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Green, *ActorOwner->GetName());



	APawn* OwningPawn = AIOwner->GetPawn();

	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for (AActor* Actor : ActorsWithTag)
	{
		GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Orange, *Actor->GetName());

		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetToFollowSelector.SelectedKeyName, ClosestActor);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceToTargetSelector.SelectedKeyName, ClosestDistance);

}
