//
// Created by Dev on 10/20/2017.
//
#pragma once

#include <Core/Assert.h>

namespace Djinn::Core {
#define TEST_EXPECTED(X, ...) {if (!(X)) { DJINN_THROW Djinn::EAssertionFailed(); }}
#define TEST_EXPECTED_FALSE(X, ...) {if ((X) != false) { DJINN_THROW Djinn::EAssertionFailed(); }}
}