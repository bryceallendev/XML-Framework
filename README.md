# XML-Framework

## Description

### Background
A college class project that processes XML data through an open-layered architecture framework.

**Class:** Object-Oriented Programming, University of Akron (Spring 2021)

### Features
- Parsing XML with a variety of push and pull parsers.
- XPath evaluation.
- XSLT transformation.
- Combining these operations as needed by the applications.

### Structure
The open-layered architecture framework, XMLFramework, includes:

- Applications
    - Applications using the XMLFramework.
    - An xmlpipeline application for a CLI that flexibly uses
      the XMLFramework.
- XMLFramework 
    - As described above.
- XMLToolkit
    - Set of C++ classes for processing XML.
    - Used to implement the XMLFramework, but can be used directly.
    - XMLParser belongs here.
- XMLLibrary
    - Set of C functions for processing XML.
    - Used to implement the XMLToolkit, but can be used directly.
    
## Installation

### Requirements
The app, xmlpipeline makes use of [CLI11](https://github.com/CLIUtils/CLI11) for its parsing command-line options. When cmake is run, the necessary files are downloaded to the subdirectory *external/*. On Windows devices, the download may be unsuccessful, due to your system settings. If this is the case, then download the file CLI11.hpp and save it in your build directory under *external/*.

### Build
*See [BUILD.md](https://github.com/bryceallendev/XML-Framework/blob/main/BUILD.md) file.*