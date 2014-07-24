#pragma once

namespace prz {
    namespace utl {
        namespace ZMatrix {

            template<typename T>
            void Allocate(T*** matrix, unsigned int size) {
                *matrix = new T*[size];
                for (unsigned int i = 0; i < size; ++i) {
                    (*matrix)[i] = new T[size];
                }
            }

            template<typename T>
            void AllocateAndCopy(T*** dst, T const * const * src, unsigned int size) {
                *dst = new T*[size];
                for (unsigned int i = 0; i < size; ++i) {
                    (*dst)[i] = new T[size];
                    for (unsigned int j = 0; j < size; ++j) {
                        (*dst)[i][j] = src[i][j];
                    }
                }
            }

            template<typename T>
            void Deallocate(T*** matrix, unsigned int size) {
                for (unsigned int i = 0; i < size; ++i) {
                    delete[](*matrix)[i];
                }
                delete[](*matrix);
                *matrix = nullptr;
            }

        }
    }
}
