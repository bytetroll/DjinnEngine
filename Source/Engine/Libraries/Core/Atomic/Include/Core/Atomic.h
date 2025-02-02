//
// Created by Dev on 9/3/2016.
//

#pragma once

#include <Core/Types.h>

namespace Djinn {
    namespace Core {
        enum class AtomicOperation {
            RELAXED, // No ordering constraints
            CONSUME, // TODO: Description
            ACQUIRE, // TODO: Description
            RELEASE, // TODO: Description
            ACQ_REL, // TODO: Description
            SEQ_CST, // TODO: Description
        };

        // Load from this atomic
        template<typename T>
        static T AtomicLoad(const T& value, AtomicOperation op = AtomicOperation::SEQ_CST) {
            T tmp;
            __atomic_load(&value, &tmp, static_cast<int>(op));
            return tmp;
        }

        // Store to this atomic
        template<typename T>
        static  T AtomicStore(T& dest, const T &val, AtomicOperation op = AtomicOperation::SEQ_CST) {
            T copy = val;
            __atomic_store(&dest, &copy, static_cast<int>(op));
            return copy;
        }

        // Exchange to this atomic
        template<typename T>
        static T AtomicExchange(T& dest, const T &newVal, AtomicOperation op = AtomicOperation::SEQ_CST) {
            T copy = newVal;
            return __atomic_exchange_n(&dest, copy, static_cast<int>(op));
        }

        // Compare exchange to this atomic
        template<typename T>
        static bool AtomicCmpExchange(T& dest, const T &cmpVal, const T &newVal, AtomicOperation op = AtomicOperation::SEQ_CST) {
            T cmpCopy = cmpVal;
            T copy = newVal;
            return __atomic_compare_exchange_n(&dest, &cmpCopy, copy, false, static_cast<int>(op), static_cast<int>(op));
        }

        // Compare exchange to this atomic
        template<typename T>
        static bool AtomicCmpExchangeWeak(T& dest, const T &cmpVal, const T &newVal, AtomicOperation op = AtomicOperation::SEQ_CST) {
            T cmpCopy = cmpVal;
            T copy = newVal;
            return __atomic_compare_exchange_n(&dest, &cmpCopy, copy, true, static_cast<int>(op), static_cast<int>(op));
        }

        // ...
        template<typename T>
        static  T AtomicIncrement(T& dest, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_add_fetch(&dest, 1, static_cast<int>(op));
        }

        // ...
        template<typename T>
        static  T AtomicDecrement(T& dest, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_sub_fetch(&dest, 1, static_cast<int>(op));
        }

        // Add to this atomic
        template<typename T>
        static  T AtomicAddFetch(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_add_fetch(&dest, amount, static_cast<int>(op));
        }

        // Sub to this atomic
        template<typename T>
        static  T AtomicSubFetch(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_sub_fetch(&dest, amount, static_cast<int>(op));
        }

        // And on this atomic
        template<typename T>
        static  T AtomicAndFetch(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_and_fetch(&dest, amount, static_cast<int>(op));
        }

        // XOr on this atomic
        template<typename T>
        static T AtomicXOrFetch(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_xor_fetch(&dest, amount, static_cast<int>(op));
        }

        // Or on this atomic
        template<typename T>
        static T AtomicOrFetch(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_or_fetch(&dest, amount, static_cast<int>(op));
        }

        // NAnd on this atomic
        template<typename T>
        static  T AtomicNAndFetch(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_nand_fetch(&dest, amount, static_cast<int>(op));
        }

        // Add to this atomic
        template<typename T>
        static  T AtomicFetchAdd(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_fetch_add(&dest, amount, static_cast<int>(op));
        }

        // Sub to this atomic
        template<typename T>
        static  T AtomicFetchSub(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_fetch_sub(&dest, amount, static_cast<int>(op));
        }

        // And on this atomic
        template<typename T>
        static  T AtomicFetchAnd(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_fetch_and(&dest, amount, static_cast<int>(op));
        }

        // XOr on this atomic
        template<typename T>
        static  T AtomicFetchXOr(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_fetch_xor(&dest, amount, static_cast<int>(op));
        }

        // Or on this atomic
        template<typename T>
        static  T AtomicFetchOr(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_fetch_or(&dest, amount, static_cast<int>(op));
        }

        // NAnd on this atomic
        template<typename T>
        static  T AtomicFetchNAnd(T& dest, T amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
            return __atomic_fetch_nand(&dest, amount, static_cast<int>(op));
        }

