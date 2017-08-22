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

#include <cstdlib>
#include <iostream>
#include <string>
#include "acuracia.h" 

using namespace std;
using namespace cv;

acuracia::acuracia(Mat *img1, Mat *img2, vector<KeyPoint> *key11, vector<KeyPoint> *key22, vector< DMatch > *match)
{
    this->imagem1 = *img1;
    this->imagem2 = *img2;
    //this->keypoints1 = *key11;
    //this->keypoints2 = *key22;
    //this->matches = *match;
    
    Mat img_matches;
    //drawMatches(  imagem1 , keypoints1,imagem2, keypoints2, matches, img_matches );
    //imshow("entro a la funcion", img_matches ); 
    waitKey(0);
}