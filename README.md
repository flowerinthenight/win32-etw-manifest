# win32-etw-manifest
Generic ETW manifest file with a "key: value" format for events.

# How to compile the manifest file:
```
mc -um <name>.man
```

# Compile .rc to .res:
```
rc <input>.rc
```

# Create a dll from the .res file:
```
link -dll -noentry -out:<out>.dll <input>.res
```