        template<class T>
        class Atomic {
        public:
            Atomic(const T &value = T()) : value(value) {
                // ...
            }

            // Load from this atomic
            operator T() {
                return Load();
            }

            // Store to this atomic
            T operator=(const T &val) {
                return Store(val);
            }

            // Load from this atomic
            T Load(AtomicOperation op = AtomicOperation::SEQ_CST) const {
                T tmp;
                __atomic_load(&value, &tmp, static_cast<int>(op));
                return tmp;
            }

            // Store to this atomic
            T Store(const T &val, AtomicOperation op = AtomicOperation::SEQ_CST) {
                T copy = val;
                __atomic_store(&value, &copy, static_cast<int>(op));
                return copy;
            }

            // Exchange to this atomic
            T Exchange(const T &newVal, AtomicOperation op = AtomicOperation::SEQ_CST) {
                T copy = newVal;
                return __atomic_exchange_n(&value, copy, static_cast<int>(op));
            }

            // Compare exchange to this atomic
            bool CmpExchange(const T &cmpVal, const T &newVal, AtomicOperation op = AtomicOperation::SEQ_CST) {
                T cmpCopy = cmpVal;
                T copy = newVal;
                return __atomic_compare_exchange_n(&value, &cmpCopy, copy, false, static_cast<int>(op), static_cast<int>(op));
            }

            // Compare exchange to this atomic
            bool CmpExchangeWeak(const T &cmpVal, const T &newVal, AtomicOperation op = AtomicOperation::SEQ_CST) {
                T cmpCopy = cmpVal;
                T copy = newVal;
                return __atomic_compare_exchange_n(&value, &cmpCopy, copy, true, static_cast<int>(op), static_cast<int>(op));
            }

        protected:
            T value;
        };

        class AtomicCounter : public Atomic<Int64> {
        public:
            AtomicCounter(Int64 value = Int64()) : Atomic(value) {
                // ...
            }

            // Store to this atomic
            Int64 operator=(const Int64 &val) {
                return Store(val);
            }

            // Assignment Operators
            Int64 operator+=(Int64 amount) { return AddFetch(amount); }

            Int64 operator-=(Int64 amount) { return SubFetch(amount); }

            Int64 operator&=(Int64 amount) { return AndFetch(amount); }

            Int64 operator%=(Int64 amount) { return XOrFetch(amount); }

            Int64 operator|=(Int64 amount) { return OrFetch(amount); }

            Int64 operator^=(Int64 amount) { return NAndFetch(amount); }

            // Step Operators
            Int64 operator++() { return AddFetch(1); }

            Int64 operator--() { return SubFetch(1); }

            Int64 operator++(int) { return FetchAdd(1); }

            Int64 operator--(int) { return FetchSub(1); }

            // Add to this atomic
            Int64 AddFetch(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_add_fetch(&value, amount, static_cast<int>(op));
            }

            // Sub to this atomic
            Int64 SubFetch(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_sub_fetch(&value, amount, static_cast<int>(op));
            }

            // And on this atomic
            Int64 AndFetch(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_and_fetch(&value, amount, static_cast<int>(op));
            }

            // XOr on this atomic
            Int64 XOrFetch(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_xor_fetch(&value, amount, static_cast<int>(op));
            }

            // Or on this atomic
            Int64 OrFetch(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_or_fetch(&value, amount, static_cast<int>(op));
            }

            // NAnd on this atomic
            Int64 NAndFetch(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_nand_fetch(&value, amount, static_cast<int>(op));
            }

            // Add to this atomic
            Int64 FetchAdd(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_fetch_add(&value, amount, static_cast<int>(op));
            }

            // Sub to this atomic
            Int64 FetchSub(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_fetch_sub(&value, amount, static_cast<int>(op));
            }

            // And on this atomic
            Int64 FetchAnd(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_fetch_and(&value, amount, static_cast<int>(op));
            }

            // XOr on this atomic
            Int64 FetchXOr(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_fetch_xor(&value, amount, static_cast<int>(op));
            }

            // Or on this atomic
            Int64 FetchOr(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_fetch_or(&value, amount, static_cast<int>(op));
            }

            // NAnd on this atomic
            Int64 FetchNAnd(Int64 amount, AtomicOperation op = AtomicOperation::SEQ_CST) {
                return __atomic_fetch_nand(&value, amount, static_cast<int>(op));
            }
        };
    }
}