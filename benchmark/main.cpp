#include <benchmark/benchmark.h>

#include <fontconfig/fontconfig.h>


void FontMatch(benchmark::State& state, bool apply_substitutions, const char* pattern_name) {
    FcPattern *pattern = FcNameParse((FcChar8*) pattern_name);

    if (apply_substitutions) {
        FcConfigSubstitute(nullptr, pattern, FcMatchPattern);
        FcDefaultSubstitute(pattern);
    }

    while (state.KeepRunning()) {
        FcResult result;
        FcPattern *found = FcFontMatch(nullptr, pattern, &result);
        FcPatternDestroy(found);
    }

    FcPatternDestroy(pattern);
}

/* Benchmarks based on real application usage */
BENCHMARK_CAPTURE(FontMatch, common_kde_1,          true, "Noto Sans:pixelsize=13:file=/usr/share/fonts/truetype/NotoSans-Regular.ttf:index=0");
BENCHMARK_CAPTURE(FontMatch, common_gnome_1,        true, "Tahoma-9:slant=0:weight=80:width=100:pixelsize=12:verticallayout=False:dpi=96:lang=c:prgname=gedit");
BENCHMARK_CAPTURE(FontMatch, common_chromium_1,     true, "Noto Sans-9.99976:slant=0:weight=80:width=100:pixelsize=13.333:verticallayout=False:dpi=96:lang=en-us:prgname=chromium");
BENCHMARK_CAPTURE(FontMatch, common_firefox_1,      true, ":pixelsize=13");

/* Artificial benchmarks */
BENCHMARK_CAPTURE(FontMatch, empty_pattern,            false, "");

BENCHMARK_CAPTURE(FontMatch, existing_file,            false, ":file=/usr/share/fonts/truetype/NotoSans-Regular.ttf");
BENCHMARK_CAPTURE(FontMatch, not_existing_file,        false, ":file=/usr/share/fonts/truetype/DoesNotExist.ttf");
BENCHMARK_CAPTURE(FontMatch, existing_file_with_globs, false, ":file=/usr/share/fonts/*/NotoSans-Regular.ttf");

BENCHMARK_CAPTURE(FontMatch, existing_family,          false, "Noto Sans");
BENCHMARK_CAPTURE(FontMatch, not_existing_family,      false, "Does Not Exist");


void FontSort(benchmark::State& state, bool apply_substitutions, const char* pattern_name) {
    FcPattern *pattern = FcNameParse((FcChar8*) pattern_name);

    if (apply_substitutions) {
        FcConfigSubstitute(nullptr, pattern, FcMatchPattern);
        FcDefaultSubstitute(pattern);
    }

    while (state.KeepRunning()) {
        FcResult result;
        FcFontSet *set = FcFontSort(nullptr, pattern, FcFalse, nullptr, &result);
        FcFontSetDestroy(set);
    }

    FcPatternDestroy(pattern);
}


/* Benchmarks based on real application usage */

// Observed at least in kate, dolphin, ark, konsole, kcalc
BENCHMARK_CAPTURE(FontSort, common_kde_1,      true,  "Noto Sans:slant=0");
BENCHMARK_CAPTURE(FontSort, common_kde_2,      true,  "Liberation Mono,monospace:slant=0");

// Observed in Firefox
BENCHMARK_CAPTURE(FontSort, common_firefox_1, true,  "-moz-default:scalable=True:lang=en-us");
BENCHMARK_CAPTURE(FontSort, common_firefox_2, true,  "sans-serif:scalable=True:lang=en-us");
BENCHMARK_CAPTURE(FontSort, common_firefox_3, true,  "monospace:scalable=True:lang=en-us");

/* Artificial benchmarks */

BENCHMARK_CAPTURE(FontSort, empty_pattern, false, "");


void FontList(benchmark::State& state, bool apply_substitutions, const char* pattern_name) {
    FcPattern *pattern = FcNameParse((FcChar8*) pattern_name);

    if (apply_substitutions) {
        FcConfigSubstitute(nullptr, pattern, FcMatchPattern);
        FcDefaultSubstitute(pattern);
    }

    FcObjectSet *object_set = FcObjectSetBuild (FC_FAMILY, FC_STYLE, FC_FILE, (char *) 0);

    while (state.KeepRunning()) {
        FcFontSet *set = FcFontList(nullptr, pattern, object_set);
        FcFontSetDestroy(set);
    }

    FcPatternDestroy(pattern);
}

/* Benchmarks based on real application usage */
BENCHMARK_CAPTURE(FontList, empty_pattern, false, "");

/* Artificial benchmarks */
BENCHMARK_CAPTURE(FontList, existing_file,            false, ":file=/usr/share/fonts/truetype/NotoSans-Regular.ttf");
BENCHMARK_CAPTURE(FontList, not_existing_file,        false, ":file=/usr/share/fonts/truetype/DoesNotExist.ttf");
BENCHMARK_CAPTURE(FontList, existing_file_with_globs, false, ":file=/usr/share/fonts/*/NotoSans-Regular.ttf");

BENCHMARK_CAPTURE(FontList, existing_family,          false, "Noto Sans");
BENCHMARK_CAPTURE(FontList, not_existing_family,      false, "Does Not Exist");


// BENCHMARK_MAIN();

int main(int argc, char** argv) {
    FcInit();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    FcFini();
}
