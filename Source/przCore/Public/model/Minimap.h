#pragma once

namespace prz {
    namespace mdl {

        namespace EMinimapCell {
            enum Type {
                Unknown,
                SolidRock,
                Empty,
                UpStaircase,
                DownStaircase,
                Player
            };
        }

        class ZMinimap {
        public:
            ZMinimap(unsigned int size, const EMinimapCell::Type* cells);
            virtual ~ZMinimap();

            unsigned int GetSize() const;
            EMinimapCell::Type GetCell(unsigned int x, unsigned int y) const;

        private:
            bool IndicesAreValid(unsigned int x, unsigned int y) const;
            unsigned int CalcLinearIndex(unsigned int x, unsigned int y) const;

            unsigned int mSize;
            EMinimapCell::Type* mCells;
        };

    }
}

