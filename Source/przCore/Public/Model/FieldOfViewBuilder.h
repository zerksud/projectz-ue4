#pragma once

#include "Model/DungeonLevel.h"
#include "Model/FieldOfView.h"

namespace prz {
namespace mdl {

class ZFieldOfViewBuilder {
public:
    ZFieldOfViewBuilder() = delete;

    static ZFieldOfView Build(const ZPosition& position, int viewDistance, const ZDungeonLevel& dungeonLevel);
};

}
}
