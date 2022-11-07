module;

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

export module libav;

#if _MSC_VER
#	pragma comment(lib, "libav.lib")
#endif

extern "C" {
export {
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/opt.h"
}
}

#undef EOF

#define DCL(x) constexpr inline auto x = AV_##x
#define EXP(x) export constexpr inline auto AV_##x = libav::x
#define DCLERR(x) constexpr inline auto x = AVERROR_##x
#define EXPERR(x) export constexpr inline auto AVERROR_##x = libav::error::x

// this is the bare minimum!
// to export all libav macro definitions use X-macros

export namespace libav {
DCL(TIME_BASE);

namespace error {
DCLERR(EOF);
} // namespace error
} // namespace libav

#undef AV_TIME_BASE
#undef AVERROR_EOF

EXP(TIME_BASE);
EXPERR(EOF);
