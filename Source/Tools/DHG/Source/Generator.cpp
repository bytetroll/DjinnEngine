//
// Created by Dev on 5/28/2018.
//

#include <DHG/Generator.h>
#include <DHG/MetaData.h>
#include <Platform/FileStream.h>
#include <Core/Set.h>
#include <iostream>
#include <Platform/FileSystem.h>

DJINN_NS2(DHG);

namespace {
    Core::String GraphSanitize(Core::String file) {
        file = Core::String(file, file.LastIndexOf("\\/") + 1, file.Length());
        return file;
    }
}

bool Generator::Generate(const GeneratorJob &job) {
    return GenerateDHG(job) && GenerateGraph(job);
}

bool Generator::GenerateGraph(const GeneratorJob &job) {
    {
        // Attempt to open file
        Platform::FileStream stream(Platform::FileMode::eWrite, job.graphOutput);
        if (!stream.IsGood()) {
            return false;
        }

        // Stream helper
        Core::StringStream out(&stream);

        for (auto &edge : job.md->includeEdges) {
            out << edge.first << " -> " << edge.second << "\n";
        }
    }

    {
        // Attempt to open file
        Platform::FileStream stream(Platform::FileMode::eWrite, job.graphShortOutput);
        if (!stream.IsGood()) {
            return false;
        }

        // Stream helper
        Core::StringStream out(&stream);

        Core::Set<USize> visited;

        out << "digraph IncludeGraph {\n";
        for (auto &header : job.headers) {
            out << "\tstart -> " << Core::ComputeCRC32(header.AsCharArray()) << ";\n";
        }
        out << "\n";
        for (auto &edge : job.md->includeEdges) {
            out << "\t" << Core::ComputeCRC32(edge.first) << " [label=\"" << GraphSanitize(edge.first) << "\"];\n";
            out << "\t" << Core::ComputeCRC32(edge.second) << " [label=\"" << GraphSanitize(edge.second) << "\"];\n";
        }
        for (auto &edge : job.md->includeEdges) {
            auto hash = CombineHash(Core::ComputeCRC32(edge.first), Core::ComputeCRC32(edge.second));
            if (!visited.Contains(hash)) {
                visited.Add(hash);
                out << "\t" << Core::ComputeCRC32(edge.first) << " -> " << Core::ComputeCRC32(edge.second) << ";\n";
            }
        }
        out << "}";

        // Helper bat file
        Platform::FileStream batStream(Platform::FileMode::eWrite, Platform::FileSystem::CombinePaths(Platform::FileSystem::GetDirectory(job.graphOutput), "png.bat"));
        if (!batStream.IsGood()) {
            return false;
        }

        // Stream helper
        Core::StringStream batOut(&batStream);

        batOut << "\"" << DJINN_PATH_EXTERN  << "/graphviz/windows/bin/dot.exe\" -Tpng \"" << job.graphShortOutput << "\" -o \"" << job.graphShortOutput << ".png\"\n";
        batOut << "\"" << job.graphShortOutput << ".png\"";
    }

    return true;
}

