/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   feel2_jpg;
    const int            feel2_jpgSize = 61485;

    extern const char*   Felix_Orange_png;
    const int            Felix_Orange_pngSize = 315030;

    extern const char*   Royal_Flush_png;
    const int            Royal_Flush_pngSize = 312135;

    extern const char*   feel_jpg;
    const int            feel_jpgSize = 296840;

    extern const char*   _7up_png;
    const int            _7up_pngSize = 301740;

    extern const char*   Donald_Duck_png;
    const int            Donald_Duck_pngSize = 443279;

    extern const char*   knob_png;
    const int            knob_pngSize = 1010797;

    extern const char*   ENHAIGHTER_png;
    const int            ENHAIGHTER_pngSize = 75641;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 8;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
