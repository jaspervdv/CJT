# CJT
Library supporting the conversion of cityJSON data to and from OCCT 

## How to build
To build CJT two other libaries are required: 
[OpenCascade](https://dev.opencascade.org/release) and [nhollman/json](https://github.com/nlohmann/json).

Building the libary is made easy with cmake:
1. Set the "main folder" as source code location
2. Set the build binaries folder to the "main folder/build"

## How to link
CJT can be found in cmake with "find_package()"*. 
This will expose the "OpenCASCADE_INCLUDE_DIR", "OpenCASCADE_LIBRARY_DIR" and "CJT_Foundation_LIBRARIES" variables.
Examples how to use these can be found in the examples folder.

*"find_package()" only functions if CJT's binaries have been build in the "main folder/build" path.

## How to use
The example files have examples included that have extensive descriptions of every step that is taken. 
Added to this in the header files short descriptions can be found covering every major function.

## Current issues
* During the coversion from the OpenCASCADE to CityJSON format not every face is orientated properly.
* Only Solid and Multisurface CityJSON objects are supported