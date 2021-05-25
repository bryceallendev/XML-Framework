# Build XMLFramework

The build is based on CMake and located:

* In the subdirectory build
* External to the source directory, a genuine "out-of-source" build

To generate build files, first move to the build directory:

```console
cd
cd XMLFramework
cd build
```

Then, generate build files:

```console
cmake ..
```

The ".." is the parent directory.

Finally, to build:

```console
make
```

## Build Assumptions

* Each *.cpp file in the directory *apps* is a separate application program
* Any *.cpp and *.hpp files can be added to *src* and *include*
  for apps, framework, and toolkit
* The subdirectory *bin* of  the build files contains generated applications

## Out-Of-Source Builds

You can also build outside of the XMLFramework subdirectory *build*.

First, create a directory as a sibling (in the same directory) as
your XMLFramework, and move into it:

```console
mkdir XMLFramework-Build
cd XMLFramework-Build
```

Then generate your build, using the path to your XMLFramework source directory

```console
cmake ../XMLFramework
```

## Build Type

The default CMake build is for *Debug*. For a faster program, build for *Release*:

```console
cmake -DCMAKE_BUILD_TYPE=Release
```

Note that for Visual Studio, you still have to specify a Release build in the IDE.

## Other Generators

CMake has many different generators. For each platform, there is
a default, but that can be changed. To see the list:

```console
cmake -h
```

Ninja is a Make replacement that is faster with less latency.
If you have Ninja installed, you can generate a `ninja` build with:

```console
cmake .. -G Ninja
```

Then, instead of "make" use "ninja", e.g.,

```console
ninja 
ninja clean
ninja run
```
