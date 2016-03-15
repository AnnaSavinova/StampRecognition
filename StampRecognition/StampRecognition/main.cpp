#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>

int main( int argc, char* argv[] )
{
    IplImage* image = 0;
    // ��� �������� ������� ������ ����������
    char* filename = argc >= 2 ? argv[1] : "Image0.jpg";
    // �������� �������� (� ��������� ������)
    image = cvLoadImage( filename, CV_LOAD_IMAGE_GRAYSCALE );

    printf( "[i] image: %s\n", filename );
    assert( image != 0 );

    // �������� ������������ �����������
    IplImage* src = cvLoadImage( filename );

    // ��������� ������ ��� ������
    CvMemStorage* storage = cvCreateMemStorage( 0 );
    // ���������� �����������
    cvSmooth( image, image, CV_GAUSSIAN, 5, 5 );
    // ����� ������
    CvSeq* results = cvHoughCircles(
        image,
        storage,
        CV_HOUGH_GRADIENT,
        10,
        image->width/5
        );
    // ����������� �� ������ � ������ �� �� ������������ �����������
    for( int i = 0; i < results->total; i++ ) {
        float* p = ( float* ) cvGetSeqElem( results, i );
        CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
        cvCircle( src, pt, cvRound( p[2] ), CV_RGB( 0xff, 0, 0 ) );
    }
    // ����������
    cvNamedWindow( "cvHoughCircles", 1 );
    cvShowImage( "cvHoughCircles", src );

    // ��� ������� �������
    cvWaitKey( 0 );

    // ����������� �������
    cvReleaseMemStorage( &storage );
    cvReleaseImage( &image );
    cvReleaseImage( &src );
    cvDestroyAllWindows();
    return 0;
}