bool Generator::GenerateDHG(const GeneratorJob &job) {
    // Attempt to open file
    Platform::FileStream stream(Platform::FileMode::eWrite, job.output);
    if (!stream.IsGood()) {
        return false;
    }

    // Stream helper
    Core::StringStream out(&stream);

    int counter = 0;

    out << "/* Djinn Header Tool - Generated File */\n";
    out << "#include <Reflection/PluginAPI.h>\n";
    out << "#include <Reflection/ClassInfo.h>\n";
    out << "#include <Reflection/MethodInfo.h>\n";
    out << "#include <Reflection/ParameterInfo.h>\n";
    out << "#include <Reflection/MemberInfo.h>\n";
    out << "#include <Reflection/EnumInfo.h>\n";
    out << "#include <Reflection/EnumMemberInfo.h>\n";
    out << "#include <Reflection/TypeInfo.h>\n";
    out << "#include <Host/IRegistry.h>\n";
    for (auto &header : job.headers) {
        out << "#include <" << header << ">\n";
    }
    out << "\n";

    out << "using namespace Djinn;\n";
    out << "using namespace Djinn::Reflection;\n";
    for (auto name : job.md->namespaceAliases) {
        out << "using namespace " << name << "; /* ALIAS */\n";
    }
    out << "\n";

    out << "extern \"C\" DJINN_EXPORT void DJINN_PLUGINAPI_REFLECT(Host::IRegistry *registry, IReflector* reflector) {\n";

    for (auto &_class : job.md->classes) {
        out << "\tif constexpr(COM::HasKCID<" << _class->namespaceName << ">::kValue && Core::IsBaseOf<COM::IUnknown, " << _class->namespaceName << ">::kValue) ";
        out << "registry->InstallClass<" << _class->namespaceName << ">();\n";
    }

    if (job.md->classes.Size() > 0) {
        out << "\n";
    }

    for (auto &_class : job.md->classes) {
        Core::String cuid = Core::String("c") + counter++;
        Core::String iuid = Core::String("i") + counter++;

        out << "\t/* Begin Class: " << _class->namespaceName << " */\n";

        out << "\tCore::StackArray8<ClassID> " << iuid << ";\n";
        for (auto iid : _class->bases) {
            out << "\t" << iuid << ".Add(ClassID(Core::ComputeCRC64(\"" << iid << "\")));\n";
        }
        out << "\tauto " << cuid << " = new ClassInfo<" << _class->namespaceName << ">(" << iuid << ");\n";

        out << "\t{\n";
        for (auto &member : _class->members) {
            Core::String fuid = Core::String("f") + counter++;

            out << "\t\t/* Member: " << member.name << " */\n";
            out << "\t\tauto " << fuid << " = new MemberInfo<" << _class->namespaceName << ", ";
            out << GenerateType(member.type);
            out << ", ";
            out << "&" << _class->namespaceName << "::" << member.name << ">(";
            out << cuid << ", ";
            GenerateTypeInfo(member.type, out);
            out << ", ";
            out << "\"" << member.name << "\"";
            out << ");\n";
            for (int i = 0; i < member.annotation.count; i++) {
                out << "\t\t{\n";
                out << "\t\t\tstatic auto attr = Djinn::Attributes::" << member.annotation.attributes[i].creator << ";\n";
                out << "\t\t\t" << fuid << "->AddAttribute(&attr);\n";
                out << "\t\t}\n";
            }
            out << "\t\t" << cuid << "->AddMember(" << fuid << ");\n";
            out << "\n";
        }
        for (auto &method : _class->methods) {
            Core::String muid = Core::String("m") + counter++;
            Core::String puid = Core::String("p") + counter++;

            out << "\t\t/* Method: " << method.name << " */\n";
            out << "\t\tCore::Array<ParameterInfo> " << puid << ";\n";
            for (std::size_t i = 0; i < method.parameters.Size(); i++) {
                out << "\t\t" << puid << ".Add(ParameterInfo(";
                GenerateTypeInfo(method.parameters[i].type, out);
                out << ", ";
                out << "\"" << method.parameters[i].name << "\"";
                out << "));\n";
            }
            out << "\t\tauto " << muid << " = new MethodCreator<" << _class->namespaceName << ", ";
            out << GenerateType(method.returnType);
            for (std::size_t i = 0; i < method.parameters.Size(); i++) {
                out << ", ";
                out << GenerateType(method.parameters[i].type);
            }
            out << ">::";
            if (method.isConst) {
                out << "ConstMethodInfo";
            } else {
                out << "MethodInfo";
            }
            out << "<" << "&" << _class->namespaceName << "::" << method.name << ">(";
            out << cuid << ", ";
            GenerateTypeInfo(method.returnType, out);
            out << ", ";
            out << puid << ", ";
            out << "\"" << method.name << "\"";
            out << ");\n";
            for (int i = 0; i < method.annotation.count; i++) {
                out << "\t\t{\n";
                out << "\t\t\tstatic auto attr = Djinn::Attributes::" << method.annotation.attributes[i].creator << ";\n";
                out << "\t\t\t" << muid << "->AddAttribute(&attr);\n";
                out << "\t\t}\n";
            }
            out << "\t\t" << cuid << "->AddMethod(" << muid << ");\n";
            out << "\n";
        }
        out << "\t}\n";

        for (int i = 0; i < _class->annotation.count; i++) {
            out << "\t{\n";
            out << "\t\tstatic auto attr = Djinn::Attributes::" << _class->annotation.attributes[i].creator << ";\n";
            out << "\t\t" << cuid << "->AddAttribute(&attr);\n";
            out << "\t}\n";
        }

        if (_class->bases.Size() > 0) {
            //out << "\tregistry->InstallClass<" << _class->namespaceName << ">();\t";
        }
        out << "\treflector->AddClass(" << cuid << ");\n";
        out << "\t/* End Class: " << _class->name << " */\n\n";
    }

    for (auto &_enum : job.md->enums) {
        Core::String euid = Core::String("e") + counter++;
        Core::String iuid = Core::String("i") + counter++;

        out << "\t/* Begin Enum: " << _enum->namespaceName << " */\n";

        out << "\tauto " << euid << " = new EnumInfo<" << _enum->namespaceName << ">(";
        GenerateTypeInfo(_enum->type, out);
        out << ");\n";

        out << "\t{\n";
        for (auto &member : _enum->members) {
            Core::String fuid = Core::String("f") + counter++;

            out << "\t\t/* Member: " << member.name << " */\n";
            out << "\t\tauto " << fuid << " = new EnumMemberInfo<" << _enum->namespaceName << ">(";
            out << euid << ", \"" << member.name << "\", " << member.value;
            out << ");\n";
            for (int i = 0; i < member.annotation.count; i++) {
                out << "\t\t{\n";
                out << "\t\t\tstatic auto attr = Djinn::Attributes::" << member.annotation.attributes[i].creator << ";\n";
                out << "\t\t\t" << fuid << "->AddAttribute(&attr);\n";
                out << "\t\t}\n";
            }
            out << "\t\t" << euid << "->AddMember(" << fuid << ");\n";
            out << "\n";
        }
        out << "\t}\n";

        for (int i = 0; i < _enum->annotation.count; i++) {
            out << "\t{\n";
            out << "\t\tstatic auto attr = Djinn::Attributes::" << _enum->annotation.attributes[i].creator << ";\n";
            out << "\t\t" << euid << "->AddAttribute(&attr);\n";
            out << "\t}\n";
        }

        out << "\treflector->AddEnum(" << euid << ");\n";
        out << "\t/* End Enum: " << _enum->name << " */\n\n";
    }

    out << "}\n";
    return true;
}

