#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cctype>
#include <climits>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;

/* Analyze & flip image. Image is saved in a file, we will need to flip it as required
 * and save to another file.The program works with a header( 8 bytes ) and its data.
 * Header stores an information about a number of channels needed for a pixel and size of the channel.
 * Data can be write as big-endian or little-endian, header holds that information .
 */

const uint16_t ENDIAN_LITTLE = 0x4949;
const uint16_t ENDIAN_BIG    = 0x4d4d;

#endif /* __PROGTEST__ */

// structure of a header. Size of each field is known(each field 2B)
// so fixed size data types are used ( uint16_t)
#pragma pack(push,1)
struct ImageHeader {
    uint16_t endian;
    uint16_t width;
    uint16_t height;
    uint16_t channels;
};
#pragma pack(pop)

class  Image{
private:
    size_t 				width_;             //width of the image
    size_t				height_;            // height of the image
    uint16_t            channelsCount_;     // count of channels, possible values are 1,3,4
    uint16_t			channelSize_;       // channel size, possible values are 1,8,16
    uint16_t            channel_;           // channel information from a header
public:
    struct Pixel;                                           // structure to represent pixel
    vector<vector<Pixel>> data;                                // array of image data
    enum class Endianity : uint8_t {unknown, little, big};
    Endianity 		endianity_;                             // current image endianity
    // recieves information from a header and sets endianity of the data
    Endianity assignEndian(uint16_t endian);
    // tries to read image from file, returns false if failed to parse header or broken data
    bool readFromFile(const char * srcFileName);
    // write to a file, returns false if ...
    bool writeToFile(const char  * dstFileName);
    // recieves information from a header and sets channelcount and channel size
    bool assignChannels();
    // helper function to print out the data that was red
    //void printData();
    void flipHorizontal();
    void flipVertical();
    void flipHorVer();
    Image();
};
//-----------------------------------------------------------------------------------
// Structure to represent a pixel. Vector size depends on the size of number of channels required
struct Image::Pixel{
    vector<uint16_t> pixel;
    Pixel(uint16_t n);
};
//-----------------------------------------------------------------------------------
Image::Pixel::Pixel(uint16_t n): pixel(n)
{
}
//-----------------------------------------------------------------------------------
/*
void Image::printData(){
    for( int i = 0 ; i < height_; i++){
        for( int j = 0; j < width_; j++){
            for(int k = 0 ; k < 3 ; k ++){
                cout << data[i][j].pixel[k] << " ";
            }
        }
        cout << endl;
    }
}*/
//-----------------------------------------------------------------------------------
Image::Image()
        : width_(0)
        , height_(0)
        , channelsCount_(0)
        , channelSize_(0)
        , channel_(0)
{
}
//-----------------------------------------------------------------------------------
Image::Endianity Image::assignEndian(uint16_t endian){
    if( endian == 18761){
        return Image::Endianity::little;
    }else if( endian == 19789){
        return Image::Endianity::big;
    }else return Image::Endianity::unknown;
}
//-----------------------------------------------------------------------------------
bool Image::assignChannels() {
    if( channel_ == 12 ){
        channelsCount_ = 1;
        channelSize_ = 8;
    }else if( channel_ == 13){
        channelsCount_ = 4;
        channelSize_ = 16;
    }else if( channel_ == 14){
        channelsCount_ = 3;
        channelSize_ = 8;
    }else if(channel_ == 15 ){
        channelsCount_ = 4;
        channelSize_ = 8;
    }else if ( channel_ == 16) {
        channelsCount_ = 1;
        channelSize_ = 16;
    }else{
        return false;
    }

    return true;
}
//-----------------------------------------------------------------------------------
bool Image::readFromFile(const char * srcFileName) {
    char c;
    ifstream myFile(srcFileName, ios::in | ios::binary);
    if (myFile.fail()) {
        myFile.close();
        return false;
    }
    // reading of the header
    vector<char> headerRawData(sizeof(ImageHeader));
    myFile.read(headerRawData.data(), sizeof(ImageHeader));
    if(!myFile) {
        myFile.close();
        return false;
    }
    ImageHeader *header = reinterpret_cast<ImageHeader *>(headerRawData.data());
    //assigning data to the image
    endianity_ = assignEndian(header->endian);
    channel_ = header->channels;
    width_ = header->width;
    height_ = header->height;

    if( endianity_ == Endianity::big){
        width_ = (width_ >> 8) | (width_ << 8);
        height_ = (height_ >> 8) | (height_ << 8);
        channel_ = (channel_ >> 8) | (channel_ << 8);
    }else if (endianity_ == Endianity::unknown) {
        myFile.close();
        return false;
    }
    if( !assignChannels()){
        return false;
    }
    if( width_ < 1 || height_ < 1){
        return false;
    }
    data.resize(height_, vector<Pixel>(width_, channelsCount_ * (channelSize_/8)));
    // reads data into a array of pixels
    uint16_t k = 0;
    size_t j = 0;
    size_t i = 0;
    for (i = 0; i < height_; i++) {
        for (j = 0; j < width_; j++) {
            for ( k = 0; k < channelsCount_; k++) {
                myFile.read(reinterpret_cast<char *>(&data[i][j].pixel[k]), sizeof(char));
                if ( myFile.eof() && ((k != (channelsCount_) ) || ( j != (width_)) || ( i !=(height_)))) {
                    myFile.close();
                    return false;
                }else if( i == (height_ -1 ) && (j == width_ -1 ) && k == (channelsCount_ -1 )){
                    for( int t = 0; t < (channelSize_/8) ; t++)
                        myFile.read((&c), sizeof(char));
                    if( !myFile.eof()){
                        myFile.close();
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
//-----------------------------------------------------------------------------------
// Function swaps first and last pixel moves towards the center of the height
void Image::flipVertical(){
    size_t height = height_;
    for( size_t i = 0 ; i < height_/2;i++){
        for( size_t j = 0; j < width_; j++){
            swap(data[i][j],data[height - 1][j]);
        }
        height --;
    }
}
//-----------------------------------------------------------------------------------
// Function to swaps first and last pixel and moves towards the center of the row
void Image::flipHorizontal(){
    size_t width = width_;
    for( size_t i = 0; i < height_ ; i ++){
        for( size_t j = 0; j < width ; j++){
            swap(data[i][j],data[i][width - 1]);
            width--;
        }
        width = width_;
    }
}
//-----------------------------------------------------------------------------------
void Image::flipHorVer(){
    flipHorizontal();
    flipVertical();
}
//-----------------------------------------------------------------------------------
bool Image::writeToFile(const char  * dstFileName ){

    std::fstream myFile(dstFileName, ios::out | ios::binary) ;
    if( myFile.fail()){
        myFile.close();
        return false;
    }
    uint16_t little = 18761;
    uint16_t big = 19789;
    // writing of a header
    if( endianity_ == Endianity::little){
        myFile.write(reinterpret_cast<char *>(&little), sizeof(uint16_t));
    }
    if( endianity_ == Endianity::big){
        myFile.write(reinterpret_cast<char *>(&big), sizeof(uint16_t));
    }
    myFile.write(reinterpret_cast<char *>(&width_), sizeof(uint16_t));
    myFile.write(reinterpret_cast<char *>(&height_), sizeof(uint16_t));
    myFile.write(reinterpret_cast<char *>(&channel_), sizeof(uint16_t));
    if(myFile.fail()){
        myFile.close();
        return false;
    }
    myFile.seekg(8);
    // writes array of pixel into a file
    for( size_t i = 0 ; i < height_; i++) {
        for( size_t j = 0 ; j < width_ ; j++){
            for( size_t k = 0 ; k < channelsCount_; k++) {
                myFile.write(reinterpret_cast<char *>(&data[i][j].pixel[k]), sizeof(char));
            }
        }
    }

    if( myFile.good() ) {
        myFile.close();
        return true;
    }else {
        myFile.close();
        return false;
    }
}
//-----------------------------------------------------------------------------------
bool flipImage ( const char  * srcFileName,
                 const char  * dstFileName,
                 bool          flipHorizontal,
                 bool          flipVertical )
{
    Image img;
    if ( !img.readFromFile(srcFileName)){
        return false;
    }
    if( flipHorizontal & !flipVertical){
        img.flipHorizontal();
    }else if( !flipHorizontal & flipVertical){
       img.flipVertical();
    }else if( flipHorizontal & flipVertical){
      img.flipHorVer();
    }
    if( !img.writeToFile(dstFileName)){
        return false;
    }
    return true;
}
//-----------------------------------------------------------------------------------
// Function recieves two file name and compare them, return false if missmatch is found
bool identicalFiles ( const char * fileName1,
                      const char * fileName2 ){
    fstream File1( fileName1, ios::in | ios::binary);
    fstream File2( fileName2, ios::in | ios::binary);
    char tmp1 = 0, tmp2 = 0;

    while( File1 && File2) {
        File1.read((&tmp1), sizeof(char));
        File2.read((&tmp2), sizeof(char));
        if( tmp1 != tmp2){
            return false;
        }
    }
    return true;
}

#ifndef __PROGTEST__


int main ( void )
{
    assert ( !flipImage ( "2.bin", "output_09.img", true, false ) );
    assert ( flipImage ( "input_00.img", "output_00.img", true, false )
             && identicalFiles ( "output_00.img", "ref_00.img" ) );

    assert ( flipImage ( "input_01.img", "output_01.img", false, true )
             && identicalFiles ( "output_01.img", "ref_01.img" ) );

    assert ( flipImage ( "input_02.img", "output_02.img", true, true )
             && identicalFiles ( "output_02.img", "ref_02.img" ) );

    assert ( flipImage ( "input_03.img", "output_03.img", false, false )
             && identicalFiles ( "output_03.img", "ref_03.img" ) );

    assert ( flipImage ( "input_04.img", "output_04.img", true, false )
             && identicalFiles ( "output_04.img", "ref_04.img" ) );

    assert ( flipImage ( "input_05.img", "output_05.img", true, true )
             && identicalFiles ( "output_05.img", "ref_05.img" ) );

    assert ( flipImage ( "input_06.img", "output_06.img", false, true )
             && identicalFiles ( "output_06.img", "ref_06.img" ) );

    assert ( flipImage ( "input_07.img", "output_07.img", true, false )
             && identicalFiles ( "output_07.img", "ref_07.img" ) );

    assert ( flipImage ( "input_08.img", "output_08.img", true, true )
             && identicalFiles ( "output_08.img", "ref_08.img" ) );

    assert ( ! flipImage ( "input_09.img", "output_09.img", true, false ) );

    // extra inputs (optional & bonus tests)
    assert ( flipImage ( "extra_input_00.img", "extra_out_00.img", true, false )
             && identicalFiles ( "extra_out_00.img", "extra_ref_00.img" ) );
    assert ( flipImage ( "extra_input_01.img", "extra_out_01.img", false, true )
             && identicalFiles ( "extra_out_01.img", "extra_ref_01.img" ) );
    assert ( flipImage ( "extra_input_02.img", "extra_out_02.img", true, false )
             && identicalFiles ( "extra_out_02.img", "extra_ref_02.img" ) );
    assert ( flipImage ( "extra_input_03.img", "extra_out_03.img", false, true )
             && identicalFiles ( "extra_out_03.img", "extra_ref_03.img" ) );
    assert ( flipImage ( "extra_input_04.img", "extra_out_04.img", true, false )
             && identicalFiles ( "extra_out_04.img", "extra_ref_04.img" ) );
    assert ( flipImage ( "extra_input_05.img", "extra_out_05.img", false, true )
             && identicalFiles ( "extra_out_05.img", "extra_ref_05.img" ) );
    assert ( flipImage ( "extra_input_06.img", "extra_out_06.img", true, false )
             && identicalFiles ( "extra_out_06.img", "extra_ref_06.img" ) );
    assert ( flipImage ( "extra_input_07.img", "extra_out_07.img", false, true )
             && identicalFiles ( "extra_out_07.img", "extra_ref_07.img" ) );
    assert ( flipImage ( "extra_input_08.img", "extra_out_08.img", true, false )
             && identicalFiles ( "extra_out_08.img", "extra_ref_08.img" ) );
    assert ( flipImage ( "extra_input_09.img", "extra_out_09.img", false, true )
             && identicalFiles ( "extra_out_09.img", "extra_ref_09.img" ) );
    assert ( flipImage ( "extra_input_10.img", "extra_out_10.img", true, false )
             && identicalFiles ( "extra_out_10.img", "extra_ref_10.img" ) );
    assert ( flipImage ( "extra_input_11.img", "extra_out_11.img", false, true )
             && identicalFiles ( "extra_out_11.img", "extra_ref_11.img" ) );

    return 0;
}
#endif /* __PROGTEST__ */
