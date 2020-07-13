// gcc fontlist.c -lfontconfig

#include <stdio.h>
#include <stdlib.h>
#include <fontconfig/fontconfig.h>

int main()
{
    FcPattern *pat;
    FcFontSet *fs;
    FcObjectSet *os;
    // FcChar8 *s;
    FcConfig *config;
    FcBool result;
    FcChar8 *directory = NULL;
    FcStrList *fdlist = NULL;
    int i;

    result = FcInit();
    config = FcConfigGetCurrent();
    FcConfigSetRescanInterval(config, 0);

    // list font directories
    printf("Font Directories\n");
    fdlist = FcConfigGetFontDirs(config);
    while ((directory = FcStrListNext(fdlist))) {
        printf("%s\n", directory);
    }
    FcStrListDone(fdlist);

   // show the fonts (debugging)
    pat = FcPatternCreate();
    os = FcObjectSetBuild (FC_FAMILY, FC_STYLE, FC_LANG, (char *) 0);
    fs = FcFontList(config, pat, os);
    printf("Total fonts: %d\n", fs->nfont);

    for (i=0; fs && i < fs->nfont; i++) {
        FcChar8 *file, *style, *family;

        FcPattern *font = fs->fonts[i];
        // FcPatternPrint(font);
        // s = FcNameUnparse(font);
        if (FcPatternGetString(font, FC_FAMILY, 0, &family) == FcResultMatch) {
            printf("Family: %s\n", family);
        }
        if (FcPatternGetString(font, FC_STYLE, 0, &style) == FcResultMatch) {
            printf("Style: %s\n", style);
        }
        // Note - FcPatternGetString(font, FC_FILE, 0, &file) returns 1
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch) {
            printf("File: %s\n", file);
        }
        // printf("Font: %s\n", s);
        // free(s);
    }
    if (fs)
        FcFontSetDestroy(fs);
}
