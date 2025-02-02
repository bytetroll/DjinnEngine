#include <Base/Application.h>
#include <Base/DefaultLogs.h>
#include <Reflection/IReflectionRegistry.h>
#include <Reflection/IClassInfo.h>
#include <Reflection/IMethodInfo.h>
#include <Reflection/IMemberInfo.h>

#include "TestFiles/MyClass.h"
#include "TestFiles/MyAnnotation.h"

DJINN_NS2(Reflection);

void PrintMyClass(Base::Application &app, IReflectionRegistry* registry) {
    // Create an instance
    //  ! Note that the outer parameter is null, the outer is essentially the owner, there is no owner (parents ran off)
    MyClass* instance;
    Always(app.GetRegistry()->CreateClass(nullptr, &instance));

    // Get the reflection info
    IClassInfo* info;
    CheckVoid(registry->GetClass<MyClass>(&info));

    IMethodInfo* method = info->GetMethod("VerySpecialFunction");
    Always(method);

    // GetDynamic returns a dynamic delegate, the parameter represents what its bound to
    // Note: Dynamic delegates ALWAYS verify the signature, it's a simple uint comparison.
    auto result = method->GetDynamic(instance).Invoke<Core::String>();

    InfoLog(&app).Write("Function returned '", result, "'");

    CheckVoid(instance->Release());
}

void PrintAnnotatedClasses(Base::Application &app, IReflectionRegistry* registry) {
    // Find all classes with our attribute
    CheckVoid(registry->VisitAttributeClasses(Djinn::Attributes::MyAttribute::kCID, [&](IClassInfo* info, IAttribute* attr) {
        InfoLog(&app).Write("Found class: ", info->GetName());

        // Iterate all members
        for (IMemberInfo* member : info->GetMembers()) {
            auto attr = member->GetAttribute<Djinn::Attributes::MyAttribute>();

            if (attr)
                InfoLog(&app).Write("\t Member : ", member->GetName(), " but actually wants to be called '", attr->name, "'");
            else
                InfoLog(&app).Write("\t Member : ", member->GetName());
        }
    }));
}

void EntryPointReflection(Base::Application &app) {
    InfoLog(&app).Write("[Reflection]");

    // The pipeline hosts instances of shared interfaces and classes
    Host::IPipeline* pipe = app.GetRegistry()->GetPipeline();

    // Get the reflection registry
    IReflectionRegistry* registry;
    Always(pipe->GetInterface(&registry));

    // Do stuff!
    PrintMyClass(app, registry);
    PrintAnnotatedClasses(app, registry);
}
