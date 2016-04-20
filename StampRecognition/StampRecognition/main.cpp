#include "StampRecognition.h"

int main( int argc, char* argv[] )
{
    // имя файла с информацией о картинках задается первым аргументом
    char* imagesInfo = argc >= 2 ? argv[1] : "";
    // вторым аргументом передаем путь до папки с изображениями
    char* imagesPathPrefix = argc >= 3 ? argv[2] : "";
    std::string imageSavePathPrefix( imagesPathPrefix );
    imageSavePathPrefix += "result\\";
    std::ifstream in;
    in.open( imagesInfo, std::ifstream::in );
    if( !in.is_open() ) {
        throw std::invalid_argument( "Невозможно открыть файл с информацией о изображениях" );
    }

    std::ofstream scoresOut;
    std::string scoresPath( imageSavePathPrefix );
    scoresPath += "scores.csv";

    while( !in.eof() ) {
        std::string imageName;
        in >> imageName;
        std::string imagePath = imagesPathPrefix + imageName;
        std::string imageSavePath = imageSavePathPrefix + imageName;

        int count, centerX, centerY, radius;
        in >> count;
        std::vector< CStampRecognition::CCircle > answers;

        for( int i = 0; i < count; ++i ) {
            in >> centerX;
            in >> centerY;
            in >> radius;
            std::cout << imagePath << " " << centerX << " " << centerY << " " << radius << std::endl;
            answers.push_back( CStampRecognition::CCircle( centerX, centerY, radius ) );
        }
        CStampRecognition sr( imagePath.c_str(), answers, imageSavePath, scoresPath );
        sr.DoHough();
    }
    in.close();

    system( "pause" );
    return 0;
}