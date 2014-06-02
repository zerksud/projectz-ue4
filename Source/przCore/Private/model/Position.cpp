#include "przCorePrivatePCH.h"
#include "model/Position.h"

#include "utils/StringHelpers.h"

namespace prz {
    namespace mdl {
        ZPositionDiff::ZPositionDiff() {
            mdX = 0;
            mdY = 0;
        }

        ZPositionDiff::ZPositionDiff(int dXValue, int dYValue) {
            mdX = dXValue;
            mdY = dYValue;
        }

        ZPositionDiff::~ZPositionDiff() {
        }

        int ZPositionDiff::GetdX() const {
            return mdX;
        }

        int ZPositionDiff::GetdY() const {
            return mdY;
        }

        std::string ZPositionDiff::ToString() const {
            return utl::ZString::Format("[%d;%d]", mdX, mdY);
        }

        ZPosition::ZPosition() {
            mX = 0;
            mY = 0;
        }

        ZPosition::ZPosition(const ZPosition& other) {
            mX = other.mX;
            mY = other.mY;
        }

        ZPosition::ZPosition(int xValue, int yValue) {
            mX = xValue;
            mY = yValue;
        }

        ZPosition::~ZPosition() {
        }

        int ZPosition::GetX() const {
            return mX;
        }

        int ZPosition::GetY() const {
            return mY;
        }

        const ZPosition ZPosition::operator+(const ZPositionDiff& diff) const {
            int x = mX + diff.GetdX();
            int y = mY + diff.GetdY();

            return ZPosition(x, y);
        }

        const ZPositionDiff ZPosition::operator -(const ZPosition& other) const {
            int dx = mX - other.mX;
            int dy = mY - other.mY;

            return ZPositionDiff(dx, dy);
        }

        bool ZPosition::operator ==(const ZPosition& other) const {
            return (mX == other.mX && mY == other.mY);
        }

        ZPosition& ZPosition::operator=(const ZPosition& other) {
            mX = other.mX;
            mY = other.mY;

            return *this;
        }

        std::string ZPosition::ToString() const {
            return utl::ZString::Format("[%d;%d]", mX, mY);
        }

    }
}
