# Binding

Provides infrastructure for plugin loading and language interoperability.

## Plugin System

Cross-platform dynamic library loading with type-safe C ABI contracts.

### DynamicLibrary

Low-level wrapper around platform-specific library loading (`LoadLibrary`/`dlopen`).

```cpp
cppaste::DynamicLibrary library("path/to/plugin.dll");
auto result = library.load();
if (result) {
    auto fn = library.get_symbol<void(*)()>("function_name");
}
library.unload();
```

### PluginInterface

CRTP base for defining type-safe plugin contracts:

```cpp
struct GamePlugin : cppaste::PluginInterface<GamePlugin>
{
    using CreateFn = void* (*)(void* context);
    using DestroyFn = void (*)(void* instance);

    static constexpr std::string_view create_symbol = "game_create";
    static constexpr std::string_view destroy_symbol = "game_destroy";
};
```

### PluginLoader

Type-safe plugin loading using a `PluginInterface`:

```cpp
cppaste::PluginLoader<GamePlugin> loader("game_plugin.dll");
if (loader.load()) {
    auto create = loader.get_function<GamePlugin::CreateFn>(GamePlugin::create_symbol);
}
```

### PluginRegistry

Manages multiple loaded plugins by ID:

```cpp
cppaste::PluginRegistry<GamePlugin> registry;
registry.load("path/to/plugin.dll", "my_plugin");
auto* loader = registry.get("my_plugin");
registry.unload("my_plugin");
```

## Language Interoperability

- C ABI generation for cross-language binding
- Metadata association for external interpreters
- nanobind integration helpers
