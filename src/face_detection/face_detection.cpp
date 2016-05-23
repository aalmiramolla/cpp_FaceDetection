// COPYRIGHT (c) 2016 Alejandro Almira Molla <aalmiramolla at gmail dot com>
//
// GNU GENERAL PUBLIC LICENSE
//    Version 3, 29 June 2007
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "face_detection/face_detection.hpp"

FaceDetection::FaceDetection(std::string haarcascade_file) {
	std::vector<cv::Mat> images();
	std::vector<cv::Mat> detected_faces();
	this->haarcascade_file = haarcascade_file;
}

void
FaceDetection::obtain_images(std::string images_path) {

    DIR *dir;
    struct dirent *ent;

    if((dir = opendir (images_path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if(std::string(ent->d_name).compare(".") != 0 && std::string(ent->d_name).compare("..") != 0) {
                if (T_RECURSIVE && ent->d_type == DT_DIR) {
                    std::string next_dir(images_path);
                    next_dir += std::string(ent->d_name);
                    next_dir += std::string("/");
					obtain_images(next_dir);
                    // std::vector<cv::Mat> images_aux(obtain_images(next_dir));
                    // this->images.insert(std::end(this->images), std::begin(images_aux), std::end(images_aux));
                }
                else {
                    if (std::string(ent->d_name).find(".png") != std::string::npos || std::string(ent->d_name).find(".jpg") != std::string::npos) {
                        std::string photo(images_path);
                        photo += std::string(ent->d_name);
                        this->images.push_back(cv::imread(photo));
                    }
                }
            }
        }

        closedir (dir);
    }
}

void
FaceDetection::detect_faces(cv::Mat frame) {
    cv::CascadeClassifier face_cascade;
    std::vector<cv::Rect> faces;
    std::vector<cv::Mat> images_of_faces;
    cv::Mat frame_gray;
    cv::Mat my_face_image;

    // Load haarcacade
    if( !face_cascade.load( haarcascade_file ) )
        std::cerr << "ERROR: Can't load " <<  haarcascade_file << std::endl;

    // Convert image to grey
    cv::cvtColor( frame, frame_gray, CV_BGR2GRAY );
    // Equalise image
    cv::equalizeHist( frame_gray, frame_gray );

    // Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.5, 5, 0, cv::Size(50, 50) );

    // Process faces of the image
    for( size_t i = 0; i < faces.size(); ++i ) {

        // Set the points of the face
        cv::Point up_left( faces[i].x, faces[i].y);
        cv::Point down_right( faces[i].x + faces[i].width, faces[i].y +  faces[i].height);

        // Draw a rectangle on the image
        // cv::rectangle(frame,up_left,down_right,cv::Scalar(255,0,0));

        // Crop image to obtain only faces
        cv::Rect my_face (up_left, down_right);
        my_face_image = cv::Mat(frame,my_face);

        // Save the face cropped on vector
        detected_faces.push_back(my_face_image);
    }
}

std::vector<cv::Mat>
FaceDetection::get_images() {
	return images;
}

std::vector<cv::Mat>
FaceDetection::get_detected_faces() {
	return detected_faces;
}

cv::Mat
FaceDetection::get_image_at_index(unsigned int idx) {
	if ( idx > 0 && idx < images.size() )
		return images[idx];
	return cv::Mat();
}

std::string
FaceDetection::get_haarcascade_file() {
	return haarcascade_file;
}


std::vector<cv::Mat> process_images(FaceDetection face_detection) {
    std::vector<cv::Mat> faces;

    for (size_t i = 0; i < face_detection.get_images().size(); i++) {
        // Detect faces in image
		face_detection.detect_faces(face_detection.get_image_at_index(i));
        std::vector<cv::Mat> faces_aux = face_detection.get_detected_faces();
        // Insert in vector, faces found in image
        faces.insert(std::end(faces), std::begin(faces_aux), std::end(faces_aux));
    }

    return faces;
}

cv::Mat create_mosaic(std::vector<cv::Mat> images) {
    cv::Mat result;
    cv::Mat _result;
    cv::Mat image;

    // Size of face
    cv::Size size(128,128);

    // Obtain best average rows ans columns
    int rows = (unsigned int) (sqrt(images.size()) + 0.5);
    int columns = rows;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            unsigned int idx = row * columns + col;
            if (col == 0) {
                // Auxiliary result is first image of row
                if (idx >= images.size()) {
                    _result = cv::Mat(size,CV_8UC3, cv::Scalar(0,0,0));
                }
                else {
                    _result = images[idx];
                }
                cv::resize(_result, _result, size);
            }
            else {
                if (idx >= images.size()) {
                    image = cv::Mat(size,CV_8UC3, cv::Scalar(0,0,0));
                }
                else {
                    image = images[idx];
                }
                cv::resize(image, image, size);
                // Append to _result new image
                cv::hconcat(_result, image, _result);
            }
        }
        if (row == 0) {
            // Result is first row
            result = _result;
        }
        else {
            // Else append new row to result
            cv::vconcat(result, _result, result);
        }
    }

    cv::imwrite("./result.png", result);

    return result;
}

void save_faces(std::vector<cv::Mat> faces, std::string directory) {
    for (size_t i = 0; i < faces.size(); i++) {
        cv::Mat face = faces[i];
        std::string name(directory);
        std::string _name(std::to_string(i));
        // Insert three 0's at beginning of number
        _name.insert(0, 3, '0');
		// Add extension to directory
        _name += ".png";
        name += _name;

        cv::imwrite(name, face);
    }
}
