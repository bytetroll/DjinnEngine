// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

// @Nate: This is a "partially" STL compliant iterator base class since C++ 17 marks the potential end of iterator
//        support.  This iterator class implements the basic functionality of the C++ STL "iterator" class,
//        but will not work with std::iterator_traits<> as this is not a compiler-backed iterator.
//        In the event that this needs to be a compiler-backed (which may become useless if and when iterator
//        support is removed from the language), the fields of this class need to be renamed as the current
//        iterator specification demands specific names (which are used by the compiler).  If one or more of these
//        names are wrong, the compiler will default this to an empty struct.
//
//                  Compliant Names
//                  -----------------------------------------------------------------
//                      <CategoryType>      iterator_category
//                      <ValueType>         value_type
//                      <PointerDiffType>   difference_type
//                      <PointerType>       pointer
//                      <ReferenceType>     reference

template< class CategoryType, class ValueType, class PointerDifferenceType, class PointerType = ValueType*,
          class ReferenceType = ValueType& >
struct IteratorBase {
public:
    using Category = CategoryType;
    using Value = ValueType;
    using PointerDifference = PointerDifferenceType;

    using Pointer = PointerType;
    using Reference = ReferenceType;
};