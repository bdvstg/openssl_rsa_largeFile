# Trace Library

## Author
Karanyak Andrew, from Kazakhstan and working in Taiwan's ITRI

## Intro
- c++ library to print stderr with difference color
- library is maintain by cmake
- work on Windows and Linux
- Android also should be work

## Usage

#### Project Setting
- add_subdirectory(trace) in your CMakeLists.txt and target_link_libraries trace
- set cmake option TRACE_ENABLED to ON

#### Code
```
#include<trace/utils.h>
#define  TRACE_TAG  "tag_prefix_to_string"
// switch what to show
Trace::SetLevel(Trace::LEVEL_ZERO); // do not show
Trace::SetLevel(Trace::LEVEL_INFO); // show only info
Trace::SetLevel(Trace::LEVEL_ERROR); // and error
Trace::SetLevel(Trace::LEVEL_WARNING); // and warning
Trace::SetLevel(Trace::LEVEL_DEBUG); // show all
// print difference level's string
TRACE_MESSAGE("Info String %d %s", 100, "eeeeee"); // like printf
TRACE_ERROR("Error String");
TRACE_WARNING("Warning String");
TRACE_DEBUG("Debug String");
```
#### Known Issue
```
if(true)
    TRACE_XXXXX("");
else
    int qqq = 10;
```

will compile error, because it will expand as
```
if(true)
{
    //XXXXXX
};
else
    int qqq = 10;
```

instead, below is OK
```
if(true) {
    TRACE_XXXXX("");
}
else
    int qqq = 10;
```