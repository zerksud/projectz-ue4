#pragma once

#include "Utils/ZServiceManager.h"

#define REGISTER_SERVICE(serviceTypeName, serviceInstance) \
    SERVICE_MANAGER_REGISTER_SERVICE(prz::utl::ZServices::GetInstance(), serviceTypeName, serviceInstance)

#define GET_SERVICE(serviceTypeName) \
    SERVICE_MANAGER_GET_SERVICE(prz::utl::ZServices::GetInstance(), serviceTypeName)

#define UNREGISTER_SERVICE(serviceTypeName) \
    SERVICE_MANAGER_UNREGISTER_SERVICE(prz::utl::ZServices::GetInstance(), serviceTypeName)

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
