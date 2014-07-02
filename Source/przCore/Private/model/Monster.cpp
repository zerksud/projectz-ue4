#include "przCorePrivatePCH.h"
#include "model/Monster.h"

#include "utils/Services.h"
#include "utils/IUniqueIdRegistry.h"

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

        ZDirection& ZMonster::GetDirection() {
            return mDirection;
        }
    }
}
