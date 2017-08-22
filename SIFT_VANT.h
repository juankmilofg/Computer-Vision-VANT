#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <string>
#include <iostream>

#include <time.h>
#include <sys/time.h>

using namespace std;
using namespace cv;

#ifndef _SIFT_VANT
#define _SIFT_VANT
class SIFT_VANT
{
        private:
            Mat imagem1;
            Mat imagem2; 
            int pontos1, pontos2, pontos_matches, pontos_matches_total;
            long tempo1, tempo2, tempo_casamento;
            vector<KeyPoint> keypointsfil11, keypointsfil22;
            Mat descriptorsfil11, descriptorsfil22;
            vector< DMatch > matches;
            
        public:
            SIFT_VANT(Mat *img1,Mat *img2);
            long tempo_1( );
            long tempo_2( );
            long tempo__casamento( );
            int pontos_1( );
            int pontos_2( );
            int pontos_casamento( );
            int pontos_casamento_total( );
            vector<KeyPoint> key1( );
            vector<KeyPoint> key2( );
            Mat desc1( );
            Mat desc2( );
            vector< DMatch > keymatches( );
            virtual ~SIFT_VANT( )
            {
                imagem1.~Mat();
                imagem2.~Mat();
            }           
};
#endif 
