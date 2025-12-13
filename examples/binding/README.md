# Binding Example

This example demonstrates cppaste's plugin and binding infrastructure for loading dynamic libraries at runtime.

## Overview

The example shows how to:

1. **Define a plugin interface** using cppaste's `PluginInterface` CRTP
2. **Create a C ABI contract** that plugins implement
3. **Load/unload plugins** at runtime using `PluginLoader`
4. **Use lifecycle services** for plugin state management

## Key Components

### Plugin Contract

Defines the C ABI that plugins must export:

- `plugin_create(context) -> instance` - Create plugin instance
- `plugin_destroy(instance)` - Destroy plugin instance
- `plugin_get_info() -> PluginInfo` - Get plugin metadata

### PluginHost

High-level wrapper that manages plugin loading, instance creation, and lifecycle.

## Usage

```cpp
import example.binding;

// Create a plugin host
example::binding::PluginHost host("path/to/plugin.dll");

// Load and create instance
if (host.Load()) {
    host.Create(nullptr);  // Pass context if needed

    // Get plugin info
    if (auto info = host.GetInfo()) {
        std::println("Loaded: {} v{}", info->name, info->version);
    }

    // Cleanup
    host.Destroy();
    host.Unload();
}
```

## Creating a Plugin

Plugins export C functions matching the contract:

```cpp
#include "plugin_interface.h"

struct MyPlugin {
    // Plugin state
};

extern "C" {

CPPASTE_EXPORT void* plugin_create(void* context) {
    return new MyPlugin();
}

CPPASTE_EXPORT void plugin_destroy(void* instance) {
    delete static_cast<MyPlugin*>(instance);
}

CPPASTE_EXPORT PluginInfo plugin_get_info() {
    return {"MyPlugin", "1.0.0", "Author"};
}

}
```
