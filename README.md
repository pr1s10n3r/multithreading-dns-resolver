## Generate Compile Commands File

From `build` directory, run this command:

```sh
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
```

Then, move the `build/compile_commands.json` file to your project root.