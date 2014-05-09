#pragma once

#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace prz {
    namespace utils {
        class ZServiceManager {
        public:
            ZServiceManager();
            virtual ~ZServiceManager();

            template<typename TServiceType> void Register(TServiceType* instance) {
                ZServiceBox* box = new ZServiceBox();
                box->instance = instance;
                box->destructor = [instance]() {
                    delete instance;
                };

                std::type_index index = std::type_index(typeid(TServiceType));

                ZServiceMap::iterator pos = mServiceMap.find(index);
                if (pos != mServiceMap.end()) {
                    pos->second->destructor();
                    delete pos->second;
                }

                mServiceMap[index] = box;
            }

            template<typename TServiceType> TServiceType* Resolve() {
                std::type_index index = std::type_index(typeid(TServiceType));
                ZServiceMap::iterator pos = mServiceMap.find(index);
                if (pos != mServiceMap.end()) {
                    return static_cast<TServiceType*>(pos->second->instance);
                }

                return nullptr;
            }

        private:
            typedef std::function<void()> ZServiceDestructor;
            struct ZServiceBox {
                void* instance = nullptr;
                ZServiceDestructor destructor;
            };
            typedef std::unordered_map<std::type_index, ZServiceBox*> ZServiceMap;

            ZServiceMap mServiceMap;
        };
    }
}
