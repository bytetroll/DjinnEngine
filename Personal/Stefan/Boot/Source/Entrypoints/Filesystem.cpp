#include <Base/Application.h>
#include <Base/DefaultLogs.h>
#include <Platform/FileStream.h>
#include <Core/StringStream.h>
#include <Base/IFileSystem.h>
#include <Base/FileHandle.h>

DJINN_NS2(Base);
DJINN_NS2(Platform);

void NativeFileStuff(Base::Application &app) {
    {
        FileStream file(FileMode::eWrite, "Data.txt");

        Core::StringStream stream(&file);
        stream << "This is a file!";
    }

    {
        FileStream file(FileMode::eRead, "Data.txt");

        Core::StringStream stream(&file);
        InfoLog(&app).Write("Native File: '", stream.ReadAll(), "'");
    }
}

void VirtualFileStuff(Base::Application &app) {
    // The pipeline hosts instances of shared interfaces and classes
    Host::IPipeline* pipe = app.GetRegistry()->GetPipeline();

    // Get the filesystem
    IFileSystem* fs;
    Always(pipe->GetInterface(&fs));

    // Open our virtual file inside Runtime/
    FileHandle handle;
    Always(fs->OpenStream("Runtime/MyFile.txt"_path, FileMode::eRead, &handle));

    Core::StringStream stream(handle.GetStream());
    InfoLog(&app).Write("Virtual File: '", stream.ReadAll(), "'");
}

void EntryPointFilesystem(Base::Application &app) {
    InfoLog(&app).Write("[Filesystem]");

    // The filesystem is not loaded by default
    // So load it!
    CheckVoid(app.GetRegistry()->LoadPlugin("Plugins.Base.Filesystem"));

    NativeFileStuff(app);
    VirtualFileStuff(app);
}
