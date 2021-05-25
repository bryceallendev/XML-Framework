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
