#include "CMyTracker1.hpp"

#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

#define MIN_FEATURES 50
#define WINDOW_W 16
#define WINDOW_H 16

CMyTracker1::CMyTracker1()
{
}

CMyTracker1::~CMyTracker1()
{
}

bool CMyTracker1::AddNewPoints(void)
{
	return prevKpts.size() <= MIN_FEATURES;
}

void CMyTracker1::detectFeatures(cv::Mat &currImg)
{
	cv::FastFeatureDetector detector(50);
	cv::BriefDescriptorExtractor extractor(32); //this is really 32 x 8 matches since they are binary matches packed into bytes

	detector.detect(currImg,currKpts);
	extractor.compute(currImg,currKpts,currDesc);
}

void CMyTracker1::timeUpdate(void)
{
	std::swap(currKpts,prevKpts);
	cv::swap(currDesc,prevDesc);
}

bool CMyTracker1::Track(cv::Mat &currImg)
{

	/// TODO : ���� �����ϱ� ���ٴ� �����Ÿ� �����ؾ� �� ��.
	if(prevKpts.empty() || prevKpts.size() <= MIN_FEATURES)
	{
		detectFeatures(currImg);
		timeUpdate();
		/// �� �������� ��� kpts�� desc�� prev�� �����Ǿ���.
		return false;
	}

	if(AddNewPoints())
	{
		detectFeatures(currImg);
	}

	/// ���⿡���� tracking�� ����.
	/// �츮�� �����ִ°��� desc�� kpts.
	/// kpts �ٹ� W ��ŭ Ž���� �ϰ� binary desc�� �����ϰ� score�� �ű��.
	/// imageROI = image(cv::Rect(385,270,width,height)); �� roi�� ���� �� �� �ֱ���.
	/// Kpts �� iterator�� ���鼭. �� �ٹ� W��ŭ patch ROI �� �����ϰ�. bindesc�� �̾Ƽ� ���ϰ� ���� �켱������ �ű��.
	std::vector<cv::KeyPoint> trackedKpts;

	/// patch size �� 48�̴�.
	cv::BriefDescriptorExtractor extractor(32); //this is really 32 x 8 matches since they are binary matches packed into bytes

	cv::Mat patchDesc;
	std::vector<std::vector<cv::DMatch>> matches;

	for(unsigned int i=0;i<prevKpts.size();i++)
	{
		int x = (int)prevKpts[i].pt.x;
		int y = (int)prevKpts[i].pt.y;

		/// �ٹ� 16x16�� �����Ͽ� pushback.
		for(unsigned int w=x+WINDOW_W/2;w>(x-WINDOW_W/2);w--)
		{
			if(w<0)
				break;
			for(unsigned int h=y+WINDOW_H/2;h>(y-WINDOW_H/2);h--)
			{
				if(h<0)
					break;
				// do  e.g.   
				//printf("(%d,%d) ",h,w);
				trackedKpts.push_back(cv::KeyPoint(w,h,prevKpts[i].size));
			}
		}

		/// Descriptor ���.
		extractor.compute(currImg,trackedKpts,patchDesc);

		/// purge
		//trackedKpts.clear();
		//patchDesc.deallocate();
		
		printf(".");
	}
	printf("#");
}