#include "CMyTracker1.hpp"

#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

#define MIN_FEATURES 50

CMyTracker1::CMyTracker1()
{
}

CMyTracker1::~CMyTracker1()
{
}

bool CMyTracker1::Track(cv::Mat &currImg)
{
	cv::FastFeatureDetector detector(50);
	cv::BriefDescriptorExtractor extractor(32); //this is really 32 x 8 matches since they are binary matches packed into bytes

	/// TODO : ���� �����ϱ� ���ٴ� �����Ÿ� �����ؾ� �� ��.
	if(prevKpts.empty() || prevKpts.size() <= MIN_FEATURES)
	{
		detector.detect(currImg,currKpts);
		extractor.compute(currImg,currKpts,currDesc);
		std::swap(currKpts,prevKpts);
		cv::swap(currDesc,prevDesc);

		/// �� �������� ��� kpts�� desc�� prev�� �����Ǿ���.
		return false;
	}

	/// ���⿡���� tracking�� ����.
	/// �츮�� �����ִ°��� desc�� kpts.
	/// kpts �ٹ� W ��ŭ Ž���� �ϰ� binary desc�� �����ϰ� score�� �ű��.
	/// imageROI = image(cv::Rect(385,270,width,height)); �� roi�� ���� �� �� �ֱ���.
	/// Kpts �� iterator�� ���鼭. �� �ٹ� W��ŭ patch ROI �� �����ϰ�. bindesc�� �̾Ƽ� ���ϰ� ���� �켱������ �ű��.
}