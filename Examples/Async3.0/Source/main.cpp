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
 on 12/28/2017.
//

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Assert.h>
#include <Core/SlotArray.h>
#include <Core/Atomic.h>
#include <Core/Pool.h>
#include <Core/Delegate.h>
#include <Core/DynamicDelegate.h>
#include <Async/ContextAPI.h>
#include <COM/IUnknown.h>
#include <Platform/Thread.h>
#include <Platform/ConditionalVar.h>
#include <Core/IndexArray.h>
#include <Core/Timer.h>
#include <Platform/ConcurrentQueue.h>
#include <Core/FixedArray.h>
#include <Core/Queue.h>
#include <Platform/User.h>
#include <cmath>
#include <Async/IScheduler.h>
#include <Host/Registry.h>
#include <Async/Scheduler.h>
#include <Host/ScopedClass.h>
#include <Async/Framework.h>
#include <Async/Worker.h>
#include <Async/Async.h>
#include <Core/SharedPtr.h>
#include <Async/Group.h>
#include <Math/Vector3.h>
#include <Math/Quat.h>
#include <Math/Matrix4.h>
#include <Base/Application.h>
#include <Base/ISystem.h>

DJINN_NS();

Core::AtomicCounter counter{0};
Async::JobGroup pcounter;

Async::CallGraph callGraph;

class Ass {
public:
    Core::AtomicCounter Test(Core::AtomicCounter &cc) {
        cc++;
        return cc;
    }

    void PrintString(const Core::String &str) {
        std::cout << str.AsCharArray();
    }

    /// Async methods
    Asyncf(Test);

    Asyncf(PrintString);
};

constexpr USize batchSize = 1'000'000;
//constexpr USize batchSize = 1'000;
Core::Array<Async::ScheduledJob> batch(batchSize);

void _main() {
    //Core::AtomicCounter aaa;
    //auto test = Ass().TestAsync(aaa);

    counter = 0;
    pcounter.Set(0);
    pcounter.SetCounter(0);

    /*Async::ParameterizedScheduleInfo paramInfo;
    paramInfo.delegate = [&](int a) {
        counter += a;
    };
    paramInfo.counterInfo.post.counter = &pcounter;
    paramInfo.counterInfo.post.op = Async::CounterOp::eAdd;
    info.counterInfo.post.argument = 1;
    paramInfo.stack = {1};*/

    pcounter.Increment(batchSize);

    for (USize i = 0; i < batchSize; i++) {
        batch[i] = {};
        batch[i].group = &pcounter;
        batch[i].post.op = Async::CounterOp::eAdd;
        batch[i].post.argument = 1;
        batch[i].info.delegate = [&] {
            counter++;
        };
    }
    Async::Schedule(batchSize, batch.Ptr());

    Async::Yield(&pcounter);

    std::cout << "pre-yield\n";
    Async::Yield();
    std::cout << "conditional, counter: " << counter.Load() << "\n";
}

Asyncg(_main);

Core::AtomicCounter counterb;

void CounterJob(int a) {
    counterb += a;
}

Asyncg(CounterJob);

void Compute() {
    Ass ass;

    Async::Group group;
    auto t0 = ass.PrintStringAsync(group, "a,");
    auto t1 = ass.PrintStringAsync(group, "b,");
    auto t2 = ass.PrintStringAsync(group, "c,");
    auto t3 = ass.PrintStringAsync(group, "d,");

    group.Wait();

    std::cout << "\n";

    ass.PrintStringAsync("a,").Wait();
    ass.PrintStringAsync("b,").Wait();
    ass.PrintStringAsync("c,").Wait();
    ass.PrintStringAsync("d,").Wait();

    std::cout << "\n";

    std::cout << "\n";

    counterb = 0;

    Async::Group group2;
    auto j0 = CounterJobAsync(group2, 1);
    auto j1 = CounterJobAsync(group2, 2);
    auto j2 = CounterJobAsync(group2, 3);
    auto j3 = CounterJobAsync(group2, 4);

    group2.Wait();

    Always(counterb.Load() == (1 + 2 + 3 + 4));
}

Asyncg(Compute);

struct Entity {
    Math::Quaternion rotation;
    Math::Matrix4 transform;
    Math::Vector3 position;
};

void UpdateEntity(Entity *entity) {
    entity->position = {3, 3, 3};
    entity->rotation = Math::Quaternion(Math::Vector3(1, 0, 0), 0);
    entity->transform = Math::Matrix4::Rotate(entity->rotation) * Math::Matrix4::Translate(entity->position);
}

const USize size = 10;
//const USize size = 10'000;
Core::Array<Entity> entities1(size);
Core::Array<Entity> entities2(size);

void Distribute() {
    //Sleep(300);

    for (int k = 0; k < 100; k++) {

        {

            for (int x = 0; x < 10; x++) {
                Core::Timer timer;
                for (USize i = 0; i < size; i++) {
                    UpdateEntity(&entities1[i]);
                }

                //std::cout << "linear  : " << timer.Delta() << "s\n";
                //Sleep(20);
            }
        }

        //Sleep(300);

        {

            for (int i = 0; i < 10; i++) {
                Core::Timer timer;

                Async::Group group;
                if (k == 0 && i == 0) {
                    std::cout << &group << "\n";
                }
                Async::Foreach(group, size, [&](USize i) {
                    UpdateEntity(&entities2[i]);
                });

                group.Wait();

                //std::cout << "parallel: " << timer.Delta() << "s\n";
                //Sleep(20);
            }
        }

        //Sleep(300);.

        /*Math::Matrix4 final;
        for (USize i = 0; i < size; i++) {
            final *= entities1[i].transform;
        }
        for (USize i = 0; i < size; i++) {
            final *= entities2[i].transform;
        }
        std::cout << " ,, " << final.x.x << " ,, \n";*/
    }

}

Asyncg(Distribute);

Core::String GetName() {
    return "fuck_me";
}

Asyncg(GetName);

void TaskTest() {
    auto name = GetNameAsync();

    std::cout << name.GetValue() << "\n";
}

Asyncg(TaskTest);

void Waiter(const Async::Task<void> &task) {
    task.Wait();
}

void Worker() {
    Sleep(1000);
}

Asyncg(Worker);

Asyncg(Waiter);

void Minimal() {
    auto work = WorkerAsync();

    for (UInt32 i = 0; i < 8; i++) {
        WaiterAsync(work);
    }

    work.Wait();
}

Asyncg(Minimal);

class TestSystem : public Base::ISystem {
public:
    DJINN_COM_CLASS();

    TestSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISystem(registry, outer, host) {

    }

    COM::Result Destroy() override {
        return COM::kOK;
    }

    COM::Result Run() override {
        USize batches = 10'000;

        //Async::SetCallGraph(&callGraph);

        for (USize i = 0; i < batches; i++) {
            Core::Timer timer;
            _mainAsync().Wait();
            std::cout << "delta: " << timer.Delta() << ", " << counter.Load() << "\n";

            callGraph.Flush();
        }

        // OK
        return COM::kOK;
    }
};

int main() {
    std::cout << std::fixed;

    auto rtti = Bindg(main).GetRTTI();

    // Create application
    Base::Application app;
    Always(app.Initialize({}));

    // Create test system
    auto testSystem = new TestSystem(app.GetRegistry(), nullptr);

    // Install system
    Always(app.Register(testSystem));

    // Blocked run
    Always(app.Run(true));

    // ...
    return 0;
}