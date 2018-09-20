

If the function encounters an error ...

assimp-4.1.0 library mImporter.ReadFile() -> ObjFileParser::getMaterialLib()

An error may occur when there is only an .obj file and there is no .mtl file.

Add the following lines and build the Library again.

ObjFileParser.cpp
622 line
    #include <sys/stat.h>

622 line.
    struct stat fileInfo;
    if (stat(absName.c_str(), &fileInfo) != 0)
        return;