Core::String Generator::GenerateType(const MetaType &type) {
    Core::String str = "";
    if (type.layerCount == 0) {
        return "#error#";
    }

    Int32 fptrIndex = -1;
    for (int i = type.layerCount - 1; i >= 0; i--) {
        switch (type.layers[i].type) {
            case MetaType::Layer::eID:
                str = type.layers[i].id.namespaceName;
                if (type.layers[i].id.templateArgumentCount != 0) {
                    str += "<";
                    for (UInt32 j = 0; j < type.layers[i].id.templateArgumentCount; j++) {
                        if (j != 0) {
                            str += ", ";
                        }

                        if (type.layers[i].id.templateArguments[j].isValue) {
                            str += type.layers[i].id.templateArguments[j].value;
                        } else {
                            str += GenerateType(type.layers[i].id.templateArguments[j].type);
                        }
                    }
                    str += ">";
                }
                break;
            case MetaType::Layer::eVoid:
                str = "void";
                break;
            case MetaType::Layer::eBool:
                str = "bool";
                break;
            case MetaType::Layer::eUChar:
                str = "unsigned char";
                break;
            case MetaType::Layer::eChar:
                str = "char";
                break;
            case MetaType::Layer::eShort:
                str = "short";
                break;
            case MetaType::Layer::eUShort:
                str = "unsigned short";
                break;
            case MetaType::Layer::eLong:
                str = "long";
                break;
            case MetaType::Layer::eLongLong:
                str = "long long";
                break;
            case MetaType::Layer::eULong:
                str = "unsigned long";
                break;
            case MetaType::Layer::eULongLong:
                str = "unsigned long long";
                break;
            case MetaType::Layer::eWChar:
                str = "wchar_t";
                break;
            case MetaType::Layer::eFloat:
                str = "float";
                break;
            case MetaType::Layer::eDouble:
                str = "double";
                break;
            case MetaType::Layer::eInt:
                str = "int";
                break;
            case MetaType::Layer::eUInt:
                str = "unsigned int";
                break;
            case MetaType::Layer::ePointer:
                str = str + "*";
                break;
            case MetaType::Layer::eRef:
                str = str + "&";
                break;
            case MetaType::Layer::eArray:
                str = str + "[" + Core::String(type.layers[i].arrayDimension) + "]";
                break;
            case MetaType::Layer::eFPtr:
                fptrIndex = i;
                break;
        }
        if (type.layers[i].isConst && !str.StartsWith("const ")) {
            if (type.layers[i].type == MetaType::Layer::ePointer) {
                str = str + " const";
            } else {
                str = "const " + str;
            }
        }
    }

    if (fptrIndex != -1) {
        auto inner = str;
        str = GenerateType(*type.layers[fptrIndex].fptr.returnType);
        if (type.layers[fptrIndex].fptr.classType) {
            if (!type.layers[fptrIndex].fptr.isShorthand) {
                str += "(";
                if (type.layers[fptrIndex].fptr.classType) {
                    str += GenerateType(*type.layers[fptrIndex].fptr.classType) + "::";
                }
                str += inner + ")";
            }
        } else {
            if (type.layerCount != 1) {
                str += "(" + inner + ")";
            }
        }
        str += "(";
        for (UInt32 j = 0; j < type.layers[fptrIndex].fptr.argumentCount; j++) {
            if (j != 0) {
                str += ", ";
            }
            str += GenerateType(type.layers[fptrIndex].fptr.arguments[j]);
        }
        str += ")";
    }

    return str;
}

