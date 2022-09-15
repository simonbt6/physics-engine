#include "decoder.hpp"

using namespace Engine::Video;

Decoder::Decoder(const std::string& filepath)
:m_Capture(filepath), m_Width(0), m_Height(0)
{
    if (!m_Capture.isOpened())
    {
        Util::Log::Error(std::format("Failed to open {} video file.", filepath));
        return;
    }

    m_Width = (int32_t)m_Capture.get(cv::CAP_PROP_FRAME_WIDTH);
    m_Height = (int32_t)m_Capture.get(cv::CAP_PROP_FRAME_HEIGHT);
}

Decoder::~Decoder()
{
    m_Capture.release();
    cv::destroyAllWindows();
}

FrameStatus Decoder::nextFrame(cv::Mat& frame)
{
    m_Capture >> frame;
    return frame.empty() ? FrameStatus::EMPTY : FrameStatus::OK;
}