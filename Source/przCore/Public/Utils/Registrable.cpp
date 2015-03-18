#include "przCorePCH.h"
#include "Utils/Registrable.h"

#include "Utils/Services.h"
#include "Utils/IUniqueIdRegistry.h"
#include "Utils/LOG.h"

namespace prz {
namespace utl {

const ZIdType ZRegistrable::kNoId = 0;

ZRegistrable::ZRegistrable() : mId(kNoId) {
}

ZRegistrable::~ZRegistrable() {
    if (IsRegistered()) {
        LOGE("id = %d wasn't released", mId);
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
