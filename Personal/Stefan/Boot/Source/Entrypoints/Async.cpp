#include <Base/Application.h>
#include <Base/DefaultLogs.h>
#include <Core/Timer.h>

DJINN_NS();

constexpr uint32_t kIterations = 256'000'000;

Core::String GetString() {
    return "VeryAdvanced";
}

// Helper that creates a GetStringAsync
// Notice the g? The g stands for global!
Asyncg(GetString);

void BuildBlob(Core::Array<int>& blob) {
    for (uint32_t i = 0; i < 1000; i++) {
        blob.Add(i);
    }
}

Asyncg(BuildBlob);

void BuildAsset(Base::Application &app) {
    // Notice how this function called inside the "main" thread can wait on tasks? That's allowed!

    Core::Array<int> blob;

    auto jobA = BuildBlobAsync(blob);
    jobA.Wait();

    auto jobB = BuildBlobAsync(blob);
    jobB.Wait();

    auto jobC = BuildBlobAsync(blob);
    jobC.Wait();
}

void SumLinear(Base::Application &app) {
    Core::Timer timer;

    uint32_t sum = 0;

    for (uint32_t i = 0; i < kIterations; i++) {
        sum += i;
    }

    InfoLog(&app).Write("Linear sum: ", timer.Delta(), "s");
}

void SumParallel(Base::Application &app) {
    Core::Timer timer;

    Core::AtomicCounter sum{ 0 };

    // A group represents a collection of tasks
    Async::Group group;

    // Chunked is a helper, similar to Foreach, that avoids calling a function per iteration
    Async::Chunked(group, kIterations, [&](uint32_t start, uint32_t end) {
        uint32_t localSum = 0;
        for (uint32_t i = start; i < end; i++) {
            localSum += i;
        }

        sum += localSum;
    });

    // Wait for all tasks to complete
    group.Wait();

    InfoLog(&app).Write("Parallel sum: ", timer.Delta(), "s");
}

class MyClass {
public:
    int GetLife() {
        return 42;
    }

    // Helper that wraps a method
    // Notice how its no longer a g but an f!
    Asyncf(GetLife);
};

void EntryPointAsync(Base::Application &app) {
    InfoLog(&app).Write("[Async]");

    auto task = GetStringAsync();

    // Calling GetValue will wait for the implicitly task
    InfoLog(&app).Write("Async string: '", task.GetValue(), "'");

    BuildAsset(app);
    SumLinear(app);
    SumParallel(app);

    InfoLog(&app).Write("Async life: ", MyClass().GetLifeAsync().GetValue(), "");
}
