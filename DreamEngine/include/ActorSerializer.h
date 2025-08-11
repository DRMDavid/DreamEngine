
#pragma once
#include "ECS/Actor.h"
#include "Prerequisites.h"
namespace ActorSerializer {
	bool
		saveActorsToFile(const std::string& filename, const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);
	bool
		loadActorsFromFile(const std::string& filename, std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);
}