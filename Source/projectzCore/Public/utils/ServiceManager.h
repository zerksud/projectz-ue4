#pragma once

#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "Core.h"

namespace prz {
    namespace utils {
        class PROJECTZCORE_API ZServiceManager {
        public:
            ZServiceManager();
            virtual ~ZServiceManager();

            template<typename TServiceType> bool Register(TServiceType* instance) {
                if (!instance) {
                    //LOGE("Can't register service of type %s with nullptr instance", typeid(TServiceType).name());
                    return false;
                }

                ZServiceBox* box = new ZServiceBox();
                box->instance = instance;
                box->destructor = [instance]() {
                    delete instance;
                };

                std::type_index index = std::type_index(typeid(TServiceType));

                ZServiceMap::iterator pos = mServiceMap.find(index);
                if (pos != mServiceMap.end()) {
                    delete pos->second;
                }

                mServiceMap[index] = box;

                return true;
            }

            template<typename TServiceType> TServiceType* GetService() {
                std::type_index index = std::type_index(typeid(TServiceType));
                ZServiceMap::iterator pos = mServiceMap.find(index);
                if (pos != mServiceMap.end()) {
                    return static_cast<TServiceType*>(pos->second->instance);
                }

                return nullptr;
            }

            template<typename TServiceType> bool Unregister() {
                std::type_index index = std::type_index(typeid(TServiceType));
                ZServiceMap::iterator pos = mServiceMap.find(index);
                if (pos != mServiceMap.end()) {
                    delete pos->second;
                    return true;
                }

                return false;
            }

        private:
            typedef std::function<void()> ZServiceDestructor;
            struct ZServiceBox {
                void* instance = nullptr;
                ZServiceDestructor destructor;

                ~ZServiceBox() {
                    if (destructor) {
                        destructor();
                    }
                }
            };
            typedef std::unordered_map<std::type_index, ZServiceBox*> ZServiceMap;

            ZServiceMap mServiceMap;
        };
    }
}
