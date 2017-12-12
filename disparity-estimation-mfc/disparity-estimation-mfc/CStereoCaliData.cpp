#include "stdafx.h"
#include "CStereoCaliData.h"

bool matIsEqual(const cv::Mat mat1, const cv::Mat mat2) {
	if (mat1.empty() && mat2.empty()) {
		return true;
	}
	if (mat1.cols != mat2.cols || mat1.rows != mat2.rows || mat1.dims != mat2.dims ||
		mat1.channels() != mat2.channels()) {
		return false;
	}
	if (mat1.size() != mat2.size() || mat1.channels() != mat2.channels() || mat1.type() != mat2.type()) {
		return false;
	}
	int nrOfElements1 = mat1.total()*mat1.elemSize();
	if (nrOfElements1 != mat2.total()*mat2.elemSize()) return false;
	bool lvRet = memcmp(mat1.data, mat2.data, nrOfElements1) == 0;
	return lvRet;
}


bool CStereoCaliData::getData(CCameraDS* pCam, bool fullCheck)
{
	if (!checkObjPoints(fullCheck) || getImgSize(pCam))
		return false;
	cv::stereoCalibrate(pL->worldPointsVect, pL->cornersVect, pR->cornersVect, pL->pData->cameraMatirx, pL->pData->distCoeffs, pR->pData->cameraMatirx, pR->pData->distCoeffs, imgSize, R, T, E, F);
	T.at<double>(0, 0) = -T.at<double>(0, 0);
	return true;
}

bool CStereoCaliData::checkObjPoints(bool fullCheck)
{
	if (pL == NULL || pR == NULL)
	{
		AfxMessageBox(_T("缺少标定器！"));
		return false;
	}
	if (pL->pData == NULL || pR->pData == NULL)
	{
		AfxMessageBox(_T("缺少单目标定数据"));
		return false;
	}
	if (pL->worldPointsVect.size() != pR->worldPointsVect.size())
	{
		AfxMessageBox(_T("两个摄像头标定数据不符：图片数目不同"));
		return false;
	}

	if (fullCheck)
	{
		for (int i = 0; i < pL->worldPointsVect.size(); i++)
		{
			if (pL->worldPointsVect[i].size() != pR->worldPointsVect[i].size())
			{
				AfxMessageBox(_T("两个摄像头标定数据不符：角点数目不同"));
				return false;
			}
			for (int j = 0; j < pL->worldPointsVect[i].size(); j++)
			{
				cv::Point3f p1 = pL->worldPointsVect[i].at(j);
				cv::Point3f p2 = pR->worldPointsVect[i].at(j);
				if (p1.x != p2.x || p1.y != p2.y || p1.z != p2.z)
				{
					AfxMessageBox(_T("两个摄像头标定数据不符：角点坐标不同"));
					return false;
				}
			}
		}
	}
	return false;
}

bool CStereoCaliData::getImgSize(CCameraDS * pCam)
{
	if (pCam == NULL)
	{
		AfxMessageBox(_T("相机未初始化！"));
		return false;
	}
	imgSize = cv::Size(pCam->GetWidth(), pCam->GetHeight());
	return true;
}

CStereoCaliData::CStereoCaliData()
{
}


CStereoCaliData::~CStereoCaliData()
{
}

void CStereoCaliData::openFile(std::string path)
{
	cv::FileStorage fs(path, cv::FileStorage::READ);
	pL = new CCalibrater;
	pL->pData = new CCalibrationData;
	pR = new CCalibrater;
	pR->pData = new CCalibrationData;
	fs["l_mtx"] >> pL->pData->cameraMatirx;
	fs["l_dist"] >> pL->pData->distCoeffs;
	fs["l_cornersVect"] >> pL->cornersVect;
	fs["l_worldPointsVect"] >> pL->worldPointsVect;
	fs["r_mtx"] >> pR->pData->cameraMatirx;
	fs["r_dist"] >> pR->pData->distCoeffs;
	fs["r_cornersVect"] >> pR->cornersVect;
	fs["r_worldPointsVect"] >> pR->worldPointsVect;
	fs["R"] >> R;
	fs["T"] >> T;
	fs["E"] >> E;
	fs["F"] >> F;
	fs["imgSize"] >> imgSize;
	fs.release();


}

void CStereoCaliData::saveFile(std::string path)
{
	cv::FileStorage fs(path, cv::FileStorage::WRITE);
	cv::Mat l_mtx = pL->pData->cameraMatirx;
	cv::Mat l_dist = pL->pData->distCoeffs;
	cv::Mat r_mtx = pR->pData->cameraMatirx;
	cv::Mat r_dist = pR->pData->distCoeffs;
	fs << "l_mtx" << l_mtx << "l_dist" << l_dist; 
	fs << "r_mtx" << r_mtx << "r_dist" << r_dist; 
	fs << "R" << R << "T" << T << "E" << E << "F" << F << "imgSize" << imgSize;
	fs << "l_cornersVect" << pL->cornersVect << "l_worldPointsVect" << pL->worldPointsVect;
	fs << "r_cornersVect" << pR->cornersVect << "r_worldPointsVect" << pR->worldPointsVect;
	fs.release();

}
