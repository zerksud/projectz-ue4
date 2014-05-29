#include "projectzTestPrivatePCH.h"

#include "utils/NotificationCenter.h"

namespace prz {
    namespace testing {

        struct ValueOwner {
            int value;
        };

        typedef utl::INotificationCenter::ZNotificationEventHandler ZNotificationEventHandler;

        class NotificationCenterTest : public ::testing::Test {
        protected:
            static const char* kEmptyNotification;
            static const char* kSomeNotification;
            static const char* kAnotherNotification;

            static const ZNotificationEventHandler kEmptyHandler;

            static const int kDefaultValue = 0;
            static const int kSomeValue = 42;
            static const int kAnotherValue = 1337;

            virtual void SetUp() override {
                nc = new utl::ZNotificationCenter();
            }

            virtual void TearDown() override {
                delete nc;
            }

            utl::INotificationCenter* nc;
        };

        const char* NotificationCenterTest::kEmptyNotification = "";
        const char* NotificationCenterTest::kSomeNotification = "some_notification";
        const char* NotificationCenterTest::kAnotherNotification = "another_notification";

        const ZNotificationEventHandler NotificationCenterTest::kEmptyHandler = [](const utl::ZDictionary& dict) {};

        ZNotificationEventHandler makeIncrementValueHandler(ValueOwner* owner, int valueIncrement) {
            return[owner, valueIncrement](const utl::ZDictionary& dict) {
                owner->value += valueIncrement;
            };
        }

        TEST_F(NotificationCenterTest, AddObserver_EmptyNotificationsAreNotPermitted) {
            ValueOwner owner = {kDefaultValue};
            bool success = nc->AddObserver(kEmptyNotification, &owner, kEmptyHandler);

            ASSERT_FALSE(success);
        }

        TEST_F(NotificationCenterTest, AddObserver_NullOwnersAreNotPermitted) {
            bool success = nc->AddObserver(kSomeNotification, nullptr, kEmptyHandler);

            ASSERT_FALSE(success);
        }

        TEST_F(NotificationCenterTest, AddObserver_NullHandlersAreNotPermitted) {
            ValueOwner owner = {kDefaultValue};
            bool success = nc->AddObserver(kSomeNotification, &owner, nullptr);

            ASSERT_FALSE(success);
        }

        TEST_F(NotificationCenterTest, AddObserver_ObserverCantBeAddedTwiceForSameNotification) {
            ValueOwner owner = {kDefaultValue};
            nc->AddObserver(kSomeNotification, &owner, kEmptyHandler);
            bool success = nc->AddObserver(kSomeNotification, &owner, kEmptyHandler);

            ASSERT_FALSE(success);
        }

        TEST_F(NotificationCenterTest, AddObserver_NormalObserverCanBeAddedSuccessfully) {
            ValueOwner owner = {kDefaultValue};
            bool success = nc->AddObserver(kSomeNotification, &owner, kEmptyHandler);

            ASSERT_TRUE(success);
        }

        TEST_F(NotificationCenterTest, RemoveObserver_HandlerIsNotFiredForRemovedObserver) {
            ValueOwner owner = {kDefaultValue};
            nc->AddObserver(kSomeNotification, &owner, makeIncrementValueHandler(&owner, kSomeValue));
            nc->RemoveObserver(kSomeNotification, &owner);
            nc->PostNotification(kSomeNotification);

            ASSERT_EQ(kDefaultValue, owner.value);
        }

        TEST_F(NotificationCenterTest, RemoveObserver_ObserversAreRemovedPerNotification) {
            ValueOwner owner = {kDefaultValue};
            nc->AddObserver(kSomeNotification, &owner, makeIncrementValueHandler(&owner, kSomeValue));
            nc->AddObserver(kAnotherNotification, &owner, makeIncrementValueHandler(&owner, kAnotherValue));

            nc->RemoveObserver(kSomeNotification, &owner);

            nc->PostNotification(kSomeNotification);
            nc->PostNotification(kAnotherNotification);

            ASSERT_EQ(kAnotherValue, owner.value);
        }

