# Composition Design

With standard reflection, we extract versioned metadata from interfaces to provide various ways of composing your code.

## Functional

### Features

Events

Delegates

Signals

## Services

Services are a component based framework that provide composable behaviors and type injection with polymorphic properties.

Note: The name "Service" is used to avoid confusion with C++20 modules.

### Service Features

### Comparison of Polymorphic Types

#### Templates with Concepts

.Advantages

- TODO

.Disadvantages

- TODO

#### Inheritance

.Advantages

- TODO

.Disadvantages

- TODO

## Lifecycle

Services can implement lifecycle management for controlled initialization and shutdown.

### LifecycleState

Services progress through defined states:

- **Uninstalled** - Not loaded into memory
- **Installed** - Loaded but dependencies not resolved
- **Resolved** - Dependencies satisfied, ready to start
- **Starting** - Transitioning to active
- **Active** - Running normally
- **Stopping** - Transitioning back to resolved

### ILifecycle

Interface for lifecycle-aware services:

```cpp
struct ILifecycle {
    virtual auto Install() -> bool = 0;
    virtual auto Resolve() -> bool = 0;
    virtual auto Start() -> bool = 0;
    virtual void Stop() = 0;
    virtual void Uninstall() = 0;
    virtual auto GetState() const -> LifecycleState = 0;
};
```

### LifecycleMixin

Mixin providing state management with transition validation:

```cpp
class MyService : public ILifecycle, public LifecycleMixin
{
    auto Start() -> bool override {
        if (!CanTransitionTo(LifecycleState::Starting))
            return false;
        SetState(LifecycleState::Starting);
        // ... initialization ...
        SetState(LifecycleState::Active);
        return true;
    }
};
```

### LifecycleService

Wrapper that adds lifecycle management to any implementation:

```cpp
LifecycleService<MyImpl> service(args...);
service.SetCallbacks({
    .on_active = [](MyImpl& impl) { /* started */ },
    .on_stopping = [](MyImpl& impl) { /* stopping */ }
});
service.Install();
service.Resolve();
service.Start();
```

## Package

A collection of services

## Plugins

Application unaware composition
