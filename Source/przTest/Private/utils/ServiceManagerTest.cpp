#include "przTestPrivatePCH.h"

#include "utils/ServiceManager.h"

namespace prz {
    namespace testing {
        class SomeService {
        public:
            static const int kDestructorValue = 1337;

            SomeService(int* value = nullptr) : mValue(value) {
            }

            virtual ~SomeService() {
                if (mValue) {
                    *mValue = kDestructorValue;
                }
            }

        private:
            int* mValue;
        };

        class AnotherService {
        };

        class ServiceManagerTest : public ::testing::Test {
        protected:
            static const int kSomeValue = 42;
        };

        class ServiceManagerWithInstanceTest : public ServiceManagerTest {
        protected:
            utl::ZServiceManager sm;
        };

        TEST_F(ServiceManagerWithInstanceTest, Register_NormalServicesAreAddedSuccessfully) {
            bool success = SERVICE_MANAGER_REGISTER_SERVICE(sm, prz::testing::SomeService, new SomeService());

            ASSERT_TRUE(success);
        }

        TEST_F(ServiceManagerWithInstanceTest, Register_EmptyServiceTypeNamesAreNotPermitted) {
            bool success = sm.Register<SomeService>(new SomeService(), "");

            ASSERT_FALSE(success);
        }

        TEST_F(ServiceManagerWithInstanceTest, Register_NullServiceInstancesAreNotPermitted) {
            bool success = SERVICE_MANAGER_REGISTER_SERVICE(sm, prz::testing::SomeService, nullptr);

            ASSERT_FALSE(success);
        }

        TEST_F(ServiceManagerWithInstanceTest, Register_DestroysOldService) {
            int value = kSomeValue;

            SERVICE_MANAGER_REGISTER_SERVICE(sm, prz::testing::SomeService, new SomeService(&value));
            SERVICE_MANAGER_REGISTER_SERVICE(sm, prz::testing::SomeService, new SomeService());

            ASSERT_EQ(SomeService::kDestructorValue, value);
        }

        TEST_F(ServiceManagerWithInstanceTest, Register_RewritesOldServiceWithNewOne) {
            SomeService* service = new SomeService();
            SomeService* anotherService = new SomeService();

            SERVICE_MANAGER_REGISTER_SERVICE(sm, prz::testing::SomeService, service);
            SERVICE_MANAGER_REGISTER_SERVICE(sm, prz::testing::SomeService, anotherService);

            ASSERT_EQ(anotherService, SERVICE_MANAGER_GET_SERVICE(sm, prz::testing::SomeService));
        }

        TEST_F(ServiceManagerWithInstanceTest, Get_ReturnsSetService) {
            SomeService* service = new SomeService();
            SERVICE_MANAGER_REGISTER_SERVICE(sm, prz::testing::SomeService, service);

            ASSERT_EQ(service, SERVICE_MANAGER_GET_SERVICE(sm, prz::testing::SomeService));
        }

        TEST_F(ServiceManagerWithInstanceTest, Get_ReturnsNullIfHasNotService) {
            SomeService* service = SERVICE_MANAGER_GET_SERVICE(sm, prz::testing::SomeService);

            ASSERT_EQ(nullptr, service);
        }

        TEST_F(ServiceManagerWithInstanceTest, Get_ReturnsExactlyAddedServiceByItsType) {
            SomeService* service = new SomeService();
            AnotherService* anotherService = new AnotherService();

            SERVICE_MANAGER_REGISTER_SERVICE(sm, prz::testing::SomeService, service);
            SERVICE_MANAGER_REGISTER_SERVICE(sm, prz::testing::AnotherService, anotherService);

            ASSERT_EQ(service, SERVICE_MANAGER_GET_SERVICE(sm, prz::testing::SomeService));
            ASSERT_EQ(anotherService, SERVICE_MANAGER_GET_SERVICE(sm, prz::testing::AnotherService));
        }

        TEST_F(ServiceManagerTest, DestroysAddedServiceInDestructor) {
            utl::ZServiceManager* sm = new utl::ZServiceManager();

            int value = kSomeValue;
            SERVICE_MANAGER_REGISTER_SERVICE(*sm, prz::testing::SomeService, new SomeService(&value));

            delete sm;

            ASSERT_EQ(SomeService::kDestructorValue, value);
        }

        TEST_F(ServiceManagerWithInstanceTest, Unregister_DestroysUnregisteredService) {
            int value = kSomeValue;
            SERVICE_MANAGER_REGISTER_SERVICE(sm, prz::testing::SomeService, new SomeService(&value));

            EXPECT_TRUE(SERVICE_MANAGER_UNREGISTER_SERVICE(sm, prz::testing::SomeService));
            ASSERT_EQ(SomeService::kDestructorValue, value);
        }

        TEST_F(ServiceManagerWithInstanceTest, Unregister_ReturnsFalseIfUnregisteringServiceWasNotRegistered) {
            ASSERT_FALSE(SERVICE_MANAGER_UNREGISTER_SERVICE(sm, prz::testing::SomeService));
        }
    }
}
