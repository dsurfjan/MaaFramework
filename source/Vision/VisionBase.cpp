#include "VisionBase.h"

#include "Utils/NoWarningCV.h"

#include "Utils/ImageIo.hpp"
#include "MaaUtils/Logger.hpp"
#include "Utils/StringMisc.hpp"
#include "Utils/Time.hpp"

MAA_VISION_NS_BEGIN

VisionBase::VisionBase(InstanceInternalAPI* inst) : inst_(inst) {}

VisionBase::VisionBase(InstanceInternalAPI* inst, const cv::Mat& image) : inst_(inst)
{
    set_image(image);
}

VisionBase::VisionBase(InstanceInternalAPI* inst, const cv::Mat& image, const cv::Rect& roi) : inst_(inst)
{
    set_image(image);
    set_roi(roi);
}

void VisionBase::set_image(const cv::Mat& image)
{
    image_ = image;
#ifdef MAA_DEBUG
    image_draw_ = image.clone();
#endif

    set_roi(roi_);
}

void VisionBase::set_roi(const cv::Rect& roi)
{
    roi_ = correct_roi(roi, image_);
}

cv::Rect VisionBase::correct_roi(const cv::Rect& roi, const cv::Mat& image)
{
    if (image.empty()) {
        LogError << "image is empty" << VAR(image);
        return roi;
    }
    if (roi.empty()) {
        return { 0, 0, image.cols, image.rows };
    }

    cv::Rect res = roi;
    if (image.cols < res.x) {
        LogError << "roi is out of range" << VAR(image) << VAR(res);
        res.x = image.cols - res.width;
    }
    if (image.rows < res.y) {
        LogError << "roi is out of range" << VAR(image) << VAR(res);
        res.y = image.rows - res.height;
    }

    if (res.x < 0) {
        LogWarn << "roi is out of range" << VAR(image) << VAR(res);
        res.x = 0;
    }
    if (res.y < 0) {
        LogWarn << "roi is out of range" << VAR(image) << VAR(res);
        res.y = 0;
    }
    if (image.cols < res.x + res.width) {
        LogWarn << "roi is out of range" << VAR(image) << VAR(res);
        res.width = image.cols - res.x;
    }
    if (image.rows < res.y + res.height) {
        LogWarn << "roi is out of range" << VAR(image) << VAR(res);
        res.height = image.rows - res.y;
    }
    return res;
}

MAA_VISION_NS_END