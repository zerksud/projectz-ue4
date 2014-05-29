#include "projectzCorePrivatePCH.h"
#include "model/Monster.h"

#include "utils/Services.h"
#include "utils/IUniqueIdRegistry.h"

namespace prz {
    namespace mdl {
        ZMonster ZMonster::CreateMonster() {
            ZMonster monster;

            utils::IUniqueIdRegistry* registry = utils::ZServices::GetInstance().GetService<utils::IUniqueIdRegistry>();
            if (registry) {
                registry->AssignUniqueId(&monster);
            }

            return monster;
        }

        ZMonster::ZMonster() {
        }

        ZMonster::~ZMonster() {
        }

        ZDirection& ZMonster::GetDirection() {
            return mDirection;
        }
    }
}
