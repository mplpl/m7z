# m7z
The m7z is Objective-C framework that makes it easy to manage archive files. It supports a wide variety of formats and can create archives, browse their content, update them (add/remove items) and extract their items.

The following archives are supported:

(read-write)
* 7z
* zip
* tar (excluding support for removing files)
* win

(read-only)
* rar
* xar
* iso
* udf
* lha
* lzh
* arj
* rpm
* cpio
* tgz, tbz, tbz2, tb2, taz
* bz
* bz2
* gz
* lzma
* xz
* Z          
* cab
* chm
* msi
* sfs
* vdi
* vmdk
* vhd
* img
* fat
* hfs
* hfsx
* adz

The framework support macOS x86_64 and macOS arm64.

The framework bundles the p7z 16.02 source code.


## Building m7z framework

The package consists of two components:
* lib7z - C++ library built on top of p7z (p7z 16.02 source code is included), that can easily be linked to any C, C++, or Objective-C project
* m7z - the actual framework, that wraps lib7z into Objective-C structs and classes

### Cloning the project

Run the following command:
```
git clone https://github.com/mplpl/m7z
```

### Building lib7z

To build lib7z, Xcode needs to be installed.

1. Go to the cloned project, and then to the 'lib7z' folder

2. Run make
```
make
```
This will build the lib7z.so fat library, containing macOS x86_64 as well as macOS arm64 code.
The above also build unit tests and sample program

3. Run unit tests
```
make runtests
```

You can also run the sample program
```
cd sample
./sample
```
Take a look at the sample source code in Main.cpp file in the same folder.


### Building m7z

1. Open the m7z project in Xcode

2. Select 'm7z' schema - if no schema exists, create a new one for the 'm7z' target and name it 'm7z'

3. Archive the product (select 'Archive' from the 'Product' menu)

4. Once archiving is complete, you will get Project Organized window with the newly created m7z archive selected. Click the 'Distribute Content' button and then select 'Build Products.

5. You will be asked where to store the outcome of the build - select any folder you want. The process creates a subdirectory with the name 'm7z XXXX' where XXXX is the current timestamp (for instance 'm7z 2022-10-10 10-05-42'). The built framework is located in the 'Products/Library' subfolder and is called m7z.framework.

To run unit tests:

1. Select m7zTests schema - if no schema exist, create a new one for 'm7zTests' target and name it 'm7zTests'

2. Run tests by selecting the 'Test' item from the 'Product' menu

Take a look at m7z/unittest/mz7Tests.m file to see how to use m7z framework in your projects.
