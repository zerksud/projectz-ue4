// Copyright 2014-2017 Max Molodtsov <zerksud@gmail.com>
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

#include "Utils/ZServiceManager.h"

#define REGISTER_SERVICE(serviceTypeName, serviceInstance) \
    SERVICE_MANAGER_REGISTER_SERVICE(prz::utl::ZServices::GetInstance(), serviceTypeName, serviceInstance)

#define GET_SERVICE(serviceTypeName) \
    SERVICE_MANAGER_GET_SERVICE(prz::utl::ZServices::GetInstance(), serviceTypeName)

#define UNREGISTER_SERVICE(serviceTypeName) \
    SERVICE_MANAGER_UNREGISTER_SERVICE(prz::utl::ZServices::GetInstance(), serviceTypeName)

#define UNREGISTER_ALL_SERVICES() \
    SERVICE_MANAGER_UNREGISTER_ALL_SERVICES(prz::utl::ZServices::GetInstance())

namespace prz {
namespace utl {

class PRZCORE_API ZServices {
public:
    ZServices() = delete;
    virtual ~ZServices() = delete;

    static ZServiceManager& GetInstance();
};

}
}
