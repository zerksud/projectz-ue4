#include "projectzCorePrivatePCH.h"
#include "model/Monster.h"

#include "utils/Services.h"
#include "utils/IUniqueIdRegistry.h"

namespace prz {
    namespace mdl {
        ZMonster ZMonster::CreateMonster() {
            ZMonster monster;

            utl::IUniqueIdRegistry* registry = utl::ZServices::GetInstance().GetService<utl::IUniqueIdRegistry>();
            if (registry) {
                registry->AssignUniqueId(&monster);
            }

            return monster;
        }

        ZDirection& ZMonster::GetDirection() {
            return mDirection;
        }
    }
}
