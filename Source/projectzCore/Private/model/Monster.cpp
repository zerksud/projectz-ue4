#include "projectzCorePrivatePCH.h"

#include "model/Monster.h"

namespace prz {
    namespace mdl {
        ZMonster::ZMonster() {
        }

        ZMonster::~ZMonster() {
        }

        ZDirection& ZMonster::GetDirection() {
            return mDirection;
        }
    }
}
