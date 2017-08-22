/*
Federal University of Minas Gerais - Brazil LITC
Computational Intelligence and Technology Laboratory. 
http://litc.cpdee.ufmg.br. 
Code developed by: 
Juan Camilo Fonseca Galindo - juankmilofg@ufmg.br
*/

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

#ifndef acuracia
#define acuracia
class acuracia
{
        private:
            Mat imagem1;
            Mat imagem2; 
            vector<KeyPoint> keypoints1, keypoints2;
            Mat descriptors1, descriptors2;
            vector< DMatch > matches;
            
        public:
            acuracia(Mat *img1,Mat *img2);//, vector<KeyPoint> *key11, vector<KeyPoint> *key22, vector< DMatch > *match);
            virtual ~acuracia( )
            {
                imagem1.~Mat();
                imagem2.~Mat();
            }           
};
#endif 