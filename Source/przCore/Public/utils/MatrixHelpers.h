#pragma once

namespace prz {
    namespace utl {
        namespace ZMatrix {

            template<typename T>
            void Allocate(T*** matrix, unsigned int width, unsigned int height) {
                *matrix = new T*[height];
                for (unsigned int i = 0; i < height; ++i) {
                    (*matrix)[i] = new T[width];
                }
            }

            template<typename T>
            void Allocate(T*** matrix, unsigned int size) {
                Allocate<T>(matrix, size, size);
            }

            template<typename T>
            void AllocateAndCopy(T*** dst, T const * const * src, unsigned int width, unsigned int height) {
                *dst = new T*[height];
                for (unsigned int i = 0; i < height; ++i) {
                    (*dst)[i] = new T[width];
                    for (unsigned int j = 0; j < width; ++j) {
                        (*dst)[i][j] = src[i][j];
                    }
                }
            }

            template<typename T>
            void AllocateAndCopy(T*** dst, T const * const * src, unsigned int size) {
                AllocateAndCopy<T>(dst, src, size, size);
            }

            template<typename T>
            void Deallocate(T*** matrix, unsigned int height) {
                for (unsigned int i = 0; i < height; ++i) {
                    delete[](*matrix)[i];
                }
                delete[](*matrix);
                *matrix = nullptr;
            }

        }
    }
}