        TEST_F(NotificationCenterTest, RemoveObserver_NotAddedObserverCantBeRemoved) {
            ValueOwner owner = {kDefaultValue};
            bool success = nc->RemoveObserver(kSomeNotification, &owner);

            ASSERT_FALSE(success);
        }

        TEST_F(NotificationCenterTest, RemoveObserver_AddedObserverCanBeRemovedSuccessfully) {
            ValueOwner owner = {kDefaultValue};
            nc->AddObserver(kSomeNotification, &owner, kEmptyHandler);
            bool success = nc->RemoveObserver(kSomeNotification, &owner);

            ASSERT_TRUE(success);
        }

        TEST_F(NotificationCenterTest, RemoveObserver_AddedObserverCantBeRemovedUsingAnotherNotification) {
            ValueOwner owner = {kDefaultValue};
            nc->AddObserver(kSomeNotification, &owner, makeIncrementValueHandler(&owner, kSomeValue));
            nc->RemoveObserver(kAnotherNotification, &owner);
            nc->PostNotification(kSomeNotification);

            ASSERT_EQ(kSomeValue, owner.value);
        }

        TEST_F(NotificationCenterTest, PostNotification_HandlerIsFiredOnceOnSingleNotification) {
            ValueOwner owner = {kDefaultValue};
            nc->AddObserver(kSomeNotification, &owner, makeIncrementValueHandler(&owner, kSomeValue));
            nc->PostNotification(kSomeNotification);

            ASSERT_EQ(kSomeValue, owner.value);
        }

        TEST_F(NotificationCenterTest, PostNotification_HandlerIsFiredTwiceOnDoubleNotification) {
            ValueOwner owner = {kDefaultValue};
            nc->AddObserver(kSomeNotification, &owner, makeIncrementValueHandler(&owner, kSomeValue));

            const int notificationCount = 2;
            for (int i = 0; i < notificationCount; ++i) {
                nc->PostNotification(kSomeNotification);
            }

            ASSERT_EQ(kSomeValue * notificationCount, owner.value);
        }

        TEST_F(NotificationCenterTest, PostNotification_HandlerIsNotFiredOnAnotherNotification) {
            ValueOwner owner = {kDefaultValue};
            nc->AddObserver(kSomeNotification, &owner, makeIncrementValueHandler(&owner, kSomeValue));

            nc->PostNotification(kAnotherNotification);

            ASSERT_EQ(kDefaultValue, owner.value);
        }

        TEST_F(NotificationCenterTest, PostNotification_EachHandlerIsFiredOnlyOnHisNotification) {
            ValueOwner owner = {kDefaultValue};
            ValueOwner anotherOwner = {kDefaultValue};

            nc->AddObserver(kSomeNotification, &owner, makeIncrementValueHandler(&owner, kSomeValue));
            nc->AddObserver(kAnotherNotification, &anotherOwner, makeIncrementValueHandler(&anotherOwner, kAnotherValue));

            nc->PostNotification(kSomeNotification);
            nc->PostNotification(kAnotherNotification);

            ASSERT_EQ(kSomeValue, owner.value);
            ASSERT_EQ(kAnotherValue, anotherOwner.value);
        }

        TEST_F(NotificationCenterTest, PostNotification_EmptyNotificationsAreNotPermitted) {
            bool success = nc->PostNotification(kEmptyNotification);

            ASSERT_FALSE(success);
        }

        TEST_F(NotificationCenterTest, PostNotification_NormalNotificationCanBePostedSuccessfully) {
            bool success = nc->PostNotification(kSomeNotification);

            ASSERT_TRUE(success);
        }

        TEST_F(NotificationCenterTest, PostNotification_NotificationArgumentsArePassedToObserver) {
            ValueOwner owner = {kDefaultValue};

            nc->AddObserver(kSomeNotification, &owner, [&owner](const utl::ZDictionary& dict) {
                owner.value = dict["value"].AsInt();
            });

            utl::ZDictionary dict;
            dict["value"] = kSomeValue;

            nc->PostNotification(kSomeNotification, dict);

            ASSERT_EQ(kSomeValue, owner.value);
        }
    }
}
