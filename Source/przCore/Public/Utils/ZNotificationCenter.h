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

#include <unordered_map>
#include <utility>
#include <vector>

#include "Utils/INotificationCenter.h"

namespace prz {
namespace utl {

class PRZCORE_API ZNotificationCenter : public INotificationCenter {
public:
    ZNotificationCenter() = default;
    ZNotificationCenter(const ZNotificationCenter& other) = delete;
    ZNotificationCenter& operator=(const ZNotificationCenter& other) = delete;
    virtual ~ZNotificationCenter();

    virtual bool AddObserver(const std::string& name, void* observerOwner, ZNotificationEventHandler handler) override;
    virtual bool RemoveObserver(const std::string& name, void* observerOwner) override;

    virtual bool PostNotification(const std::string& name) override;
    virtual bool PostNotification(const std::string& name, const ZDictionary& dict) override;

private:
    struct ZObserver {
        void* observerOwner;
        ZNotificationEventHandler handler;

        ZObserver(void* pObserverOwner, ZNotificationEventHandler pHandler) {
            this->observerOwner = pObserverOwner;
            this->handler = pHandler;
        }
    };

    typedef std::vector<ZObserver> ZObserverList;
    typedef std::unordered_map<std::string, ZObserverList*> ZObserverListTable;
    typedef std::pair<std::string, ZObserverList*> ZObserverListTablePair;

    ZObserverListTable mObservers;
};

}
}
