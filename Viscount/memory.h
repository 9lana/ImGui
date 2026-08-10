#include <cstdint>
#include <cstdio>
#include <cstring>
uintptr_t GetBaseAdress(char* libname) {
    uintptr_t base = 0;
    char line[1024];
    FILE* file = fopen("/proc/self/maps", "r");
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, libname)) {
            if (sscanf(line, "%lx-%*lx", &base)) {
                fclose(file);
                return base;
            }
        } 
    }
    fclose(file);
    return 0;
}
