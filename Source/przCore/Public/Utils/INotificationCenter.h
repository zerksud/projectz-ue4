// Copyright 2016 Max Molodtsov
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

#include "Utils/ZDictionary.h"
#include "Utils/StandardLibrary/ZFunctional.h"

namespace prz {
namespace utl {

class INotificationCenter {
public:
    virtual ~INotificationCenter() = default;

    typedef utl::ZFunction<void(const ZDictionary& dict)> ZNotificationEventHandler;

    virtual bool AddObserver(const ZString& name, void* observerOwner, ZNotificationEventHandler handler) = 0;
    virtual bool RemoveObserver(const ZString& name, void* observerOwner) = 0;

    virtual bool PostNotification(const ZString& name) = 0;
    virtual bool PostNotification(const ZString& name, const ZDictionary& dict) = 0;
};

}
}
