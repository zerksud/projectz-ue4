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

#pragma once

#include "Platform.h"

#include <limits>
#include <unordered_set>

#include "Utils/IUniqueIdRegistry.h"

namespace prz {
namespace utl {

class PRZCORE_API ZUniqueIdRegistry : public IUniqueIdRegistry {
public:
    ZUniqueIdRegistry(ZIdType maxId = std::numeric_limits<ZIdType>::max());
    ZUniqueIdRegistry(const ZUniqueIdRegistry& other) = delete;
    ZUniqueIdRegistry& operator=(const ZUniqueIdRegistry& other) = delete;
    virtual ~ZUniqueIdRegistry() = default;

    bool AssignUniqueId(ZRegistrable* object) override;
    bool ReleaseUniqueId(ZRegistrable* object) override;

private:
    typedef std::unordered_set<ZIdType> ZReleasedIdSet;

    ZIdType mMaxId;
    ZIdType mNextFreeId;
    ZReleasedIdSet mReleasedIds;
};

}
}
