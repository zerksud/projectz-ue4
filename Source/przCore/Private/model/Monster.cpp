#include "przCorePrivatePCH.h"
#include "model/Monster.h"

#include "utils/Services.h"
#include "utils/IUniqueIdRegistry.h"
#include "utils/LOG_ANSI.h"

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
    }
}
