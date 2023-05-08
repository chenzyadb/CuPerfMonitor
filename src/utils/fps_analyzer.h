#pragma once

#include "utils/cu_misc.h"
#include "utils/cu_log.h"

class FpsAnalyzer {
    public:
        FpsAnalyzer();
        ~FpsAnalyzer();

        typedef struct {
            int fps;
            int jank;
            int bigJank;
            int maxFrameTime;
        } FpsData;

        void StartAnalysis();
        FpsData GetFpsData();

    private:
        uint64_t clearSfBufTime_;
        
        std::string GetWindowName();
};