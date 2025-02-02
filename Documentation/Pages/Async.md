# Async Documentation

# Initialization

Most features of the Async framework require a running framework, the initialization of such
framework is ***automatically*** created if the `ePipeline` flag was given to the plugin loading.

However it can also be done manually:

```cpp
// Initialize the scheduler
Host::ScopedClass<Async::Scheduler> scheduler(registry);
if (!scheduler->Initialize(Platform::User::GetCoreCount())) {
    ... error handling ...
}

// Initialize the framework
Host::ScopedClass<Async::Framework> framework(registry);
if (!framework->Initialize(scheduler.GetValue())) {
    ... error handling ...
}
```

***Note that the features described below ( Tasks, Groups, etc... ) require a running outer job
that allow for the abstraction of the outer framework otherwise will not work.*** Because of this it's
recommended that any async behaviour does not take place outside the `Application().Run(...)` helper.

# Tasks

Tasks represent a handle to a job that's still in progress, a number of preprocessors exist
to automatize asynchronous versions of methods:

- `Asyncg(NAME)` : Global asynchronous function
- `Asyncf(NAME)` : Member asynchronous function

All of the above preprocessors create two new methods with the name `<NAME>Async`. Where one
of these methods allow for Grouped yields (More on that later).

The use of these new functions is fairly simple:

```cpp
class Foo {
public:
    /// Get height at position
    float GetHeightAt(float x, float y);

    /// Asynchronous methods
    Asyncf(GetHeightAt);
};
```

```cpp
Task<float> height = Foo().GetHeightAtAsync(25.f, 25.f);
```

In the above case explicit typing was used, however `auto`'ed types are preferred.

The `Task<T>` wrapper has numerous helper methods:

- `height.HasCompleted()` : The task can be manually checked for completion
- `height.Wait()` : The task can be manually waited for, may yield if not already completed
- `height.GetValue()` : Will get the value if already completed, otherwise will yield and then get the value
- `ImplicitCast(height)` : Tasks can be implicitely casted to its resulting type, this has the same effect as `.GetValue()` and therefore may yield

# Task groups

Sometimes one may need to wait for multiple tasks to complete, groups offer this in a lightweight fashion.

```cpp
Async::Group group;
auto h0 = Foo().GetHeightAtAsync(group, 25.f, 25.f);
auto h1 = Foo().GetHeightAtAsync(group, 15.f, 15.f);
auto h2 = Foo().GetHeightAtAsync(group, 5.f,  5.f );

group.Wait();
```

As seen above the `Asyncg/f` preprocessors offer an overload that takes an `Async::Group` as an
argument, ***however, the tasks returned by the asynchronous functions `h0 h1 h2` when yielded will wait for all previously scheduled tasks int he same group***.

# Foreach

In order to effectively distribute tasks a helper method named `Async::Foreach` exists, it takes in the
number of iterations and an action lambda:

```cpp
Async::Foreach(entities.Size(), [&](USize i) {
    UpdateEntity(&entities[i]);
});
```

# Events

Events offer a cheap signal / wait pattern across threads / jobs, example:

```cpp
Async::Event myEvent;

void Init() {
    ... do work ...

    myEvent.Signal();

    ... do more work ...
}

Asyncg(Init);

void Launch() {
    InitAsync();

    // Wait for the "myEvent.Signal()" pass
    myEvent.Wait();
}
```

While the `Signal` method is lightweight the `Wait` method may result in a yield.
