#include "projectzTestPrivatePCH.h"

#include "gtest/gtest.h"

#include "utils/Dictionary.h"

namespace prz {
    namespace testing {
        class DictionaryTestWithInstance: public ::testing::Test {
        protected:
            static const char* kSomeKey;
            static const int kSomeInt = 42;
            static const bool kSomeBool = true;

            utils::ZDictionary dict;
        };

        const char* DictionaryTestWithInstance::kSomeKey = "some_key";
        
        TEST_F(DictionaryTestWithInstance, IntCanBeStoredAndRetrievedByKey) {
            dict[kSomeKey] = kSomeInt;

            ASSERT_EQ(kSomeInt, dict[kSomeKey].AsInt());
        }

        TEST_F(DictionaryTestWithInstance, BoolCanBeStoredAndRetrievedByKey) {
            dict[kSomeKey] = kSomeBool;

            ASSERT_EQ(kSomeBool, dict[kSomeKey].AsBool());
        }

        TEST_F(DictionaryTestWithInstance, DictCanBeCopiedByAssignmentOperator) {
            dict[kSomeKey] = kSomeInt;
            utils::ZDictionary anotherDict(dict);

            ASSERT_EQ(kSomeInt, anotherDict[kSomeKey].AsInt());
        }
    }
}
