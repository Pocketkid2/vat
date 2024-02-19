#include "analyze_audio_track.h"

int analyze_audio_track(int argc, char **argv);

module_t analyze_audio_track_module = {
    .long_name = "analyze_audio_track",
    .short_name = "aat",
    .function = analyze_audio_track
};

int analyze_audio_track(int argc, char **argv) {
    printf("analyze_audio_track\n");
    return 0;
}