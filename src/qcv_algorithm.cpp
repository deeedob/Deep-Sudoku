#include "qcv_algorithm.hpp"

QCvImageProcessor::QCvImageProcessor(QObject *parent) : QObject(parent)
{
    camera = Q_NULLPTR;

    algorithm = "gray";

    cvAlgorithm = new QCvAlgorithm();
    workerThread = new QThread();
    connect(cvAlgorithm, &QCvAlgorithm::errorOccured, [this](QString msg)
    {
        emit errorOccured(msg);
    });
    connect(cvAlgorithm, &QCvAlgorithm::imageProcessed, [this](QString fname)
    {
        emit imageProcessed(fname);
    });
    cvAlgorithm->moveToThread(workerThread);
}

QCvImageProcessor::~QCvImageProcessor()
{
    delete workerThread;
    delete cvAlgorithm;
}

void QCvImageProcessor::setCamera(QVariant v)
{
    QObject *o = qvariant_cast<QObject *>(v);
    camera = qvariant_cast<QCamera *>(o->property("mediaObject"));
    //camera->setCaptureMode(QCamera::CaptureStillImage);
    imageCapture = new QImageCapture();
    //camera->focus()->setFocusMode(QCameraFocus::ContinuousFocus);
    //camera->focus()->setFocusPointMode(QCameraFocus::FocusPointAuto);
    connect(imageCapture, &QImageCapture::imageSaved, [this](int id, const QString &fileName)
    {
        Q_UNUSED(id);
        processSavedImage(fileName);
    });
}

void QCvImageProcessor::capture()
{
    if(imageCapture->isReadyForCapture())
    {
        //imageCapture->capture(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
        imageCapture->captureToFile(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    }
    else
    {
        emit errorOccured("Camera is not ready to capture.");
    }
}

void QCvImageProcessor::processSavedImage(const QString &fname)
{
    emit processStarted();

    qApp->processEvents();

    cvAlgorithm->inputFileName = fname;
    if(algorithm == "none")
    {
        cvAlgorithm->doNone();
    }
    else if(algorithm == "dft")
    {
        cvAlgorithm->doDft();
    }
    else if(algorithm == "canny")
    {
        cvAlgorithm->doCanny();
    }
    else if(algorithm ==  "gray")
    {
        cvAlgorithm->doGray();
    }
    else
    {
        emit errorOccured("Unknown algorithm : " + algorithm);
    }
}

void QCvAlgorithm::doNone()
{
    using namespace cv;
    Mat image = imread(inputFileName.toStdString());

    if( image.empty())
    {
        emit errorOccured("Capture image is empty.");
    }
    else
    {
        imwrite(inputFileName.toStdString(), image);
        emit imageProcessed(inputFileName);
    }
}

void QCvAlgorithm::doDft()
{
    using namespace cv;
    Mat image = imread(inputFileName.toStdString(), ImreadModes::IMREAD_GRAYSCALE);

    if( image.empty())
    {
        emit errorOccured("Capture image is empty.");
    }
    else
    {
        Mat padded;                            //expand input image to optimal size
        int m = getOptimalDFTSize( image.rows );
        int n = getOptimalDFTSize( image.cols ); // on the border add zero values
        copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

        Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
        Mat complexI;
        merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

        dft(complexI, complexI);            // this way the result may fit in the source matrix

        // compute the magnitude and switch to logarithmic scale
        // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
        split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
        magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
        Mat magI = planes[0];

        magI += Scalar::all(1);                    // switch to logarithmic scale
        log(magI, magI);

        // crop the spectrum, if it has an odd number of rows or columns
        magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

        // rearrange the quadrants of Fourier image  so that the origin is at the image center
        int cx = magI.cols/2;
        int cy = magI.rows/2;

        Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
        Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
        Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
        Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

        Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);

        q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
        q2.copyTo(q1);
        tmp.copyTo(q2);

        normalize(magI, magI, 0, 255, NORM_MINMAX); // Transform the matrix with float values into a
        // viewable image form (float between values 0 and 1).

        // image: original (black and white)
        // magI: fourier

        //cvtColor(magI, magI, CV_GRAY2RGB);
        cvtColor(magI, magI, COLOR_GRAY2RGB);

        imwrite(inputFileName.toStdString(), magI);

        emit imageProcessed(inputFileName);
    }
}

void QCvAlgorithm::doCanny()
{
    using namespace cv;
    Mat image = imread(inputFileName.toStdString());

    if( image.empty())
    {
        emit errorOccured("Capture image is empty.");
    }
    else
    {
        Mat imageGray, dstImage, detectedEdges;
        dstImage.create( image.size(), image.type() );
        cvtColor( image, imageGray, COLOR_GRAY2RGB );
        blur( imageGray, detectedEdges, Size(3,3) );
        Canny( detectedEdges, detectedEdges, 50, 150, 3 );
        dstImage = Scalar::all(0);
        image.copyTo( dstImage, detectedEdges);
        imwrite(inputFileName.toStdString(), dstImage);
        emit imageProcessed(inputFileName);
    }
}

void QCvAlgorithm::doGray()
{
    using namespace cv;
    Mat image = imread(inputFileName.toStdString(), ImreadModes::IMREAD_GRAYSCALE);

    if( image.empty())
    {
        emit errorOccured("Capture image is empty.");
    }
    else
    {
        Mat grayImage;
        cvtColor(image, grayImage, COLOR_GRAY2RGB);
        imwrite(inputFileName.toStdString(), grayImage);
        emit imageProcessed(inputFileName);
    }
}

void QCvImageProcessor::setAlgorithm(QString alg)
{
    algorithm = alg;
}