void Generator::GenerateTypeInfo(const MetaType &type, Core::StringStream &out, bool ptr) {
    if (ptr) {
        out << "new TypeInfo(";
    }
    out << "TypeInfo::Make<";
    out << GenerateType(type);
    out << ">(";

    for (int i = 0; i < type.layerCount; i++) {
        auto &layer = type.layers[i];

        if (i != 0) {
            out << ", ";
        }

        switch (layer.type) {
            case MetaType::Layer::eID:
                out << "TypeLayer::Class(ToID<" << layer.id.namespaceName << ">(), \"layer.id.namespaceName\")";
                break;
            case MetaType::Layer::eVoid:
                out << "TypeLayer::Primitive(TypeLayer::eVoid)";
                break;
            case MetaType::Layer::eBool:
                out << "TypeLayer::Primitive(TypeLayer::eBool)";
                break;
            case MetaType::Layer::eUChar:
                out << "TypeLayer::Primitive(TypeLayer::eUChar)";
                break;
            case MetaType::Layer::eChar:
                out << "TypeLayer::Primitive(TypeLayer::eChar)";
                break;
            case MetaType::Layer::eShort:
                out << "TypeLayer::Primitive(TypeLayer::eShort)";
                break;
            case MetaType::Layer::eUShort:
                out << "TypeLayer::Primitive(TypeLayer::eUShort)";
                break;
            case MetaType::Layer::eLong:
                out << "TypeLayer::Primitive(TypeLayer::eLong)";
                break;
            case MetaType::Layer::eLongLong:
                out << "TypeLayer::Primitive(TypeLayer::eLongLong)";
                break;
            case MetaType::Layer::eULong:
                out << "TypeLayer::Primitive(TypeLayer::eULong)";
                break;
            case MetaType::Layer::eULongLong:
                out << "TypeLayer::Primitive(TypeLayer::eULongLong)";
                break;
            case MetaType::Layer::eWChar:
                out << "TypeLayer::Primitive(TypeLayer::eWChar)";
                break;
            case MetaType::Layer::eFloat:
                out << "TypeLayer::Primitive(TypeLayer::eFloat)";
                break;
            case MetaType::Layer::eDouble:
                out << "TypeLayer::Primitive(TypeLayer::eDouble)";
                break;
            case MetaType::Layer::eInt:
                out << "TypeLayer::Primitive(TypeLayer::eInt)";
                break;
            case MetaType::Layer::eUInt:
                out << "TypeLayer::Primitive(TypeLayer::eUInt)";
                break;
            case MetaType::Layer::ePointer:
                out << "TypeLayer::Primitive(TypeLayer::ePointer)";
                break;
            case MetaType::Layer::eRef:
                out << "TypeLayer::Primitive(TypeLayer::eRef)";
                break;
            case MetaType::Layer::eArray:
                out << "TypeLayer::Array(" << layer.arrayDimension << ")";
                break;
            case MetaType::Layer::eFPtr:
                out << "TypeLayer::FPtr( ";
                GenerateTypeInfo(*layer.fptr.returnType, out, true);
                out << ", " << layer.fptr.isConst << ", " << layer.fptr.isShorthand;
                for (UInt32 j = 0; j < layer.fptr.argumentCount; j++) {
                    out << ", ";
                    GenerateTypeInfo(layer.fptr.arguments[j], out, true);
                }
                out << ")";
                break;
        }
    }
    out << ")";
    if (ptr) {
        out << ")";
    }
}
