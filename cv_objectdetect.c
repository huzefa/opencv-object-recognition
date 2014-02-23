#include <stdio.h>
#include "cv.h"
#include "highgui.h"

CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;

void detect(IplImage *img) {
    int i;
    CvSeq *object = cvHaarDetectObjects(
            img,
            cascade,
            storage,
            1.5,				// Scale factor
            2,					// Minimum neighbours
            1,					// CV_HAAR_DO_CANNY_PRUNING
            cvSize( 30,30),		// Minsize
            cvSize(640,480) );	// Maxsize

    for(i = 0; i < (object? object->total : 0); i++) {
        CvRect *r = ( CvRect* )cvGetSeqElem( object, i );
        cvRectangle( img,
			cvPoint( r->x, r->y ),
			cvPoint( r->x + r->width, r->y + r->height ),
			CV_RGB( 255, 0, 0 ), 2, 8, 0 );
                
        //printf("%d,%d\nnumber =%d\n",r->x,r->y,object->total);
    }

    cvShowImage( "video", img );
}
int main(int argc, char **argv) {
    CvCapture *capture;
    IplImage  *frame;
    int       key;
    char      *filename = "usb.xml"; // Change to the name of classifier

    cascade = (CvHaarClassifierCascade *) cvLoad(filename, 0, 0, 0);
    storage = cvCreateMemStorage(0);
    capture = cvCaptureFromCAM(0);

    // Check
    // assert(cascade && storage && capture);

    cvNamedWindow("video", 1);

    while(1) {
        frame = cvQueryFrame(capture);
        detect(frame);
        key = cvWaitKey(50);
    }

    cvReleaseImage(&frame);
    cvReleaseCapture(&capture);
    cvDestroyWindow("video");
    cvReleaseHaarClassifierCascade(&cascade);
    cvReleaseMemStorage(&storage);

    return 0;
}
