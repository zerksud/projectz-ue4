#include "przCorePCH.h"
#include "Model/ZMonster.h"

#include "Utils/ZServices.h"
#include "Utils/IUniqueIdRegistry.h"
#include "Utils/LOG.h"

namespace prz {
namespace mdl {

ZMonster* ZMonster::CreateMonster() {
    ZMonster* monster = new ZMonster();

    utl::IUniqueIdRegistry* registry = GET_SERVICE(prz::utl::IUniqueIdRegistry);
    if (registry) {
        registry->AssignUniqueId(monster);
    }

    return monster;
}

void ZMonster::DestroyMonster(ZMonster** monster) {
    if (monster == nullptr) {
        LOGE("Can't destroy nullptr as ZMonster**");
        return;
    }

    if (*monster != nullptr && (*monster)->IsRegistered()) {
        utl::IUniqueIdRegistry* registry = GET_SERVICE(prz::utl::IUniqueIdRegistry);
        if (registry) {
            registry->ReleaseUniqueId(*monster);
        }
    }

    delete *monster;
    *monster = nullptr;
}

ZDirection& ZMonster::GetDirection() {
    return mDirection;
}

unsigned int ZMonster::GetViewDistance() const {
    return 8;
}

void ZMonster::UpdateFieldOfView(ZFieldOfView&& newFieldOfView) {
    mFieldOfView = newFieldOfView;
}

const ZFieldOfView& ZMonster::GetFieldOfView() const {
    return mFieldOfView;
}

}
}
