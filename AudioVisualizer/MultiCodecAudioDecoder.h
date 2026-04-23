#ifndef MULTICODECAUDIODECODER_H
#define MULTICODECAUDIODECODER_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
}

class MultiCodecAudioDecoder {
public:
    struct AudioInfo {
        int sample_rate;
        int channels;
        int64_t duration_ms;
        std::string codec_name;
        std::string format_name;
        int bit_rate;
    };

    MultiCodecAudioDecoder() = default;

    ~MultiCodecAudioDecoder() {
        close();
    }

    bool open(const std::string& filepath);

    AudioInfo getInfo() const;

    std::vector<float> decodeAll();


private:
    void decodePacket(AVPacket* pkt, AVFrame* frame, std::vector<float>& out);

    void close();

    AVFormatContext* fmt_ctx_   = nullptr;
    AVCodecContext*  codec_ctx_ = nullptr;
    SwrContext*      swr_ctx_   = nullptr;
    const AVCodec*   codec_     = nullptr;
    int audio_stream_idx_       = -1;
};

#endif