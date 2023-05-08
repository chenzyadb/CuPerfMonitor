#include "fps_analyzer.h"

FpsAnalyzer::FpsAnalyzer() { }

FpsAnalyzer::~FpsAnalyzer() { }

void FpsAnalyzer::StartAnalysis() 
{
    clearSfBufTime_ = GetTimeStampMs();
    system("/system/bin/dumpsys SurfaceFlinger --latency-clear");
}

FpsAnalyzer::FpsData FpsAnalyzer::GetFpsData()
{
    uint64_t refreshPeriod = 0;
    std::vector<uint64_t> desiredPresentTimes{};
    std::vector<uint64_t> actualPresentTimes{};
    std::vector<uint64_t> frameReadyTimes{};

    uint64_t dumpSfBufTime = GetTimeStampMs();
    std::string command = "/system/bin/dumpsys SurfaceFlinger --latency \"" + GetWindowName() + "\"";
    FILE* fp = popen(command.c_str(), "r");
    if (fp) {
        char buffer[1024] = { 0 };
        fgets(buffer, sizeof(buffer), fp);
        refreshPeriod = atoi(buffer);
        while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
            unsigned long long desiredPresentTime = 0, actualPresentTime = 0, frameReadyTime = 0;
            sscanf(buffer, "%llu %llu %llu", &desiredPresentTime, &actualPresentTime, &frameReadyTime);
            if (desiredPresentTime > 0 && actualPresentTime > 0 && frameReadyTime > 0) {
                desiredPresentTimes.emplace_back(desiredPresentTime);
                actualPresentTimes.emplace_back(actualPresentTime);
                frameReadyTimes.emplace_back(frameReadyTime);
            }
        }
        pclose(fp);
    }

    FpsData data;
    if (desiredPresentTimes.size() < 2) {
        data.fps = 0;
        data.jank = 0;
        data.bigJank = 0;
        data.maxFrameTime = 0;
        return data;
    }

    std::sort(desiredPresentTimes.begin(), desiredPresentTimes.end());
    std::sort(actualPresentTimes.begin(), actualPresentTimes.end());
    std::sort(frameReadyTimes.begin(), frameReadyTimes.end());
    {
        int frameCount = actualPresentTimes.size();
        int timeMs = dumpSfBufTime - clearSfBufTime_;
        data.fps = frameCount * 1000 / timeMs;
    }
    {
        int jankCount = 0;
        int bigJankCount = 0;
        int maxFrameTime = 0;
        for (int idx = 1; idx < frameReadyTimes.size(); idx++) {
            int frameTimeMs = (frameReadyTimes[idx] - frameReadyTimes[idx - 1]) / 1000000;
            if (frameTimeMs > maxFrameTime) {
                maxFrameTime = frameTimeMs;
            }
            if (frameTimeMs > 1000 / 24 * 3) {
                bigJankCount++;
            } else if (frameTimeMs > 1000 / 24 * 2) {
                jankCount++;
            }
        }
        data.jank = jankCount;
        data.bigJank = bigJankCount;
        data.maxFrameTime = maxFrameTime;
    }

    return data;
}

std::string FpsAnalyzer::GetWindowName()
{
    std::string windowName = "";
    // /system/bin/dumpsys SurfaceFlinger --list | grep ^SurfaceView
    // SDK31: SurfaceView[tv.danmaku.bili/com.bilibili.video.videodetail.VideoDetailsActivity](BLAST)#0
    // SDK30: SurfaceView - tv.danmaku.bili/com.bilibili.video.videodetail.VideoDetailsActivity#0
    FILE* fp = popen("/system/bin/dumpsys SurfaceFlinger --list", "r");
    if (fp) {
        char buffer[1024] = { 0 };
        while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
            if (CheckRegex(buffer, "^SurfaceView")) {
                windowName = GetPrevString(buffer, '\n');
            }
        }
        pclose(fp);
    }

    return windowName;
}
