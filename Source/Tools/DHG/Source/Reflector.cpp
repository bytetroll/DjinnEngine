//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 5/28/2018.
//

#include <clang-c/Index.h>
#include <cstring>
#include <DHG/Reflector.h>
#include <DHG/MetaData.h>
#include <Core/Memory.h>
#include <iostream>

DJINN_NS2(DHG);

Reflector::Reflector(MetaData *md) : md(md) {

}

void Reflector::Reflect(ParserBlob *blob, const Core::String &name) {
    visitedIncludes.Clear();
    ReflectIncludeEdges(blob, clang_getFile(blob->unit, name.AsCharArray()), name.AsCharArray());
    Reflect(blob, clang_getTranslationUnitCursor(blob->unit));
}

void Reflector::ReflectIncludeEdges(ParserBlob *blob, CXFile file, const char *from) {
    auto functor = [&](CXCursor cur, CXSourceRange range) {
        const char *name = clang_getCString(clang_getFileName(clang_getIncludedFile(cur)));
        if (!name) {
            name = "_missing_";
        }
        md->includeEdges.Add({from, name});
        if (!visitedIncludes.Contains(name)) {
            visitedIncludes.Add(name);
            ReflectIncludeEdges(blob, clang_getFile(blob->unit, name), name);
        }
        return CXVisit_Continue;
    };

    CXCursorAndRangeVisitor visitor;
    visitor.context = &functor;
    visitor.visit = [](void *context, CXCursor cur, CXSourceRange range) {
        return (*reinterpret_cast<decltype(functor) * > (context))(cur, range);
    };
    clang_findIncludesInFile(blob->unit, file, visitor);
}

