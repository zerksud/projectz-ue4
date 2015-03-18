#pragma once

namespace prz {
namespace utl {

typedef unsigned int ZIdType;

class ZRegistrable {
public:
    static const ZIdType kNoId;

    ZRegistrable();
    ZRegistrable(const ZRegistrable& other) = default;
    ZRegistrable& operator=(const ZRegistrable& other) = default;
    virtual ~ZRegistrable();

    ZIdType GetId() const;
    void SetId(ZIdType id);
    bool IsRegistered() const;

private:
    ZIdType mId;
};

}
}