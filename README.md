# win32-etw-manifest

Generic ETW manifest file with a "key: value" format for events.
Check out [this blog post](http://flowerinthenight.com/blog/2016/03/01/etw-part1) for more information.

### How to compile the manifest file

```
mc -um <name>.man
```

### Compile .rc to .res

```
rc <input>.rc
```

### Create a dll from the .res file

```
link -dll -noentry -out:<out>.dll <input>.res
```

### Register the manifest to the system

```
wevtutil im <manifest_file>.man /rf:"<full_path_to_noentry_dll>" /mf:"<full_path_to_noentry_dll>"
```

### Remove the manifest from the system:

```
wevtutil um <manifest_file>.man
```

# License

[The MIT License](./LICENSE.md)
