#include "przTestPrivatePCH.h"

#include "utils/ServiceManager.h"

namespace prz {
    namespace testing {
        class SomeService {
        public:
            static const int kDestructorValue = 1337;
            static const char* kName;

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

        const char* SomeService::kName = "SomeService";

        class AnotherService {
        public:
            static const char* kName;
        };

        const char* AnotherService::kName = "AnotherService";

        class ServiceManagerTest : public ::testing::Test {
        protected:
            static const int kSomeValue = 42;
        };

        class ServiceManagerWithInstanceTest : public ServiceManagerTest {
        protected:
            utl::ZServiceManager sm;
        };

        TEST_F(ServiceManagerWithInstanceTest, SetService_NormalServicesAreAddedSuccessfully) {
            bool success = sm.Register<SomeService>(new SomeService(), SomeService::kName);

            ASSERT_TRUE(success);
        }

        TEST_F(ServiceManagerWithInstanceTest, SetService_EmptyServiceTypeNamesAreNotPermitted) {
            bool success = sm.Register<SomeService>(new SomeService(), "");

            ASSERT_FALSE(success);
        }

        TEST_F(ServiceManagerWithInstanceTest, SetService_NullServiceInstancesAreNotPermitted) {
            bool success = sm.Register<SomeService>(nullptr, SomeService::kName);

            ASSERT_FALSE(success);
        }

        TEST_F(ServiceManagerWithInstanceTest, SetService_DestroysOldService) {
            int value = kSomeValue;

            sm.Register<SomeService>(new SomeService(&value), SomeService::kName);
            sm.Register<SomeService>(new SomeService(), SomeService::kName);

            ASSERT_EQ(SomeService::kDestructorValue, value);
        }

        TEST_F(ServiceManagerWithInstanceTest, SetService_RewritesOldServiceWithNewOne) {
            SomeService* service = new SomeService();
            SomeService* anotherService = new SomeService();

            sm.Register<SomeService>(service, SomeService::kName);
            sm.Register<SomeService>(anotherService, SomeService::kName);

            ASSERT_EQ(anotherService, sm.GetService<SomeService>(SomeService::kName));
        }

        TEST_F(ServiceManagerWithInstanceTest, GetService_ReturnsSetService) {
            SomeService* service = new SomeService();
            sm.Register<SomeService>(service, SomeService::kName);

            ASSERT_EQ(service, sm.GetService<SomeService>(SomeService::kName));
        }

        TEST_F(ServiceManagerWithInstanceTest, GetService_ReturnsNullIfHasNotService) {
            SomeService* service = sm.GetService<SomeService>(SomeService::kName);

            ASSERT_EQ(nullptr, service);
        }

        TEST_F(ServiceManagerWithInstanceTest, GetService_ReturnsExactlyAddedServiceByItsType) {
            SomeService* service = new SomeService();
            AnotherService* anotherService = new AnotherService();

            sm.Register<SomeService>(service, SomeService::kName);
            sm.Register<AnotherService>(anotherService, AnotherService::kName);

            ASSERT_EQ(service, sm.GetService<SomeService>(SomeService::kName));
            ASSERT_EQ(anotherService, sm.GetService<AnotherService>(AnotherService::kName));
        }

        TEST_F(ServiceManagerTest, DestroysAddedServiceInDestructor) {
            utl::ZServiceManager* sm = new utl::ZServiceManager();

            int value = kSomeValue;
            sm->Register<SomeService>(new SomeService(&value), SomeService::kName);

            delete sm;

            ASSERT_EQ(SomeService::kDestructorValue, value);
        }

        TEST_F(ServiceManagerWithInstanceTest, Unregister_DestroysUnregisteredService) {
            int value = kSomeValue;
            sm.Register<SomeService>(new SomeService(&value), SomeService::kName);

            EXPECT_TRUE(sm.Unregister<SomeService>(SomeService::kName));
            ASSERT_EQ(SomeService::kDestructorValue, value);
        }

        TEST_F(ServiceManagerWithInstanceTest, Unregister_ReturnsFalseIfUnregisteringServiceWasNotRegistered) {
            ASSERT_FALSE(sm.Unregister<SomeService>(SomeService::kName));
        }
    }
}
