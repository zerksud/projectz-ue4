#pragma once

#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace prz {
    namespace utils {
        class ServiceManager {
        public:
            static ServiceManager& GetInstance() {
                static ServiceManager instance;
                return instance;
            }

            template<typename ServiceType> void Register(ServiceType* instance) {
                ServiceBox* box = new ServiceBox();
                box->instance = instance;
                box->destructor = [instance]() {
                    delete instance;
                };

                std::type_index index = std::type_index(typeid(ServiceType));

                ServiceMap::iterator pos = mServiceMap.find(index);
                if (pos != mServiceMap.end()) {
                    pos->second->destructor();
                    delete pos->second;
                }

                mServiceMap[index] = box;
            }

            template<typename ServiceType> ServiceType* Resolve() {
                std::type_index index = std::type_index(typeid(ServiceType));
                ServiceMap::iterator pos = mServiceMap.find(index);
                if (pos != mServiceMap.end()) {
                    return static_cast<ServiceType*>(pos->second->instance);
                }

                return nullptr;
            }

        private:
            ServiceManager();
            virtual ~ServiceManager();

            typedef std::function<void()> ServiceDestructor;
            struct ServiceBox {
                void* instance = nullptr;
                ServiceDestructor destructor;
            };
            typedef std::unordered_map<std::type_index, ServiceBox*> ServiceMap;

            ServiceMap mServiceMap;
        };
    }
}
