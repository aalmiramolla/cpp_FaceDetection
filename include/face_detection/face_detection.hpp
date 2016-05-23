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

#pragma once

#include <iostream>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dirent.h>
#include <cmath>
#include <sys/stat.h>

#include "face_detection/globals.hpp"

class FaceDetection {
private:
    std::string haarcascade_file;
    std::vector<cv::Mat> images;
	std::vector<cv::Mat> detected_faces;
	
public:
	// Constructor and desctructor
    FaceDetection (std::string haarcascade_file);
    // virtual ~FaceDetection ();

    // Add images from a path
	void obtain_images(std::string images_path);
    // Detect faces in this frame
	void detect_faces(cv::Mat frame);

	// Getters and setters
	std::vector<cv::Mat> get_images();
	std::string get_haarcascade_file();
	cv::Mat get_image_at_index(unsigned int idx);

};

// Processes a vector of images and save all faces in a vector
std::vector<cv::Mat> process_images(FaceDetection face_detection);

// Create a mosaic from images given in a vector
cv::Mat create_mosaic(std::vector<cv::Mat>);

// Save the faces on a folder
void save_faces(std::vector<cv::Mat>, std::string);
