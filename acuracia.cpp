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
//#include "opencv2/nonfree/features2d.hpp"

#include <time.h>
#include <sys/time.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include "acuracia.h" 

using namespace std;
using namespace cv;


acuracia::acuracia(Mat *img1,Mat *img2, vector<KeyPoint> *key11, vector<KeyPoint> *key22, vector< DMatch > *match, int theta, int phi, int psi, int imagem)
{
    this->imagem1 = *img1;
    this->imagem2 = *img2;
    this->keypoints1 = *key11;
    this->keypoints2 = *key22;
    this->matches = *match;
    
   //Mat img_matches;
   //drawMatches(  imagem1 , keypoints1,imagem2, keypoints2, matches, img_matches );
   //imshow("acuracia", img_matches ); 
   //waitKey(0);
   //cout<<matches.size()<<endl;
   Rect caja; 
   caja.width = 200; 
   caja.height = 200; 
   caja.x = 0;       
   caja.y = 0; 
   int x_0, x_1, x_2, x_3, x_4;
   int y_0, y_1, y_2, y_3, y_4;
   float yaw, pitch, roll;
   int xx,yy;
   CvSize size_ut;
   
    switch ( theta )
           {
           case 1: 
             switch ( phi )
                {
                case 1: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(800,800);
                        xx=0; yy=0;
                        yaw=0; pitch=0; roll=0;
                        x_0=0; x_1=400; x_2=0;   x_3=400;
                        y_0=0; y_1=0;   y_2=400; y_3=400; 
                    break;
                    case 2: 
                        size_ut = cvSize(800,800);
                        xx=800; yy=0;
                        yaw=90; pitch=0; roll=0;
                        x_0=0; x_1=400; x_2=0;   x_3=400;
                        y_0=0; y_1=0;   y_2=400; y_3=400; 
                    break;
                    case 3: 
                        size_ut = cvSize(1110,1110);
                        xx=1100; yy=700;
                        yaw=150; pitch=0; roll=0;
                        x_0=400; x_1=200; x_2=430; x_3=270;
                        y_0=150; y_1=300; y_2=370; y_3=470; 
                    break;
                    }
                break;
                case 2: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(930,930);
                        xx=928; yy=140;
                        yaw=0; pitch=0; roll=100;
                        x_0=115; x_1=80;  x_2=380;   x_3=340;
                        y_0=130; y_1=440;   y_2=170; y_3=540; 
                    break;
                    case 2: 
                        size_ut = cvSize(950,950);
                        xx=790; yy=928;
                        yaw=90; pitch=0; roll=100;
                        x_0=110; x_1=70; x_2=455;   x_3=170;
                        y_0=125; y_1=450;   y_2=145; y_3=500; 
                    break;
                    case 3: 
                        size_ut = cvSize(1050,1050);
                        xx=280; yy=1025;
                        yaw=150; pitch=0; roll=100;
                        x_0=360; x_1=105; x_2=510; x_3=200;
                        y_0=105; y_1=200; y_2=410; y_3=520; 
                    break;
                    }
                break;
                case 3: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(1100,1100);
                        xx=1090; yy=700;
                        yaw=0; pitch=0; roll=150;
                        x_0=300; x_1=340; x_2=470;   x_3=400;
                        y_0=145; y_1=240;   y_2=360; y_3=400; 
                    break;
                    case 2: 
                        size_ut = cvSize(1100,1100);
                        xx=420; yy=1096;
                        yaw=90; pitch=0; roll=150;
                        x_0=366; x_1=140; x_2=490; x_3=400;
                        y_0=177; y_1=240; y_2=360; y_3=400; 
                    break;
                    case 3: 
                        size_ut = cvSize(1120,1120);
                        xx=-10; yy=700;
                        yaw=150; pitch=0; roll=150;
                        x_0=310; x_1=150; x_2=430; x_3=270;
                        y_0=130; y_1=250; y_2=370; y_3=470; 
                    break;
                    }
                break;
                }
           break;
           case 2:
             switch ( phi )
                {
                case 1:
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(832,832);
                        xx=0; yy=0;
                        yaw=0; pitch=16; roll=0;
                        x_0=0; x_1=400; x_2=0;   x_3=400;
                        y_0=0; y_1=0;   y_2=400; y_3=400; 
                    break;
                    case 2: 
                        size_ut = cvSize(832,832);
                        xx=800; yy=0;
                        yaw=90; pitch=16; roll=0;
                        x_0=0; x_1=400; x_2=0;   x_3=400;
                        y_0=0; y_1=0;   y_2=400; y_3=400; 
                    break;
                    case 3: 
                        size_ut = cvSize(1110,1110);
                        xx=1100; yy=700;
                        yaw=150; pitch=16; roll=0;
                        x_0=400; x_1=200; x_2=430; x_3=270;
                        y_0=150; y_1=300; y_2=370; y_3=470; 
                    break;
                    }
                break;
                case 2: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(970,970);
                        xx=965; yy=140;
                        yaw=0; pitch=16; roll=100;
                        x_0=115; x_1=80;  x_2=380;   x_3=340;
                        y_0=130; y_1=440;   y_2=130; y_3=500; 
                    break;
                    case 2: 
                        size_ut = cvSize(975,975);
                        xx=790; yy=970;
                        yaw=90; pitch=16; roll=100;
                        x_0=110; x_1=70; x_2=455;   x_3=170;
                        y_0=125; y_1=450;   y_2=145; y_3=500; 
                    break;
                    case 3: 
                        size_ut = cvSize(1070,1070);
                        xx=280; yy=1045;
                        yaw=150; pitch=16; roll=100;
                        x_0=360; x_1=105; x_2=510; x_3=200;
                        y_0=105; y_1=200; y_2=410; y_3=510; 
                    break;
                    }
                break;
                case 3: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(1140,1140);
                        xx=1130; yy=700;
                        yaw=0; pitch=16; roll=150;
                        x_0=300; x_1=340; x_2=470;   x_3=400;
                        y_0=145; y_1=240;   y_2=360; y_3=400; 
                    break;
                    case 2: 
                        size_ut = cvSize(1135,1135);
                        xx=400; yy=1130;
                        yaw=90; pitch=16; roll=150;
                        x_0=366; x_1=140; x_2=490; x_3=400;
                        y_0=177; y_1=240; y_2=350; y_3=400; 
                    break;
                    case 3: 
                        size_ut = cvSize(1120,1120);
                        xx=-10; yy=700;
                        yaw=150; pitch=16; roll=150;
                        x_0=310; x_1=150; x_2=430; x_3=270;
                        y_0=130; y_1=250; y_2=370; y_3=470; 
                    break;
                    }
                break;
                }
           break;
           case 3:
             switch ( phi )
                {
                case 1:
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(945,945);
                        xx=0; yy=0;
                        yaw=0; pitch=32; roll=0;
                        x_0=0; x_1=400; x_2=0;   x_3=400;
                        y_0=0; y_1=0;   y_2=400; y_3=400; 
                    break;
                    case 2: 
                        size_ut = cvSize(945,945);
                        xx=800; yy=0;
                        yaw=90; pitch=32; roll=0;
                        x_0=0; x_1=400; x_2=0;   x_3=400;
                        y_0=0; y_1=0;   y_2=400; y_3=400; 
                    break;
                    case 3: 
                        size_ut = cvSize(1230,1230);
                        xx=1220; yy=700;
                        yaw=150; pitch=32; roll=0;
                        x_0=400; x_1=200; x_2=430; x_3=270;
                        y_0=150; y_1=290; y_2=340; y_3=440; 
                    break;
                    }
                break;
                case 2: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(1100,1100);
                        xx=1095; yy=140;
                        yaw=0; pitch=32; roll=100;
                        x_0=115; x_1=80;  x_2=380;   x_3=350;
                        y_0=130; y_1=340;   y_2=130; y_3=400; 
                    break;
                    case 2: 
                        size_ut = cvSize(1105,1105);
                        xx=790; yy=1100;
                        yaw=90; pitch=32; roll=100;
                        x_0=110; x_1=70; x_2=440;   x_3=170;
                        y_0=125; y_1=450;   y_2=145; y_3=500; 
                    break;
                    case 3: 
                        size_ut = cvSize(1135,1135);
                        xx=260; yy=1120;
                        yaw=150; pitch=32; roll=100;
                        x_0=360; x_1=105; x_2=510; x_3=200;
                        y_0=115; y_1=210; y_2=360; y_3=500; 
                    break;
                    }
                break;
                case 3: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(1300,1300);
                        xx=1290; yy=700;
                        yaw=0; pitch=32; roll=150;
                        x_0=300; x_1=340; x_2=460; x_3=400;
                        y_0=145; y_1=240; y_2=300; y_3=300; 
                    break;
                    case 2: 
                        size_ut = cvSize(1285,1285);
                        xx=400; yy=1280;
                        yaw=90; pitch=32; roll=150;
                        x_0=300; x_1=140; x_2=390; x_3=350;
                        y_0=130; y_1=240; y_2=350; y_3=350; 
                    break;
                    case 3: 
                        size_ut = cvSize(1300,1300);
                        xx=10; yy=760;
                        yaw=150; pitch=32; roll=150;
                        x_0=300; x_1=150; x_2=370; x_3=320;
                        y_0=130; y_1=250; y_2=320; y_3=420; 
                    break;
                    }
                break;
                }
            break;
            case 4:
            switch ( phi )
                {
                case 1:
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(1196,1196);
                        xx=0; yy=0;
                        yaw=0; pitch=48; roll=0;
                        x_0=0; x_1=400; x_2=0;   x_3=400;
                        y_0=0; y_1=0;   y_2=250; y_3=250; 
                    break;
                    case 2: 
                        size_ut = cvSize(1196,1196);
                        xx=800; yy=0;
                        yaw=90; pitch=48; roll=0;
                        x_0=0; x_1=300; x_2=0;   x_3=300;
                        y_0=0; y_1=0;   y_2=400; y_3=400; 
                    break;
                    case 3: 
                        size_ut = cvSize(1450,1450);
                        xx=1450; yy=700;
                        yaw=150; pitch=48; roll=0;
                        x_0=310; x_1=170; x_2=380; x_3=220;
                        y_0=160; y_1=340; y_2=320; y_3=390; 
                    break;
                    }
                break;
                case 2: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(1390,1390);
                        xx=1385; yy=140;
                        yaw=0; pitch=48; roll=100;
                        x_0=115; x_1=80;  x_2=380;   x_3=370;
                        y_0=130; y_1=140;   y_2=130; y_3=200; 
                    break;
                    case 2: 
                        size_ut = cvSize(1405,1405);
                        xx=790; yy=1400;
                        yaw=90; pitch=48; roll=100;
                        x_0=110; x_1=70; x_2=200;   x_3=170;
                        y_0=125; y_1=450;   y_2=145; y_3=500; 
                    break;
                    case 3: 
                        size_ut = cvSize(1300,1300);
                        xx=210; yy=1250;
                        yaw=150; pitch=48; roll=100;
                        x_0=360; x_1=105; x_2=510; x_3=200;
                        y_0=125; y_1=230; y_2=340; y_3=450; 
                    break;
                    }
                break;
                case 3: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(1640,1640);
                        xx=1630; yy=700;
                        yaw=0; pitch=48; roll=150;
                        x_0=300; x_1=320; x_2=390; x_3=350;
                        y_0=145; y_1=250; y_2=100; y_3=100; 
                    break;
                    case 2: 
                        size_ut = cvSize(1625,1625);
                        xx=400; yy=1620;
                        yaw=90; pitch=48; roll=150;
                        x_0=200; x_1=140; x_2=270; x_3=200;
                        y_0=180; y_1=240; y_2=340; y_3=300; 
                    break;
                    case 3: 
                        size_ut = cvSize(1570,1570);
                        xx=10; yy=860;
                        yaw=150; pitch=48; roll=150;
                        x_0=300; x_1=150; x_2=340; x_3=320;
                        y_0=130; y_1=250; y_2=280; y_3=220; 
                    break;
                    }
                break;
                }
            break;
            case 5:
            switch ( phi )
                {
                case 1:
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(1824,1820);
                        xx=0; yy=0;
                        yaw=0; pitch=64; roll=0;
                        x_0=0; x_1=200; x_2=400; x_3=600;
                        y_0=0; y_1=50;  y_2=0;   y_3=50; 
                    break;
                    case 2: 
                        size_ut = cvSize(1824,1820);;
                        xx=800; yy=0;
                        yaw=90; pitch=64; roll=0;
                        x_0=0; x_1=50;  x_2=0;   x_3=50;
                        y_0=0; y_1=200; y_2=400; y_3=600; 
                    break;
                    case 3: 
                        size_ut = cvSize(2000,2000);
                        xx=2000; yy=700;
                        yaw=150; pitch=64; roll=0;
                        x_0=100; x_1=230; x_2=445; x_3=200;
                        y_0=338; y_1=290; y_2=132; y_3=270; 
                    break;
                    }
                break;
                case 2: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(2170,2170);
                        xx=2170; yy=140;
                        yaw=0; pitch=64; roll=100;
                        x_0=108; x_1=225;  x_2=224; x_3=470;
                        y_0=108; y_1=50;   y_2=70;  y_3=110; 
                    break;
                    case 2: 
                        size_ut = cvSize(2170,2170);
                        xx=790; yy=2140;
                        yaw=90; pitch=64; roll=100;
                        x_0=108; x_1=50;  x_2=70;   x_3=90;
                        y_0=108; y_1=225; y_2=224;  y_3=470; 
                    break;
                    case 3: 
                        size_ut = cvSize(1750,1750);
                        xx=120; yy=1660;
                        yaw=150; pitch=64; roll=100;
                        x_0=50;  x_1=180; x_2=445; x_3=150;
                        y_0=375; y_1=405; y_2=172; y_3=420; 
                    break;
                    }
                break;
                case 3: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(2500,2500);
                        xx=2500; yy=700;
                        yaw=0; pitch=64; roll=150;
                        x_0=185; x_1=360; x_2=386; x_3=235;
                        y_0=100; y_1=50; y_2=70; y_3=103; 
                    break;
                    case 2: 
                        size_ut = cvSize(2500,2500);
                        xx=400; yy=2500;
                        yaw=90; pitch=64; roll=150;
                        x_0=80; x_1=70; x_2=80; x_3=80;
                        y_0=230; y_1=350; y_2=386; y_3=230; 
                    break;
                    case 3: 
                        size_ut = cvSize(2100,2100);
                        xx=10; yy=1160;
                        yaw=150; pitch=64; roll=150;
                        x_0=180; x_1=300; x_2=318; x_3=340;
                        y_0=265; y_1=160; y_2=143; y_3=170; 
                    break;
                    }
                break;
                }
            break;
            case 6:
            switch ( phi )
                {
                case 1:
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(2900,2900);
                        xx=0; yy=0;
                        yaw=0; pitch=76; roll=0;
                        x_0=0; x_1=200; x_2=400; x_3=600;
                        y_0=0; y_1=0;  y_2=0;   y_3=0; 
                    break;
                    case 2: 
                        size_ut = cvSize(1824,1820);;
                        xx=800; yy=0;
                        yaw=90; pitch=76; roll=0;
                        x_0=0; x_1=00;  x_2=0;   x_3=0;
                        y_0=0; y_1=200; y_2=400; y_3=600; 
                    break;
                    case 3: 
                        size_ut = cvSize(2000,2000);
                        xx=2000; yy=700;
                        yaw=150; pitch=76; roll=0;
                        x_0=100; x_1=230; x_2=445; x_3=200;
                        y_0=338; y_1=290; y_2=132; y_3=270; 
                    break;
                    }
                break;
                case 2: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(2170,2170);
                        xx=2170; yy=140;
                        yaw=0; pitch=76; roll=100;
                        x_0=108; x_1=225;  x_2=224; x_3=440;
                        y_0=108; y_1=50;   y_2=70;  y_3=100; 
                    break;
                    case 2: 
                        size_ut = cvSize(2170,2170);
                        xx=790; yy=2140;
                        yaw=90; pitch=76; roll=100;
                        x_0=108; x_1=50;  x_2=70;   x_3=90;
                        y_0=108; y_1=225; y_2=224;  y_3=430; 
                    break;
                    case 3: 
                        size_ut = cvSize(1750,1750);
                        xx=120; yy=1660;
                        yaw=150; pitch=76; roll=100;
                        x_0=100;  x_1=180; x_2=445; x_3=150;
                        y_0=375; y_1=405; y_2=172; y_3=420; 
                    break;
                    }
                break;
                case 3: 
                    switch ( psi )
                    {
                    case 1: 
                        size_ut = cvSize(2500,2500);
                        xx=2500; yy=700;
                        yaw=0; pitch=76; roll=150;
                        x_0=185; x_1=250; x_2=146; x_3=220;
                        y_0=80; y_1=80; y_2=90; y_3=83; 
                    break;
                    case 2: 
                        size_ut = cvSize(2500,2500);
                        xx=400; yy=2500;
                        yaw=90; pitch=76; roll=150;
                        x_0=80; x_1=70; x_2=100; x_3=80;
                        y_0=230; y_1=180; y_2=200; y_3=230; 
                    break;
                    case 3: 
                        size_ut = cvSize(2100,2100);
                        xx=10; yy=1160;
                        yaw=150; pitch=76; roll=150;
                        x_0=474; x_1=340; x_2=358; x_3=575;
                        y_0=140; y_1=180; y_2=173; y_3=0; 
                    break;
                    }
                break;
                }
            break;
           }
    switch (imagem)
    {
         case 0:
            	caja.x = x_0;       
		caja.y = y_0;
		break;
         case 1:
                caja.x = x_1;       
		caja.y = y_1;
            break;
	 case 2:
                caja.x = x_2;       
		caja.y = y_2;
            break;
	 case 3:
            	caja.x = x_3;       
		caja.y = y_3;
            break;
       }
           
    int match_outlier=0;
    int match_inlier=0;
    int match_desv=0;
    
    double Homografica[3][3];
    yaw=-yaw*3.1416/180.0; pitch=-pitch*3.1416/180.0; roll=-roll*3.1416/180.0;
    Homografica[0][0] = ((cos(yaw)*cos(roll))/(cos(pitch)))-(sin(yaw)*sin(roll));
    Homografica[0][1] = ((cos(yaw)*sin(roll))/(cos(pitch)))+(sin(yaw)*cos(roll));
    Homografica[0][2] = xx;
    Homografica[1][0] = -((sin(yaw)*cos(roll))/(cos(pitch)))-(cos(yaw)*sin(roll));
    Homografica[1][1] = -((sin(yaw)*sin(roll))/(cos(pitch)))+(cos(yaw)*cos(roll));
    Homografica[1][2] = yy;
    Homografica[2][0] = 0;
    Homografica[2][1] = 0;
    Homografica[2][2] = 1;
    
    Mat homography_matrix( 2, 3, CV_64F , Homografica);
    Mat homography_matrix_inv( 2, 3, CV_64F);
    //cout << "H: "<<homography_matrix << endl;
    
    invertAffineTransform(homography_matrix, homography_matrix_inv);
    //cout << "H-1: "<<homography_matrix_inv << endl;
    
    double Homograficainv[3][3];
    
    for(int i = 0; i < homography_matrix_inv.rows; i++)
    {
        const double* Mi = homography_matrix_inv.ptr<double>(i);
        for(int j = 0; j < homography_matrix_inv.cols; j++){
            Homograficainv[i][j] = Mi[j];
        }
    }
    Homograficainv[2][0] = 0;
    Homograficainv[2][1] = 0;
    Homograficainv[2][2] = 1;
                
    double ww=size_ut.width;
    double hh=size_ut.height;
    double sizeancho=ww/800; 
    double sizealtura=hh/800;
    //cout << size_ut.width << " " << sizeancho << " " << endl;
    //cout << size_ut.height << " " << sizealtura << " " << endl;
    
    
    

    if ((keypoints1.size()!=0)&&(keypoints1.size()!=0))
    {
        for(unsigned int i = 0; i < matches.size(); i++)
            {
                double distancia1;
                double distancia2;
                
                double x1 = keypoints1[i].pt.x;
                double y1 = keypoints1[i].pt.y;
                double x2 = keypoints2[i].pt.x;
                double y2 = keypoints2[i].pt.y;
                
                double x1casado = (x2 * Homografica[0][0] + y2 *Homografica[0][1] + Homografica[0][2])/sizeancho -caja.x ;
                double y1casado = (x2 * Homografica[1][0] + y2 *Homografica[1][1] + Homografica[1][2])/sizealtura - caja.y ;
                
                double x1aux = (x1 + caja.x)*sizeancho ;
                double y1aux = (y1 + caja.y)*sizeancho ;
                
                double x2casado = (x1aux * Homograficainv[0][0] + y1aux *Homograficainv[0][1] + Homograficainv[0][2]);
                double y2casado = (x1aux * Homograficainv[1][0] + y1aux *Homograficainv[1][1] + Homograficainv[1][2]);
                
                double distx1 =x1-x1casado;
                double disty1 =y1-y1casado;
                distancia1 =sqrt(pow(distx1, 2)+pow(disty1, 2));
                
                double distx2 =x2-x2casado;
                double disty2 =y2-y2casado;
                distancia2 =sqrt(pow(distx2, 2)+pow(disty2, 2));
                
                //cout<<"distancia1 "<< distancia1<<endl;
                //cout<<"distancia2 "<< distancia2<<endl;
                
                distanciatotal += distancia1 + distancia2;
                //cout<< "x1 " << x1 <<  " y1 " << y1 << " xcasado " << xcasado << " ycasado " << ycasado << endl; 
            }
        distanciatotal= distanciatotal/matches.size();
    //cout<<"media distancia "<< distanciatotal<<endl;
    }
    
}

double acuracia::distancia( )
{
  return distanciatotal;
}


