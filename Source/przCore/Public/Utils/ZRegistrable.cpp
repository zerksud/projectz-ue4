// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "przCorePCH.h"
#include "Utils/ZRegistrable.h"

#include "Utils/ZServices.h"
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
