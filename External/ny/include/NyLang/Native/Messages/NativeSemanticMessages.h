//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Diagnostic/Message.h>
#include <NyLang/Frontend/Symbol/Symbol.h>
#include <list>
#include <NyLang/Frontend/Symbol/SymbolTree.h>
#include <assert.h>
#include <NyLang/Native/Symbols/SymbolType.h>
#include <NyLang/Core/Assert.h>

namespace NyLang {
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageScopeInstantiation, MessageSeverity::eInfo, SymbolComponent*, symbol, stream << "Inside instantiation "; symbol->PrettyPrint(stream));
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageScopeFuncEval, MessageSeverity::eInfo, SymbolComponent*, symbol, stream << "Inside function evaluation "; symbol->PrettyPrint(stream));
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageScopeMixin, MessageSeverity::eInfo, String, mixin, stream << "Inside mixin '" << mixin << "'");

    LANG_SIMPLEMESSAGE(SemanticMessageExpectedVector, MessageSeverity::eError, stream << "Expected vector value");
    LANG_SIMPLEMESSAGE(SemanticMessageMixinNoStatic, MessageSeverity::eError, stream << "Mixin runtime scope expected outer static scope");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedVectorType, MessageSeverity::eError, stream << "Expected vector type");
    LANG_SIMPLEMESSAGE(SemanticMessageDeclExpectedType, MessageSeverity::eError, stream << "Declaration expected a type");
    LANG_SIMPLEMESSAGE(SemanticMessageStaticTypeUsedInRuntime, MessageSeverity::eError, stream << "Static types are not allowed in runtime scopes");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedPlacement, MessageSeverity::eError, stream << "Expected a placement");
    LANG_SIMPLEMESSAGE(SemanticMessageBaseNoStorage, MessageSeverity::eError, stream << "Object bases cannot have storage information");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedObjectBase, MessageSeverity::eError, stream << "Expected a base type");
    LANG_SIMPLEMESSAGE(SemanticMessageNoAccessLHS, MessageSeverity::eError, stream << "Cannot access lhs");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedCallable, MessageSeverity::eError, stream << "Expected a callable function or spawnable type");
    LANG_SIMPLEMESSAGE(SemanticMessageDuplicateVariable, MessageSeverity::eError, stream << "A variable with the same name exists in the same scope");
    LANG_SIMPLEMESSAGE(SemanticMessageDuplicateUsing, MessageSeverity::eError, stream << "Cannot alias over an existing symbol in the same scope");
    LANG_SIMPLEMESSAGE(SemanticMessageNoSuperFrame, MessageSeverity::eError, stream << "Expected a super frame (function, class, etc...)");
    LANG_SIMPLEMESSAGE(SemanticMessageNoStaticClassFrame, MessageSeverity::eError, stream << "Expected a static class super frame");
    LANG_SIMPLEMESSAGE(SemanticMessageNoMemberFrame, MessageSeverity::eError, stream << "Expected an outer object for member accessor");
    LANG_SIMPLEMESSAGE(SemanticMessagePrivateSuperFrame, MessageSeverity::eError, stream << "Super frame is inaccessible");
    LANG_SIMPLEMESSAGE(SemanticMessageSignedUnsignedMismatch, MessageSeverity::eError, stream << "Signed unsigned mismatch");
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessageArgumentCountMismatch, MessageSeverity::eError, int, expected, int, given, stream << "Expected " << expected << " arguments, but given " << given << " arguments");
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessageIntegerBitWidthMismatch, MessageSeverity::eError, int, a, int, b, stream << "Bitwidth mismatch, cannot cast an integer of bitwidth " << a << " to an integer of bitwidth " << b);
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessageFPBitWidthMismatch, MessageSeverity::eError, int, a, int, b, stream << "Bitwidth mismatch, cannot cast a floating point of bitwidth" << a << " to a floating point of bitwidth " << b);
    LANG_SIMPLEMESSAGE(SemanticMessageFailedAutoDeduce, MessageSeverity::eError, stream << "Failed to auto deduce template parameter");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedType, MessageSeverity::eError, stream << "Expected a type");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedNamed, MessageSeverity::eError, stream << "Expected a named symbol");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedDeclaration, MessageSeverity::eError, stream << "Expected a declaration");
    LANG_SIMPLEMESSAGE(SemanticMessageReturnExpectedFunctionOrScope, MessageSeverity::eError, stream << "Expected outer function or scoped block to return from");
    LANG_SIMPLEMESSAGE(SemanticMessageUnknownSymbol, MessageSeverity::eError, stream << "Could not resolve symbol '" << tok.Value.Str.Value << "'");
    LANG_SIMPLEMESSAGE(SemanticMessageIncompleteType, MessageSeverity::eError, stream << "Using an incomplete type");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedTemplatedType, MessageSeverity::eError, stream << "Expected a templated type");
    LANG_SIMPLEMESSAGE(SemanticMessageFailedTemplatedResolve, MessageSeverity::eError, stream << "Failed to resolve templated type");
    LANG_SIMPLEMESSAGE(SemanticMessageFailedFutureResolve, MessageSeverity::eError, stream << "Failed to resolve future templated type");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedTypeOrValue, MessageSeverity::eError, stream << "Expected either a type or value");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedValue, MessageSeverity::eError, stream << "Expected a value");
    LANG_SIMPLEMESSAGE(SemanticMessageConstMismatch, MessageSeverity::eError, stream << "Cannot cast const to non const");
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageRefMismatch, MessageSeverity::eError, bool, refRHS, stream << "Cannot cast " << (!refRHS ? "non ref to ref" : "ref to non ref") << ", explicit conversion is required");
    LANG_SIMPLEMESSAGE(SemanticMessageUninitializedValue, MessageSeverity::eError, stream << "Value may not be initialized");
    LANG_SIMPLEMESSAGE(SemanticMessageNoOverride, MessageSeverity::eError, stream << "No base function to override");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedFunctionOrDelegate, MessageSeverity::eError, stream << "Expected a function or a delegate value");
    LANG_SIMPLEMESSAGE(SemanticMessageNoOverrideNotVirtual, MessageSeverity::eError, stream << "Attempting to override a non virtual base function");
    LANG_SIMPLEMESSAGE(SemanticMessageExistingBaseNoOverride, MessageSeverity::eError, stream << "A virtual base function with same signature already exists, override is needed");
    LANG_SIMPLEMESSAGE(SemanticMessageBadPropertyFunction, MessageSeverity::eError, stream << "Unknown property function signature, must be either get or set format");
    LANG_SIMPLEMESSAGE(SemanticMessageValueNotTemplated, MessageSeverity::eError, stream << "Value not templated");
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageValueTypeMismatch, MessageSeverity::eError, bool, expectedType, stream << "Value type mismatch, cannot cast " << (expectedType ? "value to type" : "type to value"));
    LANG_SIMPLEMESSAGE(SemanticMessageCircularDependency, MessageSeverity::eError, stream << "Circular dependency, cannot resolve");
    LANG_SIMPLEMESSAGE(SemanticMessageAssignTemporary, MessageSeverity::eError, stream << "Attempting to assign a temporary value");
    LANG_SIMPLEMESSAGE(SemanticMessageCantAssign, MessageSeverity::eError, stream << "No assignment");
    LANG_SIMPLEMESSAGE(SemanticMessageForeachExpectedVar, MessageSeverity::eError, stream << "Foreach expression expected a control variable");
    LANG_SIMPLEMESSAGE(SemanticMessageForeachExpectedContainer, MessageSeverity::eError, stream << "Foreach expression expected a container");
    LANG_SIMPLEMESSAGE(SemanticMessageForeachContainerNotIterable, MessageSeverity::eError, stream << "Foreach container is not iterable");
    LANG_SIMPLEMESSAGE(SemanticMessageFutureNoAuto, MessageSeverity::eError, stream << "A future type required an auto scope");
    LANG_SIMPLEMESSAGE(SemanticMessageNoMatch, MessageSeverity::eError, stream << "Values are not matchable");
    LANG_SIMPLEMESSAGE(SemanticMessageNoExpand, MessageSeverity::eError, stream << "Cannot expand lhs");
    LANG_SIMPLEMESSAGE(SemanticMessageNoInterval, MessageSeverity::eError, stream << "Cannot create interval");
    LANG_SIMPLEMESSAGE(SemanticMessageTypeValueIsAuto, MessageSeverity::eError, stream << "Cannot declare type or auto from value");
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageUnimplementedFeature, MessageSeverity::eError, String, feature, stream << "Used a feature that is currently not implemented: " << feature);
    LANG_SIMPLEMESSAGE(SemanticMessageNoStaticEval, MessageSeverity::eError, stream << "Cannot perform static evaluation");
    LANG_SIMPLEMESSAGE(SemanticMessageArrayIndexerNotInt, MessageSeverity::eError, stream << "Cannot index array with non integer value");
    LANG_SIMPLEMESSAGE(SemanticMessageArrayNoop, MessageSeverity::eError, stream << "Unknown array operation, expected either value or type on lhs");
    LANG_SIMPLEMESSAGE(SemanticMessageLHSNotArray, MessageSeverity::eError, stream << "Cannot index non array or pointer type");
    LANG_SIMPLEMESSAGE(SemanticMessageNoUnaryOp, MessageSeverity::eError, stream << "No unary operation");
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageExpectedStaticValue, MessageSeverity::eError, SymbolTypeInfo*, type, {
        stream << "Expected a static value";
        if (type) {
            stream << " of type ";
            type->PrettyPrint(stream);
        }
    });
    LANG_SIMPLEMESSAGE(SemanticMessageArraySizeInvalid, MessageSeverity::eError, stream << "Array size must be greater than 0");
    LANG_SIMPLEMESSAGE(SemanticMessageArrayIndexOOB, MessageSeverity::eError, stream << "Array index out of bounds");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedBool, MessageSeverity::eError, stream << "Expected boolean value");
    LANG_SIMPLEMESSAGE(SemanticMessageStringCastInt8, MessageSeverity::eError, stream << "Invalid string cast, expected int8! type");
    LANG_SIMPLEMESSAGE(SemanticMessageUnknownArray, MessageSeverity::eError, stream << "Unknown array type, expected either a set or values or declarations");
    LANG_SIMPLEMESSAGE(SemanticMessageMismatchedArrayValue, MessageSeverity::eError, stream << "Values in array must have same types");
    LANG_SIMPLEMESSAGE(SemanticMessageReturnTypeMismatch, MessageSeverity::eError, stream << "Return type does not match function return type");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedString, MessageSeverity::eError, stream << "Expected static string value");
    LANG_SIMPLEMESSAGE(SemanticMessageNoResolveEmptyArray, MessageSeverity::eError, stream << "Could not resolve type on empty array");
    LANG_SIMPLEMESSAGE(SemanticMessageNoResolveShorthandArray, MessageSeverity::eError, stream << "Could not resolve shorthand array");
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessageShorthandArrayCountMismatch, MessageSeverity::eError, int, expected, int, given, stream << "Expected " << expected << " elements in shorthand array, but given " << given << " elements");
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessageArraySizeMismatch, MessageSeverity::eError, int, expected, int, given, stream << "Expected " << expected << " elements in array, but given " << given << " elements");
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessagePackCountMismatch, MessageSeverity::eError, int, expected, int, given, stream << "Expected " << expected << " elements in pack, but given " << given << " elements");
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageNoBuiltin, MessageSeverity::eError, String, message, stream << "No builtin with name '" << message << "'");
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageUserError, MessageSeverity::eError, String, message, stream << "[User] " << message);
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageUserWarning, MessageSeverity::eWarning, String, message, stream << "[User] " << message);
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageUserInfo, MessageSeverity::eInfo, String, message, stream << "[User] " << message);
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessageInvalidCast, MessageSeverity::eError, Symbol*, from, Symbol*, to, {
        stream << "Invalid cast from ";
        from->PrettyPrint(stream);
        stream << " to ";
        to->PrettyPrint(stream);
    });
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessageIntervalDifferentTypes, MessageSeverity::eError, Symbol*, from, Symbol*, to, {
        stream << "Cannot create an interval with different types, ";
        from->PrettyPrint(stream);
        stream << " .. ";
        to->PrettyPrint(stream);
    });
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessageIntervalCantCompare, MessageSeverity::eError, Symbol*, from, Symbol*, to, {
        stream << "Cant compare interval of type ";
        from->PrettyPrint(stream);
        stream << " to value of type ";
        to->PrettyPrint(stream);
    });
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessageFailedTemplateSubstitution, MessageSeverity::eError, Symbol*, from, Symbol*, to, {
        stream << "Invalid template substitution from ";
        from->PrettyPrint(stream);
        stream << " to ";
        to->PrettyPrint(stream);
    });
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageNoAccessSymbolCandidates, MessageSeverity::eError, std::list<SymbolSearch<>>, candidates, {
        // Mode?
        Assert(candidates.size() > 0, "SemanticMessageNoAccessSymbolCandidates expects candidates");
        if (candidates.size() > 1) {
            stream << "Symbol candidates:";
        }

        // Compose message
        stream.Push();
        for (SymbolSearch<> &search : candidates) {
            if (candidates.size() > 1) {
                stream << "\n";
            }

            stream << "Cannot access ";
            search.pair.symbol->PrettyPrint(stream);
            stream << " with remote ";
            switch (search.remoteVisibility) {
                case SymbolVisibility::ePublic:stream << "visibility 'public'"; break;
                case SymbolVisibility::eProtected:stream << "visibility 'protected'"; break;
                case SymbolVisibility::ePrivate:stream << "visibility 'private'"; break;
                case SymbolVisibility::eInaccessible:stream << "external visibility 'private'"; break;
            }
            stream << " (And local visibility ";
            switch (search.visibility) {
                case SymbolVisibility::ePublic:stream << "'public'"; break;
                case SymbolVisibility::eProtected:stream << "'protected'"; break;
                case SymbolVisibility::ePrivate:stream << "'private'"; break;
            }
            stream << ")";
        }
        stream.Pop();
    });

    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageAmbiguousSymbol, MessageSeverity::eError, std::list<Symbol*>, symbols, {
        stream << "Ambiguous symbol, candidates:";

        // ...
        stream.Push();
        for (auto &&symbol : symbols) {
            stream << "\n";
            symbol->PrettyPrint(stream);
        }
        stream.Pop();
    });

    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageUninitializedSymbols, MessageSeverity::eError, std::list<Symbol*>, symbols, {
        stream << "All candidate symbols are uninitialized:";

        // ...
        stream.Push();
        for (auto &&symbol : symbols) {
            stream << "\n";
            symbol->PrettyPrint(stream);
        }
        stream.Pop();
    });

    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageNoFrameSymbols, MessageSeverity::eError, std::list<Symbol*>, symbols, {
        stream << "All candidate symbols require an unavailable super frame:";

        // ...
        stream.Push();
        for (auto &&symbol : symbols) {
            stream << "\n";
            symbol->PrettyPrint(stream);
        }
        stream.Pop();
    });

}
