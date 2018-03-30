# Network Monitor

## Building

Currently, this project requires `cmake >= v3.9.x`

`mkdir build`  
`cd build`  
`cmake ..`  
`cmake --build .`

The built files will be in `build/src/Debug`.

Make sure to copy `sqlite3.dll` to `build/src/Debug`.

## Dependencies

Below is a list of included dependencies that the projects uses to do what it does:

- [Sqlite3](https://www.sqlite.org/index.html)
- [cJSON](https://github.com/DaveGamble/cJSON)
