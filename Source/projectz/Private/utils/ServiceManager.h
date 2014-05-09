#pragma once

#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace prz {
    namespace utils {
        class ZServiceManager {
        public:
            static ZServiceManager& GetInstance() {
                static ZServiceManager instance;
                return instance;
            }

            template<typename ServiceType> void Register(ServiceType* instance) {
                ZServiceBox* box = new ZServiceBox();
                box->instance = instance;
                box->destructor = [instance]() {
                    delete instance;
                };

                std::type_index index = std::type_index(typeid(ServiceType));

                ZServiceMap::iterator pos = mServiceMap.find(index);
                if (pos != mServiceMap.end()) {
                    pos->second->destructor();
                    delete pos->second;
                }

                mServiceMap[index] = box;
            }

            template<typename ServiceType> ServiceType* Resolve() {
                std::type_index index = std::type_index(typeid(ServiceType));
                ZServiceMap::iterator pos = mServiceMap.find(index);
                if (pos != mServiceMap.end()) {
                    return static_cast<ServiceType*>(pos->second->instance);
                }

                return nullptr;
            }

        private:
            ZServiceManager();
            virtual ~ZServiceManager();

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
