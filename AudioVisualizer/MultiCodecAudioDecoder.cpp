#include "MultiCodecAudioDecoder.h"

// Open any audio file — FFmpeg auto-detects codec (MP3, AAC, FLAC, OGG, etc.)
bool MultiCodecAudioDecoder::open(const std::string& filepath) {
    // 1. Open the file and detect format
    if (avformat_open_input(&fmt_ctx_, filepath.c_str(), nullptr, nullptr) < 0)
        throw std::runtime_error("Cannot open file: " + filepath);

        // 2. Retrieve stream info
    if (avformat_find_stream_info(fmt_ctx_, nullptr) < 0)
        throw std::runtime_error("Cannot find stream info");

        // 3. Find the best audio stream
    int stream_idx = av_find_best_stream(
        fmt_ctx_, AVMEDIA_TYPE_AUDIO, -1, -1, &codec_, 0
    );
    if (stream_idx < 0)
        throw std::runtime_error("No audio stream found");

    audio_stream_idx_ = stream_idx;
    AVStream* stream = fmt_ctx_->streams[stream_idx];

    // 4. Allocate and configure codec context
    codec_ctx_ = avcodec_alloc_context3(codec_);
    avcodec_parameters_to_context(codec_ctx_, stream->codecpar);

    // 5. Open the codec (works for MP3, AAC, FLAC, OPUS, VORBIS, etc.)
    if (avcodec_open2(codec_ctx_, codec_, nullptr) < 0)
        throw std::runtime_error("Cannot open codec");

    // 6. Setup resampler → output: stereo, 44100Hz, interleaved float32
    swr_ctx_ = swr_alloc();
    av_opt_set_chlayout(swr_ctx_,  "in_chlayout",  &codec_ctx_->ch_layout, 0);
    av_opt_set_int(swr_ctx_,       "in_sample_rate",  codec_ctx_->sample_rate, 0);
    av_opt_set_sample_fmt(swr_ctx_, "in_sample_fmt", codec_ctx_->sample_fmt, 0);

    AVChannelLayout stereo = AV_CHANNEL_LAYOUT_STEREO;
    av_opt_set_chlayout(swr_ctx_,  "out_chlayout",  &stereo, 0);
    av_opt_set_int(swr_ctx_,       "out_sample_rate",  44100, 0);
    av_opt_set_sample_fmt(swr_ctx_, "out_sample_fmt", AV_SAMPLE_FMT_FLT, 0);
    swr_init(swr_ctx_);

    return true;
}

// Get metadata about the opened file
MultiCodecAudioDecoder::AudioInfo MultiCodecAudioDecoder::getInfo() const {
    AudioInfo info{};
    info.sample_rate  = codec_ctx_->sample_rate;
    info.channels     = codec_ctx_->ch_layout.nb_channels;
    info.duration_ms  = (fmt_ctx_->duration * 1000) / AV_TIME_BASE;
    info.codec_name   = codec_->name;           // "mp3", "aac", "flac", etc.
    info.format_name  = fmt_ctx_->iformat->name;
    info.bit_rate     = codec_ctx_->bit_rate;
    return info;
}

// Decode entire file → returns interleaved float PCM (stereo, 44100Hz)
std::vector<float> MultiCodecAudioDecoder::decodeAll() {
    std::vector<float> pcm_output;
    AVPacket* pkt   = av_packet_alloc();
    AVFrame*  frame = av_frame_alloc();

    while (av_read_frame(fmt_ctx_, pkt) >= 0) {
        if (pkt->stream_index == audio_stream_idx_) {
            decodePacket(pkt, frame, pcm_output);
        }
        av_packet_unref(pkt);
    }

    // Flush decoder
    decodePacket(nullptr, frame, pcm_output);

    av_frame_free(&frame);
    av_packet_free(&pkt);
    return pcm_output;
}

void MultiCodecAudioDecoder::decodePacket(AVPacket* pkt, AVFrame* frame, std::vector<float>& out) {
    avcodec_send_packet(codec_ctx_, pkt);

    while (true) {
        int ret = avcodec_receive_frame(codec_ctx_, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
        if (ret < 0) throw std::runtime_error("Decode error");

        // Resample to float32 stereo 44100Hz
        int out_samples = av_rescale_rnd(
            swr_get_delay(swr_ctx_, codec_ctx_->sample_rate) + frame->nb_samples,
            44100, codec_ctx_->sample_rate, AV_ROUND_UP
        );

        std::vector<float> buf(out_samples * 2);
        uint8_t* out_ptr = reinterpret_cast<uint8_t*>(buf.data());

        int converted = swr_convert(
            swr_ctx_, &out_ptr, out_samples,
            (const uint8_t**)frame->extended_data, frame->nb_samples
        );

        out.insert(out.end(), buf.begin(), buf.begin() + converted * 2);
    }
}

void MultiCodecAudioDecoder::close() {
    if (swr_ctx_)   { swr_free(&swr_ctx_);           swr_ctx_   = nullptr; }
    if (codec_ctx_) { avcodec_free_context(&codec_ctx_); codec_ctx_ = nullptr; }
    if (fmt_ctx_)   { avformat_close_input(&fmt_ctx_);   fmt_ctx_   = nullptr; }
}