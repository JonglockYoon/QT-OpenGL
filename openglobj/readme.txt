
assimp-4.1.0 library mImporter.ReadFile() 실행중 ObjFileParser::getMaterialLib()
함수에서 error를 만나는 경우가 있으면..

.obj 파일만 있고 .mtl파일이 없을때 error가 발생할수 있다.

622 line
    #include <sys/stat.h>

622 line.
    struct stat fileInfo;
    if (stat(absName.c_str(), &fileInfo) != 0)
        return;
