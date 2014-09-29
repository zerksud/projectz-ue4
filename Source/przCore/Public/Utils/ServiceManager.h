#pragma once

#include <functional>
#include <list>
#include <string>
#include <unordered_map>

#include "Core.h"

#define SERVICE_MANAGER_REGISTER_SERVICE(serviceManagerInstance, serviceTypeName, serviceInstance) \
    ((serviceManagerInstance).Register< ::serviceTypeName >(serviceInstance, #serviceTypeName))

#define SERVICE_MANAGER_GET_SERVICE(serviceManagerInstance, serviceTypeName) \
    ((serviceManagerInstance).Get< ::serviceTypeName >(#serviceTypeName))

#define SERVICE_MANAGER_UNREGISTER_SERVICE(serviceManagerInstance, serviceTypeName) \
    ((serviceManagerInstance).Unregister< ::serviceTypeName >(#serviceTypeName))

namespace prz {
    namespace utl {
        class PRZCORE_API ZServiceManager {
        public:
            ZServiceManager();
            virtual ~ZServiceManager();

            template<typename TServiceType> bool Register(TServiceType* instance, std::string typeName) {
                if (typeName.empty()) {
                    //LOGE("Can't register service of empty type");
                    return false;
                }

                if (!instance) {
                    //LOGE("Can't register service of type '%s' with nullptr instance", typeName);
                    return false;
                }

                ZServiceBox* box = new ZServiceBox();
                box->instance = instance;
                box->destructor = [instance]() {
                    delete instance;
                };

                auto pos = mServiceMap.find(typeName);
                if (pos != mServiceMap.end()) {
                    mServiceRegisterOrder.remove(pos->second);
                    delete pos->second;
                }

                mServiceRegisterOrder.push_back(box);
                mServiceMap[typeName] = box;

                return true;
            }

            template<typename TServiceType> TServiceType* Get(std::string typeName) {
                auto pos = mServiceMap.find(typeName);
                if (pos != mServiceMap.end()) {
                    return static_cast<TServiceType*>(pos->second->instance);
                }

                return nullptr;
            }

            template<typename TServiceType> bool Unregister(std::string typeName) {
                auto pos = mServiceMap.find(typeName);
                if (pos != mServiceMap.end()) {
                    mServiceRegisterOrder.remove(pos->second);
                    delete pos->second;
                    mServiceMap.erase(pos);

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
            typedef std::unordered_map<std::string, ZServiceBox*> ZServiceMap;
            typedef std::list<ZServiceBox*> ZServiceRegisterOrder;

            ZServiceMap mServiceMap;
            ZServiceRegisterOrder mServiceRegisterOrder;
        };
    }
}
