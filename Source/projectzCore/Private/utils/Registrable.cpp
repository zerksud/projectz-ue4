#include "projectzCorePrivatePCH.h"
#include "utils/Registrable.h"

#include "utils/Services.h"
#include "utils/IUniqueIdRegistry.h"
#include "utils/LOG_ANSI.h"

namespace prz {
    namespace utils {

        ZRegistrable::ZRegistrable() : mId(kNoId) {
        }

        ZRegistrable::~ZRegistrable() {
            if (IsRegistered()) {
                IUniqueIdRegistry* registry = utils::ZServices::GetInstance().GetService<utils::IUniqueIdRegistry>();
                if (registry) {
                    registry->ReleaseUniqueId(this);
                } else {
                    LOGE("Can't release id = %d cause there is no IUniqueIdRegistry service", mId);
                }
            }
        }

        ZIdType ZRegistrable::GetId() const {
            return mId;
        }

        void ZRegistrable::SetId(ZIdType id) {
            mId = id;
        }

        bool ZRegistrable::IsRegistered() const {
            return mId != kNoId;
        }

    }
}
