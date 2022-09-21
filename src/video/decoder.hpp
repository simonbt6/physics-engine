#pragma once

#include <opencv2/opencv.hpp>

#include "../def.hpp"

namespace Engine
{
    namespace Video
    {
        enum class FrameStatus
        {
            OK,
            EMPTY
        };

        class Decoder
        {
        private:
            cv::VideoCapture m_Capture;
            int32_t m_Width, m_Height;

        public:
            Decoder(const std::string& filepath);
            ~Decoder();

            FrameStatus nextFrame(cv::Mat& frame);

            inline int32_t getWidth() const { return m_Width; }
            inline int32_t getHeight() const { return m_Height; }

            inline int32_t getFrequency() const { return (int32_t) m_Capture.get(cv::CAP_PROP_FPS); }
        };
    };
};