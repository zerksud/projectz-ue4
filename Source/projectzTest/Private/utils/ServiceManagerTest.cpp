#include "projectzTestPrivatePCH.h"

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
            utils::ZServiceManager sm;
        };

        TEST_F(ServiceManagerTest, SetService_NormalServicesAreAddedSuccessfully) {
            bool success = sm.Register<SomeService>(new SomeService());

            ASSERT_TRUE(success);
        }

        TEST_F(ServiceManagerTest, SetService_NullServiceInstancesAreNotPermitted) {
            bool success = sm.Register<SomeService>(nullptr);

            ASSERT_FALSE(success);
        }

        TEST_F(ServiceManagerTest, SetService_DestroysOldService) {
            int value = kSomeValue;

            sm.Register<SomeService>(new SomeService(&value));
            sm.Register<SomeService>(new SomeService());

            ASSERT_EQ(SomeService::kDestructorValue, value);
        }

        TEST_F(ServiceManagerTest, SetService_RewritesOldServiceWithNewOne) {
            SomeService* service = new SomeService();
            SomeService* anotherService = new SomeService();

            sm.Register<SomeService>(service);
            sm.Register<SomeService>(anotherService);

            ASSERT_EQ(anotherService, sm.GetService<SomeService>());
        }

        TEST_F(ServiceManagerTest, GetService_ReturnsSetService) {
            SomeService* service = new SomeService();
            sm.Register<SomeService>(service);

            ASSERT_EQ(service, sm.GetService<SomeService>());
        }

        TEST_F(ServiceManagerTest, GetService_ReturnsNullIfHasNotService) {
            SomeService* service = sm.GetService<SomeService>();

            ASSERT_EQ(nullptr, service);
        }

        TEST_F(ServiceManagerTest, GetService_ReturnsExactlyAddedServiceByItsType) {
            SomeService* service = new SomeService();
            AnotherService* anotherService = new AnotherService();

            sm.Register<SomeService>(service);
            sm.Register<AnotherService>(anotherService);

            ASSERT_EQ(service, sm.GetService<SomeService>());
            ASSERT_EQ(anotherService, sm.GetService<AnotherService>());
        }

        TEST_F(ServiceManagerTest, DestroysAddedServiceInDestructor) {
            utils::ZServiceManager* sm = new utils::ZServiceManager();

            int value = kSomeValue;
            sm->Register<SomeService>(new SomeService(&value));

            delete sm;

            ASSERT_EQ(SomeService::kDestructorValue, value);
        }
    }
}
