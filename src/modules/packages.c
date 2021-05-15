#include "fastfetch.h"

#include <string.h>
#include <dirent.h>

#define FF_PACKAGES_MODULE_NAME "Packages"
#define FF_PACKAGES_NUM_FORMAT_ARGS 6

static uint32_t getNumElements(const char* dirname, unsigned char type) {
    uint32_t num_elements = 0;
    DIR * dirp;
    struct dirent *entry;

    dirp = opendir(dirname);
    if(dirp == NULL)
        return 0;

    while((entry = readdir(dirp)) != NULL) {
        if(entry->d_type == type)
            ++num_elements;
    }

    if(type == DT_DIR)
        num_elements -= 2; // accounting for . and ..

    closedir(dirp);

    return num_elements;
}

static uint32_t getNumStrings(const char* filename, const char* needle)
{
    FILE* file = fopen(filename, "r");
    if(file == NULL)
        return 0;

    uint32_t count = 0;

    char* line = NULL;
    size_t len = 0;

    while(getline(&line, &len, file) != EOF)
    {
        if(strstr(line, needle) != NULL)
            ++count;
    }

    if(line != NULL)
        free(line);

    fclose(file);

    return count;
}

static void getActiveBranchManjaro(FFstrbuf buffer)
{
    if(ffParsePropFile("/etc/pacman-mirrors.conf", "Branch = ", &buffer) && buffer.length == 0)
        ffStrbufSetS(&buffer, "stable");
}

void ffPrintPackages(FFinstance* instance)
{
    uint32_t pacman = getNumElements("/var/lib/pacman/local", DT_DIR);
    uint32_t xbps = getNumElements("/var/db/xbps", DT_REG);
    uint32_t dpkg = getNumStrings("/var/lib/dpkg/status", "Status: ");
    uint32_t flatpak = getNumElements("/var/lib/flatpak/app", DT_DIR);
    uint32_t snap = getNumElements("/snap", DT_DIR);

    uint32_t all = pacman + xbps + dpkg + flatpak + snap;

    if(all == 0)
    {
        ffPrintError(instance, FF_PACKAGES_MODULE_NAME, 0, &instance->config.packagesKey, &instance->config.packagesFormat, FF_PACKAGES_NUM_FORMAT_ARGS, "No packages from known package managers found");
        return;
    }
    FFstrbuf manjaroBranch;
    ffStrbufInit(&manjaroBranch);
    getActiveBranchManjaro(manjaroBranch);
    ffStrbufRecalculateLength(&manjaroBranch); // needed for later length comparison...

    if(instance->config.packagesFormat.length == 0)
    {
        ffPrintLogoAndKey(instance, FF_PACKAGES_MODULE_NAME, 0, &instance->config.batteryKey);

        #define FF_PRINT_PACKAGE(name) \
        if(name > 0) \
        { \
            printf("%u ("#name")", name); \
            if((all = all - name) > 0) \
                printf(", "); \
        };

        if(pacman > 0)
        {
            printf("%u (pacman)", pacman);
            if(manjaroBranch.length > 0)
                printf("[%s]", manjaroBranch.chars);
            if((all = all - pacman) > 0)
                printf(", ");
        };
        FF_PRINT_PACKAGE(xbps)
        FF_PRINT_PACKAGE(dpkg)
        FF_PRINT_PACKAGE(flatpak)
        FF_PRINT_PACKAGE(snap)

        //Fix linter warning of unused value of all
        (void) all;

        #undef FF_PRINT_PACKAGE

        putchar('\n');
    }
    else
    {
        ffPrintFormatString(instance, FF_PACKAGES_MODULE_NAME, 0, &instance->config.packagesKey, &instance->config.packagesFormat, NULL, FF_PACKAGES_NUM_FORMAT_ARGS, (FFformatarg[]){
            {FF_FORMAT_ARG_TYPE_UINT, &all},
            {FF_FORMAT_ARG_TYPE_UINT, &pacman},
            {FF_FORMAT_ARG_TYPE_UINT, &xbps},
            {FF_FORMAT_ARG_TYPE_UINT, &dpkg},
            {FF_FORMAT_ARG_TYPE_UINT, &flatpak},
            {FF_FORMAT_ARG_TYPE_UINT, &snap}
        });
    }
    ffStrbufDestroy(&manjaroBranch);
}