void Reflector::Reflect(ParserBlob *blob, CXCursor root) {
    CX_CXXAccessSpecifier specifier = CX_CXXPublic;

    // Visit all children
    Visit(root, [&](CXCursor c) {
        switch (c.kind) {
            case CXCursor_UsingDirective: {
                Visit(c, [&](CXCursor itCursor) {
                    switch (itCursor.kind) {
                        case CXCursor_NamespaceRef: {
                            // Get location
                            auto loc = clang_getCursorLocation(itCursor);

                            // Only reflect alias in given header
                            if (clang_Location_isFromMainFile(loc)) {
                                auto name = clang_getCString(clang_getCursorSpelling(itCursor));
                                //md->namespaceAliases.Add(name);
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    return CXChildVisit_Recurse;
                });
                break;
            }
            case CXCursor_ClassTemplate:
            case CXCursor_FunctionTemplate: {
                return CXChildVisit_Continue;
            }
            case CXCursor_CXXAccessSpecifier: {
                auto loc = clang_getCursorLocation(c);
                if (clang_Location_isFromMainFile(loc)) {
                    specifier = clang_getCXXAccessSpecifier(c);
                }
                break;
            }
            case CXCursor_StructDecl:
            case CXCursor_UnionDecl:
            case CXCursor_ClassDecl: {
                // Ignore inaccessible and template classes
                if (specifier == CX_CXXProtected || specifier == CX_CXXPrivate) {
                    return CXChildVisit_Continue;
                }

                // New accessor
                auto previousSpecifier = specifier;
                if (c.kind == CXCursor_StructDecl || c.kind == CXCursor_UnionDecl) {
                    specifier = CX_CXXPublic;
                } else {
                    specifier = CX_CXXPrivate;
                }

                // Use canonical version
                // c = clang_getCanonicalCursor(c);

                // Ignore forward declarations
                if (clang_equalCursors(clang_getCursorDefinition(c), clang_getNullCursor())) {
                    specifier = previousSpecifier;
                    return CXChildVisit_Recurse;
                }

                // Ignore anonymous names
                if (clang_Cursor_isAnonymous(c)) {
                    specifier = previousSpecifier;

                    // Any sub class is also ignored
                    return CXChildVisit_Continue;
                }

                // Get location
                auto loc = clang_getCursorLocation(c);

                // Only reflect classes in given header
                if (!clang_Location_isFromMainFile(loc)) {
                    // May be a macro expansion, get expansion location instead
                    CXFile file;
                    unsigned line;
                    unsigned column;
                    unsigned offset;
                    clang_getExpansionLocation(clang_getCursorLocation(c), &file, &line, &column, &offset);
                    auto expandedLoc = clang_getLocation(clang_Cursor_getTranslationUnit(c), file, line, column);

                    if (!clang_Location_isFromMainFile(expandedLoc)) {
                        clang_getFileLocation(clang_getCursorLocation(c), &file, &line, &column, &offset);

                        specifier = previousSpecifier;
                        return CXChildVisit_Recurse;
                    }
                }

                // Reflect class
                ReflectClass(blob, c, specifier);
                Reflect(blob, c);

                specifier = previousSpecifier;
                return CXChildVisit_Continue;
            }
            case CXCursor_EnumDecl: {
                // Ignore inaccessible and template enums
                if (specifier == CX_CXXProtected || specifier == CX_CXXPrivate) {
                    return CXChildVisit_Continue;
                }

                // Ignore anonymous names
                if (clang_Cursor_isAnonymous(c)) {
                    // Any sub enum is also ignored
                    return CXChildVisit_Continue;
                }

                // Get location
                auto loc = clang_getCursorLocation(c);

                // Only reflect enums in given header
                if (!clang_Location_isFromMainFile(loc)) {
                    // May be a macro expansion, get expansion location instead
                    CXFile file;
                    unsigned line;
                    unsigned column;
                    unsigned offset;
                    clang_getExpansionLocation(clang_getCursorLocation(c), &file, &line, &column, &offset);
                    auto expandedLoc = clang_getLocation(clang_Cursor_getTranslationUnit(c), file, line, column);

                    if (!clang_Location_isFromMainFile(expandedLoc)) {
                        return CXChildVisit_Recurse;
                    }
                }

                // Ignore forward declarations
                if (clang_equalCursors(clang_getCursorDefinition(c), clang_getNullCursor())) {
                    return CXChildVisit_Recurse;
                }

                // Reflect enum
                ReflectEnum(blob, c, specifier);
                Reflect(blob, c);

                return CXChildVisit_Continue;
            }
            default:
                break;
        }
        return CXChildVisit_Recurse;
    });
}

void Reflector::ReflectEnum(ParserBlob *blob, CXCursor enumCursor, CX_CXXAccessSpecifier access) {
    auto name = clang_getCString(clang_getCursorDisplayName(enumCursor));


    // Prepare class
    auto _enum = new MetaEnum();
    _enum->name = name;
    _enum->namespaceName = GetTypeName(clang_getCursorType(enumCursor));
    _enum->annotation = classAnnotations;
    md->enums.Add(_enum);

    if (!ReflectType(clang_getEnumDeclIntegerType(enumCursor), _enum->type)) {
        return;
    }

    // Dump
    classAnnotations = {};

    // Visit all
    Visit(enumCursor, [&](CXCursor itCursor) {
        switch (itCursor.kind) {
            case CXCursor_EnumConstantDecl: {
                // Ignore static and inaccessible fields
                if (access == CX_CXXProtected || access == CX_CXXPrivate) {
                    break;
                }

                MetaEnumMember member;
                member.name = clang_getCString(clang_getCursorSpelling(itCursor));
                member.value = clang_getEnumConstantDeclValue(itCursor);
                member.annotation = ReflectAnnotation(itCursor);

                if (member.annotation.ignore) {
                    break;
                }

                _enum->members.Add(member);
                break;
            }
            default:
                break;
        }
        return CXChildVisit_Continue;
    });
}

void Reflector::ReflectClass(ParserBlob *blob, CXCursor classCursor, CX_CXXAccessSpecifier access) {
    auto name = clang_getCString(clang_getCursorDisplayName(classCursor));

    // TODO: Some classes are not marked as anonymous, why?
    if (std::strcmp(name, "") == 0) {
        return;
    }

    // Ignore cursors that are referenced mutliple times
    if (md->reflectedMap.Contains(classCursor))
        return;

    md->reflectedMap.Add(classCursor);

    // Class annotations?
    auto attrPrefix = "__DHG_ClassAttr";
    if (std::strncmp(name, attrPrefix, std::strlen(attrPrefix)) == 0) {
        classAnnotations = ReflectAnnotation(classCursor);
        return;
    }

    // Skip?
    if (classAnnotations.ignore)
        return;

    // Prepare class
    auto _class = new MetaClass();
    _class->name = name;
    _class->namespaceName = GetTypeName(clang_getCursorType(classCursor));
    _class->annotation = classAnnotations;
    md->classes.Add(_class);

    // Dump
    classAnnotations = {};

    // Determine bases
    Visit(classCursor, [&](CXCursor itCursor) {
        switch (itCursor.kind) {
            case CXCursor_CXXBaseSpecifier: {
                _class->bases.Add(clang_getCString(clang_getTypeSpelling(clang_getCursorType(itCursor))));
                break;
            }
            default:
                break;
        }
        return CXChildVisit_Continue;
    });

    // Visit all
    Visit(classCursor, [&](CXCursor itCursor) {
        switch (itCursor.kind) {
            case CXCursor_CXXAccessSpecifier: {
                access = clang_getCXXAccessSpecifier(itCursor);
                break;
            }
            case CXCursor_FieldDecl: {
                // Ignore static and inaccessible fields
                if (access == CX_CXXProtected || access == CX_CXXPrivate) {
                    break;
                }

                MetaMember member;
                if (!ReflectType(clang_getCursorType(itCursor), member.type)) {
                    break;
                }
                member.name = clang_getCString(clang_getCursorDisplayName(itCursor));
                member.annotation = ReflectAnnotation(itCursor);

                if (member.annotation.ignore) {
                    break;
                }

                _class->members.Add(member);
                break;
            }
            case CXCursor_CXXMethod: {
                // Ignore static and inaccessible methods
                if (clang_CXXMethod_isStatic(itCursor) || access == CX_CXXProtected || access == CX_CXXPrivate) {
                    break;
                }

                // Ignore deleted methods
                auto availability = clang_getCursorAvailability(itCursor);
                if (availability == CXAvailability_NotAvailable || availability == CXAvailability_NotAccessible) {
                    break;
                }

                MetaMethod method;
                if (!ReflectType(clang_getCursorResultType(itCursor), method.returnType)) {
                    break;
                }
                method.annotation = ReflectAnnotation(itCursor);
                method.isConst = static_cast<bool>(clang_CXXMethod_isConst(itCursor));

                if (method.annotation.ignore) {
                    break;
                }

                const char *name = clang_getCString(clang_getCursorDisplayName(itCursor));
                for (std::size_t i = 0, counter = 1u + (std::strstr(name, "operator()(") == name);
                     i < std::strlen(name); i++) {
                    if (name[i] == '(' && (--counter) == 0) {
                        char *_name = new char[i + 1];
                        _name[i] = 0;
                        std::memcpy(_name, name, i);
                        method.name = _name;
                        break;
                    }
                }

                if (!method.name) {
                    method.name = name;
                }

                bool ok = true;
                Visit(itCursor, [&](CXCursor paramCursor) {
                    switch (paramCursor.kind) {
                        case CXCursor_ParmDecl: {
                            method.parameters.Add(MetaParameter());
                            auto &param = method.parameters.Last();
                            if (!ReflectType(clang_getCursorType(paramCursor), param.type)) {
                                ok = false;
                                break;
                            }
                            param.name = clang_getCString(clang_getCursorDisplayName(paramCursor));
                            break;
                        }
                        default:
                            break;
                    }
                    return CXChildVisit_Continue;
                });
                if (!ok) {
                    break;
                }

                _class->methods.Add(method);
                break;
            }
            default:
                break;
        }
        return CXChildVisit_Continue;
    });
}

template<typename F>
void Reflector::Visit(CXCursor cursor, F &&action) {
    clang_visitChildren(
            cursor,
            [](CXCursor c, CXCursor parent, CXClientData user) {
                return (*static_cast<F *>(user))(c);
            },
            &action
    );
}

bool Reflector::ReflectType(CXType cxType, MetaType &type) {
    type = {};
    type.namespaceName = GetTypeName(cxType);

    for (;;) {
        type.layers[type.layerCount].isConst = static_cast<bool>(clang_isConstQualifiedType(cxType));

        while (cxType.kind == CXType_Typedef) {
            cxType = clang_getCanonicalType(cxType);
        }

        switch (cxType.kind) {
            default:
                std::cerr << "Unknown type kind: " << cxType.kind << "\n";
                return false;
            case CXType_Auto:
                cxType = clang_getCanonicalType(cxType);
                break;
            case CXType_Unexposed:
                cxType = clang_getCanonicalType(cxType);
                break;
            case CXType_Void:
                type.layers[type.layerCount++].type = MetaType::Layer::eVoid;
                return true;
            case CXType_Bool:
                type.layers[type.layerCount++].type = MetaType::Layer::eBool;
                return true;
            case CXType_Char_U:
                type.layers[type.layerCount++].type = MetaType::Layer::eUChar;
                return true;
            case CXType_UChar:
                type.layers[type.layerCount++].type = MetaType::Layer::eUChar;
                return true;
            case CXType_UShort:
                type.layers[type.layerCount++].type = MetaType::Layer::eUShort;
                return true;
            case CXType_UInt:
                type.layers[type.layerCount++].type = MetaType::Layer::eUInt;
                return true;
            case CXType_ULong:
                type.layers[type.layerCount++].type = MetaType::Layer::eULong;
                return true;
            case CXType_ULongLong:
                type.layers[type.layerCount++].type = MetaType::Layer::eULongLong;
                return true;
            case CXType_Char_S:
                type.layers[type.layerCount++].type = MetaType::Layer::eChar;
                return true;
            case CXType_SChar:
                type.layers[type.layerCount++].type = MetaType::Layer::eChar;
                return true;
            case CXType_WChar:
                type.layers[type.layerCount++].type = MetaType::Layer::eWChar;
                return true;
            case CXType_Short:
                type.layers[type.layerCount++].type = MetaType::Layer::eShort;
                return true;
            case CXType_Int:
                type.layers[type.layerCount++].type = MetaType::Layer::eInt;
                return true;
            case CXType_Long:
                type.layers[type.layerCount++].type = MetaType::Layer::eLong;
                return true;
            case CXType_LongLong:
                type.layers[type.layerCount++].type = MetaType::Layer::eLongLong;
                return true;
            case CXType_Float:
                type.layers[type.layerCount++].type = MetaType::Layer::eFloat;
                return true;
            case CXType_Double:
                type.layers[type.layerCount++].type = MetaType::Layer::eDouble;
                return true;
            case CXType_Pointer:
                type.layers[type.layerCount++].type = MetaType::Layer::ePointer;
                cxType = clang_getPointeeType(cxType);
                break;
            case CXType_BlockPointer:
                type.layers[type.layerCount++].type = MetaType::Layer::ePointer;
                cxType = clang_getPointeeType(cxType);
                break;
            case CXType_LValueReference:
                type.layers[type.layerCount++].type = MetaType::Layer::eRef;
                cxType = clang_getPointeeType(cxType);
                break;
            case CXType_RValueReference:
                type.layers[type.layerCount++].type = MetaType::Layer::eRef;
                cxType = clang_getPointeeType(cxType);
                break;
            case CXType_FunctionProto:
                type.layers[type.layerCount].type = MetaType::Layer::eFPtr;
                type.layers[type.layerCount].fptr.isConst = static_cast<bool>(clang_isConstQualifiedType(cxType));
                type.layers[type.layerCount].fptr.isShorthand = static_cast<bool>(!clang_isPODType(cxType));
                if (!ReflectType(clang_getResultType(cxType),
                                 *(type.layers[type.layerCount].fptr.returnType = new MetaType()))) {
                    return false;
                }
                if (CXType _classType;
                        (_classType = clang_Type_getClassType(cxType)).kind != CXType_Invalid) {
                    if (!ReflectType(_classType, *(type.layers[type.layerCount].fptr.classType = new MetaType()))) {
                        return false;
                    }
                } else {
                    type.layers[type.layerCount].fptr.classType = nullptr;
                }
                type.layers[type.layerCount].fptr.argumentCount = static_cast<UInt32>(clang_getNumArgTypes(cxType));
                type.layers[type.layerCount].fptr.arguments = new MetaType[type.layers[type.layerCount].fptr.argumentCount];
                for (UInt32 i = 0; i < type.layers[type.layerCount].fptr.argumentCount; i++) {
                    if (!ReflectType(clang_getArgType(cxType, i), type.layers[type.layerCount].fptr.arguments[i])) {
                        return false;
                    }
                }
                type.layerCount++;
                return true;
            case CXType_Enum:
            case CXType_Record:
            case CXType_Elaborated: {
                auto name = clang_getCString(clang_getCursorDisplayName(clang_getTypeDeclaration(cxType)));

                auto _class = clang_getTypeDeclaration(cxType);

                type.layers[type.layerCount].id.name = clang_getCString(clang_getCursorDisplayName(_class));
                type.layers[type.layerCount].id.namespaceName = GetTypeName(cxType);

                type.layers[type.layerCount].type = MetaType::Layer::eID;
                /*if (int argCount; (argCount = clang_Type_getNumTemplateArguments(cxType)) != -1 && _class.kind != CXCursor_TypeAliasDecl) {
                    *std::strchr(type.layers[type.layerCount].id.name, '<') = 0;
                    *std::strchr(type.layers[type.layerCount].id.namespaceName, '<') = 0;
                    type.layers[type.layerCount].id.templateArgumentCount = static_cast<UInt32>(argCount);
                    type.layers[type.layerCount].id.templateArguments = new TemplateArgument[type.layers[type.layerCount].id.templateArgumentCount];
                    for (int i = 0; i < argCount; i++) {
                        switch (clang_(_class, static_cast<UInt32>(i))) {
                            case CXTemplateArgumentKind_Null:
                            case CXTemplateArgumentKind_Invalid:
                                break;
                            case CXTemplateArgumentKind_Type:
                            case CXTemplateArgumentKind_Template:
                            case CXTemplateArgumentKind_TemplateExpansion:
                            case CXTemplateArgumentKind_Pack: {
                                auto argType = clang_Cursor_getTemplateArgumentType(_class, static_cast<UInt32>(i));
                                if (!ReflectType(argType, type.layers[type.layerCount].id.templateArguments[i].type)) {
                                    return false;
                                }
                                return false;
                            }
                            case CXTemplateArgumentKind_NullPtr:
                            case CXTemplateArgumentKind_Integral:
                            case CXTemplateArgumentKind_Expression:
                            case CXTemplateArgumentKind_Declaration: {
                                long long argValue = clang_Cursor_getTemplateArgumentValue(_class, static_cast<UInt32>(i));
                                type.layers[type.layerCount].id.templateArguments[i].isValue = true;
                                type.layers[type.layerCount].id.templateArguments[i].value = argValue;
                                return false;
                            }
                        }
                    }
                    type.layerCount++;
                }
                if (int argCount; (argCount = clang_Cursor_getNumTemplateArguments(_class)) != -1 && _class.kind != CXCursor_TypeAliasDecl) {
                    *std::strchr(type.layers[type.layerCount].id.name, '<') = 0;
                    *std::strchr(type.layers[type.layerCount].id.namespaceName, '<') = 0;
                    type.layers[type.layerCount].id.templateArgumentCount = static_cast<UInt32>(argCount);
                    type.layers[type.layerCount].id.templateArguments = new TemplateArgument[type.layers[type.layerCount].id.templateArgumentCount];
                    for (int i = 0; i < argCount; i++) {
                        switch (clang_Cursor_getTemplateArgumentKind(_class, static_cast<UInt32>(i))) {
                            case CXTemplateArgumentKind_Null:
                            case CXTemplateArgumentKind_Invalid:
                                break;
                            case CXTemplateArgumentKind_Type:
                            case CXTemplateArgumentKind_Template:
                            case CXTemplateArgumentKind_TemplateExpansion:
                            case CXTemplateArgumentKind_Pack: {
                                auto argType = clang_Cursor_getTemplateArgumentType(_class, static_cast<UInt32>(i));
                                if (!ReflectType(argType, type.layers[type.layerCount].id.templateArguments[i].type)) {
                                    return false;
                                }
                                return false;
                            }
                            case CXTemplateArgumentKind_NullPtr:
                            case CXTemplateArgumentKind_Integral:
                            case CXTemplateArgumentKind_Expression:
                            case CXTemplateArgumentKind_Declaration: {
                                long long argValue = clang_Cursor_getTemplateArgumentValue(_class, static_cast<UInt32>(i));
                                type.layers[type.layerCount].id.templateArguments[i].isValue = true;
                                type.layers[type.layerCount].id.templateArguments[i].value = argValue;
                                return false;
                            }
                        }
                    }
                    type.layerCount++;
                } else*/ {
                    type.layers[type.layerCount].id.templateArgumentCount = 0;
                    type.layerCount++;
                }
                return true;
            }
            case CXType_ConstantArray:
                type.layers[type.layerCount].type = MetaType::Layer::eArray;
                type.layers[type.layerCount++].arrayDimension = clang_getArraySize(cxType);
                cxType = clang_getArrayElementType(cxType);
                break;
        }
    }
}

MetaAnnotation Reflector::ReflectAnnotation(CXCursor cursor) {
    MetaAnnotation annot = {};

    Visit(cursor, [&](CXCursor child) {
        switch (child.kind) {
            case CXCursor_UnexposedAttr:
            case CXCursor_IBActionAttr:
            case CXCursor_IBOutletAttr:
            case CXCursor_IBOutletCollectionAttr:
            case CXCursor_AnnotateAttr:
            case CXCursor_AsmLabelAttr:
            case CXCursor_PackedAttr:
            case CXCursor_NoDuplicateAttr:
            case CXCursor_VisibilityAttr: {
                const char *str = clang_getCString(clang_getCursorDisplayName(child));

                if (std::strcmp(str, "Ignore") == 0) {
                    annot.ignore = true;
                }

                auto len = std::strlen(str);
                auto name = new char[len + 1];
                name[len] = 0;
                std::memcpy(name, str, len);
                annot.attributes[annot.count].creator = name;
                annot.count++;
                break;
            }
            default:
                break;
        }
        return CXChildVisit_Continue;
    });

    return annot;
}

const char *Reflector::GetTypeName(CXType type) {
    auto _class = clang_getTypeDeclaration(type);

    Core::String name = clang_getCString(clang_getCursorDisplayName(_class));
    if (name == "Entry") {
        bool b = false;
    }

    CXCursor parent = _class;
    for (;;) {
        parent = clang_getCursorSemanticParent(parent);
        switch (parent.kind) {
            case CXCursor_ClassDecl:
            case CXCursor_Namespace:
            case CXCursor_StructDecl:
            case CXCursor_UnionDecl: {
                name = Core::String(clang_getCString(clang_getCursorDisplayName(parent))) + "::" + name;
                break;
            }
            default: {
                char *ptr = new char[name.Length() + 1];
                ptr[name.Length()] = 0;
                Core::Memory::Copy(ptr, name.AsCharArray(), name.Length());
                return ptr;
            }
        }
    }
}

