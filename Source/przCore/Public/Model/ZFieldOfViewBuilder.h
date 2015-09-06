#pragma once

#include "Model/ZDungeonLevel.h"
#include "Model/ZFieldOfView.h"

namespace prz {
namespace mdl {

class ZFieldOfViewBuilder {
public:
    ZFieldOfViewBuilder() = delete;

    static ZFieldOfView Build(const ZPosition& position, int viewDistance, const ZDungeonLevel& dungeonLevel);
    
private:
    ZPosition mFieldOfViewCenter;
    int mViewDistance;
    const ZDungeonLevel& mCurrentDungeonLevel;
};

}
}
