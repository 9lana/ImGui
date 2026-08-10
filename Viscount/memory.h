#include <cstdint>
#include <cstdio>
#include <cstring>
uintptr_t GetBaseAdress(const char* libname) {
    uintptr_t base = 0;
    char line[1024];
    FILE* file = fopen("/proc/self/maps", "r");
    if (!file) {
        return 0;
    }
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, libname)) {
            if (sscanf(line, "%lx-%*lx", &base) == 1) {
                fclose(file);
                return base;
            }
        } 
    }
    fclose(file);
    return 0;
}
